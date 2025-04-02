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

	int32_t readFile(const std::string& filename, MIndices::Array &voxelArray) override;
	int32_t exportToFile(const std::string& filename, const MIndices::Array& voxelArray) override;
	int32_t exportToFile(const std::string& filename, const std::vector<MIndices::Triangle>& triArray) override;
	int32_t exportToFile(const std::string& filename, const std::vector<MIndices::AnglePair>& pairs) override;
};
