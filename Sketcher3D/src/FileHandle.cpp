#include "FileHandle.h"
#include <fstream>

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




