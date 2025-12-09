#include "pch.h"
#include "Matrix4D.h"

Matrix4D::Matrix4D()
{
    Mat4 = std::vector<std::vector<double>>(4, std::vector<double>(4, 0.0));
}

std::vector<std::vector<double>>
Matrix4D::operator*(std::vector<std::vector<double>>& other)
{
    int rowsA = Mat4.size();        // 4
    int colsA = Mat4[0].size();     // 4
    int rowsB = other.size();       // 4
    int colsB = other[0].size();    // 1

    std::vector<std::vector<double>> result(rowsA,
        std::vector<double>(colsB, 0.0));

    for (int i = 0; i < rowsA; ++i)
        for (int j = 0; j < colsB; ++j) {
            result[i][j] = 0;
            for (int k = 0; k < colsA; ++k)
                result[i][j] += Mat4[i][k] * other[k][j];
        }
           

    return result;
}

Matrix4D Matrix4D::operator*(const Matrix4D& other)
{
    Matrix4D result;

    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            result.Mat4[i][j] = 0.0;
            for (int k = 0; k < 4; ++k)
                result.Mat4[i][j] += Mat4[i][k] * other.Mat4[k][j];
        }
    }

    return result;
}

Matrix4D Matrix4D::getTranslationMatrix(double tx, double ty, double tz)
{
    Matrix4D m;
    m.Mat4 =
    {
        {1, 0, 0, tx},
        {0, 1, 0, ty},
        {0, 0, 1, tz},
        {0, 0, 0, 1}
    };
    return m;
}

Matrix4D Matrix4D::getScalingMatrix(double sx, double sy, double sz)
{
    Matrix4D m;
    m.Mat4 =
    {
        {sx, 0,  0,  0},
        {0,  sy, 0,  0},
        {0,  0,  sz, 0},
        {0,  0,  0,  1}
    };
    return m;
}

Matrix4D Matrix4D::getRotationXMatrix(double angle)
{
    Matrix4D m;
    double c = cos(angle);
    double s = sin(angle);

    m.Mat4 =
    {
        {1,  0,  0, 0},
        {0,  c, -s, 0},
        {0,  s,  c, 0},
        {0,  0,  0, 1}
    };

    return m;
}

Matrix4D Matrix4D::getRotationYMatrix(double angle)
{
    Matrix4D m;
    double c = cos(angle);
    double s = sin(angle);

    m.Mat4 =
    {
        { c, 0, s, 0},
        { 0, 1, 0, 0},
        {-s, 0, c, 0},
        { 0, 0, 0, 1}
    };

    return m;
}

Matrix4D Matrix4D::getRotationZMatrix(double angle)
{
    Matrix4D m;
    double c = cos(angle);
    double s = sin(angle);

    m.Mat4 =
    {
        { c, -s, 0, 0},
        { s,  c, 0, 0},
        { 0,  0, 1, 0},
        { 0,  0, 0, 1}
    };

    return m;
}
