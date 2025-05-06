#include "FileIOHandler.h"
#include <stdint.h>
#include <fstream>
#include <sstream>
#include <istream>
#include <iostream>
#include "Structures.h"

bool FileIOHandler::readFile(const std::string& filename, MIndices::Vector3D& voxelArray)
{
	size_t totalElemNum =0, elemNum = 0, i = 0, k = 0, j = 0;
	std::ifstream file(filename);
	if (!file)
	{
		std::cerr << "Cannot open the file: " << filename << std::endl;
		return false;
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
		return false;
	}

	while (file >> elemNum >> i >> j >> k)
	{
		voxelArray.SetElement(i - 1, j - 1, k - 1, MIndices::ElementTypes::FILLED);
	}

	if (file.bad())
	{
		std::cerr << "IO failure. Possible corrupt data read." << std::endl;
		return false;
	}
	return true;
}

bool FileIOHandler::exportToFile(const std::string& filename, const MIndices::Vector3D& voxelArray)
{
	//ToDo implement export for .txt files
	return true;
}

bool FileIOHandler::exportToFile(const std::string& filename, const std::vector<MIndices::Triangle>& triArray)
{
	//ToDo implement export for .txt files
	return true;
}

bool FileIOHandler::exportToFile(const std::string& filename, const std::vector<MIndices::AnglePair>& pairs)
{
	std::ostringstream buffer;
	for (const auto& pair : pairs)
	{
		buffer << "Azimuth : " << pair.Azimuth() << " Elevation : " << pair.Elevation() << " Tabecular  Number : " << pair.TbTn() << " Tabecular Thickness : " << pair.TbTh() << std::endl;
	}
	std::ofstream file(filename, std::ios::app);	//use trunc to clear file contents
	if (!file)
	{
		std::cerr << " Error while writing to file : " << filename << std::endl;
		return false;
	}
	file << buffer.str();
	return true;
}
