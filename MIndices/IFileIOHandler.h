#pragma once
#include <string>
#include "Structures.h"

 class IFileIOHandler
{
public:
	virtual ~IFileIOHandler() = default;
	virtual int readFile(const std::string& filename, std::vector<MIndices::Voxel> voxelArray) = 0;
	virtual int exportToFile(std::string& filename) = 0;
};