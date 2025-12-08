#pragma once
#include "Exports.h"
#include <vector>
#include <cmath>

class GEOMETRYENGINE3D_API Matrix4D
{
public:
    std::vector<std::vector<double>> Mat4;

    Matrix4D();

    // 4x4 * 4x1 (point)
    std::vector<std::vector<double>> operator*(std::vector<std::vector<double>>& other);

    // 4x4 * 4x4 (matrix)
    Matrix4D operator*(const Matrix4D& other);

    // Static creators
    static Matrix4D getTranslationMatrix(double tx, double ty, double tz);
    static Matrix4D getScalingMatrix(double sx, double sy, double sz);
    static Matrix4D getRotationXMatrix(double angle);
    static Matrix4D getRotationYMatrix(double angle);
    static Matrix4D getRotationZMatrix(double angle);
};
