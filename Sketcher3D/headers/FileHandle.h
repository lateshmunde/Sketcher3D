#pragma once
#include "Shape.h"
#include "ShapeCreator.h"

class FileHandle
{
public:
	static bool saveToFile(const std::string& fileName,
		const std::vector<std::shared_ptr<Shape>>& shapes);

    static bool saveToFileGNUPlot(const std::string& fileName,
        const std::vector<std::shared_ptr<Shape>>& shapes);

	//static std::vector<std::shared_ptr<Shape>> loadFromFile(const std::string& fileName);

	//static std::vector <Point> readSTL(const std::string& fileName);
	static Triangulation readSTL(const std::string& fileName);
};



// Read STL branch - read stl with 36 points in pts vector
// OpenGL branch - no reading STL
// Triangulation BRanch - read stl with pts vector, triangle vector and map