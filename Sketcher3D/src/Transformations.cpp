#include "stdafx.h"
#include <cmath>
#include "Transformations.h"
#include "Constants.h"



Transformations::Transformations(){}

Transformations::~Transformations(){}

Matrix Transformations::translate(double transX = 0, double transY = 0, double transZ = 0)
{
    Matrix transMat = Matrix::getIdentity();
    transMat(0, 3) = transX;
    transMat(1, 3) = transY;
    transMat(2, 3) = transZ;
    return transMat;
}



Matrix Transformations::scale(double scaleFac = 0)
{
    Matrix transToOrigin = translate(0,0,0);

    Matrix scaleMat(4, 4);
    scaleMat = Matrix::getIdentity();
    scaleMat(0, 0) = scaleFac;
    scaleMat(1, 1) = scaleFac;
    scaleMat(2, 2) = scaleFac;
    return scaleMat* transToOrigin;
}

Matrix Transformations::rotationX(double degreeX = 0)  {
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

Matrix Transformations::rotationY(double degreeY = 0) {
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


Matrix Transformations::rotationZ(double degreeZ = 0) {
    Matrix transToOrigin = translate(0, 0, 0);

    Matrix rotZMat(4, 4);
    rotZMat = Matrix::getIdentity();
    double radAngZ = degreeZ * MathConstants::PI / 180.0;
    double cosZ = cos(radAngZ);
    double sinZ = sin(radAngZ);

    rotZMat(0, 0) = cosZ;  rotZMat(0, 2) = -sinZ;
    rotZMat(2, 0) = sinZ;  rotZMat(2, 2) = cosZ;
    return rotZMat * transToOrigin;
}

Matrix Transformations::getMatrix()  
{
    Matrix result = Matrix::getIdentity();

    Matrix trans = translate();
    Matrix scaleM = scale();
    Matrix rotX = rotationX();
    Matrix rotY = rotationY();
    Matrix rotZ = rotationZ();
   
    return rotZ * rotY * rotX * scaleM * trans;
}


Point Transformations::getPtMatrix(const Point& p)
{
    Matrix pt(4, 1);
    pt(0, 1) = p.getX();
    pt(1, 1) = p.getY();
    pt(2, 1) = p.getZ();
    pt(3, 1) = 1.0;

    Matrix result = getMatrix();
    result = result * pt;

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
    return Point(X, Y, Z);
}
