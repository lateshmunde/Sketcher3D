#include "pch.h"
#include "Cube.h"


Cube::Cube(const std::string& name, double side)
	: Shape("Cube", name), mSide(side) {
	build();
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

	Point n1(0, 0, -1);
	Point n2(0, 0, 1);
	Point n3(0, 1, 0);
	Point n4(0, -1, 0);
	Point n5(1, 0, 0);
	Point n6(-1, 0, 0);
	int p0Ind = mTriag.addPoint(Point(x, y, z));
	int p1Ind = mTriag.addPoint(Point(x + mSide, y, z));
	int p2Ind = mTriag.addPoint(Point(x + mSide, y + mSide, z));

	mTriag.addTriangle(p0Ind, p1Ind, p2Ind, n1); // front

	int p3Ind = mTriag.addPoint(Point(x, y + mSide, z));
	mTriag.addTriangle(p0Ind, p3Ind, p2Ind, n1); // front


	int p4Ind = mTriag.addPoint(Point(x, y, z + mSide));
	int p5Ind = mTriag.addPoint(Point(x + mSide, y, z + mSide));
	int p6Ind = mTriag.addPoint(Point(x + mSide, y + mSide, z + mSide));
	
	mTriag.addTriangle(p4Ind, p5Ind, p6Ind, n2); // back
	
	int p7Ind = mTriag.addPoint(Point(x, y + mSide, z + mSide));
	mTriag.addTriangle(p4Ind, p7Ind, p6Ind, n2); // back

	mTriag.addTriangle(p2Ind, p6Ind, p7Ind, n3); // top
	mTriag.addTriangle(p7Ind, p3Ind, p2Ind, n3); // top

	mTriag.addTriangle(p0Ind, p1Ind, p5Ind, n4); // bottom
	mTriag.addTriangle(p5Ind, p4Ind, p0Ind, n4); // bottom

	mTriag.addTriangle(p2Ind, p1Ind, p5Ind, n5); // right
	mTriag.addTriangle(p5Ind, p6Ind, p2Ind, n5); // right

	mTriag.addTriangle(p0Ind, p4Ind, p7Ind, n6); // left
	mTriag.addTriangle(p7Ind, p3Ind, p0Ind, n6); // left

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

