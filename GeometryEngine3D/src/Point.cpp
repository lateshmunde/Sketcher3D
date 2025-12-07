#include "pch.h"
#include "Point.h"
#include <iostream>
#include <tuple>
#include <cmath>

Point::Point(): mX(0), mY(0), mZ(0) {}

Point::Point(double x, double y, double z)
	: mX(x), mY(y), mZ(z){}

double Point::distance(const Point& other) const
{
	return std::sqrt((mX - other.mX) * (mX - other.mX) +
		             (mY - other.mY) * (mY - other.mY) +
		             (mZ - other.mZ) * (mZ - other.mZ));
}
double Point::getX() const { return mX; }
double Point::getY() const { return mY; }
double Point::getZ() const { return mZ; }

void Point::setX(const double& x) { mX = x; }
void Point::setY(const double& y) { mY = y; }
void Point::setZ(const double& z) { mZ = z; }


void Point::writeXYZ(std::ostream& out) const
{
	out << mX << " " << mY << " " << mZ <<"\n";
}

bool Point::operator==(const Point& p2) const
{
	return std::tie(mX, mY, mZ) == std::tie(p2.mX, p2.mY, p2.mZ);
}

bool Point::operator<(const Point& other) const 
{ 
	if (mX != other.mX) return mX < other.mX; if (mY != other.mY) return mY < other.mY; return mZ < other.mZ; 
}