#include "ShapeCreator.h"

//reader() : split string by spaces, coverts it into vector
std::vector<std::string> ShapeCreator::reader(const std::string& line)
{
	std::stringstream ss(line);
	std::string str;
	std::vector<std::string> strVec;

	while (ss >> str)
		strVec.push_back(str);

	return strVec;
}


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

Cube ShapeCreator::createCube(const std::string& name, double side)
{
	if (side <= 0)
		std::cout << "Invalid input for Cube";

	return Cube(name, side);
}

Sphere ShapeCreator::createSphere(const std::string& name, double radius)
{
	if (radius <= 0)
		std::cout << "Invalid input for Sphere";

	return Sphere(name, radius);
}

Cylinder ShapeCreator::createCylinder(const std::string& name, double radius, double height)
{
	if (radius = 0 || height <= 0)
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

// createFromString(): used when loading from file
std::unique_ptr<Shape> ShapeCreator::createFromString(const std::string& line)
{
	std::vector<std::string> params = reader(line);
	if (params.size() < 1)
		return nullptr;

	std::string type = params[0];

	if (type == "Cuboid")
	{
		if (params.size() != 5) // Cuboid cb1 L W H
			return nullptr;

		double L;
		double W;
		double H;
		size_t i = 0;
		if (!toDouble(params[i + 2], L) ||
			!toDouble(params[i + 3], W) ||
			!toDouble(params[i + 4], H))
			return nullptr;

		Cuboid cb = createCuboid( params[i+1], L, W, H);
		return std::make_unique<Cuboid>(cb);
	}

	if (type == "Cube")
	{
		if (params.size() != 3) // Cube c L
			return nullptr;

		double L;
		size_t i = 0;
		if (!toDouble(params[i + 2 ], L))
			return nullptr;

		Cube c = createCube(params[i+1],L);
		return std::make_unique<Cube>(c);
	}

	if (type == "Sphere")
	{
		if (params.size() != 3) // Sphere s R
			return nullptr;

		double R;
		size_t i = 0;
		if (!toDouble(params[i + 2], R))
			return nullptr;

		Sphere s = createSphere(params[i + 1], R);
		return std::make_unique<Sphere>(s);
	}

	if (type == "Cylinder")
	{
		if (params.size() != 4) // Cylinder cy1 R H
			return nullptr;

		double R;
		double H;
		size_t i = 0;
		if (!toDouble(params[i + 2], R) ||
			!toDouble(params[i + 3], H))
			return nullptr;

		Cylinder cy = createCylinder(params[i + 1], R, H); 
		return std::make_unique<Cylinder>(cy);
	}

	if (type == "Cone")
	{
		if (params.size() != 4) // Cone con R H
			return nullptr;

		double R;
		double H;
		size_t i = 0;
		if (!toDouble(params[i + 2], R) ||
			!toDouble(params[i + 3], H))
			return nullptr;

		Cone con = createCone(params[i + 1], R, H);
		return std::make_unique<Cone>(con);
	}

	if (type == "Pyramid")
	{
		if (params.size() != 5) // Pyramid py BL BW H
			return nullptr;

		double BL;
		double BW;
		double H;
		size_t i = 0;
		if (!toDouble(params[i + 2], BL) ||
			!toDouble(params[i + 3], BW) ||
			!toDouble(params[i + 4], H))
			return nullptr;

		Pyramid py = createPyramid(params[i + 1], BL, BW, H);
		return std::make_unique<Pyramid>(py);
	}


	return nullptr; // Unknown shape type
}
	 


