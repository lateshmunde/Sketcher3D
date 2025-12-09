#pragma once
#include "Exports.h"
#include "Matrix.h"
#include "Shape.h"

class GEOMETRYENGINE3D_API Transformations
{
public:
	Transformations();
	~Transformations();

	static Point calculatePivot(const std::vector<Point>& vertices); //centroid calculation
	static std::vector<Point> applyTransform(std::vector<Point>& vertices, Matrix& matrix);
	
	static std::vector<float> translate(std::vector<float>& vec, double transX = 0, double transY = 0, double transZ = 0);
	static std::vector<float> scale(std::vector<float>& vec, double scaleX = 1, double scaleY = 1, double scaleZ = 1);
	static std::vector<float> rotationX(std::vector<float>& vec, double degreeX = 0);
	static std::vector<float> rotationY(std::vector<float>& vec, double degreeY = 0);
	static std::vector<float> rotationZ(std::vector<float>& vec, double degreeZ = 0);
};