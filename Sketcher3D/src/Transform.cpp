#include "Transform.h"

Point Transform::calculatePivot(const std::vector<Point>& vertices)
{
    double cx = 0, cy = 0, cz = 0;

    for (const auto& p : vertices)
    {
        cx += p.getX();
        cy += p.getY();
        cz += p.getZ();
    }

    cx /= vertices.size();
    cy /= vertices.size();
    cz /= vertices.size();

    return Point(cx, cy, cz);
}

void Transform::ApplyTransform(std::vector<Point>& vertices, Matrix4D& matrix)
{
    for (Point& p : vertices)
    {
        std::vector<std::vector<double>> point =
        {
            {p.getX()},
            {p.getY()},
            {p.getZ()},
            {1.0}
        };

        std::vector<std::vector<double>> result = matrix * point;

        p.setX(result[0][0]);
        p.setY(result[1][0]);
        p.setZ(result[2][0]);
    }
}

void Transform::Translate(std::vector<Point>& vertices, double tx, double ty, double tz)
{
    Matrix4D T = Matrix4D::Translation(tx, ty, tz);
    ApplyTransform(vertices, T);
}

void Transform::Scale(std::vector<Point>& vertices, double sx, double sy, double sz)
{
    Point pivot = calculatePivot(vertices);

    Matrix4D T1 = Matrix4D::Translation(-pivot.getX(), -pivot.getY(), -pivot.getZ());
    Matrix4D S = Matrix4D::Scaling(sx, sy, sz);
    Matrix4D T2 = Matrix4D::Translation(pivot.getX(), pivot.getY(), pivot.getZ());

    Matrix4D final = T2 * S * T1;

    ApplyTransform(vertices, final);
}

void Transform::RotateX(std::vector<Point>& vertices, double angle)
{
    Point pivot = calculatePivot(vertices);

    Matrix4D T1 = Matrix4D::Translation(-pivot.getX(), -pivot.getY(), -pivot.getZ());
    Matrix4D R = Matrix4D::RotationX(angle);
    Matrix4D T2 = Matrix4D::Translation(pivot.getX(), pivot.getY(), pivot.getZ());

    Matrix4D final = T2 * R * T1;

    ApplyTransform(vertices, final);
}

void Transform::RotateY(std::vector<Point>& vertices, double angle)
{
    Point pivot = calculatePivot(vertices);

    Matrix4D T1 = Matrix4D::Translation(-pivot.getX(), -pivot.getY(), -pivot.getZ());
    Matrix4D R = Matrix4D::RotationY(angle);
    Matrix4D T2 = Matrix4D::Translation(pivot.getX(), pivot.getY(), pivot.getZ());

    Matrix4D final = T2 * R * T1;

    ApplyTransform(vertices, final);
}

void Transform::RotateZ(std::vector<Point>& vertices, double angle)
{
    Point pivot = calculatePivot(vertices);

    Matrix4D T1 = Matrix4D::Translation(-pivot.getX(), -pivot.getY(), -pivot.getZ());
    Matrix4D R = Matrix4D::RotationZ(angle);
    Matrix4D T2 = Matrix4D::Translation(pivot.getX(), pivot.getY(), pivot.getZ());

    Matrix4D final = T2 * R * T1;

    ApplyTransform(vertices, final);
}
