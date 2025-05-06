#pragma once
#include <string>
#include <vector>
#include "Structures.h"
#include "Vector3D.h"
#include "AnglePair.h"

class IFileIOHandler
{
public:
	virtual ~IFileIOHandler() = default;
	virtual bool readFile(const std::string& filename, MIndices::Vector3D &voxelArray) = 0;
	virtual bool exportToFile(const std::string& filename, const MIndices::Vector3D &voxelArray) = 0;
	virtual bool exportToFile(const std::string& filename, const std::vector<MIndices::Triangle> &triArray) = 0;
	virtual bool exportToFile(const std::string& filename, const std::vector<MIndices::AnglePair> &pairs) = 0;
};