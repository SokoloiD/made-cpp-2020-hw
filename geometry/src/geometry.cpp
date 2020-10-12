//
// Created by asokolov on 09.10.2020.
//
#include "geometry.h"
#include <cmath>
/*
 *
 * class Point
 *
 */
Point::Point(double x_, double y_) : x(x_), y(y_) {}
bool Point::operator==(const Point &p) const {
    return fabs(x - p.x) < EPSILON && fabs(y - p.y) < EPSILON;
}
bool Point::operator!=(const Point &p) const {
    return !(*this == p);
};
double pointDistance(const Point &p1, const Point &p2) {
    return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
}
Point getMid(const Point &p1, const Point &p2) {
    return Point((p1.x + p2.x) / 2., (p1.y + p2.y) / 2.);
}
/*---
 *
 * class LINE
 *
 */
Line::Line(const Point &p1, const Point &p2) {

    b_ = p2.x - p1.x;
    a_ = p1.y - p2.y;
    c_ = -(p1.x * a_ + p1.y * b_);
}
Line::Line(const Point &p1, double angle) {
    a_ = -angle;
    b_ = 1;
    c_ = -(p1.x * a_ + p1.y * b_);
}
Line::Line(const double &offset, const double &angle) {
    a_ = -angle;
    b_ = 1;
    c_ = -offset;
}
// конструктор перпендикулярной линии, проходящей через точку.
Line::Line(const Line &line, const Point &p1) {
    a_ = line.b_;
    b_ = -line.a_;
    c_ = -(p1.x * a_ + p1.y * b_);
}
// возвращает биссектрису , проходящую через a
Line getBissectriceLine(const Point &a, const Point &b, const Point &c) {
    double abLen = pointDistance(a, b);
    double acLen = pointDistance(a, c);
    Point bisectPoint =  Point((b.x * acLen + c.x * abLen) / (abLen + acLen),
    (b.y * acLen + c.y * abLen) / (abLen + acLen));
    return Line(a,bisectPoint);
}

bool Line::operator==(const Line &other) {
    return fabs(det2(a_, b_, other.a_, other.b_)) < EPSILON &&
           fabs(det2(a_, -c_, other.a_, -other.c_)) < EPSILON;
}
bool Line::operator!=(const Line &other) {
    return !(*this == other);
}
double Line::getAngle() const { return -a_ / b_; }
double Line::getOffset() const { return -c_ / b_; }
// точка пересечения с линией (лин уравнения , правило крамера_
Point Line::crossLine(const Line &other) {
    double detMain = det2(a_, b_, other.a_, other.b_);
    double detY = det2(a_,- c_, other.a_, -other.c_);
    double detX = det2(-c_, b_, -other.c_, other.b_);
    return Point(detX / detMain, detY / detMain);
}
double det2(const double &a1, const double &b1, const double &a2, const double &b2) {
    return a1 * b2 - a2 * b1;
}


/*-----------------------
 * class Shape
 *
 -----------------------------
 */

void Shape::rotateVertices_(const Point &center, double angle) {

    long double angleRad = angle/M_PI*180.;
    long double cosAngle = cos(angleRad), sinAngle = sin(angleRad), dx, dy;
    for (auto & vertice : vertices_) {
        dx = vertice.x - center.x;
        dy = vertice.y - center.y;
        vertice.x = center.x + dx * cosAngle - dy * sinAngle;
        vertice.y = center.y + dx * sinAngle + dy * cosAngle;
    }
}
void Shape::reflexVertices_(const Point &center) {
    double dx, dy;
    for (auto & vertice : vertices_) {
        dx = vertice.x - center.x;
        dy = vertice.y - center.y;
        vertice.x = center.x - dx;
        vertice.y = center.y - dy;
    }
}
void Shape::reflexVertices_(const Line &axis) {
    double dx, dy;
    for (auto & vertice : vertices_) {
        Line ortLine(axis, vertice);//прямая, перпендикулярная оси симм через точку.
        Point ortoCrossPoint = ortLine.crossLine(axis);

        // симметрия относительно точки
        dx = vertice.x - ortoCrossPoint.x;
        dy = vertice.y - ortoCrossPoint.y;
        vertice.x =  ortoCrossPoint.x - dx;
        vertice.y =  ortoCrossPoint.y - dy;
    }
}
void Shape::scaleVertices_(const Point &center, double coefficient) {
    double dx, dy;
    for (auto & vertice : vertices_) {
        dx = vertice.x - center.x;
        dy = vertice.y - center.y;
        vertice.x = center.x + coefficient * dx;
        vertice.y = center.y + coefficient * dy;
    }
}

