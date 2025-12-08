#include "pch.h"
#include "Cylinder.h"



Cylinder::Cylinder(const std::string& name, double radius, double height)
	:Shape("Cylinder", name), mRadius(radius), mHeight(height) {
	build();
}

void Cylinder::build()
{
	double x = 0;
	double y = 0;
	double z = 0;
	Point baseCenter(x, y, z);
	Point topCenter(x, y, mHeight);

	std::vector<int> bPtsIndex;
	std::vector<int> tPtsIndex;
	int baseCenterInd = mTriag.addPoint(baseCenter);
	int topCenterInd = mTriag.addPoint(topCenter);

	bPtsIndex.push_back(mTriag.addPoint(Point(x + mRadius * cos(0), y + mRadius * sin(0), z)));
	tPtsIndex.push_back(mTriag.addPoint(Point(x + mRadius * cos(0), y + mRadius * sin(0), z + mHeight)));

	int number = 72;
	double dTheta = 2 * MathConstants::PI / number; // 0 to 180

	for (int i = 1; i <= number; i++)
	{
		double theta = i * dTheta;
		double x_ = mRadius * cos(theta);
		double y_ = mRadius * sin(theta);

		bPtsIndex.push_back(mTriag.addPoint(Point(x + x_, y + y_, z)));
		tPtsIndex.push_back(mTriag.addPoint(Point(x + x_, y + y_, z + mHeight)));

		// each 0.5 degree section has 4 triangles.
		mTriag.addTriangle(baseCenterInd, bPtsIndex[i - 1], bPtsIndex[i]);		// Base circle center, two points on it's circumference
		mTriag.addTriangle(bPtsIndex[i], tPtsIndex[i], bPtsIndex[i - 1]);		// Cylinder surface triangle: b1, t1, b0 
		mTriag.addTriangle(bPtsIndex[i - 1], tPtsIndex[i], tPtsIndex[i - 1]);	// Cylinder surface triangle: b0, t1, t0
		mTriag.addTriangle(tPtsIndex[i - 1], tPtsIndex[i], topCenterInd);		// Top circle center, two points on it's circumference
	}
}

void Cylinder::save(std::ostream& out) const
{
	out << getType() << " " << getName() << " "
		<< "R " << mRadius << " H " << mHeight << "\n";
}

double Cylinder::getRadius() const { return mRadius; }
double Cylinder::getHeight() const { return mHeight; }

void Cylinder::saveForGnu(std::ostream& out) const
{
	std::vector<std::vector<Point>> vec;
	std::vector<Point> pts;
	double x = 0;
	double y = 0;
	double z = 0;

	int number = 72;
	double dTheta = 2 * MathConstants::PI / number;

	for (int i = 0; i <= number; i++) //base
	{
		double theta = i * dTheta;
		double x_ = mRadius * cos(theta);
		double y_ = mRadius * sin(theta);
		pts.emplace_back(x + x_, y + y_, z);
	}
	double x_ = mRadius * cos(0); // first point
	double y_ = mRadius * sin(0);
	pts.emplace_back(x + x_, y + y_, z);
	vec.push_back(pts);
	pts.clear();

	for (int i = 0; i <= number; i++) // height
	{
		double theta = i * dTheta;
		x_ = mRadius * cos(theta);
		y_ = mRadius * sin(theta);
		pts.emplace_back(x + x_, y + y_, z + mHeight);
	}
	x_ = mRadius * cos(0); // first point
	y_ = mRadius * sin(0);
	pts.emplace_back(x + x_, y + y_, z + mHeight);
	vec.push_back(pts);
	pts.clear();

	for (int i = 0; i <= number; i++)
	{
		double theta = i * dTheta;
		x_ = mRadius * cos(theta);
		y_ = mRadius * sin(theta);
		pts.emplace_back(x + x_, y + y_, z);
		pts.emplace_back(x + x_, y + y_, z + mHeight);
		vec.push_back(pts);
		pts.clear();
	}

	for (const std::vector <Point>& ptsVec : vec)
	{
		for (const Point& pt : ptsVec)
		{
			pt.writeXYZ(out);
		}
		out << "\n\n";
	}
	out << "\n\n";
}