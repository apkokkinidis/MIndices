#pragma once
#include <string>
#include <vector>

#include "Vector3D.h"
#include "IFileIOHandler.h"
#include "Structures.h"

class FileIOHandler : public IFileIOHandler
{
public:
	FileIOHandler() = default;
	~FileIOHandler() override = default;

	bool readFile(const std::string& filename, MIndices::Vector3D &voxelArray) override;
	bool exportToFile(const std::string& filename, const MIndices::Vector3D& voxelArray) override;
	bool exportToFile(const std::string& filename, const std::vector<MIndices::Triangle>& triArray) override;
	bool exportToFile(const std::string& filename, const std::vector<MIndices::AnglePair>& pairs) override;
};
