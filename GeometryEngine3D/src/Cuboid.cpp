#include "pch.h"
#include "Cuboid.h"

Cuboid::Cuboid(const std::string& name, double length, double width, double height)
	: Shape("Cuboid", name), mLength(length), mWidth(width), mHeight(height) {
	build();
}

void Cuboid::build()
{
	double x = 0;
	double y = 0;
	double z = 0;

	int p0Ind = mTriag.addPoint(Point(x, y, z));
	int p1Ind = mTriag.addPoint(Point(x + mLength, y, z));
	int p2Ind = mTriag.addPoint(Point(x + mLength, y + mWidth, z));

	mTriag.addTriangle(p0Ind, p1Ind, p2Ind); // front

	int p3Ind = mTriag.addPoint(Point(x, y + mWidth, z));
	mTriag.addTriangle(p0Ind, p2Ind, p3Ind); // front

	int p4Ind = mTriag.addPoint(Point(x, y, z + mHeight));
	int p5Ind = mTriag.addPoint(Point(x + mLength, y, z + mHeight));
	int p6Ind = mTriag.addPoint(Point(x + mLength, y + mWidth, z + mHeight));

	mTriag.addTriangle(p4Ind, p5Ind, p6Ind); // back

	int p7Ind = mTriag.addPoint(Point(x, y + mWidth, z + mHeight));
	mTriag.addTriangle(p4Ind, p6Ind, p7Ind); // back

	mTriag.addTriangle(p7Ind, p6Ind, p2Ind); // top
	mTriag.addTriangle(p7Ind, p2Ind, p3Ind); // top

	mTriag.addTriangle(p0Ind, p5Ind, p1Ind); // bottom
	mTriag.addTriangle(p0Ind, p4Ind, p5Ind); // bottom

	mTriag.addTriangle(p5Ind, p1Ind, p2Ind); // right
	mTriag.addTriangle(p5Ind, p2Ind, p6Ind); // right

	mTriag.addTriangle(p0Ind, p4Ind, p7Ind); // left
	mTriag.addTriangle(p0Ind, p3Ind, p7Ind); // left
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