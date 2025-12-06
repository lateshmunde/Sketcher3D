#pragma once
#include "Exports.h"
#include "Point.h"
#include "Triangle.h"
#include <Vector>

class GEOMETRYENGINE3D_API Triangulation
{
private:
	std::vector <Point> mPoints;
	std::vector <Triangle> mTriangles;

public:
	Triangulation();
	~Triangulation();

	int addPoint(const Point& p);
	void addTriangle(int a, int b , int c);

	const std::vector <Point> getPoints() const;
	const std::vector <Triangle> getTriangles() const;
};