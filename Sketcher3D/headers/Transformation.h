#pragma once
#include <vector>
#include "Point.h"
class Transformation {
public:
	std::vector<std::vector<double>> TransformationMatrix;
	Transformation();
	static Point applyTranslation(Point p);
};