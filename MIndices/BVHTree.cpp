// Author Kokkinidis Anastasios
//BVHTree.cpp
#include <stdint.h>
#include "BVHTree.h"

using namespace MIndices;

BVHTree::BVHTree()
{
	root = nullptr;
}

BVHTree::~BVHTree()
{
	DeleteTree(&root);
}

BVHNode* BVHTree::GetRoot()
{
	return root;
}

void BVHTree::DeleteTree(BVHNode** tree)
{
	DeleteSubTree(*tree);
	*tree = nullptr;
}


void BVHTree::DeleteSubTree(BVHNode* node)
{
	if (node == nullptr)
	{
		return;
	}
	DeleteSubTree(node->left);
	DeleteSubTree(node->right);
	delete node;
}

bool BVHTree::TreeIsEmpty() const noexcept
{
	return root == nullptr;
}

BoundingBox3D BVHTree::ComputeBounds(const Triangle& triangle) const
{
	BoundingBox3D* bbox = NULL;
	Point3D minPoint3D, maxPoint3D;
	minPoint3D = triangle.p[0];
	maxPoint3D = triangle.p[0];

	for (int32_t i = 1; i <= 2; i++)
	{
		//find min
		if (triangle.p[i].x < minPoint3D.x)
		{
			minPoint3D.x = triangle.p[i].x;
		}
		if (triangle.p[i].y < minPoint3D.y)
		{
			minPoint3D.y = triangle.p[i].y;
		}
		if (triangle.p[i].z < minPoint3D.z)
		{
			minPoint3D.z = triangle.p[i].z;
		}
		//find max
		if (triangle.p[i].x > maxPoint3D.x)
		{
			maxPoint3D.x = triangle.p[i].x;
		}
		if (triangle.p[i].y > maxPoint3D.y)
		{
			maxPoint3D.y = triangle.p[i].y;
		}
		if (triangle.p[i].z > maxPoint3D.z)
		{
			maxPoint3D.z = triangle.p[i].z;
		}
	}
	bbox = new BoundingBox3D((size_t)minPoint3D.x,
		(size_t)minPoint3D.y,
		(size_t)minPoint3D.z,
		(size_t)maxPoint3D.x,
		(size_t)maxPoint3D.y,
		(size_t)maxPoint3D.z);
	return *bbox;
}

BoundingBox3D BVHTree::ComputeBounds(const vector<Triangle>& triangles) const
{
	assert(triangles.size() > 0);
	BoundingBox3D* bbox = NULL;
	Point3D mint32_triPoint, maxTriPoint;
	mint32_triPoint = triangles[0].p[0];
	maxTriPoint = triangles[0].p[0];
	for (size_t i = 0; i < triangles.size() - 1; i++)
	{
		for (size_t j = 1; j <= 2; j++)
		{
			//find min
			if (triangles[i].p[j].x < mint32_triPoint.x)
			{
				mint32_triPoint.x = triangles[i].p[j].x;
			}
			if (triangles[i].p[j].y < mint32_triPoint.y)
			{
				mint32_triPoint.y = triangles[i].p[j].y;
			}
			if (triangles[i].p[j].z < mint32_triPoint.z)
			{
				mint32_triPoint.z = triangles[i].p[j].z;
			}
			//find max
			if (triangles[i].p[j].x > maxTriPoint.x)
			{
				maxTriPoint.x = triangles[i].p[j].x;
			}
			if (triangles[i].p[j].y > maxTriPoint.y)
			{
				maxTriPoint.y = triangles[i].p[j].y;
			}
			if (triangles[i].p[j].z > maxTriPoint.z)
			{
				maxTriPoint.z = triangles[i].p[j].z;
			}
		}
	}
	bbox = new BoundingBox3D((size_t)mint32_triPoint.x,
		(size_t)mint32_triPoint.y,
		(size_t)mint32_triPoint.z,
		(size_t)maxTriPoint.x,
		(size_t)maxTriPoint.y,
		(size_t)maxTriPoint.z);
	return *bbox;
}

void BVHTree::TopDownBuildObjectMedian(vector<Triangle>& triangles) noexcept
{
	if (triangles.size() > MAX_TRIANGLES_PER_LEAF)
	{
		//Insert root node
		BoundingBox3D rootBox = ComputeBounds(triangles);
		root = new BVHNode(rootBox, triangles);
		//Find longest axis
		Axis longestAxis = FindLongestAxisOfBBox(rootBox);

		//Sort elements along the longest axis
		QuickSortTri(triangles, 0, (int32_t)triangles.size() - 1, longestAxis);

		//PartitionArray
		vector<Triangle> S1, S2;
		size_t k = PartitionSet(triangles, S1, S2);

		BoundingBox3D lbox = ComputeBounds(S1);
		root->left = new BVHNode(lbox, S1);

		BoundingBox3D rbox = ComputeBounds(S2);
		root->right = new BVHNode(rbox, S2);

		//Free memory from parent so that only leaf nodes contain triangular data
		root->ClearTriangles();

		//recursively build left subtree
		TopDownBuildObjectMedian(root->left, S1);
		//recursively build right subtree
		TopDownBuildObjectMedian(root->right, S2);
	}
}

