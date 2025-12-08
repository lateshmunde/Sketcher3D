#include "pch.h"
#include "Cone.h"

void Cone::build()
{
}

Cone::Cone(const std::string& name,double radius, double height)
	:Shape("Cone", name), mRadius(radius), mHeight(height){}

void Cone::save(std::ostream& out) const
{
	out <<getType() << " " << getName() << " " 
		<< "R " << mRadius << " H " << mHeight << "\n";
}

double Cone::getRadius() const { return mRadius; }
double Cone::getHeight() const { return mHeight; }
double Cone::getSlantHeight() const
{
	return std::sqrt((mRadius * mRadius) + (mHeight * mHeight));
}


void Cone::saveForGnu(std::ostream& out) const
{
	std::vector<std::vector<Point>>vec ;
	std::vector<Point> pts;
	double x = 0;
	double y = 0;
	double z = 0;

	int number = 72;
	double dTheta = 2 * MathConstants::PI / number; // 0 to 180
	
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

	for (int i = 0; i <= number; i++)
	{
		double theta = i * dTheta;
		x_ = mRadius * cos(theta);
		y_ = mRadius * sin(theta);
		pts.emplace_back(x + x_, y + y_, z);
		pts.emplace_back(x, y, z + mRadius);
	}
	vec.push_back(pts);
	pts.clear();

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