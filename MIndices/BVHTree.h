// Author Kokkinidis Anastasios
//BVHTree.h
#pragma once

#include "BoundingBox.h"
#include "Structures.h"
#include "BVHNode.h"
#include <vector>
#include <stack>
#include <assert.h>
#include <omp.h>

namespace MIndices
{
	constexpr uint32_t MAX_TRIANGLES_PER_LEAF = 1;

	class BVHTree
	{
	public:
		BVHTree();
		~BVHTree();

		//Hierarchy construction functions
		//Top-down build methods using standard recursion
		void TopDownBuildObjectMedian(std::vector<Triangle>& triangles) noexcept;
		void TopDowwBuildObjectMedian_Parralel(std::vector<Triangle>& triangles) noexcept;

		//Bottom-up build methods
		void BottomUpBuild(std::vector<Triangle>& triangles) noexcept;	//ToDo implementation missing

		//Compute bounds functions
		BoundingBox3D ComputeBounds(const std::vector<Triangle>& triangles) const;
		BoundingBox3D ComputeBounds(const Triangle& triangle) const;

		//utility functions
		float CalculateSurfaceAreaOfBox(const BoundingBox3D& box) const noexcept;
		int32_t FindDepth(BVHNode* node) const noexcept;
		bool TreeIsEmpty() const noexcept;
		Axis FindLongestAxisOfBBox(const BoundingBox3D& box) const noexcept;
		void CopyTriangles(std::vector<Triangle>& triDest, const std::vector<Triangle>& triOrig, size_t start, size_t end) const noexcept;

		//traversal functions
		void RayTraceNodes(BVHNode* node, const Ray& r, std::vector<Point3D>& outpoints) const noexcept;
		void RayTraceNodesPreEdges(BVHNode* node, const Ray& r, std::vector<Point3D>& outpoints, std::vector<double>& outT) const noexcept;	//ToDO unused method
		void DFSTraverse(BVHNode* node, int32_t& visitedNodes, int32_t& visitedLeafs) const noexcept;
		void PrecomputeEdges(BVHNode* node, int32_t& out);	//ToDO unused method


		//Returns a Pointer to the root of the tree
		BVHNode* GetRoot();
		void SetRoot(BVHNode* node);

	private:
		BVHNode* root;

		//Hierarchy construction functions
		//Top-down build methods using standard recursion
		void TopDownBuildObjectMedian(BVHNode* pnode, std::vector<Triangle>& triangles) noexcept;

		//Partitions a vector of triangles into 2 sets
		size_t PartitionSet(const std::vector<Triangle>& triangles, std::vector<Triangle>& S1, std::vector<Triangle>& S2) const;

		//sorting functions
		void QuickSortTri(std::vector<Triangle>& tri, int32_t lo, int32_t hi, Axis axis) noexcept;
		COORD_TYPE SelectPointAxis(const Point3D& p, Axis axis) noexcept;
		int32_t partitionTriXAxis(std::vector<Triangle>& tri, int32_t lo, int32_t hi) noexcept;
		int32_t partitionTriYAxis(std::vector<Triangle>& tri, int32_t lo, int32_t hi) noexcept;
		int32_t partitionTriZAxis(std::vector<Triangle>& tri, int32_t lo, int32_t hi) noexcept;

		//Compute centroid functions
		Point3D ComputeCentroidOfTriangle(const Triangle& t) const noexcept;
		Point3D ComputeCentroidOfBoundingBox(const BoundingBox3D& box) const noexcept;

		//memory functions
		void DeleteSubTree(BVHNode* node);
	};
}
