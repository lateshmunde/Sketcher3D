#include "stdafx.h"
#include <cmath>
#include "Transformations.h"
#include "Constants.h"



Transformations::Transformations(){}

Transformations::~Transformations(){}

Point Transformations::calculatePivot(const std::vector<Point>& vertices)
{
    double cx = 0;
    double cy = 0;
    double cz = 0;

   /* for (const auto& p : vertices)
    {*/
        /*cx += p.getX();
        cy += p.getY();
        cz += p.getZ();*/
    //}
   /* int size = vertices.size();
    cx /= size;
    cy /= size;
    cz /= size;*/

    return Point(cx, cy, cz);
}

//void Transformations::applyTransform(const std::vector<Point>& vertices, Matrix& matrix) const
std::vector<Point> Transformations::applyTransform(std::vector<Point>& vertices, Matrix& matrix)
{
    std::vector<Point> transformedPts;

    for (Point& p : vertices)
    {
        Matrix pt(4, 1);
        pt(0, 0) = p.getX();
        pt(1, 0) = p.getY();
        pt(2, 0) = p.getZ();
        pt(3, 0) = 1.0;

        Matrix  result = matrix * pt;

       /* p.setX(result(0, 0));
        p.setY(result(1, 0));
        p.setZ(result(2, 0));*/

        double X = result(0, 0);
        double Y = result(1, 0);
        double Z = result(2, 0);
        transformedPts.emplace_back(X, Y, Z);
    }
    return transformedPts;
}

std::vector<Point> Transformations::translate(std::vector<Point>& vertices, double transX, double transY, double transZ)
{

    Matrix transMat = Matrix::getTranslationMatrix(transX, transY, transZ);
    std::vector<Point> transformedPts = applyTransform(vertices, transMat);
    return transformedPts;
}

std::vector<Point> Transformations::scale(std::vector<Point>& vertices, double scaleX, double scaleY, double scaleZ)
{
    Point pivot = calculatePivot(vertices); //Centroid

    Matrix translate1 = Matrix::getTranslationMatrix(-pivot.getX(), -pivot.getY(), -pivot.getY());
    Matrix scaleMat = Matrix::getScalingMatrix(scaleX, scaleY, scaleZ);
    Matrix translate2 = Matrix::getTranslationMatrix(pivot.getX(), pivot.getY(), pivot.getY());

    Matrix transformed = translate2 * scaleMat * translate1;
    std::vector<Point> transformedPts = applyTransform(vertices, transformed);

    return transformedPts;
}

std::vector<Point> Transformations::rotationX(std::vector<Point>& vertices, double degreeX)  {
    Point pivot = calculatePivot(vertices); //Centroid

    Matrix translate1 = Matrix::getTranslationMatrix(-pivot.getX(), -pivot.getY(), -pivot.getY());
    Matrix rotateXMat = Matrix::getRotationXMatrix(degreeX);
    Matrix translate2 = Matrix::getTranslationMatrix(pivot.getX(), pivot.getY(), pivot.getY());

    Matrix transformed = translate2 * rotateXMat * translate1;
    std::vector<Point> transformedPts = applyTransform(vertices, transformed);

    return transformedPts;
}

std::vector<Point> Transformations::rotationY(std::vector<Point>& vertices, double degreeY) {
    Point pivot = calculatePivot(vertices); //Centroid

    Matrix translate1 = Matrix::getTranslationMatrix(-pivot.getX(), -pivot.getY(), -pivot.getY());
    Matrix rotateYMat = Matrix::getRotationYMatrix(degreeY);
    Matrix translate2 = Matrix::getTranslationMatrix(pivot.getX(), pivot.getY(), pivot.getY());

    Matrix transformed = translate2 * rotateYMat * translate1;
    std::vector<Point> transformedPts = applyTransform(vertices, transformed);

    return transformedPts;
}

std::vector<Point> Transformations::rotationZ(std::vector<Point>& vertices, double degreeZ) {
    Point pivot = calculatePivot(vertices); //Centroid

    Matrix translate1 = Matrix::getTranslationMatrix(-pivot.getX(), -pivot.getY(), -pivot.getY());
    Matrix rotateZMat = Matrix::getRotationZMatrix(degreeZ);
    Matrix translate2 = Matrix::getTranslationMatrix(pivot.getX(), pivot.getY(), pivot.getY());

    Matrix transformed = translate2 * rotateZMat * translate1;
    std::vector<Point> transformedPts = applyTransform(vertices, transformed);
    return transformedPts;
}
