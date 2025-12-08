#include "pch.h"
#include "Triangulation.h"

Triangulation::Triangulation(){}
Triangulation::~Triangulation(){}

int Triangulation::addPoint(const Point& p)
{
    // Already exists? return existing index
   //find(p) returns: iterator pointing to the entry if found and end() if not found
    auto itr = pointIndex.find(p);
    if (itr != pointIndex.end()) 
        return itr->second; //if found return value(index) for that key(point)

    // Otherwise add new unique point
    auto index = int(mPoints.size()); //new index
    mPoints.push_back(p);
    pointIndex[p] = index;
    return index;
}

void Triangulation::addTriangle(int a, int b, int c)
{
    mTriangles.emplace_back(a, b, c);
}

std::vector<float> Triangulation::getDataForOpenGl() const
{
    std::vector<float> oglData;

    for (Triangle t : mTriangles)
    {
        oglData.push_back(float(mPoints[t.m1].getX()));
        oglData.push_back(float(mPoints[t.m1].getY()));
        oglData.push_back(float(mPoints[t.m1].getZ()));

        oglData.push_back(float(mPoints[t.m2].getX()));
        oglData.push_back(float(mPoints[t.m2].getY()));
        oglData.push_back(float(mPoints[t.m2].getZ()));

        oglData.push_back(float(mPoints[t.m3].getX()));
        oglData.push_back(float(mPoints[t.m3].getY()));
        oglData.push_back(float(mPoints[t.m3].getZ()));
    }

    return oglData;
}