void BVHTree::TopDownBuildObjectMedian(BVHNode* pnode, vector<Triangle>& triangles) noexcept
{
	if (triangles.size() > MAX_TRIANGLES_PER_LEAF)
	{
		BoundingBox3D bbox = ComputeBounds(triangles);
		//Find longest axis
		Axis longestAxis = FindLongestAxisOfBBox(bbox);

		//Sort elements along the longest axis
		QuickSortTri(triangles, 0, (int32_t)triangles.size() - 1, longestAxis);

		//PartitionArray into two sets
		vector<Triangle> S1, S2;
		size_t k = PartitionSet(triangles, S1, S2);

		//Compute left child bounding box
		BoundingBox3D lbox = ComputeBounds(S1);
		pnode->left = new BVHNode(lbox, S1);

		//Compute right child bounding box
		BoundingBox3D rbox = ComputeBounds(S2);
		pnode->right = new BVHNode(rbox, S2);

		//Free memory from parent so that only leaf nodes contain triangular data
		pnode->ClearTriangles();

		//recursively build left subtree
		TopDownBuildObjectMedian(pnode->left, S1);
		//recursively build right subtree
		TopDownBuildObjectMedian(pnode->right, S2);
	}
}

void BVHTree::BottomUpBuild(vector<Triangle>& triangles) noexcept
{
	//ToDO implement bottom up hierarchy construction.
}


void BVHTree::TopDowwBuildObjectMedian_Parralel(vector<Triangle>& triangles) noexcept
{
	const int32_t NUM_OF_THRDS = 2;

	if (triangles.size() > MAX_TRIANGLES_PER_LEAF)
	{
		//Insert root node
		BoundingBox3D rootBox = ComputeBounds(triangles);
		root = new BVHNode(rootBox, triangles);
		//Find longest axis
		Axis longestAxis = FindLongestAxisOfBBox(rootBox);

		//Sort elements along the longest axis
		QuickSortTri(triangles, 0, (int32_t)triangles.size() - 1, longestAxis);

		//PartitionArray
		vector<Triangle> S1, S2;
		size_t k = PartitionSet(triangles, S1, S2);

		BoundingBox3D lbox = ComputeBounds(S1);
		root->left = new BVHNode(lbox, S1);

		BoundingBox3D rbox = ComputeBounds(S2);
		root->right = new BVHNode(rbox, S2);

		//parallel region
		omp_set_num_threads(NUM_OF_THRDS);
#pragma omp parallel
		{
			//check the num of threads the system has given
			int32_t nthrds = omp_get_num_threads();
			if (nthrds > 1)
			{
				int32_t id = omp_get_thread_num();
				switch (id)
				{
				case 0:
					//recursively build left subtree
					TopDownBuildObjectMedian(root->left, S1);
					break;
				case 1:
					//recursively build right subtree
					TopDownBuildObjectMedian(root->right, S2);
					break;
				default:
					break;
				}
			}
			else
			{
				//recursively build left subtree
				TopDownBuildObjectMedian(root->left, S1);
				//recursively build right subtree
				TopDownBuildObjectMedian(root->right, S2);
			}
		}
	}
}

//Partitions set of triangles in two sets using the median Point of the set
size_t BVHTree::PartitionSet(const vector<Triangle>& triangles, vector<Triangle>& S1, vector<Triangle>& S2) const
{
	//Split the object at the object median of the longest axis
	size_t median = static_cast<size_t>(triangles.size() * (COORD_TYPE)0.5);
	CopyTriangles(S1, triangles, 0, median);
	CopyTriangles(S2, triangles, median, triangles.size());

	return median;
}

//Finds the longest axis for a given 3D bounding box starting with x axis
Axis BVHTree::FindLongestAxisOfBBox(const BoundingBox3D& box) const noexcept
{
	uint32_t X = (int32_t)box.Width();
	uint32_t Y = (int32_t)box.Height();;
	uint32_t Z = (int32_t)box.Depth();

	Axis longestAxis = Axis::X;
	uint32_t max = X;
	if (Y > max)
	{
		max = Y;
		longestAxis = Axis::Y;
	}
	if (Z > max)
	{
		max = Z;
		longestAxis = Axis::Z;
	}
	return longestAxis;
}

