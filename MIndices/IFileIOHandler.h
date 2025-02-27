#pragma once
#include <string>
#include <vector>
#include "Structures.h"
#include "Array.h"

class IFileIOHandler
{
public:
	virtual ~IFileIOHandler() = default;
	virtual int readFile(const std::string& filename, MIndices::Array &voxelArray) = 0;
	virtual int exportToFile(std::string& filename, const MIndices::Array &voxelArray) = 0;
	virtual int exportToFile(std::string& filename, const std::vector<MIndices::Triangle> &triArray) = 0;
};