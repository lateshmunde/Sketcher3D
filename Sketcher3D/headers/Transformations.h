#pragma once
#include "Matrix.h"
//#include "Point.h"
#include "Shape.h"

class Transformations
{
public:
	Transformations();
	~Transformations();

	static Point calculatePivot(const std::vector<Point>& vertices); //centroid calculation
	void applyTransform(const std::vector<Point>& vertices, Matrix& matrix) const ;
	
	static Matrix translate(double transX = 0, double transY = 0, double transZ = 0);
	static Matrix scale(double scaleX = 1, double scaleY = 1, double scaleZ = 1);
	static Matrix rotationX(double degreeX = 0);
	static Matrix rotationY(double degreeY = 0);
	static Matrix rotationZ(double degreeZ = 0);
	static std::vector<Point> getPtMatrix(const std::vector<Point>& vec);
	static Matrix getMatrix();
};