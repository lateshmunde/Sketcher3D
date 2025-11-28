#include "pch.h"
#include "Sphere.h"

Sphere::Sphere(const std::string& name, double radius)
	:Shape("Sphere", name), mRadius(radius){}


const std::vector<Point> Sphere::getCoordinates() const
{
	std::vector<Point> pts;
	double x = 0;
	double y = 0;
	double z = 0;

	int number = 72;
	double dTheta = MathConstants::PI / number; //polar angle, 0 to 180
	double dPhi = 2* MathConstants::PI / number; //azimuthal angle, 0 to 360

	for (int i = 0; i < number; i++)
	{
		double theta = i * dTheta;
		for (int j = 0; j < number; j++)
		{
			double phi = j * dPhi;

			double x_ = mRadius * sin(theta) * cos(phi);
			double y_ = mRadius * sin(theta) * sin(phi);
			double z_ = mRadius * cos(theta);

			pts.emplace_back(x + x_, y + y_, z + z_);
		}
	}
	return pts;
}

void Sphere::save(std::ostream& out) const
{
	out << getType() << " " << getName() 
		<< " " << "R " << mRadius << "\n";
}

double Sphere::getRadius() const { return mRadius; }

void Sphere::saveForGnu(std::ostream& out) const
{
	std::vector<std::vector<Point>> vec;
	std::vector<Point> pts;
	double x = 0;
	double y = 0;
	double z = 0;

	int number = 72;
	double dTheta = MathConstants::PI / number; // 0 to 180
	double dPhi = 2* MathConstants::PI / number; // 0 to 360
	double phi = 0;
	double theta = 0;

	for (int i = 0; i <= number; i++)
	{
		theta = i * dTheta;
		for (int j = 0; j <= number; j++)
		{
			phi = j * dPhi;

			double x_ = mRadius * sin(theta) * cos(phi);
			double y_ = mRadius * sin(theta) * sin(phi);
			double z_ = mRadius * cos(theta);

			pts.emplace_back(x + x_, y + y_, z + z_);
		}
		double x_ = mRadius * sin(theta) * cos(0);
		double y_ = mRadius * sin(theta) * sin(0);
		double z_ = mRadius * cos(theta);
		pts.emplace_back(x + x_, y + y_, z + z_);
		vec.push_back(pts);
		pts.clear();
	}

	for (int i = 0; i <= number; i++)
	{
		theta = i * dTheta;
		for (int j = 0; j <= number; j++)
		{
			phi = j * dPhi;

			double x_ = mRadius * cos(theta);
			double y_ = mRadius * sin(theta) * cos(phi);
			double z_ = mRadius * sin(theta) * sin(phi);
			pts.emplace_back(x + x_, y + y_, z + z_);
		}
		double x_ = mRadius * cos(theta);
		double y_ = mRadius * sin(theta) * cos(0);
		double z_ = mRadius * sin(theta) * sin(0);
		pts.emplace_back(x + x_, y + y_, z + z_);
		vec.push_back(pts);
		pts.clear();
	}

	for (int i = 0; i <= number; i++)
	{
		theta = i * dTheta;
		for (int j = 0; j <= number; j++)
		{
			phi = j * dPhi;

			double x_ = mRadius * sin(theta) * sin(phi);
			double y_ =  mRadius* cos(theta);
			double z_ = mRadius * sin(theta) * cos(phi);
			pts.emplace_back(x + x_, y + y_, z + z_);
		}
		double x_ = mRadius * sin(theta) * sin(0);
		double y_ = mRadius * cos(theta);
		double z_ = mRadius * sin(theta) * cos(0);
		pts.emplace_back(x + x_, y + y_, z + z_);
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
