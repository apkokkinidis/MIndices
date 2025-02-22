#pragma once
#include <string>
#include <vector>

#include "IFileIOHandler.h"
#include "Structures.h"
class FileIOHandler : public IFileIOHandler
{
public:
	FileIOHandler() = default;
	~FileIOHandler() override = default;

	int readFile(const std::string& filename, std::vector<MIndices::Voxel> voxelArray) override;
	int exportToFile(std::string& filename) override;
};
