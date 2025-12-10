#pragma once
#include <cmath>

class Mat4
{
public:
    float m[16]; // column-major (OpenGL style)

    Mat4();

    void setIdentity();

    // Basic transforms
    void translate(float tx, float ty, float tz);
    void scale(float sx, float sy, float sz);
    void rotate(float angleDeg, float x, float y, float z);

    // Matrix multiplication
    Mat4 operator*(const Mat4& other) const;

    // Apply to vector
    void transformPoint(float& x, float& y, float& z) const;
};