bool Shape::compareVertices_(const Shape &another) {
    size_t vertCnt = vertices_.size();
    if (vertCnt != another.vertices_.size()) {
        return false;
    }

    for (size_t offset = 0; offset < vertCnt; ++offset) {
        //прямая последовательность
        size_t eqCnt = 0;
        for (size_t i = 0; i < vertCnt; ++i) {
            if (vertices_[i] == another.vertices_[(i + offset) % vertCnt]) {
                eqCnt++;
            } else {
                break;
            }
        }
        if (eqCnt == vertCnt) {
            return true;
        }
        //обратная последовательность
        eqCnt = 0;
        for (size_t i = 0; i < vertCnt; ++i) {
            if (vertices_[i] == another.vertices_[(vertCnt - 1 - i + offset) % vertCnt]) {
                eqCnt++;
            } else {
                break;
            }
        }
        if (eqCnt == vertCnt) {
            return true;
        }
    }
    return false;
}
/*
 * class Polygon
 *
 */

Polygon::Polygon(const std::vector<Point> &points) {
    vertices_ = points;
}

size_t Polygon::verticesCount() const {
    return vertices_.size();
}
std::vector<Point> Polygon::getVertices() { return vertices_; }
double Polygon::perimeter() {
    double perim = 0.;
    size_t vertCnt = vertices_.size();
    auto prevPoint = vertices_[vertCnt - 1];

    for (size_t i = 0; i < vertCnt; ++i) {
        auto currPoint = vertices_[i];
        perim += pointDistance(prevPoint, currPoint);
        prevPoint = currPoint;
    }
    return perim;
}

double Polygon::area() {
    double ar = 0.;
    size_t vertCnt = vertices_.size();
    Point prevPoint = vertices_[vertCnt - 1];
    Point currPoint(0, 0);
    for (size_t i = 0; i < vertCnt; ++i) {
        currPoint = vertices_[i];
        ar += (currPoint.x + prevPoint.x) * (currPoint.y - prevPoint.y);
        prevPoint = currPoint;
    }
    return ar / 2.;
}
bool Polygon::operator==(const Shape &another) {
    auto *anotherPtr = dynamic_cast<const Polygon *>(&another);
    if (anotherPtr) {
        return compareVertices_(another);
    } else {
        return false;
    }
}
bool Polygon::operator!=(const Shape &another) {
    return !(*this == another);
}
void Polygon::rotate(Point center, double angle) {
    rotateVertices_(center, angle);
}
void Polygon::reflex(Point center) {
    reflexVertices_(center);
}
void Polygon::reflex(Line axis) {
    reflexVertices_(axis);
}

void Polygon::scale(Point center, double coefficient) {
    scaleVertices_(center, coefficient);
}
/*
 *
 * class Ellipse
 *
 *
 */

Ellipse::Ellipse(const Point &p1, const Point &p2, double summ) : a_(summ / 2.) {

    vertices_.push_back(p1);
    vertices_.push_back(p2);
    c_ = pointDistance(p1, p2) / 2.;
    e_ = c_ / a_;
    b_ = a_ * sqrt(1 - e_ * e_);
}
std::pair<Point, Point> Ellipse::focuses() {
    return std::pair<Point, Point>(vertices_[0], vertices_[1]);
}
double Ellipse::eccentricity() {
    return e_;
}
Point Ellipse::center() {
    return getMid(vertices_[0], vertices_[1]);
}

double Ellipse::perimeter() {

    return 4. * a_ * std::comp_ellint_2(e_);
}
double Ellipse::area() {
    return M_PI * a_ * b_;
}
bool Ellipse::operator==(const Shape &another) {
    auto *anotherPtr = dynamic_cast<const Ellipse *>(&another);
    if (anotherPtr) {
        return fabs(a_ - anotherPtr->a_) < EPSILON &&
               fabs(b_ - anotherPtr->b_) < EPSILON &&
               compareVertices_(another);
    } else {
        return false;
    }
}
bool Ellipse::operator!=(const Shape &another) {
    return !(*this == another);
}
void Ellipse::rotate(Point center, double angle) {
    rotateVertices_(center, angle);
}
void Ellipse::reflex(Point center) {
    reflexVertices_(center);
}
void Ellipse::reflex(Line axis) {
    reflexVertices_(axis);
}
void Ellipse::scale(Point center, double coefficient) {
    scaleVertices_(center, coefficient);
    a_ *= coefficient;
    b_ *= coefficient;
    c_ *= coefficient;
}
/*
 *
 * Circle
 *
 */

