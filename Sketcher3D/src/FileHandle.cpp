#include "FileHandle.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

bool FileHandle::saveToFile(const std::string& fileName,
	 const std::vector<std::shared_ptr<Shape>>& shapes)
{
	std::ofstream fout(fileName);
	if (!fout.is_open())
	{
		std::cerr << "Error: Cannot open file or writing: " << fileName << "\n";
		return false;
	}

	for (const auto& s : shapes)
	{
		s->save(fout);
	}

	fout.close();
	return true;

}

//std::vector<std::shared_ptr<Shape>> FileHandle::loadFromFile(const std::string& fileName)
//{
//	std::vector<std::shared_ptr<Shape>> shapes;
//
//	std::ifstream fin(fileName);
//	if (!fin.is_open())
//	{
//		std::cerr << "Error: Cannot open file or reading: " << fileName << "\n";
//		return shapes;
//	}
//
//	std::string line;
//	while (std::getline(fin, line))
//	{
//		if (line.empty())
//			continue;
//
//		//shapes.push_back(ShapeCreator::createFromString(line));
//		std::unique_ptr<Shape> shape = ShapeCreator::createFromString(line);
//
//		if (!shape)
//		{
//			std::cerr << "Warning: Invalid line in file: " << line << "\n";
//			continue;
//		}
//
//		shapes.push_back(std::move(shape));
//	}
//
//	fin.close();
//	return shapes;
//}


bool FileHandle::saveToFileGNUPlot(const std::string& fileName,
	const std::vector<std::shared_ptr<Shape>>& shapes)
{
	std::ofstream fout(fileName);
	if (!fout.is_open())
	{
		std::cerr << "Error: Cannot open file or writing: " << fileName << "\n";
		return false;
	}
	
	for (const auto& s : shapes)
	{
		fout << "#" << s->getType() << "\n";
		fout << "#" << s->getName() << "\n";
		s->saveForGnu(fout);
	}
	fout.close();
	return true;

}
 
void FileHandle::readSTL(const std::string& fileName, Triangulation& triangulation)
{
	std::ifstream fin(fileName);
	std::string line;
	std::vector<float> vertices;
	Point normal;
	std::vector<Point> points;

	while (std::getline(fin, line))
	{
		size_t normalIdx = line.find("facet normal");
		size_t vertexIdx = line.find("vertex");
		// Skip all lines that do NOT contain the word "vertex"
		if (normalIdx != std::string::npos || vertexIdx != std::string::npos)
		{
			if (normalIdx != std::string::npos) {
				std::stringstream ss(line);

				std::string facetStr; // will store "vertex"
				std::string normalStr;
				double x;
				double y;
				double z;

				ss >> facetStr >> normalStr >> x >> y >> z;

				if (ss.fail())
					continue;

				normal.setX(x);
				normal.setY(y);
				normal.setZ(z);
			}
			if (vertexIdx != std::string::npos) {
				
				std::stringstream ss(line);

				std::string word; // will store "vertex"
				double x;
				double y;
				double z;

				ss >> word >> x >> y >> z;

				if (ss.fail())
					continue;
				points.emplace_back(x, y, z);
				if (points.size() == 3) {
					int p1 = triangulation.addPoint(points[0]);
					int p2 = triangulation.addPoint(points[1]);
					int p3 = triangulation.addPoint(points[2]);
					triangulation.addTriangle(p1, p2, p3, normal);
					points.clear();

				}
			}
		}
	}

	return;
}

bool FileHandle::writeSTL(const std::string& filename, const std::vector<std::shared_ptr<Shape>>& shapes)
{
    std::ofstream file(filename);
    if (!file.is_open()) return false;

	for (const std::shared_ptr<Shape> &it : shapes)
	{
		file << "Start " << it->getType() << " mesh\n";

		const std::vector<Point>& points = it->getTriangulation().getPoints();
		const std::vector<Triangle>& triangles = it->getTriangulation().getTriangles();

		for (const Triangle& tri : triangles)
		{
			// Get the 3 points
			const Point& p1 = points[tri.m1];
			const Point& p2 = points[tri.m2];
			const Point& p3 = points[tri.m3];

			// Normal (dummy 0 0 0 for now)
			file << "  facet normal 0.0 0.0 0.0\n";
			file << "    outer loop\n";
			file << "      vertex " << p1.getX() << " " << p1.getY() << " " << p1.getZ() << "\n";
			file << "      vertex " << p2.getX() << " " << p2.getY() << " " << p2.getZ() << "\n";
			file << "      vertex " << p3.getX() << " " << p3.getY() << " " << p3.getZ() << "\n";
			file << "    endloop\n";
			file << "  endfacet\n";
		}

		file << "End " << it->getType() << " mesh\n\n";
	}
    
    return true;
}
