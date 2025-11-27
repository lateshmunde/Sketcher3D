#pragma once
#include "Cuboid.h"
#include "Sphere.h"
#include "Cylinder.h"
#include "Cone.h"
#include "Cube.h"
#include "Pyramid.h"

class ShapeCreator
{
public:
	// Create shape from a serialized line (used in FileHandle)
	//static std::unique_ptr<Shape> createFromString(const std::string& line);

	// Direct creation helpers
	static Cuboid createCuboid(const std::string& name, double length, double width, double height);
	static Cube createCube(const std::string& name, double side);
	static  Sphere createSphere(const std::string& name, double radius);
	static Cylinder createCylinder(const std::string& name, double radius, double height);
	static Cone createCone(const std::string& name, double radius, double height);
	static Pyramid createPyramid(const std::string& name, double baseLength, double baseWidth, double height);

private:
	// Helper to parse tokens
	//static std::vector<std::string> reader(const std::string& line);
	
	// Helper to check numeric conversion
	static bool toDouble(const std::string& s, double& out);

};