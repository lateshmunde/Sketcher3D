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

const std::vector<Point> Sphere::coodinatesForGLTriangle() const
{
	std::vector<Point> pts;

	int stacks = 18;
	int number = 72;
	double pi = MathConstants::PI;

	for (int i = 0; i < stacks; i++)
	{
		double latitude1 = pi * (-0.5 + (double)i / stacks);
		double latitude2 = pi * (-0.5 + (double)(i + 1) / stacks);

		double z1 = mRadius * sin(latitude1);
		double r1 = mRadius * cos(latitude1);

		double z2 = mRadius * sin(latitude2);
		double r2 = mRadius * cos(latitude2);

		for (int j = 0; j < number; j++)
		{
			double latitude1 = 2 * pi * (double)j / number;
			double latitude2 = 2 * pi * (double)(j + 1) / number;

			// First ring
			Point p1(r1 * cos(latitude1), r1 * sin(latitude1), z1);
			Point p2(r1 * cos(latitude2), r1 * sin(latitude2), z1);

			// Second ring
			Point p3(r2 * cos(latitude1), r2 * sin(latitude1), z2);
			Point p4(r2 * cos(latitude2), r2 * sin(latitude2), z2);

			// Triangle 1
			pts.push_back(p1);
			pts.push_back(p3);
			pts.push_back(p2);

			// Triangle 2
			pts.push_back(p2);
			pts.push_back(p3);
			pts.push_back(p4);
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
