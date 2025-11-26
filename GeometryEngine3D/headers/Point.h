#pragma once
#include "Exports.h"
#include <iostream>

class GEOMETRYENGINE3D_API Point
{
private:
	double mX; 
	double mY;
	double mZ;

public:
	Point();
	Point(double x, double y, double z);
	~Point() = default;

	double distance(const Point& other) const;
	double getX() const;
	double getY() const;
	double getZ() const;

	void setX(const double& x);
	void setY(const double& y);
	void setZ(const double& z);

	void writeXYZ(std::ostream& out) const;
	bool operator==(const Point&) const;
};
