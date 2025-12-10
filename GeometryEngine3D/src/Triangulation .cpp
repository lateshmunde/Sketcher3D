#include "pch.h"
#include "Triangulation.h"

Triangulation::Triangulation() {}
Triangulation::~Triangulation() {}

std::vector<Point> Triangulation::getPoints() const
{
    return mPoints;
}

std::vector<Triangle> Triangulation::getTriangles() const
{
    return mTriangles;
}


std::vector<Point> Triangulation::getNormals() const
{
    return mNormals;
}

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

void Triangulation::addTriangle(int a, int b, int c, Point normal)
{
    Triangle t(a, b, c, normal);
    mTriangles.push_back(t);
    mNormals.push_back(calculateNormal(t));
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

Point Triangulation::calculateNormal(const Triangle& tri) const
{
    Point u = mPoints[tri.m2] - mPoints[tri.m1];
    Point v = mPoints[tri.m3] - mPoints[tri.m1];

    double nx = u.getY() * v.getZ() - u.getZ() * v.getY();
    double ny = u.getZ() * v.getX() - u.getX() * v.getZ();
    double nz = u.getX() * v.getY() - u.getY() * v.getX();

    double len = sqrt(nx * nx + ny * ny + nz * nz);

    return Point(nx / len, ny / len, nz / len);
}

std::vector<float> Triangulation::getNormalForOpenGl() const
{
    std::vector<float> oglData;

    for (const Triangle& tri : mTriangles)
    {
        Point pt = calculateNormal(tri);
      
        oglData.push_back(float(pt.getX()));
        oglData.push_back(float(pt.getY()));
        oglData.push_back(float(pt.getZ()));
    }
    return oglData;
}