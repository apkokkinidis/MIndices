#pragma once
#include <string>
#include <vector>
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
		count
	};

	class MIndice
	{
	public:
		MIndice(size_t dim_x, size_t dim_y, size_t dim_z, IFileIOHandler& fileIOHandler);
		MIndice(const MIndice& other) = delete;
		~MIndice() = default;

		void Init(const std::string& filename);
		void InitMCubes(bool deleteArr = false);
		void InitBVH();
		void InitRayGrid();

		void printVoxels(std::string& filename);
		void printPairs(const std::string& filename) const;
		void printBvhDepth(const std::string& filename) const;
		int32_t ComputeIndice();
	private:
		void ExpectState(InitState expected, std::string&& func_name) const;
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
	};

}

