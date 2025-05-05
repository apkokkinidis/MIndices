#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <iterator>
#include <omp.h>
#include <chrono>

//custom headers
#include "Vector3D.h"
#include "Array3D.h"
#include "MarchingCubes.h"
#include "Ray.h"
#include "RayGrid.h"
#include "BVHTree.h"
#include "AnglePair.h"
#include "Structures.h"
#include "IFileIOHandler.h"

namespace MIndices
{

	enum class InitState
	{
		failed,
		initial,
		vector3DInit,
		marchingCubesInit,
		bvhInit,
		rayGridInit,
		computeIndice,
		complete,
		count
	};

	class MIndice
	{
	public:
		MIndice(size_t dim_x, size_t dim_y, size_t dim_z, IFileIOHandler& fileIOHandler);
		MIndice(const MIndice& other) = delete;
		~MIndice() = default;

		void Init(const std::string& filename);
		void InitMCubes(bool deleteArr = true);
		void InitBVH();
		void InitRayGrid();

		void printVoxels(std::string& filename);
		void printPairs(const std::string& filename) const;
		void printBvhDepth(const std::string& filename) const;
		int32_t ComputeIndice();
	private:
		int32_t RayTraceBVHNodes(std::vector<VecPoint3D>& outPoints);
		std::unique_ptr<Vector3D> vector3D;
		std::vector<Triangle> triArr;
		std::vector<AnglePair> pairs;
		IFileIOHandler& fileIOHandler;
		std::unique_ptr<MarchingCubes> mCubes;
		std::unique_ptr<RayGrid> rayGrid;
		std::unique_ptr<BVHTree> bvh;
		size_t dim_x, dim_y, dim_z;

		InitState state = InitState::initial;
		bool IsValidState(InitState expected) const;
		static const std::unordered_map<InitState, std::unordered_set<InitState>> stateMap;
		void UpdateState(const InitState newState, std::string&& func_name);
	};

}

