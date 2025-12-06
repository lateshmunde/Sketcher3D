#include "pch.h"
#include "Triangulation.h"

Triangulation::Triangulation(){}
Triangulation::~Triangulation(){}

int Triangulation::addPoint(std::vector <Point> pts,const Point& p)
{
	pts.push_back(p);
	int index = pts.size() - 1;
	return index;
		
}

void Triangulation::addTriangle(std::vector <Triangle> tris, int a, int b, int c)
{
	tris.emplace_back(a, b, c);
}

//const std::vector <Point> Triangulation::getPoints() const {return mPoints;}

//const std::vector <Triangle> Triangulation::getTriangles() const { return mTriangles; }