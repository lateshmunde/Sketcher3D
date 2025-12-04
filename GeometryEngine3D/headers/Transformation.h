#pragma once
#include "Exports.h"
#include <vector>
#include "Point.h"
#include "Matrix4D.h"

class GEOMETRYENGINE3D_API Transformation
{
public:
    static Point calculatePivot(const std::vector<Point>& vertices);

    static void ApplyTransform(std::vector<Point>& vertices, Matrix4D& matrix);

    static void Translate(std::vector<Point>& vertices, double tx, double ty, double tz);
    static void Scale(std::vector<Point>& vertices, double sx, double sy, double sz);
    static void RotateX(std::vector<Point>& vertices, double angle);
    static void RotateY(std::vector<Point>& vertices, double angle);
    static void RotateZ(std::vector<Point>& vertices, double angle);
};
