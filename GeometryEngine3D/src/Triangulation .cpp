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

int Triangulation::addPoint(const Point& p)
{
    // Already exists? return existing index
   //find(p) returns: iterator pointing to the entry if found and end() if not found
    auto itr = pointIndex.find(p);
    if (itr != pointIndex.end()) 
        return itr->second; //if found return value(index) for that key(point)

    // Otherwise add new unique point
    int index = mTPoints.size(); //new index
    mTPoints.push_back(p);
    pointIndex[p] = index;
    return index;
}

void Triangulation::addTriangle(int a, int b, int c)
{
    mTTriangles.emplace_back(a, b, c);
}

void Triangulation::addTriangle(std::vector <Triangle> tris, int a, int b, int c)
{
	tris.emplace_back(a, b, c);
}

const std::vector <Point> Triangulation::getPoints() const {return mTPoints;}

const std::vector <Triangle> Triangulation::getTriangles() const { return mTTriangles; }