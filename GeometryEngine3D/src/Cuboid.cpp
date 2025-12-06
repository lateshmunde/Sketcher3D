#include "pch.h"
#include "Cuboid.h"


Cuboid::Cuboid(const std::string& name, double length, double width, double height)
	: Shape("Cuboid", name), mLength(length), mWidth(width), mHeight(height){}


const std::vector<Point> Cuboid::getCoordinates() const
{
	std::vector<Point> pts;
	double x = 0;
	double y = 0;
	double z = 0;

	//Point p1(x, y, z);
	//pts.push_back(p1); // Takes an already constructed object
	pts.emplace_back(x, y, z); // takes constructor arguments to create object
	pts.emplace_back(x + mLength, y, z);
	pts.emplace_back(x + mLength, y + mWidth, z);
	pts.emplace_back(x , y + mWidth, z);

	pts.emplace_back(x , y , z + mHeight);
	pts.emplace_back(x + mLength, y , z + mHeight);
	pts.emplace_back(x + mLength, y + mWidth, z + mHeight);
	pts.emplace_back(x , y + mWidth, z + mHeight);

	return pts;
}

const std::vector<Point> Cuboid::coodinatesForGLTriangle() const
{
	std::vector<Point> pts;
	double x = 0;
	double y = 0;
	double z = 0;

	Point p1(x, y, z);
	Point p2(x + mLength, y, z);
	Point p3(x + mLength, y + mWidth, z);
	Point p4(x, y + mWidth, z);
	Point p5(x, y, z + mHeight);
	Point p6(x + mLength, y, z + mHeight);
	Point p7(x + mLength, y + mWidth, z + mHeight);
	Point p8(x, y + mWidth, z + mHeight);

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

Triangulation Cuboid::makeCube() const
{
	Triangulation cuboid;
	return cuboid;
}

void Cuboid::save(std::ostream& out) const
{
	out << getType() << " " << getName() << " "
		<< "L " << mLength << " W " << mWidth << " H " << mHeight << "\n";
}

double Cuboid::getLength() const { return mLength; }
double Cuboid::getWidth() const { return mWidth; }
double Cuboid::getHeight() const { return mHeight; }


void Cuboid::saveForGnu(std::ostream& out) const
{
	std::vector<std::vector<Point>> vec;
	std::vector<Point> pts;
	double x = 0;
	double y = 0;
	double z = 0;

	pts.emplace_back(x, y, z); //p1
	pts.emplace_back(x + mLength, y, z);//p2
	pts.emplace_back(x + mLength, y + mWidth, z);//p3
	pts.emplace_back(x, y + mWidth, z);//p4
	pts.emplace_back(x, y, z);//p1
	vec.push_back(pts);
	pts.clear();

	pts.emplace_back(x, y, z + mHeight);//p5
	pts.emplace_back(x + mLength, y, z + mHeight);//p6
	pts.emplace_back(x + mLength, y + mWidth, z + mHeight);//p7
	pts.emplace_back(x, y + mWidth, z + mHeight);//p8
	pts.emplace_back(x, y, z + mHeight);//p1
	vec.push_back(pts);
	pts.clear();

	pts.emplace_back(x, y, z);//p1
	pts.emplace_back(x, y, z + mHeight);//p5
	vec.push_back(pts);
	pts.clear();

	pts.emplace_back(x + mLength, y, z);//p2
	pts.emplace_back(x + mLength, y, z + mHeight);//p6
	vec.push_back(pts);
	pts.clear();

	pts.emplace_back(x + mLength, y + mWidth, z);//p3
	pts.emplace_back(x + mLength, y + mWidth, z + mHeight);//p7
	vec.push_back(pts);
	pts.clear();

	pts.emplace_back(x, y + mWidth, z);//p4
	pts.emplace_back(x, y + mWidth, z + mHeight);//p8
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


//push_back:
//Takes an already constructed object(either by value or by const reference).
//Copies or moves this existing object into the container.
//This involves at least one object construction(the temporary object if passed by value) 
//and then a copy or move operation into the container's memory.
//emplace_back :
//Takes constructor arguments for the object to be created.
//Constructs the object directly in - place within the container's memory, 
//using perfect forwarding of the provided arguments to the object's constructor.
//This avoids the creation of temporary objects and subsequent copy or move operations, 
//potentially leading to better performance, especially for complex or expensive - to - copy types.