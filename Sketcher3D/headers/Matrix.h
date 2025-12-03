#pragma once
#include <vector>


class Matrix {
private:
    int rows;
    int cols;
    std::vector<std::vector<double>> data;
    //static Matrix identity;

public:
    Matrix(int row = 4, int col = 4);
    ~Matrix();

    static Matrix getIdentity();
    double& operator()(int row, int col);
    Matrix operator+(const Matrix& other);
    Matrix operator*(const Matrix& other);

};
