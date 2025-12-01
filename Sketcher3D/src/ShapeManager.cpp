#include "ShapeManager.h"

void ShapeManager::addShape(std::shared_ptr<Shape> shapes)
{
	//std::shared_ptr<Shape> s1 = std::make_shared<Cuboid>(c1);
	
	mShapes.push_back(shapes);
}

const std::vector<std::shared_ptr<Shape>>& ShapeManager::getShapesVec() const
{
	return mShapes;
}


void ShapeManager::deleteShape(const int index)
{
	if ((index >= 0) && (index < mShapes.size()))
	{
		std::cout << mShapes[index];
		mShapes.erase(mShapes.begin() + index);
		std::cout << "Shape at index " << index << " is deleted.\n";
	}
	else
		std::cout << "Shape at index " << index << " is not deleted.\n";
}