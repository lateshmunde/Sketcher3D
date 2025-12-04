#include "stdafx.h"
#include <cmath>
#include "Transformations.h"
#include "Constants.h"



Transformations::Transformations(){}

Transformations::~Transformations(){}

Matrix Transformations::translate(double transX, double transY, double transZ)
{
    Matrix transMat = Matrix::getIdentity();
    transMat(0, 3) = transX;
    transMat(1, 3) = transY;
    transMat(2, 3) = transZ;
    return transMat;
}



Matrix Transformations::scale(double scaleX, double scaleY, double scaleZ)
{
    Matrix transToOrigin = translate(0,0,0);

    Matrix scaleMat(4, 4);
    scaleMat = Matrix::getIdentity();
    scaleMat(0, 0) = scaleX;
    scaleMat(1, 1) = scaleY;
    scaleMat(2, 2) = scaleZ;
    return scaleMat* transToOrigin;
}

Matrix Transformations::rotationX(double degreeX)  {
    Matrix transToOrigin = translate(0, 0, 0);

    Matrix rotXMat(4, 4);
    rotXMat = Matrix::getIdentity();
    double radAngX = degreeX * MathConstants::PI / 180.0;
    double cosX = cos(radAngX);
    double sinX = sin(radAngX);

    rotXMat(1, 1) = cosX;  rotXMat(1, 2) = -sinX;
    rotXMat(2, 1) = sinX;  rotXMat(2, 2) = cosX;
    return rotXMat* transToOrigin;
}

Matrix Transformations::rotationY(double degreeY) {
    Matrix transToOrigin = translate(0, 0, 0);

    Matrix rotYMat(4, 4);
    rotYMat = Matrix::getIdentity();
    double radAngY = degreeY * MathConstants::PI / 180.0;
    double cosY = cos(radAngY);
    double sinY = sin(radAngY);

    rotYMat(0, 0) = cosY;  rotYMat(0, 2) = sinY;
    rotYMat(2, 0) = -sinY;  rotYMat(2, 2) = cosY;
    return rotYMat * transToOrigin;
}


Matrix Transformations::rotationZ(double degreeZ) {
    Matrix transToOrigin = translate(0, 0, 0);

    Matrix rotZMat(4, 4);
    rotZMat = Matrix::getIdentity();
    double radAngZ = degreeZ * MathConstants::PI / 180.0;
    double cosZ = cos(radAngZ);
    double sinZ = sin(radAngZ);

    rotZMat(0, 0) = cosZ;  rotZMat(0, 1) = -sinZ;
    rotZMat(1, 0) = sinZ;  rotZMat(1, 1) = cosZ;
    return rotZMat * transToOrigin;
}

Matrix Transformations::getMatrix()  
{
    Matrix result = Matrix::getIdentity();

    Matrix trans = translate(7,7,7);
    Matrix scaleM = scale();
    Matrix rotX = rotationX();
    Matrix rotY = rotationY();
    Matrix rotZ = rotationZ();
   
    return rotZ * rotY * rotX * scaleM * trans;
}


std::vector<Point> Transformations::getPtMatrix(const std::vector<Point>& vec)
{
    std::vector<Point> transformedPts;

    Matrix M = getMatrix(); // all transformations 4*4 matrix
   
    for (const Point& p : vec)
    {
         Matrix pt(4, 1);
        pt(0, 0) = p.getX();
        pt(1, 0) = p.getY();
        pt(2, 0) = p.getZ();
        pt(3, 0) = 1.0;

        Matrix  result = M * pt;

        double fac = result(3, 0);
        double X;
        double Y;
        double Z;

        if (fac != 0)
        {
            X = result(0, 0) / fac;
            Y = result(1, 0) / fac;
            Z = result(2, 0) / fac;
        }
        transformedPts.emplace_back(X, Y, Z);
    }
    return transformedPts;
}


