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

