#include "matrix.h"
#include <cmath>

using namespace task;
Matrix::Matrix() : cols_(1), rows_(1) {
    arr_ = new double *[1];
    arr_[0] = new double[1];
    arr_[0][0] = 1.;
}

Matrix::Matrix(size_t rows, size_t cols) : cols_(cols), rows_(rows) {
    arr_ = new double *[rows];
    for (size_t r = 0; r < rows; ++r) {
        arr_[r] = new double[cols];
        for (size_t c = 0; c < cols; ++c) {
            if (r == c) {
                arr_[r][c] = 1.;
            } else {
                arr_[r][c] = 0.;
            }
        }
    }
}

Matrix::Matrix(const Matrix &copy) {
    rows_ = copy.rows_;
    cols_ = copy.cols_;

    arr_ = new double *[rows_];
    for (size_t r = 0; r < rows_; ++r) {
        arr_[r] = new double[cols_];
        for (size_t c = 0; c < cols_; ++c) {
            arr_[r][c] = copy.arr_[r][c];
        }
    }
}

Matrix::~Matrix() {
    if (arr_ != nullptr) {
        for (size_t r = 0; r < rows_; ++r) {
            delete[] arr_[r];
        }
        delete[] arr_;
    }
}

Matrix &Matrix::operator=(const Matrix &a) {
    if (*this == a)
        return *this;

    for (size_t r = 0; r < rows_; ++r) {
        delete[] arr_[r];
    }
    delete[] arr_;
    rows_ = a.rows_;
    cols_ = a.cols_;
    arr_ = new double *[rows_];
    for (size_t r = 0; r < rows_; ++r) {
        arr_[r] = new double[cols_];
        for (size_t c = 0; c < cols_; ++c) {
            arr_[r][c] = a.arr_[r][c];
        }
    }
    return *this;
}

double &Matrix::get(size_t row, size_t col) {
    if (row >= rows_ || col >= cols_) {
        throw OutOfBoundsException();
    }
    return arr_[row][col];
}

const double &Matrix::get(size_t row, size_t col) const {
    if (row >= rows_ || col >= cols_) {
        throw OutOfBoundsException();
    }
    return arr_[row][col];
}

void Matrix::set(size_t row, size_t col, const double &value) {
    if (row >= rows_ || col >= cols_) {
        throw OutOfBoundsException();
    }
    arr_[row][col] = value;
}

void Matrix::resize(size_t new_rows, size_t new_cols) {
    size_t oldCols = cols_;
    size_t oldRows = rows_;
    rows_ = new_rows;
    cols_ = new_cols;
    auto **newArr = new double *[rows_];

    for (size_t r = 0; r < rows_; ++r) {
        newArr[r] = new double[cols_];
        for (size_t c = 0; c < cols_; ++c) {
            if (r < oldRows && c < oldCols) {
                newArr[r][c] = arr_[r][c];
            } else {
                newArr[r][c] = 0.;
            }
        }
    }
    for (size_t r = 0; r < oldRows; ++r) {
        delete[] arr_[r];
    }
    delete[] arr_;
    arr_ = newArr;
}

std::vector<double> Matrix::getRow(size_t row) {
    if (row >= rows_) {
        throw OutOfBoundsException();
    }
    auto vRow = new std::vector<double>(cols_);
    for (size_t c = 0; c < cols_; ++c) {
        (*vRow)[c] = arr_[row][c];
    }
    return *vRow;
}

std::vector<double> Matrix::getColumn(size_t column) {
    if (column >= cols_) {
        throw OutOfBoundsException();
    }
    auto vCol = new std::vector<double>(rows_);
    for (size_t r = 0; r < rows_; ++r) {
        (*vCol)[r] = arr_[r][column];
    }
    return *vCol;
}

bool Matrix::operator==(const Matrix &a) const {
    if (cols_ != a.cols_ || rows_ != a.rows_) {
        return false;
    }
    for (size_t r = 0; r < rows_; ++r) {
        for (size_t c = 0; c < cols_; ++c) {
            if (fabs(arr_[r][c] - a.arr_[r][c]) > EPS) {
                return false;
            }
        }
    }
    return true;
}

bool Matrix::operator!=(const Matrix &a) const { return !(*this == a); }

std::ostream &task::operator<<(std::ostream &output, const Matrix &matrix) {
    for (size_t r = 0; r < matrix.rows_; ++r) {
        for (size_t c = 0; c < matrix.cols_; ++c) {
            output << matrix.arr_[r][c] << " ";
        }
    }
    output << "\n";
    return output;
}

std::istream &task::operator>>(std::istream &input, Matrix &matrix) {
    size_t rows, cols;
    double tmp;
    input >> rows;
    input >> cols;
    matrix.resize(rows, cols);
    for (size_t r = 0; r < rows; ++r) {
        for (size_t c = 0; c < cols; ++c) {
            input >> tmp;
            matrix.set(r, c, tmp);
        }
    }
    return input;
}

Row Matrix::operator[](size_t row) {
    if (row >= rows_) {
        throw OutOfBoundsException();
    }
    return Row(*this, row);
}

const Row Matrix::operator[](size_t row) const {
    if (row >= rows_) {
        throw OutOfBoundsException();
    }
    return Row(*this, row);
}

Row::Row(const Matrix &matrix, size_t row) : size_(matrix.cols_) {
    arr_ = matrix.arr_[row];
}

