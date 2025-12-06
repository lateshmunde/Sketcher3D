#include "pch.h"
#include "Cube.h"


Cube::Cube(const std::string& name, double side)
	: Shape("Cube", name), mSide(side) {}


const std::vector<Point> Cube::getCoordinates() const
{
	std::vector<Point> pts;
	double x = 0;
	double y = 0;
	double z = 0;

	//Point p1(x, y, z);
	//pts.push_back(p1); // Takes an already constructed object
	pts.emplace_back(x, y, z); // takes constructor arguments to create object
	pts.emplace_back(x + mSide, y, z);
	pts.emplace_back(x + mSide, y + mSide, z);
	pts.emplace_back(x, y + mSide, z);

	pts.emplace_back(x, y, z + mSide);
	pts.emplace_back(x + mSide, y, z + mSide);
	pts.emplace_back(x + mSide, y + mSide, z + mSide);
	pts.emplace_back(x, y + mSide, z + mSide);

	return pts;
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

Triangulation Cube::makeCube() const
{
	Triangulation cube;
	double x = 0;
	double y = 0;
	double z = 0;

	int p0 = cube.addPoint(mPoints, Point(x, y, z));
	int p1 = cube.addPoint(mPoints, Point(x + mSide, y, z));
	int p2 = cube.addPoint(mPoints, Point(x + mSide, y + mSide, z));
	int p3 = cube.addPoint(mPoints, Point(x, y + mSide, z));

	int p4 = cube.addPoint(mPoints, Point(x, y, z + mSide));
	int p5 = cube.addPoint(mPoints, Point(x + mSide, y, z + mSide));
	int p6 = cube.addPoint(mPoints, Point(x + mSide, y + mSide, z + mSide));
	int p7 = cube.addPoint(mPoints, Point(x, y + mSide, z + mSide));


	cube.addTriangle(mTriangles, p0, p1, p2);   
	cube.addTriangle(mTriangles, p0, p3, p2);
	cube.addTriangle(mTriangles, p4, p5, p6);
	cube.addTriangle(mTriangles, p4, p7, p6);
	cube.addTriangle(mTriangles, p4, p5, p1);
	cube.addTriangle(mTriangles, p4, p0, p1);
	cube.addTriangle(mTriangles, p3, p2, p6);
	cube.addTriangle(mTriangles, p3, p7, p6);
	cube.addTriangle(mTriangles, p1, p5, p6);
	cube.addTriangle(mTriangles, p1, p2, p6);
	cube.addTriangle(mTriangles, p4, p0, p3);
	cube.addTriangle(mTriangles, p4, p7, p3);
	
	return cube;
}

void Cube::save(std::ostream& out) const
{
	out << getType() << " " << getName() << " " << "S " << mSide << "\n";
}

double Cube::getSide() const { return mSide; }


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

