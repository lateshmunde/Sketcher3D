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

    static void Translate(std::vector<Point>& vertices, double tx = 0, double ty = 0, double tz = 0);
    static void Scale(std::vector<Point>& vertices, double sx = 1, double sy = 1, double sz = 1);
    static void RotateX(std::vector<Point>& vertices, double angle = 0);
    static void RotateY(std::vector<Point>& vertices, double angle = 0);
    static void RotateZ(std::vector<Point>& vertices, double angle = 0);
};
