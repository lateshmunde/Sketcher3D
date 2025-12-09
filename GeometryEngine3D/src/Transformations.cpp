#include "pch.h"
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

std::vector<float> Transformations::translate(std::vector<float>& vec, double transX, double transY, double transZ)
{
    std::vector<Point> vertices;

    for (int i = 0; i < vec.size(); i += 3)
    {
        vertices.emplace_back(vec[i], vec[i + 1], vec[i + 2]);
    }

    Matrix transMat = Matrix::getTranslationMatrix(transX, transY, transZ);
    std::vector<Point> transformedPts = applyTransform(vertices, transMat);

    for (int i = 0; i < transformedPts.size(); i++)
    {
        vec.push_back(float(transformedPts[i].getX()));
        vec.push_back(float(transformedPts[i].getY()));
        vec.push_back(float(transformedPts[i].getZ()));
    }

    return vec;
}

std::vector<float> Transformations::scale(std::vector<float>& vec, double scaleX, double scaleY, double scaleZ)
{
    std::vector<Point> vertices;

    for (int i = 0; i < vec.size(); i += 3)
    {
        vertices.emplace_back(vec[i], vec[i + 1], vec[i + 2]);
    }

    Point pivot = calculatePivot(vertices); //Centroid

    Matrix translate1 = Matrix::getTranslationMatrix(-pivot.getX(), -pivot.getY(), -pivot.getY());
    Matrix scaleMat = Matrix::getScalingMatrix(scaleX, scaleY, scaleZ);
    Matrix translate2 = Matrix::getTranslationMatrix(pivot.getX(), pivot.getY(), pivot.getY());

    Matrix transformed = translate2 * scaleMat * translate1;
    std::vector<Point> transformedPts = applyTransform(vertices, transformed);

    for (int i = 0; i < transformedPts.size(); i++)
    {
        vec.push_back(float(transformedPts[i].getX()));
        vec.push_back(float(transformedPts[i].getY()));
        vec.push_back(float(transformedPts[i].getZ()));
    }

    return vec;
}

std::vector<float> Transformations::rotationX(std::vector<float>& vec, double degreeX)
{
    std::vector<Point> vertices;

    for (int i = 0; i < vec.size(); i += 3)
    {
        vertices.emplace_back(vec[i], vec[i + 1], vec[i + 2]);
    }

    Point pivot = calculatePivot(vertices); //Centroid

    Matrix translate1 = Matrix::getTranslationMatrix(-pivot.getX(), -pivot.getY(), -pivot.getY());
    Matrix rotateXMat = Matrix::getRotationXMatrix(degreeX);
    Matrix translate2 = Matrix::getTranslationMatrix(pivot.getX(), pivot.getY(), pivot.getY());

    Matrix transformed = translate2 * rotateXMat * translate1;
    std::vector<Point> transformedPts = applyTransform(vertices, transformed);

    for (int i = 0; i < transformedPts.size(); i++)
    {
        vec.push_back(float(transformedPts[i].getX()));
        vec.push_back(float(transformedPts[i].getY()));
        vec.push_back(float(transformedPts[i].getZ()));
    }

    return vec;
}

std::vector<float> Transformations::rotationY(std::vector<float>& vec, double degreeY)
{
    std::vector<Point> vertices;

    for (int i = 0; i < vec.size(); i += 3)
    {
        vertices.emplace_back(vec[i], vec[i + 1], vec[i + 2]);
    }

    Point pivot = calculatePivot(vertices); //Centroid

    Matrix translate1 = Matrix::getTranslationMatrix(-pivot.getX(), -pivot.getY(), -pivot.getY());
    Matrix rotateYMat = Matrix::getRotationYMatrix(degreeY);
    Matrix translate2 = Matrix::getTranslationMatrix(pivot.getX(), pivot.getY(), pivot.getY());

    Matrix transformed = translate2 * rotateYMat * translate1;
    std::vector<Point> transformedPts = applyTransform(vertices, transformed);

    for (int i = 0; i < transformedPts.size(); i++)
    {
        vec.push_back(float(transformedPts[i].getX()));
        vec.push_back(float(transformedPts[i].getY()));
        vec.push_back(float(transformedPts[i].getZ()));
    }

    return vec;
}

std::vector<float> Transformations::rotationZ(std::vector<float>& vec, double degreeZ)
{
    std::vector<Point> vertices;

    for (int i = 0; i < vec.size(); i += 3)
    {
        vertices.emplace_back(vec[i], vec[i + 1], vec[i + 2]);
    }

    Point pivot = calculatePivot(vertices); //Centroid

    Matrix translate1 = Matrix::getTranslationMatrix(-pivot.getX(), -pivot.getY(), -pivot.getY());
    Matrix rotateZMat = Matrix::getRotationZMatrix(degreeZ);
    Matrix translate2 = Matrix::getTranslationMatrix(pivot.getX(), pivot.getY(), pivot.getY());

    Matrix transformed = translate2 * rotateZMat * translate1;
    std::vector<Point> transformedPts = applyTransform(vertices, transformed);

    for (int i = 0; i < transformedPts.size(); i++)
    {
        vec.push_back(float(transformedPts[i].getX()));
        vec.push_back(float(transformedPts[i].getY()));
        vec.push_back(float(transformedPts[i].getZ()));
    }

    return vec;
}
