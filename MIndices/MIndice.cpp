#include "MIndice.h"
#include "FileIOHandler.h"

using namespace MIndices;

MIndice::MIndice(size_t dim_x, size_t dim_y, size_t dim_z, IFileIOHandler& fileIOHandler) : dim_x(dim_x), dim_y(dim_y), dim_z(dim_z), fileIOHandler(fileIOHandler)
{
	arr = std::make_unique<Vector3D>(dim_x, dim_y, dim_z);
	mCubes = nullptr;
	bvh = nullptr;
	rayGrid = nullptr;
}

void MIndice::Init(const std::string& filename)
{
	fileIOHandler.readFile(filename, *arr);
}

//Initialization method for the marching cubes object.
//@deleteArr provides option to deallocate memory used for the binary voxel array
void MIndices::MIndice::InitMCubes(bool deleteArr)
{
	mCubes = std::make_unique<MarchingCubes>(std::move(arr), dim_x, dim_y, dim_z);
	mCubes->TriangulateVCubes(triArr);
	if (deleteArr)
	{
		arr.reset(nullptr);	//free memory
	}
}

void MIndices::MIndice::InitBVH()
{
	bvh = std::make_unique<BVHTree>();
	bvh->TopDownBuildObjectMedian(triArr);
	if (bvh->TreeIsEmpty())
		std::cerr << "Failed to build BVH." << std::endl;
	BVHNode* tree = bvh->GetRoot();
	int32_t treeDepth = bvh->FindDepth(tree);
	int32_t numNodes = 0;
	int32_t numLeaves = 0;
	bvh->DFSTraverse(tree, numNodes, numLeaves);
	std::cout << "Tree depth : " << treeDepth << " No.Nodes : " << numNodes << " No.Leaves : " << numLeaves << std::endl;
}

void MIndices::MIndice::InitRayGrid()
{
	rayGrid = std::make_unique<RayGrid>(dim_x, dim_y, bvh->GetRoot()->Box().Diagonal());
}

void MIndice::PrintVoxels(std::string& filename)
{
	if (arr)
	{
		fileIOHandler.exportToFile(filename, *arr);
	}
}

void MIndices::MIndice::printPairs(const std::string& filename) const
{
	if (!pairs.empty())
	{
		fileIOHandler.exportToFile(filename, pairs);
	}
}

int32_t MIndices::MIndice::ComputeIndice()
{
	if (bvh->TreeIsEmpty() || rayGrid->empty()) { return 1; }

	pairs.reserve(ELEVATION * AZIMUTH);

	for (int32_t e = 0; e < ELEVATION; ++e)
	{
		std::chrono::high_resolution_clock::time_point start_f = std::chrono::high_resolution_clock::now();
		for (int32_t a = 0; a < AZIMUTH; ++a)
		{
			std::vector<VecPoint3D> points;
			RayTraceBVHNodes(points);

			AnglePair pair{ a, e };
			pair.CalculateIndices(points);

			pairs.push_back(pair);
			rayGrid->RotateRays(DEGREE, Axis::X);
		}
		rayGrid->RotateRays(DEGREE, Axis::Z);
		std::chrono::high_resolution_clock::time_point stop_f = std::chrono::high_resolution_clock::now();
		std::chrono::seconds dur = std::chrono::duration_cast<std::chrono::seconds>(stop_f - start_f);
		std::cout << "Cycle " << e + 1 << " from: " << ELEVATION << " duration: " << dur.count() << " sec" << std::endl;
	}
	return 0;
}

int32_t MIndices::MIndice::RayTraceBVHNodes(std::vector<VecPoint3D>& outPoints)
{
	if (rayGrid->empty()) { return 1; }

	if (bvh->TreeIsEmpty()) { return 2; }

	BVHNode* root = bvh->GetRoot();
	for (const auto& ray : rayGrid->getRaySpan())
	{
		VecPoint3D points;
		bvh->RayTraceNodes(root, ray, points);
		if (!points.empty() && points.size() % 2 == 0)
		{
			//sort points based on the distance from the origin of the ray
			ray.sortVecPoint3D(points);
			outPoints.push_back({ points });
		}
	}
	return 0;
}
