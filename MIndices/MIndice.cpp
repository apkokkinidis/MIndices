#include "MIndice.h"
#include "FileIOHandler.h"
#include "ThreadPool.h"

#include <mutex>

using namespace MIndices;

//State map can be configured to allow multiple valid transitions. Currently only 1 valid transition is allowed.
const std::unordered_map<InitState, std::unordered_set<InitState>> MIndice::stateMap = {
	{InitState::initial,			{InitState::vector3DInit}},
	{InitState::vector3DInit,		{InitState::marchingCubesInit}},
	{InitState::marchingCubesInit,	{InitState::bvhInit}},
	{InitState::bvhInit,			{InitState::rayGridInit}},
	{InitState::rayGridInit,		{InitState::computeIndice}},
	{InitState::computeIndice,		{InitState::complete}},
	{InitState::complete,			{}},
	{InitState::failed,				{}}
};

MIndice::MIndice(size_t dim_x, size_t dim_y, size_t dim_z,
	IFileIOHandler& fileIOHandler, ParallelThreads threadOpts) : dim_x(dim_x), dim_y(dim_y), dim_z(dim_z),
	fileIOHandler(fileIOHandler),
	threadOpts(threadOpts)
{
	vector3D = std::make_unique<Vector3D>(dim_x, dim_y, dim_z);
	mCubes = nullptr;
	bvh = nullptr;
	rayGrid = nullptr;
}

void MIndice::Init(const std::string& filename)
{
	if (fileIOHandler.readFile(filename, *vector3D))
	{
		UpdateState(InitState::vector3DInit, "Init");
	}
	else
	{
		state = InitState::failed; //force state to failed when there is file read error.
	}
}

//Initialization method for the marching cubes object.
//@deleteArr provides option to deallocate memory used for the binary voxel array
void MIndices::MIndice::InitMCubes(bool deleteArr)
{
	UpdateState(InitState::marchingCubesInit, "InitMCubes");
	mCubes = std::make_unique<MarchingCubes>(std::move(vector3D), dim_x, dim_y, dim_z);
	mCubes->TriangulateVCubes(triArr);
	if (deleteArr)
	{
		vector3D.reset(nullptr);	//free memory
	}
	std::cout << "Initialized Marching Cubes, Triangles : " << triArr.size() << std::endl;
}

void MIndices::MIndice::InitBVH()
{
	UpdateState(InitState::bvhInit, "InitBvh");
	bvh = std::make_unique<BVHTree>();
	bvh->TopDownBuildObjectMedian(triArr);
	if (bvh->TreeIsEmpty())
	{
		state = InitState::failed;
		throw std::runtime_error("Failed to build BVH tree.");
	};
	std::cout << "Initialized Bounding Volume Hierarchy" << std::endl;
}

void MIndices::MIndice::InitRayGrid()
{
	UpdateState(InitState::rayGridInit, "InitRayGrid");
	rayGrid = std::make_unique<RayGrid>(dim_x, dim_y, bvh->GetRoot()->Box().Diagonal());
	if (rayGrid->empty())
	{
		state = InitState::failed;
		throw std::runtime_error("Failed to create ray grid.");
	}
	std::cout << "Initialized Ray Grid" << std::endl;
}

void MIndice::printVoxels(std::string& filename)
{
	if (vector3D)
	{
		fileIOHandler.exportToFile(filename, *vector3D);
	}
}

void MIndices::MIndice::printPairs(const std::string& filename) const
{
	if (!pairs.empty())
	{
		fileIOHandler.exportToFile(filename, pairs);
	}
	else
	{
		throw std::runtime_error("Failure: empty set of pairs.");
	}
}

void MIndices::MIndice::printBvhDepth(const std::string& filename) const
{
	BVHNode* tree = bvh->GetRoot();
	int32_t treeDepth = bvh->FindDepth(tree);
	int32_t numNodes = 0;
	int32_t numLeaves = 0;
	bvh->DFSTraverse(tree, numNodes, numLeaves);
	std::cout << "Tree depth : " << treeDepth << " No.Nodes : " << numNodes << " No.Leaves : " << numLeaves << std::endl;
}

int32_t MIndices::MIndice::ComputeIndice()
{
	UpdateState(InitState::computeIndice, "ComputeIndice");
	if (bvh->TreeIsEmpty() || rayGrid->empty()) { return 1; }

	pairs.reserve(ELEVATION * AZIMUTH);

	if (threadOpts == ParallelThreads::off)
	{
		return ComputeIndiceDefault();
	}
	else
	{
		try
		{
			ComputeIndiceParallel();
		}
		catch (std::runtime_error& e)
		{
			std::cerr << e.what() << std::endl;
		}
	}
	return 0;
}

int32_t MIndices::MIndice::ComputeIndiceDefault()
{
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
	UpdateState(InitState::complete, "ComputeIndice");
	return 0;
}

int32_t MIndices::MIndice::ComputeIndiceParallel()
{
	try
	{
		ThreadPool pool(threadOpts, rayGrid->getRaySpan().size());
		for (int32_t elev = 0; elev < ELEVATION; ++elev)
		{
			std::chrono::high_resolution_clock::time_point start_f = std::chrono::high_resolution_clock::now();
			auto azimuthRays = rayGrid->ComputeAzimuthRays();
			for (int32_t azmth = 0; azmth < azimuthRays.size(); ++azmth)
			{
				pool.EnqueTask([this, azmth, elev, azimuthRays]()
					{
						std::vector<VecPoint3D> points;
						RayTraceBVHNodes(points, azimuthRays[azmth]);
						AnglePair pair{ azmth, elev };
						pair.CalculateIndices(points);
						std::unique_lock<std::mutex> lock(mutex);
						pairs.push_back(pair);
						lock.unlock();
					});
			}
			std::chrono::high_resolution_clock::time_point stop_f = std::chrono::high_resolution_clock::now();
			std::chrono::seconds dur = std::chrono::duration_cast<std::chrono::seconds>(stop_f - start_f);
			std::cout << "Elevation : " << elev + 1 << " from: " << ELEVATION << " duration : " << dur.count() << std::endl;
			rayGrid->RotateRays(DEGREE, Axis::X);
		}
		//sort the pairs
		std::sort(pairs.begin(), pairs.end(), &AnglePair::SortPairs);
	}
	catch (std::runtime_error& e)
	{
		std::cerr << e.what() << std::endl;
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

int32_t MIndices::MIndice::RayTraceBVHNodes(std::vector<VecPoint3D>& outPoints, const std::vector<Ray>& rays)
{
	BVHNode* root = bvh->GetRoot();
	for (const auto& ray : rays)
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

bool MIndices::MIndice::IsValidState(InitState newState) const
{
	auto it = stateMap.find(state);
	return it != stateMap.end() ? it->second.contains(newState) : false;
}

void MIndices::MIndice::UpdateState(const InitState newState, std::string&& func_name)
{
	if (!IsValidState(newState))
	{
		throw std::runtime_error(func_name + " Error, incorrect state transititon "
			+ std::to_string(static_cast<int32_t>(state)) + " -> "
			+ std::to_string(static_cast<int32_t>(newState)));
	}
	state = newState;
}
