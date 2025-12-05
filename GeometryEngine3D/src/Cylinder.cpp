#include "pch.h"
#include "Cylinder.h"

Cylinder::Cylinder(const std::string& name, double radius, double height)
	:Shape("Cylinder", name), mRadius(radius), mHeight(height) {
}

const std::vector<Point> Cylinder::getCoordinates() const
{
	std::vector<Point> pts;
	double x = 0;
	double y = 0;
	double z = 0;

	int number = 72;
	double dTheta = 2* MathConstants::PI / number;

	for (int i = 0; i <= number; i++) //base
	{
		double theta = i * dTheta;
		double x_ = mRadius * cos(theta);
		double y_ = mRadius * sin(theta) ;
		pts.emplace_back(x + x_, y + y_, z );
	}
	for (int i = 0; i <= number; i++) // height
	{
		double theta = i * dTheta;
		double x_ = mRadius * cos(theta);
		double y_ = mRadius * sin(theta);
		pts.emplace_back(x + x_, y + y_, z + mHeight);
	}
	for (int i = 0; i <= number; i++) //surface
	{
		double theta = i * dTheta;
		double x_ = mRadius * cos(theta);
		double y_ = mRadius * sin(theta);
		for (int j = 0; j < number; j++)
		{
			double z_ = (double(j) / number) * mHeight;
			pts.emplace_back(x + x_, y + y_, z + z_);
		}
	}
	return pts;
}

const std::vector<Point> Cylinder::coodinatesForGLTriangle() const
{
	std::vector<Point> pts;
	double x = 0;
	double y = 0;
	double z = 0;

	Point p1(x, y, z);
	pts.push_back(p1);
	return pts;
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

	for (const auto& ptsVec : vec)
	{
		for (const auto& pt : ptsVec)
		{
			pt.writeXYZ(out);
		}
		out << "\n\n";
	}
	out << "\n\n";
}