#pragma once
#include <string>
#include <vector>
#include "Structures.h"
#include "Array.h"
#include "AnglePair.h"

class IFileIOHandler
{
public:
	virtual ~IFileIOHandler() = default;
	virtual int32_t readFile(const std::string& filename, MIndices::Array &voxelArray) = 0;
	virtual int32_t exportToFile(const std::string& filename, const MIndices::Array &voxelArray) = 0;
	virtual int32_t exportToFile(const std::string& filename, const std::vector<MIndices::Triangle> &triArray) = 0;
	virtual int32_t exportToFile(const std::string& filename, const std::vector<MIndices::AnglePair> &pairs) = 0;
};