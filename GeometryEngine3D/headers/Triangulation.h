#pragma once
#include "Exports.h"
#include "Point.h"
#include "Triangle.h"
#include <Vector>
#include <map>

class GEOMETRYENGINE3D_API Triangulation
{
private:
	std::vector <Point> mPoints;
	std::vector <Triangle> mTriangles; 
	std::vector <Point> mNormal; 
	std::map<Point, int> pointIndex;   // map: point - index in mPoints

public:
	Triangulation();
	~Triangulation();

	std::vector <Point> getPoints() const;
	std::vector <Triangle> getTriangles() const;

	int addPoint(const Point& p); 
	void addTriangle(int a, int b, int c, Point nomrmal = Point());
	void calculateNormal();

	std::vector<float> getDataForOpenGl() const;
	std::vector<float> getNormalForOpenGl() const;
};