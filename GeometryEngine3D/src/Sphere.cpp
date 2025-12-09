#include "pch.h"
#include "Sphere.h"

Sphere::Sphere(const std::string& name, double radius)
	:Shape("Sphere", name), mRadius(radius){
	build();
}

void Sphere::build()
{
	double x = 0;
	double y = 0;
	double z = 0;
	Point center(x, y, z);

	int stacks = 36;
	int number = 72;

	for (int i = 0; i < stacks; i++) {
		double iLatitude1 = MathConstants::PI * (-0.5 + (double)i / stacks);
		double iLatitude2 = MathConstants::PI * (-0.5 + (double)(i + 1) / stacks);

		double z1 = mRadius * sin(iLatitude1);
		double r1 = mRadius * cos(iLatitude1);

		double z2 = mRadius * sin(iLatitude2);
		double r2 = mRadius * cos(iLatitude2);

		for (int j = 0; j < number; j++)
		{
			double jLatitude1 = 2 * MathConstants::PI * (double)j / number;
			double jLatitude2 = 2 * MathConstants::PI * (double)(j + 1) / number;

			// First ring
			int idx1 = mTriag.addPoint(Point(r1 * cos(jLatitude1), r1 * sin(jLatitude1), z1));
			int idx2 = mTriag.addPoint(Point(r1 * cos(jLatitude2), r1 * sin(jLatitude2), z1));

			// Second ring
			int idx3 = mTriag.addPoint(Point(r2 * cos(jLatitude1), r2 * sin(jLatitude1), z2));
			int idx4 = mTriag.addPoint(Point(r2 * cos(jLatitude2), r2 * sin(jLatitude2), z2));

			// Triangle 1
			mTriag.addTriangle(idx1, idx2, idx3);
			// Triangle 2
			mTriag.addTriangle(idx2, idx4, idx3);
		}
	}
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
