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

	Point p1(0, 0, 0);
	Point p2(mLength, 0, 0);
	Point p3(mLength, mWidth, 0);
	Point p4(0, mWidth, 0);

	Point p5(0, 0, mHeight);
	Point p6(mLength, 0, mHeight);
	Point p7(mLength, mWidth, mHeight);
	Point p8(0, mWidth, mHeight);

	// Bottom
	pts.insert(pts.end(), { p1, p2, p3, p1, p3, p4 });

	// Top
	pts.insert(pts.end(), { p5, p6, p7, p5, p7, p8 });

	// Front
	pts.insert(pts.end(), { p1, p2, p6, p1, p6, p5 });

	// Back
	pts.insert(pts.end(), { p4, p3, p7, p4, p7, p8 });

	// Left
	pts.insert(pts.end(), { p1, p4, p8, p1, p8, p5 });

	// Right
	pts.insert(pts.end(), { p2, p3, p7, p2, p7, p6 });

	return pts;
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