#include "FileIOHandler.h"
#include <stdint.h>
#include <fstream>
#include <sstream>
#include <istream>
#include <iostream>
#include "Structures.h"

int FileIOHandler::readFile(const std::string& filename, MIndices::Array& voxelArray)
{
	size_t totalElemNum =0, elemNum = 0, i = 0, k = 0, j = 0;
	//ToDo implement read for .txt files
	std::ifstream file(filename);
	if (!file)
	{
		std::cerr << "Cannot open the file: " << filename << std::endl;
		return 1;
	}

	std::string line;
	while (std::getline(file, line))
	{
		if (!line.empty() && (std::istringstream(line) >> totalElemNum))
		{
			break;  // Successfully read totalElemNum, exit loop
		}
	}

	if (0 == totalElemNum)
	{
		std::cerr << "Wrong number of elements." << std::endl;
		return 1;
	}

	while (file >> elemNum >> i >> j >> k)
	{
		voxelArray.SetElement(i - 1, j - 1, k - 1, MIndices::ElementTypes::FILLED);
	}

	if (file.bad())
	{
		std::cerr << "IO failure. Possible corrupt data read." << std::endl;
		return 1;
	}
	return 0;
}

int FileIOHandler::exportToFile(std::string& filename, const MIndices::Array& voxelArray)
{
	//ToDo implement export for .txt files
	return 0;
}

int FileIOHandler::exportToFile(std::string& filename, const std::vector<MIndices::Triangle>& triArray)
{
	//ToDo implement export for .txt files
	return 0;
}
