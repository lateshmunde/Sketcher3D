#pragma once
#include "Exports.h"
#include "Point.h"
#include "Triangle.h"
#include <Vector>
#include <map>

class GEOMETRYENGINE3D_API Triangulation
{
private:
	std::vector <Point> mPoints; // for reading shapes from stl
	std::vector <Triangle> mTriangles; // for reading shapes from stl
	std::map<Point, int> pointIndex;   // map: point - index in mPoints

public:
	Triangulation();
	~Triangulation();

	std::vector <Point> getPoints() const;
	std::vector <Triangle> getTriangles() const;

	int addPoint(const Point& p); // for reading shapes from stl
	void addTriangle(int a, int b, int c, Point normal = Point()); // for reading shapes from stl

	std::vector<float> getNormalForOpenGl() const;
	std::vector<float> getDataForOpenGl() const;
};