//Copies triangles to subarray at the median of the original array
void BVHTree::CopyTriangles(vector<Triangle>& triDest, const vector<Triangle>& triSource, size_t indx_start, size_t indx_end) const noexcept
{
	assert(triSource.size() > 0);

	//assign the size up to the median Point
	size_t newCapacity = triSource.size() / 2;
	triDest.reserve(newCapacity);
	for (size_t i = indx_start; i < indx_end; i++)
	{
		triDest.push_back(triSource[i]);
	}
}

//Computes the centroid of a triangle
Point3D BVHTree::ComputeCentroidOfTriangle(const Triangle& t) const noexcept
{
	COORD_TYPE xCenter = (t.p[0].x + t.p[1].x + t.p[2].x) / 3.0f;
	COORD_TYPE yCenter = (t.p[0].y + t.p[1].y + t.p[2].y) / 3.0f;
	COORD_TYPE zCenter = (t.p[0].z + t.p[1].z + t.p[2].z) / 3.0f;
	return { xCenter, yCenter, zCenter };
}

//Computes the centroid of a 3D bounding box data type
Point3D BVHTree::ComputeCentroidOfBoundingBox(const BoundingBox3D& box) const noexcept
{
	COORD_TYPE xCenter = (COORD_TYPE)box.Width() * 0.5f;
	COORD_TYPE yCenter = (COORD_TYPE)box.Height() * 0.5f;
	COORD_TYPE zCenter = (COORD_TYPE)box.Depth() * 0.5f;
	Point3D centroid = { xCenter, yCenter, zCenter };
	return centroid;
}

float BVHTree::CalculateSurfaceAreaOfBox(const BoundingBox3D& box) const noexcept
{
	size_t frontArea = box.Width() * box.Height();	// x * y
	size_t topArea = box.Width() * box.Depth();	//x * z
	size_t edgeArea = box.Height() * box.Depth(); //y * z
	return (float)(2 * frontArea) + (2 * topArea) + (2 * edgeArea);
}

//Quick Sort Algorithm
void BVHTree::QuickSortTri(vector<Triangle>& tri, int32_t lo, int32_t hi, Axis axis) noexcept
{
	if (lo < hi)
	{
		int32_t partition_idx = 0;
		switch (axis)
		{
		case Axis::X: { partition_idx = partitionTriXAxis(tri, lo, hi); break; }	//partition on x axis
		case Axis::Y: { partition_idx = partitionTriYAxis(tri, lo, hi); break; }	//partition on y axis
		case Axis::Z: { partition_idx = partitionTriZAxis(tri, lo, hi); break; }	//partition on z axis
		default: break;
		}
		QuickSortTri(tri, lo, partition_idx, axis);
		QuickSortTri(tri, partition_idx + 1, hi, axis);
	}
}

COORD_TYPE MIndices::BVHTree::SelectPointAxis(const Point3D& p, Axis axis) noexcept
{
	switch (axis)
	{
	case Axis::X: { return p.x; }
	case Axis::Y: { return p.y; }
	case Axis::Z: { return p.z; }
	default: break;
	}
	return COORD_TYPE(0.0f);
}

//Partitions set using the centroid of the triangles along the x axis
int32_t BVHTree::partitionTriXAxis(vector<Triangle>& tri, int32_t lo, int32_t hi) noexcept
{
	int32_t pivot_ind = lo;
	int32_t i = lo - 1;
	int32_t j = hi + 1;
	Point3D p_i = { 0,0,0 };
	Point3D p_j = { 0,0, 0 };
	Point3D pivot = ComputeCentroidOfTriangle(tri.at(pivot_ind));
	while (i < j)
	{
		do
		{
			++i;
			p_i = ComputeCentroidOfTriangle(tri[i]);
		} while (p_i.x < pivot.x);
		do
		{
			--j;
			p_j = ComputeCentroidOfTriangle(tri[j]);
		} while (p_j.x > pivot.x);
		if (i >= j)
		{
			return j;
		}
		std::swap(tri[i], tri[j]);
		//swap i with j
	}
	return 0;
}

//Partitions set using the centroid of the triangles along the y axis
int32_t BVHTree::partitionTriYAxis(vector<Triangle>& tri, int32_t lo, int32_t hi) noexcept
{
	size_t pivot_ind = lo;
	int32_t i = lo - 1;
	int32_t j = hi + 1;
	Point3D p_i = { 0, 0, 0 };
	Point3D p_j = { 0, 0, 0 };

	Point3D pivot = ComputeCentroidOfTriangle(tri.at(pivot_ind));
	while (i < j)
	{
		do
		{
			++i;
			p_i = ComputeCentroidOfTriangle(tri[i]);
		} while (p_i.y < pivot.y);
		do
		{
			--j;
			p_j = ComputeCentroidOfTriangle(tri[j]);
		} while (p_j.y > pivot.y);
		if (i >= j)
		{
			return j;
		}
		std::swap(tri[i], tri[j]);
		//swap i with j
	}
	return 0;
}

