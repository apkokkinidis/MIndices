// Author Kokkinidis Anastasios
//BVHTree.h
#pragma once
#ifndef _BVHTree
#define _BVHTree
#include "BoundingBox.h"
#include "Structures.h"
#include "BVHNode.h"
#include <vector>
#include <assert.h>
#include <omp.h>

using std::vector;
const int MAX_TRIANGLES_PER_LEAF = 4;

class BVHTree
{
private:
	BVHNode* root;
public:
	BVHTree();
	~BVHTree();

	//Hierarchy construction functions
	//Top-down build methods
	void TopDownBuildObjectMedian(vector<TRIANGLE> &triangles) noexcept;
	void TopDowwBuildObjectMedian_Parralel(vector<TRIANGLE> &triangles) noexcept;
	void TopDownBuildObjectMedian(BVHNode* pnode, vector<TRIANGLE> &triangles) noexcept;

	//Compute bounds functions
	BoundingBox3D ComputeBounds(const vector<TRIANGLE>& triangles) const;
	BoundingBox3D ComputeBounds(const TRIANGLE& triangle) const;

	//Compute centroid functions
	Point3D ComputeCentroidOfTriangle(const TRIANGLE& t) const noexcept;
	Point3D ComputeCentroidOfBoundingBox(const BoundingBox3D& box) const noexcept;

	//sorting functions
	void QuickSortTri(vector<TRIANGLE>& tri, int lo, int hi, Axis axis) noexcept;
	size_t partitionTriXAxis(vector<TRIANGLE>& tri, int lo, int hi) noexcept;
	size_t partitionTriYAxis(vector<TRIANGLE>& tri, int lo, int hi) noexcept;
	size_t partitionTriZAxis(vector<TRIANGLE>& tri, int lo, int hi) noexcept;

	//utility functions
	float CalculateSurfaceAreaOfBox(const BoundingBox3D& box) const noexcept;
	int FindDepth(BVHNode* node) const noexcept;
	bool TreeIsEmpty() const noexcept;
	Axis FindLongestAxisOfBBox(const BoundingBox3D& box) const noexcept;
	void CopyTriangles(vector<TRIANGLE>& triDest, const vector<TRIANGLE>& triOrig, size_t start, size_t end) const noexcept;

	//traversal functions
	void RayTraceNodes(BVHNode* node, const Ray& r, vector<Point3D>& outPoints) const noexcept;
	void RayTraceNodesPreEdges(BVHNode* node, const Ray& r, vector<Point3D>& outPoints, vector<double>& outT) const noexcept;
	void DFSTraverse(BVHNode* node, int& visitedNodes, int& visitedLeafs) const noexcept;
	void PrecomputeEdges(BVHNode* node, int& out);

	//Partitions a vector of triangles into 2 sets
	size_t PartitionSet(const vector<TRIANGLE>& triangles, vector<TRIANGLE>& S1, vector<TRIANGLE>& S2) const;

	//Returns a pointer to the root of the tree
	BVHNode* GetRoot();

	//memory functions
	void DeleteSubTree(BVHNode* node);
	void DeleteTree(BVHNode** node);
};
#endif _BVHTree
