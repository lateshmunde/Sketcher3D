#include "pch.h"
#include "Pyramid.h"

Pyramid::Pyramid(const std::string& name, double baseLength, double baseWidth, double height)
	:Shape("Pyramid", name), mBaseLength(baseLength), mBaseWidth(baseWidth), mHeight(height) {
	build();
}

void Pyramid::build()
{
	double x = 0;
	double y = 0;
	double z = 0;

	double halfL = mBaseLength / 2.0; // center as origin
	double halfW = mBaseWidth / 2.0;

	int p0Ind = mTriag.addPoint(Point(x + halfL, y + halfW, z)); //b1 base points
	int p1Ind = mTriag.addPoint(Point(x + halfL, y - halfW, z)); //b2
	int p2Ind = mTriag.addPoint(Point(x - halfL, y - halfW, z)); //b3

	mTriag.addTriangle(p0Ind, p1Ind, p2Ind); //base

	int p3Ind = mTriag.addPoint(Point(x - halfL, y + halfW, z)); //b4
	mTriag.addTriangle(p2Ind, p3Ind, p0Ind); //base

	int apexInd = mTriag.addPoint(Point(x, y, z + mHeight)); //Apex

	mTriag.addTriangle(p0Ind, p1Ind, apexInd);
	mTriag.addTriangle(p2Ind, apexInd, p1Ind);
	mTriag.addTriangle(p3Ind, apexInd, p2Ind);
	mTriag.addTriangle(p0Ind, apexInd, p3Ind);
}

void Pyramid::save(std::ostream& out) const
{
	out << getType() << " " << getName() << " "
		<< "L " << mBaseLength << " W " << mBaseWidth << " H " << mHeight << "\n";
}

double Pyramid::getLength() const { return mBaseLength; }
double Pyramid::getWidth() const { return mBaseWidth; }
double Pyramid::getHeight() const { return mHeight; }
double Pyramid::getSlantHeight() const
{
	Point p1, h_pt(0, 0, mHeight);
	return p1.distance(h_pt);
}

void Pyramid::saveForGnu(std::ostream& out) const
{
	std::vector<std::vector<Point>> vec;
	std::vector<Point> pts;
	double x = 0;
	double y = 0;
	double z = 0;

	double halfL = mBaseLength / 2.0; // center as origin
	double halfW = mBaseWidth / 2.0;

	pts.emplace_back(x + halfL, y + halfW, z); //b1 base points
	pts.emplace_back(x + halfL, y - halfW, z); //b2
	pts.emplace_back(x - halfL, y - halfW, z); //b3
	pts.emplace_back(x - halfL, y + halfW, z); //b4
	pts.emplace_back(x + halfL, y + halfW, z); //b1
	vec.push_back(pts);
	pts.clear();

	pts.emplace_back(x + halfL, y + halfW, z); //b1
	pts.emplace_back(x, y, z + mHeight);//Apex point
	vec.push_back(pts);
	pts.clear();

	pts.emplace_back(x + halfL, y - halfW, z); //b2
	pts.emplace_back(x, y, z + mHeight);//Apex point
	vec.push_back(pts);
	pts.clear();

	pts.emplace_back(x - halfL, y - halfW, z); //b3
	pts.emplace_back(x, y, z + mHeight);//Apex point
	vec.push_back(pts);
	pts.clear();

	pts.emplace_back(x - halfL, y + halfW, z); //b4
	pts.emplace_back(x, y, z + mHeight);//Apex point
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