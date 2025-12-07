#pragma once
#include "Exports.h"
#include "Point.h"
#include "Triangle.h"
#include <Vector>
#include <map>

class GEOMETRYENGINE3D_API Triangulation
{
private:
	std::vector <Point> mTPoints; // for reading shapes from stl
	std::vector <Triangle> mTTriangles; // for reading shapes from stl
	std::map<Point, int> pointIndex;   // map: point - index in mPoints

public:
	Triangulation();
	~Triangulation();

	int addPoint(std::vector <Point> pts, const Point& p); // for writing shapes
	void addTriangle(std::vector <Triangle>tris, int a, int b , int c); //for writing shapes

	int addPoint(const Point& p); // for reading shapes from stl
	void addTriangle(int a, int b, int c); // for reading shapes from stl
	 
	const std::vector <Point> getPoints() const;
	const std::vector <Triangle> getTriangles() const;
};