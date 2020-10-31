
#pragma once
#include <cstddef>
#include <vector>
#include <utility>


const double EPSILON = 1.E-6;

class Point {
 public:
        explicit Point(double x_ = 0, double y_ = 0);
        bool operator==(const Point &p) const;
        bool operator!=(const Point &p) const;
        double x;
        double y;
};

double pointDistance(const Point &p1, const Point &p2);

class Line {
 public:
    Line(const Point &p1, const Point &p2);
    Line(const Point &p1, double angle);
    Line(const double &offset, const double &angle);
    // конструктор перпендикулярной линии, проходящей через p1
    Line(const Line &line, const Point &p1);
    bool operator==(const Line &other);
    bool operator!=(const Line &other);
    double getAngle() const;
    double getOffset() const;

    // точка пересечения прямых
    Point crossLine(const Line &other);

 private:
    double a_, b_, c_;  // ax + by + c = 0
};

// детерминант матрицы 2х2
double det2(const double &a1, const double &b1, const double &a2, const double &b2);

// уравнение биссектрисы угла
Line getBissectriceLine(const Point &a, const Point &b, const Point &c);


class Shape {
 public:
    virtual double perimeter() = 0;
    virtual double area() = 0;
    virtual bool operator==(const Shape &another) = 0;
    virtual bool operator!=(const Shape &another) = 0;
    virtual void rotate(Point center, double angle) = 0;
    virtual void reflex(Point center) = 0;
    virtual void reflex(Line axis) = 0;
    virtual void scale(Point center, double coefficient) = 0;

 protected:
    std::vector<Point> vertices_;
    void rotateVertices_(const Point &center, double angle);
    void reflexVertices_(const Point &center);
    void reflexVertices_(const Line &axis);
    void scaleVertices_(const Point &center, double coefficient);
    bool compareVertices_(const Shape &another);
};

class Polygon : public Shape {
 public:
    explicit Polygon(const std::vector<Point> &points);
    size_t verticesCount() const;
    std::vector<Point> getVertices();
    double perimeter() override;
    double area() override;
    bool operator==(const Shape &another) override;
    bool operator!=(const Shape &another) override;
    void rotate(Point center, double angle) override;
    void reflex(Point center) override;
    void reflex(Line axis) override;
    void scale(Point center, double coefficient) override;
};

class Ellipse : public Shape {
 public:
    Ellipse(const Point &p1, const Point &p2, double summ);
    std::pair<Point, Point> focuses();
    double eccentricity();
    Point center();

    double perimeter() override;
    double area() override;
    bool operator==(const Shape &another) override;
    bool operator!=(const Shape &another) override;
    void rotate(Point center, double angle) override;
    void reflex(Point center) override;
    void reflex(Line axis) override;
    void scale(Point center, double coefficient) override;

 protected:
    double a_, b_, c_, e_;  // оси a и b. межосевое расстояние и эксцентриситет
};

class Circle : public Ellipse {
 public:
    Circle(const Point &center, double r);
    double area() override;
    double perimeter() override;
    double radius();
};

class Rectangle : public Polygon {
 public:
    Rectangle(const Point &p1, const Point &p3, double ratio);
    Point center();
    double perimeter() override;
    double area() override;
    std::pair<Line, Line> diagonals();

 protected:
    double a_, b_;  // длины сторон
};

class Square : public Rectangle {
 public:
    Square(const Point &p1, const Point &p2);
    Circle circumscribedCircle();
    Circle inscribedCircle();
};

class Triangle : public Polygon {
 public:
    Triangle(const Point &p1, const Point &p2, const Point &p3);
    Circle circumscribedCircle();
    Circle inscribedCircle();
    Point centroid();
    Point orthocenter();
    Line EulerLine();
    Circle ninePointsCircle();
};
