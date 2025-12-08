#include "pch.h"
#include "Cone.h"



Cone::Cone(const std::string& name, double radius, double height)
	:Shape("Cone", name), mRadius(radius), mHeight(height) {
	build();
}

const std::vector<Point> Cone::coodinatesForGLTriangle() const
{
	std::vector<Point> pts;
	std::vector<Point> baseCirclePts;
	double x = 0;
	double y = 0;
	double z = 0;
	Point origin(x, y, z);
	Point apex(x, y, z + mHeight);

	int number = 72;
	double dTheta = 2 * MathConstants::PI / number; // 0 to 180
	for (int i = 0; i <= number; i++) //base
	{
		double theta = i * dTheta;
		double x_ = mRadius * cos(theta);
		double y_ = mRadius * sin(theta);
		baseCirclePts.emplace_back(x + x_, y + y_, z);
	}

	// base triangles
	for (int i = 0; i < number; i++) {
		pts.push_back(baseCirclePts[i]); pts.push_back(baseCirclePts[i + 1]); pts.push_back(origin);
	}

	// surface triangles
	for (int i = 0; i < number; i++) {
		pts.push_back(baseCirclePts[i]); pts.push_back(baseCirclePts[i + 1]); pts.push_back(apex);
	}

	return pts;
}

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

void Cone::build()
{
	double x = 0;
	double y = 0;
	double z = 0;
	Point origin(x, y, z);
	Point apex(x, y, mHeight);

	std::vector<int> bPtsIndex;
	int originInd = mTriag.addPoint(origin);
	int apexInd = mTriag.addPoint(apex);

	bPtsIndex.push_back(mTriag.addPoint(Point(x + mRadius * cos(0), y + mRadius * sin(0), z)));

	int number = 72;
	double dTheta = 2 * MathConstants::PI / number; // 0 to 180

	for (int i = 1; i <= number; i++)
	{
		double theta = i * dTheta;
		double x_ = mRadius * cos(theta);
		double y_ = mRadius * sin(theta);

		bPtsIndex.push_back(mTriag.addPoint(Point(x + x_, y + y_, z)));

		// each 5 degree section has 4 triangles.
		mTriag.addTriangle(originInd, bPtsIndex[i - 1], bPtsIndex[i]);		// Base circle center, two points on it's circumference
		mTriag.addTriangle(bPtsIndex[i], apexInd, bPtsIndex[i - 1]);		// Cone surface triangle: b1, apex, b0 
	}
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