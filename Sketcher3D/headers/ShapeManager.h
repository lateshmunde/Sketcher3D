#pragma once
#include "Shape.h"

class ShapeManager
{
private:
	std::vector<std::shared_ptr<Shape>> mShapes;

public:
	ShapeManager() = default;

	void addShape(std::shared_ptr<Shape> shapes);
	const std::vector<std::shared_ptr<Shape>>& getShapesVec() const ;
	void deleteShape(const int index);
};