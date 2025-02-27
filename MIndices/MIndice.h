#pragma once
#include <string>
#include <vector>
#include <iterator>
#include <omp.h>
#include <chrono>

//custom headers
#include "Array.h"
#include "Array3D.h"
#include "MarchingCubes.h"
#include "Ray.h"
#include "BVHTree.h"
#include "AnglePair.h"
#include "Structures.h"
#include "IFileIOHandler.h"

namespace MIndices
{
	enum class initState
	{
		noInit,
		initDefault,
		initMCubes,
		initBVH,
		RayGeneration,
		IndiceComputation
	};

	class MIndice
	{
	public:
		MIndice(size_t dim_x, size_t dim_y, size_t dim_z, IFileIOHandler& fileIOHandler);
		MIndice(const MIndice& other) = delete;
		~MIndice() = default;

		void Init(const std::string& filename);
		void InitMCubes(bool deleteArr);
		void InitBVH();

		void PrintVoxels(std::string& filename);

		void TriangulateArray();
	private:
		std::unique_ptr<Array> arr;
		std::vector<Triangle> triArr;
		IFileIOHandler& fileIOHandler;
		MarchingCubes* mCubes;
		BVHTree* bvh;
		std::vector<Triangle> triangleMesh;
		size_t dim_x, dim_y, dim_z;
	};

}

