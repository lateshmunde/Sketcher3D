#pragma once
#include <vector>
#include <cmath>

class Matrix4D
{
public:
    std::vector<std::vector<double>> Mat4;

    Matrix4D();

    // 4x4 * 4x1 (point)
    std::vector<std::vector<double>> operator*(std::vector<std::vector<double>>& other);

    // 4x4 * 4x4 (matrix)
    Matrix4D operator*(const Matrix4D& other);

    // Static creators
    static Matrix4D Translation(double tx, double ty, double tz);
    static Matrix4D Scaling(double sx, double sy, double sz);
    static Matrix4D RotationX(double angle);
    static Matrix4D RotationY(double angle);
    static Matrix4D RotationZ(double angle);
};
