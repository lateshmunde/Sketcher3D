#pragma once
#include "Exports.h"
#include <vector>
#include "Constants.h"


class GEOMETRYENGINE3D_API Matrix {
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

    // Static creators
    static Matrix getTranslationMatrix(double tx, double ty, double tz);
    static Matrix getScalingMatrix(double sx, double sy, double sz);
    static Matrix getRotationXMatrix(double angle);
    static Matrix getRotationYMatrix(double angle);
    static Matrix getRotationZMatrix(double angle);
};