Circle::Circle(const Point &center, double r) : Ellipse(center, center, 2 * r) {}
double Circle::radius() {
    return a_;
}
double Circle::perimeter() {
    return 2. * M_PI * a_;
}
double Circle::area() {
    return M_PI * a_ * a_;
}
/*
 * Class Rectangle
 *
 */
Rectangle::Rectangle(const Point &p1, const Point &p3, double ratio) : Polygon(std::vector<Point>()) {

    double cosPow2 = 1. / (1 + ratio * ratio);
    //угол А  - между стороной и диагональю
    double cosA = sqrt(cosPow2),
           sinA = sqrt(1. - cosPow2);
    double lDiag = pointDistance(p1, p3);
    a_ = lDiag * cosA;
    b_ = lDiag * sinA;
    // угол B между диагональю и осью Х
    double cosB = (p3.x - p1.x) / lDiag;
    double sinB = (p3.y - p1.y) / lDiag;
    // угол G  = B - A - между короткой стороной и осью Y
    double sinG = sinB * cosA - cosB * sinA;
    double cosG = sinA * sinB + cosA * cosB;
    Point p2(p1.x - a_ * sinG, p1.y + a_ * cosG);
    Point p4(p3.x + a_ * sinG, p3.y - a_ * cosG);
    vertices_.push_back(p1);
    vertices_.push_back(p2);
    vertices_.push_back(p3);
    vertices_.push_back(p4);
}
Point Rectangle::center() {
    return getMid(vertices_[0], vertices_[2]);
}
double Rectangle::perimeter() {
    return 2 * (a_ + b_);
}
double Rectangle::area() {
    return a_ * b_;
}
std::pair<Line, Line> Rectangle::diagonals() {
    return std::pair<Line, Line>(Line(vertices_[0], vertices_[2]), Line(vertices_[1], vertices_[3]));
}
/*
 *
 * class Square
 *
 */
Square::Square(const Point &p1, const Point &p2) : Rectangle(p1, p2, 1.) {
}
// описанная окружность
Circle Square::circumscribedCircle() {
    return Circle(center(), a_ * sqrt(2) / 2.);
}
//вписанная окружность
Circle Square::inscribedCircle() {
    return Circle(center(), a_ / 2.);
}
/*
 *
 * Class Triangle
 *
 */

Triangle::Triangle(const Point &p1, const Point &p2, const Point &p3) : Polygon({p1, p2, p3}) {
}

Circle Triangle::circumscribedCircle() {
    auto line12 = Line(vertices_[1], vertices_[2]);
    auto ortLine12 = Line(line12,getMid(vertices_[1], vertices_[2]));
    auto line02 = Line(vertices_[0], vertices_[2]);
    auto ortLine02 = Line(line02,getMid(vertices_[0], vertices_[2]));
    auto midOrtoCroccPoint =  ortLine12.crossLine(ortLine02);

    double r = pointDistance(vertices_[0], vertices_[1]) *
               pointDistance(vertices_[0], vertices_[2]) *
               pointDistance(vertices_[1], vertices_[2]) /
               (4. * area());
    return Circle(midOrtoCroccPoint, r);
}
Circle Triangle::inscribedCircle() {
    double a = pointDistance(vertices_[0], vertices_[1]),
           b = pointDistance(vertices_[0], vertices_[2]),
           c = pointDistance(vertices_[1], vertices_[2]),
           p2 = (a + b + c) / 2;
    double r = sqrt((p2 - a) * (p2 - b) * (p2 - c) / p2);
    Line bisectABAC = getBissectriceLine(vertices_[0], vertices_[1], vertices_[2]);
    Line bisectBABC = getBissectriceLine(vertices_[1], vertices_[0], vertices_[2]);
    Point bissectCrossPoint = bisectABAC.crossLine(bisectBABC);
    return Circle(bissectCrossPoint, r);
}
Point Triangle::centroid() {

    auto midLine1 = Line(vertices_[0], getMid(vertices_[1], vertices_[2]));
    auto midLine2 = Line(vertices_[1], getMid(vertices_[0], vertices_[2]));
    return midLine1.crossLine(midLine2);
}
Point Triangle::orthocenter() {
    auto line12 = Line(vertices_[1], vertices_[2]);
    auto ortLine12 = Line(line12,vertices_[0]);
    auto line02 = Line(vertices_[0], vertices_[2]);
    auto ortLine02 = Line(line02,vertices_[1]);
    return ortLine12.crossLine(ortLine02);
}

Line Triangle::EulerLine() {
    return Line(orthocenter(), circumscribedCircle().center());
}
Circle Triangle::ninePointsCircle() {
    auto bigCircle = circumscribedCircle();
    auto center = getMid(orthocenter(), bigCircle.center());
    return Circle(center, bigCircle.radius() / 2.);
}
