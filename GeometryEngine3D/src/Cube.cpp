#include "pch.h"
#include "Cube.h"


Cube::Cube(const std::string& name, double side)
	: Shape("Cube", name), mSide(side) {
	build();
}

const std::vector<Point> Cube::coodinatesForGLTriangle() const
{
	std::vector<Point> pts;
	double x = 0;
	double y = 0;
	double z = 0;

	Point p1(x, y, z);
	Point p2(x + mSide, y, z);
	Point p3(x + mSide, y + mSide, z);
	Point p4(x, y + mSide, z);
	Point p5(x, y, z + mSide);
	Point p6(x + mSide, y, z + mSide);
	Point p7(x + mSide, y + mSide, z + mSide);
	Point p8(x, y + mSide, z + mSide);

	pts.push_back(p1); pts.push_back(p2); pts.push_back(p3);
	pts.push_back(p3); pts.push_back(p4); pts.push_back(p1);
	pts.push_back(p1); pts.push_back(p5); pts.push_back(p8);
	pts.push_back(p8); pts.push_back(p4); pts.push_back(p1);
	pts.push_back(p1); pts.push_back(p5); pts.push_back(p6);
	pts.push_back(p6); pts.push_back(p1); pts.push_back(p2);
	pts.push_back(p2); pts.push_back(p6); pts.push_back(p7);
	pts.push_back(p7); pts.push_back(p2); pts.push_back(p3);
	pts.push_back(p3); pts.push_back(p7); pts.push_back(p4);
	pts.push_back(p4); pts.push_back(p7); pts.push_back(p8);
	pts.push_back(p8); pts.push_back(p7); pts.push_back(p6);
	pts.push_back(p6); pts.push_back(p5); pts.push_back(p8);

	return pts;
}

void Cube::save(std::ostream& out) const
{
	out << getType() << " " << getName() << " " << "S " << mSide << "\n";
}

double Cube::getSide() const { return mSide; }

void Cube::build()
{
	double x = 0;
	double y = 0;
	double z = 0;

	int p0Ind = mTriag.addPoint(Point(x, y, z));
	int p1Ind = mTriag.addPoint(Point(x + mSide, y, z));
	int p2Ind = mTriag.addPoint(Point(x + mSide, y + mSide, z));

	mTriag.addTriangle(p0Ind, p1Ind, p2Ind); // front

	int p3Ind = mTriag.addPoint(Point(x, y + mSide, z));
	mTriag.addTriangle(p0Ind, p3Ind, p2Ind); // front


	int p4Ind = mTriag.addPoint(Point(x, y, z + mSide));
	int p5Ind = mTriag.addPoint(Point(x + mSide, y, z + mSide));
	int p6Ind = mTriag.addPoint(Point(x + mSide, y + mSide, z + mSide));
	
	mTriag.addTriangle(p4Ind, p5Ind, p6Ind); // back
	
	int p7Ind = mTriag.addPoint(Point(x, y + mSide, z + mSide));
	mTriag.addTriangle(p4Ind, p7Ind, p6Ind); // back

	mTriag.addTriangle(p2Ind, p6Ind, p7Ind); // top
	mTriag.addTriangle(p7Ind, p3Ind, p2Ind); // top

	mTriag.addTriangle(p0Ind, p1Ind, p5Ind); // bottom
	mTriag.addTriangle(p5Ind, p4Ind, p0Ind); // bottom

	mTriag.addTriangle(p2Ind, p1Ind, p5Ind); // right
	mTriag.addTriangle(p5Ind, p6Ind, p2Ind); // right

	mTriag.addTriangle(p0Ind, p4Ind, p7Ind); // left
	mTriag.addTriangle(p7Ind, p3Ind, p0Ind); // left

}

void Cube::saveForGnu(std::ostream& out) const
{
	std::vector<std::vector<Point>> vec;
	std::vector<Point> pts;
	double x = 0;
	double y = 0;
	double z = 0;

	pts.emplace_back(x, y, z); //p1
	pts.emplace_back(x + mSide, y, z);//p2
	pts.emplace_back(x + mSide, y + mSide, z);//p3
	pts.emplace_back(x, y + mSide, z);//p4
	pts.emplace_back(x, y, z);//p1
	vec.push_back(pts);
	pts.clear();

	pts.emplace_back(x, y, z + mSide);//p5
	pts.emplace_back(x + mSide, y, z + mSide);//p6
	pts.emplace_back(x + mSide, y + mSide, z + mSide);//p7
	pts.emplace_back(x, y + mSide, z + mSide);//p8
	pts.emplace_back(x, y, z + mSide);//p1
	vec.push_back(pts);
	pts.clear();

	pts.emplace_back(x, y, z);//p1
	pts.emplace_back(x, y, z + mSide);//p5
	vec.push_back(pts);
	pts.clear();

	pts.emplace_back(x + mSide, y, z);//p2
	pts.emplace_back(x + mSide, y, z + mSide);//p6
	vec.push_back(pts);
	pts.clear();

	pts.emplace_back(x + mSide, y + mSide, z);//p3
	pts.emplace_back(x + mSide, y + mSide, z + mSide);//p7
	vec.push_back(pts);
	pts.clear();

	pts.emplace_back(x, y + mSide, z);//p4
	pts.emplace_back(x, y + mSide, z + mSide);//p8
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

