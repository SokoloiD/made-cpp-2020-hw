#pragma once

#include <iostream>
#include <vector>

namespace task {

    const double EPS = 1e-6;

    class Row;
    class Matrix;

    class OutOfBoundsException : public std::exception {};
    class SizeMismatchException : public std::exception {};
    std::ostream &operator<<(std::ostream &output, const Matrix &matrix);
    std::istream &operator>>(std::istream &input, Matrix &matrix);

    class Matrix {

    public:
        Matrix();
        Matrix(size_t rows, size_t cols);
        Matrix(const Matrix &copy);
        ~Matrix();
        Matrix &operator=(const Matrix &a);

        double &get(size_t row, size_t col);
        [[nodiscard]] const double &get(size_t row, size_t col) const;
        void set(size_t row, size_t col, const double &value);
        void resize(size_t new_rows, size_t new_cols);

        Row operator[](size_t row);
        const Row operator[](size_t row) const;


        Matrix &operator+=(const Matrix &a);
        Matrix &operator-=(const Matrix &a);
        Matrix &operator*=(const Matrix &a);
        Matrix &operator*=(const double &number);

        Matrix operator+(const Matrix &a) const;
        Matrix operator-(const Matrix &a) const;
        Matrix operator*(const Matrix &a) const;
        Matrix operator*(const double &a) const;

        Matrix operator-() const;
        Matrix operator+() const;

        [[nodiscard]] double det() const;
        void transpose();
        [[nodiscard]] Matrix transposed() const;
        [[nodiscard]] double trace() const;

        [[nodiscard]] Matrix minor(size_t row, size_t col) const;

        std::vector<double> getRow(size_t row);
        std::vector<double> getColumn(size_t column);

        bool operator==(const Matrix &a) const;
        bool operator!=(const Matrix &a) const;


        friend std::ostream &operator<<(std::ostream &output, const Matrix &matrix);
        friend std::istream &operator>>(std::istream &input, Matrix &matrix);

        friend class Row;

    private:
        size_t rows_;
        size_t cols_;
        double **arr_;
    };

    Matrix operator*(double a, const Matrix &b);

    class Row {
    public:
        Row(const Matrix &matrix, size_t row);
        double &operator[](size_t col);
        double operator[](size_t col) const;

    private:
        size_t size_;
        double *arr_;
    };

}// namespace task
