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


Triangulation FileHandle::readSTL(const std::string& fileName)
{
	std::ifstream fin(fileName);
	std::string line;

	Triangulation T;
	std::vector<Point> pts;

	while (std::getline(fin, line)) //Reads every line from STL.(fileName)
	{
		int start = line.find_first_not_of(" \t"); //returns index of first character that is not a space or a tab -> " \t"

		//find_first_not_of() returns npos when : line is empty,  line contains only tabs, spaces
		if (start == std::string::npos) //no valid position found
			continue; //if empty move to next line

		std::string required = line.substr(start); // Returns a substring beginning from index "start" to the end of the string.

		//rfind(substring, position) : Search for "substring" starting at position 0
		if (required.rfind("vertex", 0) != 0) 
			continue; //if vertex is not at positon 0, then skip line (facet, normal, etc)

		std::stringstream ss(required); //treat a string as if it were a stream
		std::string word;
		float x;
		float y;
		float z;
				
		ss >> word >> x >> y >> z; //Automatically skips spaces, Converts text "1.0" -> float 1.0

		pts.emplace_back(x, y, z);

		// When 3 points are collected - make triangle
		if (pts.size() == 3)
		{
			int i1 = T.addPoint(pts[0]); // checks duplicate, return index
			int i2 = T.addPoint(pts[1]);
			int i3 = T.addPoint(pts[2]);

			T.addTriangle(i1, i2, i3);
			pts.clear();
		}
	}

	return T;
}


