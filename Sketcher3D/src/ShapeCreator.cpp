#include "ShapeCreator.h"

//reader() : split string by spaces, coverts it into vector
//std::vector<std::string> ShapeCreator::reader(const std::string& line)
//{
//	std::stringstream ss(line);
//	std::string str;
//	std::vector<std::string> strVec;
//
//	while (ss >> str)
//		strVec.push_back(str);
//
//	return strVec;
//}


// toDouble(): convert string to double
bool ShapeCreator::toDouble(const std::string& s, double& d)
{
	try {
		d = std::stod(s);
		return true;
	}
	catch (...) {
		return false;
	}
}


Cuboid ShapeCreator::createCuboid(const std::string& name, double length, double width, double height)
{
	if (length <= 0 || width <= 0 || height <= 0)
		std::cout << "Invalid input for Cuboid";

	return Cuboid(name, length, width, height);
}

Cube ShapeCreator::createCube(const std::string& name, double side )
{
	if (side <= 0)
		std::cout << "Invalid input for Cube";

	return Cube(name, side);
}


Sphere ShapeCreator::createSphere(const std::string& name, double radius)
{
	if (radius <= 0 )
		std::cout << "Invalid input for Sphere";

	return Sphere(name, radius);
}

Cylinder ShapeCreator::createCylinder(const std::string& name, double radius, double height)
{
	if (radius = 0 || height <= 0 )
		std::cout << "Invalid input for Cylinder";

	return Cylinder(name, radius, height);
}

Cone ShapeCreator::createCone(const std::string& name, double radius, double height)
{
	if (radius <= 0 || height <= 0)
		std::cout << "Invalid input for Cone";

	return Cone(name, radius, height);
}

Pyramid ShapeCreator::createPyramid(const std::string& name, double baseLength, double baseWidth, double height)
{
	if (baseLength <= 0 || baseWidth <= 0 || height <= 0) // write fn to check poistive number
		std::cout << "Invalid input for Pyramid";

	return Pyramid(name, baseLength, baseWidth, height);
}

//// createFromString(): used when loading from file
//std::unique_ptr<Shape> ShapeCreator::createFromString(const std::string& line)
//{
//	std::vector<std::string> params = reader(line);
//	if (params.size() < 1)
//		return nullptr;
//
//	std::string type = params[0];
//
//	if (type == "Cuboid")
//	{
//		if (params.size() != 7) // Cuboid x y z L W H
//			return nullptr;
//
//		double x, y, z, L, W, H;
//		if (!toDouble(params[1], x) ||
//			!toDouble(params[2], y) ||
//			!toDouble(params[3], z) ||
//			!toDouble(params[4], L) ||
//			!toDouble(params[5], W) ||
//			!toDouble(params[6], H))
//			return nullptr;
//
//		return createCuboid( L, W, H, Point(x, y, z));
//	}
//
//	if (type == "Cube")
//	{
//		if (params.size() != 5) // Sphere x y z L
//			return nullptr;
//
//		double x, y, z, L;
//		if (!toDouble(params[1], x) ||
//			!toDouble(params[2], y) ||
//			!toDouble(params[3], z) ||
//			!toDouble(params[4], L))
//			return nullptr;
//
//		return createCube(L, Point(x, y, z));
//	}
//
//	if (type == "Sphere")
//	{
//		if (params.size() != 5) // Sphere x y z R
//			return nullptr;
//
//		double x, y, z, R;
//		if (!toDouble(params[1], x) ||
//			!toDouble(params[2], y) ||
//			!toDouble(params[3], z) ||
//			!toDouble(params[4], R))
//			return nullptr;
//
//		return createSphere( R, Point(x, y, z));
//	}
//
//	if (type == "Cylinder")
//	{
//		if (params.size() != 6) // Cylinder x y z R H
//			return nullptr;
//
//		double x, y, z, R, H;
//		if (!toDouble(params[1], x) ||
//			!toDouble(params[2], y) ||
//			!toDouble(params[3], z) ||
//			!toDouble(params[4], R) ||
//			!toDouble(params[5], H))
//			return nullptr;
//
//		return createCylinder( R, H, Point(x, y, z));
//	}
//
//	if (type == "Cone")
//	{
//		if (params.size() != 6) // Cone x y z R H
//			return nullptr;
//
//		double x, y, z, R, H;
//		if (!toDouble(params[1], x) ||
//			!toDouble(params[2], y) ||
//			!toDouble(params[3], z) ||
//			!toDouble(params[4], R) ||
//			!toDouble(params[5], H))
//			return nullptr;
//
//		return createCone(R, H, Point(x, y, z));
//	}
//
//	if (type == "Pyramid")
//	{
//		if (params.size() != 7) // Pyramid x y z BL BW H
//			return nullptr;
//
//		double x, y, z, BL, BW, H;
//		if (!toDouble(params[1], x) ||
//			!toDouble(params[2], y) ||
//			!toDouble(params[3], z) ||
//			!toDouble(params[4], BL) ||
//			!toDouble(params[5], BW) ||
//			!toDouble(params[6], H))
//			return nullptr;
//
//		return createPyramid(BL, BW, H, Point(x, y, z));
//	}
//
//
//	return nullptr; // Unknown shape type
//}
//	 
//

