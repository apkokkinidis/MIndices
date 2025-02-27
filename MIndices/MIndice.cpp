#include "MIndice.h"
#include "FileIOHandler.h"

using namespace MIndices;

MIndice::MIndice(size_t dim_x, size_t dim_y, size_t dim_z, IFileIOHandler &fileIOHandler) : dim_x(dim_x), dim_y(dim_y), dim_z(dim_z) , fileIOHandler(fileIOHandler)
{
	arr = std::make_unique<Array>(dim_x, dim_y, dim_z);
	mCubes = nullptr;
	bvh = nullptr;
}

void MIndice::Init(const std::string& filename)
{
	fileIOHandler.readFile(filename, *arr);
}

//Initialization method for the marching cubes object.
//@deleteArr provides option to deallocate memory used for the binary voxel array
void MIndices::MIndice::InitMCubes(bool deleteArr)
{
	mCubes = new MarchingCubes(std::move(arr), dim_x, dim_y, dim_z);
	mCubes->TriangulateCubes(triArr);
	if (deleteArr)
	{
		arr.reset(nullptr);	//free memory
	}
}

void MIndices::MIndice::InitBVH()
{
	bvh = new BVHTree();
	bvh->TopDownBuildObjectMedian(triangleMesh);
}

void MIndice::PrintVoxels(std::string& filename)
{
	if (arr)
	{
		fileIOHandler.exportToFile(filename, *arr);
	}
}

void MIndice::TriangulateArray()
{
	if (mCubes != nullptr)
	{
		mCubes->TriangulateCubes(triangleMesh);
	}
}
