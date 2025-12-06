#include "pch.h"
#include "Triangulation.h"

Triangulation::Triangulation(){}
Triangulation::~Triangulation(){}

int Triangulation::addPoint(const Point& p)
{
	mPoints.push_back(p);
	int index = mPoints.size() - 1;
	return index;
		
}

void Triangulation::addTriangle(int a, int b, int c)
{
	mTriangles.emplace_back(a, b, c);
}

const std::vector <Point> Triangulation::getPoints() const {return mPoints;}

const std::vector <Triangle> Triangulation::getTriangles() const { return mTriangles; }