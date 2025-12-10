#include "Mat4.h"

Mat4::Mat4()
{
    setIdentity();
}

void Mat4::setIdentity()
{
    for (int i = 0; i < 16; i++)
        m[i] = 0.0f;

    m[0] = m[5] = m[10] = m[15] = 1.0f;
}

Mat4 Mat4::operator*(const Mat4& o) const
{
    Mat4 r;

    for (int row = 0; row < 4; row++)
    {
        for (int col = 0; col < 4; col++)
        {
            r.m[col * 4 + row] =
                m[0 * 4 + row] * o.m[col * 4 + 0] +
                m[1 * 4 + row] * o.m[col * 4 + 1] +
                m[2 * 4 + row] * o.m[col * 4 + 2] +
                m[3 * 4 + row] * o.m[col * 4 + 3];
        }
    }

    return r;
}

void Mat4::translate(float tx, float ty, float tz)
{
    Mat4 t;
    t.m[12] = tx;
    t.m[13] = ty;
    t.m[14] = tz;
    *this = (*this) * t;
}

void Mat4::scale(float sx, float sy, float sz)
{
    Mat4 s;
    s.m[0] = sx;
    s.m[5] = sy;
    s.m[10] = sz;
    *this = (*this) * s;
}

void Mat4::rotate(float angleDeg, float x, float y, float z)
{
    float rad = angleDeg * 3.1415926535f / 180.0f;

    float c = std::cos(rad);
    float s = std::sin(rad);

    float len = std::sqrt(x * x + y * y + z * z);
    if (len == 0) return;
    x /= len; y /= len; z /= len;

    Mat4 r;

    r.m[0] = x * x * (1 - c) + c;
    r.m[1] = y * x * (1 - c) + z * s;
    r.m[2] = x * z * (1 - c) - y * s;

    r.m[4] = x * y * (1 - c) - z * s;
    r.m[5] = y * y * (1 - c) + c;
    r.m[6] = y * z * (1 - c) + x * s;

    r.m[8] = x * z * (1 - c) + y * s;
    r.m[9] = y * z * (1 - c) - x * s;
    r.m[10] = z * z * (1 - c) + c;

    *this = (*this) * r;
}

void Mat4::transformPoint(float& x, float& y, float& z) const
{
    float nx = m[0] * x + m[4] * y + m[8] * z + m[12];
    float ny = m[1] * x + m[5] * y + m[9] * z + m[13];
    float nz = m[2] * x + m[6] * y + m[10] * z + m[14];

    x = nx;
    y = ny;
    z = nz;
}
