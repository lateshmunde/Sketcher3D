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
 
//std::vector <Point>  FileHandle::readSTL(const std::string& fileName) { //for now, cube only
//	std::ifstream fin(fileName);
//
//	std::string line;
//	std::vector <Point> pts;
//
//	while (std::getline(fin, line)) 
//	{
//		// Remove leading spaces
//		int start = line.find_first_not_of(" \t");
//		if (start == std::string::npos)
//			continue;   // skip empty or only-space lines
//
//		std::string trimmed = line.substr(start);
//
//		// Check if line starts with "vertex"
//		if (required.rfind("vertex", 0) != 0)
//			continue;   // skip non-vertex lines
//
//		// Parse: vertex x y z
//		std::string word;
//		float x;
//		float y;
//		float z;
//
//		std::stringstream ss(required);
//		ss >> word >> x >> y >> z;   // word = "vertex"
//
//		pts.emplace_back(x, y, z);
//	}
//
//	return pts;
//}


std::vector<float> FileHandle::readSTL(const std::string& fileName)
{
	std::ifstream fin(fileName);
	std::string line;
	std::vector<float> vertices;

	while (std::getline(fin, line))
	{
		// Skip all lines that do NOT contain the word "vertex"
		if (line.find("vertex") == std::string::npos)
			continue;

		// Make a stream from the line
		std::stringstream ss(line);

		std::string word; // will store "vertex"
		float x;
		float y;
		float z;
		

		// Read: vertex x y z
		ss >> word >> x >> y >> z;

		// If conversion fails, skip
		if (ss.fail())
			continue;

		// Add coordinates to array
		vertices.push_back(x);
		vertices.push_back(y);
		vertices.push_back(z);
	}

	return vertices;
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
