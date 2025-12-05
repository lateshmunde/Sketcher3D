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
	static std::vector<Point> applyTransform(std::vector<Point>& vertices, Matrix& matrix);
	
	static std::vector<Point> translate(std::vector<Point>& vertices, double transX = 0, double transY = 0, double transZ = 0);
	static std::vector<Point> scale(std::vector<Point>& vertices, double scaleX = 1, double scaleY = 1, double scaleZ = 1);
	static std::vector<Point> rotationX(std::vector<Point>& vertices, double degreeX = 0);
	static std::vector<Point> rotationY(std::vector<Point>& vertices, double degreeY = 0);
	static std::vector<Point> rotationZ(std::vector<Point>& vertices, double degreeZ = 0);
};