double &Row::operator[](size_t col) {
    if (col >= size_) {
        throw OutOfBoundsException();
    }
    return arr_[col];
}

double Row::operator[](size_t col) const {
    if (col >= size_) {
        throw OutOfBoundsException();
    }
    return arr_[col];
}

Matrix &Matrix::operator+=(const Matrix &a) {
    if (cols_ != a.cols_ || rows_ != a.rows_) {
        throw SizeMismatchException();
    }
    for (size_t r = 0; r < rows_; ++r) {
        for (size_t c = 0; c < cols_; ++c) {
            arr_[r][c] += a.arr_[r][c];
        }
    }

    return *this;
}
Matrix &Matrix::operator-=(const Matrix &a) {
    if (cols_ != a.cols_ || rows_ != a.rows_) {
        throw SizeMismatchException();
    }
    for (size_t r = 0; r < rows_; ++r) {
        for (size_t c = 0; c < cols_; ++c) {
            arr_[r][c] -= a.arr_[r][c];
        }
    }
    return *this;
}

Matrix &Matrix::operator*=(const Matrix &a) {
    if (cols_ == a.rows_) {
        auto newRows = rows_;
        auto newCols = a.cols_;
        auto newArr = new double *[newRows];
        for (size_t r = 0; r < newRows; ++r) {
            newArr[r] = new double[newCols];
            for (size_t c = 0; c < newCols; ++c) {
                newArr[r][c] = 0.;
                for (size_t i = 0; i < cols_; ++i) {
                    newArr[r][c] += arr_[r][i] * a.arr_[i][c];
                }
            }
        }
        for (size_t r = 0; r < rows_; ++r) {
            delete[] arr_[r];
        }
        delete arr_;
        arr_ = newArr;
        cols_ = newCols;
        rows_ = newRows;
    } else {
        throw SizeMismatchException();
    }
    return *this;
}

Matrix &Matrix::operator*=(const double &number) {
    for (size_t r = 0; r < rows_; ++r) {
        for (size_t c = 0; c < cols_; ++c) {
            arr_[r][c] *= number;
        }
    }
    return *this;
}

Matrix Matrix::operator+(const Matrix &a) const {
    Matrix mTmp = *this;
    return mTmp += a;
}

Matrix Matrix::operator-(const Matrix &a) const {
    Matrix mTmp = *this;
    return mTmp -= a;
}

Matrix Matrix::operator*(const Matrix &a) const {
    Matrix mTmp = *this;
    return mTmp *= a;
}
Matrix task::operator*(double a, const Matrix &b) { return b * a; }
Matrix Matrix::operator*(const double &a) const {
    Matrix mTmp = *this;
    return mTmp *= a;
}

Matrix Matrix::operator-() const {
    Matrix mTmp = *this;
    for (size_t r = 0; r < mTmp.rows_; ++r) {
        for (size_t c = 0; c < mTmp.cols_; ++c) {
            mTmp.arr_[r][c] = -mTmp.arr_[r][c];
        }
    }
    return mTmp;
}

Matrix Matrix::operator+() const {
    Matrix mTmp = *this;
    return mTmp;
}

void Matrix::transpose() {
    size_t newRows = cols_;
    size_t newCols = rows_;
    auto newArr = new double *[newRows];
    for (size_t r = 0; r < newRows; ++r) {
        newArr[r] = new double[newCols];
        for (size_t c = 0; c < newCols; c++) {
            newArr[r][c] = arr_[c][r];
        }
    }

    for (size_t r = 0; r < rows_; ++r) {
        delete[] arr_[r];
    }
    delete arr_;
    arr_ = newArr;
    cols_ = newCols;
    rows_ = newRows;
}

Matrix Matrix::transposed() const {
    Matrix mTmp = *this;
    mTmp.transpose();
    return mTmp;
}
Matrix Matrix::minor(size_t row, size_t col) const {
    if (cols_ < 2 || rows_ < 2) {
        throw SizeMismatchException();
    }
    if (row >= rows_ || col >= cols_) {
        throw OutOfBoundsException();
    }

    Matrix m(rows_ - 1, cols_ - 1);
    size_t minorRow = 0;
    for (size_t r = 0; r < rows_; ++r) {
        if (r != row) {
            size_t minorCol = 0;
            for (size_t c = 0; c < cols_; c++) {
                if (c != col) {
                    m.arr_[minorRow][minorCol] = arr_[r][c];
                    minorCol++;
                }
            }
            minorRow++;
        }
    }
    return m;
}

double Matrix::det() const {
    if (cols_ != rows_) {
        throw SizeMismatchException();
    }
    if (cols_ == 1) {
        return arr_[0][0];
    } else if (cols_ == 2) {
        return arr_[0][0] * arr_[1][1] - arr_[1][0] * arr_[0][1];
    } else {

        double matrixDet = 0;
        for (size_t i = 0; i < cols_; ++i) {
            auto m = this->minor(0, i);
            auto d = arr_[0][i] * m.det();
            if (i % 2) {

                matrixDet -= d;
            } else {
                matrixDet += d;
            }
        }
        return matrixDet;
    }
}

double Matrix::trace() const {
    if (cols_ != rows_) {
        throw SizeMismatchException();
    }
    double mTrace = 0;
    for (size_t i = 0; i < cols_; ++i) {
        mTrace += arr_[i][i];
    }
    return mTrace;
}