//Partitions set using the centroid of the triangles along the z axis
int32_t BVHTree::partitionTriZAxis(vector<Triangle>& tri, int32_t lo, int32_t hi) noexcept
{
	size_t pivot_ind = lo;
	int32_t i = lo - 1;
	int32_t j = hi + 1;
	Point3D p_i = { 0, 0, 0 };
	Point3D p_j = { 0, 0, 0 };
	Point3D ppivot = ComputeCentroidOfTriangle(tri.at(pivot_ind));
	while (i < j)
	{
		do
		{
			++i;
			p_i = ComputeCentroidOfTriangle(tri[i]);
		} while (p_i.z < ppivot.z);
		do
		{
			--j;
			p_j = ComputeCentroidOfTriangle(tri[j]);
		} while (p_j.z > ppivot.z);
		if (i >= j)
		{
			return j;
		}
		std::swap(tri[i], tri[j]);
		//swap i with j
	}
	return 0;
}
//Traverse the tree using DFS and check for any intersections with the ray
void BVHTree::RayTraceNodes(BVHNode* node, const Ray& r, vector<Point3D>& outpoints) const noexcept
{
	if (node == nullptr)
	{
		return;
	}
	//check for a given ray if it intersects the bounding volume
	if (node->Box().RayBoxintersect(r))
	{
		if (node->IsLeafNode())
		{
			//check ray triangle intersection
			for (const auto &triangle : node->triangleSpan())
			{
				Point3D tmp_Point;
				double tmp_t;
				if (r.FastRayTriangleIntersection(triangle, tmp_Point, tmp_t))
				{
					outpoints.push_back(tmp_Point);
				}
			}
		}
		//traverse left subtree then backtrack to the right tree
		RayTraceNodes(node->left, r, outpoints);
		RayTraceNodes(node->right, r, outpoints);
	}
}

void BVHTree::RayTraceNodesPreEdges(BVHNode* node, const Ray& r, vector<Point3D>& outpoints, vector<double>& outT) const noexcept
{
	if (node == nullptr)
	{
		return;
	}
	//check for a given ray if it intersects
	if (node->Box().RayBoxintersect(r))
	{
		if (node->IsLeafNode())
		{
			if (node->HasComputedEdges())
			{
				for (vector<TriangleEdge>::iterator it = node->GetEdgeBegin(); it != node->GetEdgeEnd(); ++it)
				{
					Point3D tmp_Point;
					double tmp_t;
					if (r.FastRayTriangleIntersection(it->triangle, it->edges[0], it->edges[1], tmp_Point, tmp_t))
					{
						outpoints.push_back(tmp_Point);
						outT.push_back(tmp_t);
					}
				}
			}
			else
			{
				//check ray triangle intersection
				for (auto triangle = node->GetTriBegin(); triangle != node->GetTriEnd(); ++triangle)
				{
					Point3D tmp_Point;
					double tmp_t;
					if (r.FastRayTriangleIntersection(*triangle, tmp_Point, tmp_t))
					{
						outpoints.push_back(tmp_Point);
						outT.push_back(tmp_t);
					}
				}
			}
			//traverse left subtree then backtrack to the right tree
		}
		RayTraceNodesPreEdges(node->left, r, outpoints, outT);
		RayTraceNodesPreEdges(node->right, r, outpoints, outT);
	}
}

void BVHTree::DFSTraverse(BVHNode* node, int32_t& visitedNodes, int32_t& visitedLeafs) const noexcept
{
	//mark nodes as visited
	if (node == nullptr)
	{
		return;
	}
	if (node->IsLeafNode())
	{
		visitedLeafs++;
		//leaf has been visited
	}
	//node visited
	visitedNodes++;
	//traverse left subtree then backtrack to the right tree
	DFSTraverse(node->left, visitedNodes, visitedLeafs);
	DFSTraverse(node->right, visitedNodes, visitedLeafs);
}

void BVHTree::PrecomputeEdges(BVHNode* node, int32_t& out)
{
	if (node == nullptr)
	{
		return;
	}

	if (node->IsLeafNode())
	{
		out = node->BVHNode::ComputeEdges();
	}
	PrecomputeEdges(node->left, out);
	PrecomputeEdges(node->right, out);
}

//Finds the depth of the tree
int32_t BVHTree::FindDepth(BVHNode* node) const noexcept
{
	if (node == nullptr)
	{
		return 0;
	}
	int32_t maxLeft = FindDepth(node->left);
	int32_t maxRight = FindDepth(node->right);

	if (maxLeft > maxRight)
	{
		return maxLeft + 1;
	}
	else
	{
		return maxRight + 1;
	}
}
