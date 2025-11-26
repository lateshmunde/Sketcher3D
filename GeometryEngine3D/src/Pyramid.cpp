#include "pch.h"
#include "Pyramid.h"

Pyramid::Pyramid(const std::string& name, double baseLength, double baseWidth, double height)
	:Shape("Pyramid", name), mBaseLength(baseLength), mBaseWidth(baseWidth), mHeight(height){}

const std::vector<Point> Pyramid::getCoordinates() const
{
	std::vector<Point> pts;
	double x = 0;
	double y = 0;
	double z = 0;

	int number = 20;
	double halfL = mBaseLength / 2.0; // center as origin
	double halfW = mBaseWidth / 2.0;

	Point b1(x + halfL, y + halfW, z); // base points
	Point b2(x + halfL, y - halfW, z);
	Point b3(x - halfL, y - halfW, z);
	Point b4(x - halfL, y + halfW, z);

	pts.push_back(b1);
	pts.push_back(b2);
	pts.push_back(b3);
	pts.push_back(b4);
	Point apex(x, y, z + mHeight); //Apex point
	pts.push_back(apex);

	// Side surfaces
	// linear interpolation along each triangular face
	std::vector<Point> base = { b1, b2, b3, b4 };
	for (int i = 0; i < 4; ++i)
	{
		Point start = base[i];
		//int nxt = (i + 1) % 4;
		Point end = base[(i + 1) % 4];

		for (int j = 0; j < number; j++)
		{
			double t = double(j) / number;

			// Edge from one base corner to the next
			double ex = (1 - t) * start.getX() + t * end.getX();
			double ey = (1 - t) * start.getY() + t * end.getY();
			double ez = z; // base surface

			// For each point on base edge, interpolate to apex
			for (int k = 0; k < number; k++)
			{
				double s = double(k) / number;
				//line from (ex,ey,ez) to apex(x,y,z)
				double fx = (1 - s) * ex + s * apex.getX();
				double fy = (1 - s) * ey + s * apex.getY();
				double fz = (1 - s) * ez + s * apex.getZ();

				pts.emplace_back(fx, fy, fz);
			}
		} 
	}

	return pts;
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