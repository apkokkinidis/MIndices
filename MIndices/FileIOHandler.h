#pragma once
#include <string>
#include <vector>

#include "Array.h"
#include "IFileIOHandler.h"
#include "Structures.h"

class FileIOHandler : public IFileIOHandler
{
public:
	FileIOHandler() = default;
	~FileIOHandler() override = default;

	int readFile(const std::string& filename, MIndices::Array &voxelArray) override;
	int exportToFile(std::string& filename, const MIndices::Array& voxelArray) override;
	int exportToFile(std::string& filename, const std::vector<MIndices::Triangle>& triArray) override;
};
