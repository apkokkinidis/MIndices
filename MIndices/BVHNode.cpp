// Author Anastasios Kokkinidis 
//BVHNode.cpp
#include "BVHNode.h"

using namespace MIndices;

BVHNode::BVHNode(BoundingBox3D b, vector<Triangle> tri) : bbox(b), triangles(tri), right(nullptr), left(nullptr)
{
}

BVHNode::BVHNode(const BVHNode& other)
{
	if (this != &other)
	{
		triangles = other.triangles;
		bbox = other.bbox;
		this->right = other.right;
		this->left = other.left;
	}
}

BVHNode::~BVHNode()
{
	right = nullptr;
	left = nullptr;
	bbox.~BoundingBox3D();
	triangles.clear();
	if (!edges.empty())
	{
		edges.clear();
	}
}

bool HasEvenintersections(const vector<Triangle>& t)
{
	if (t.size() % 2 == 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}
void BVHNode::SetBBox(const BoundingBox3D& newBBox)
{
	bbox = newBBox;
}

void BVHNode::SetTriangles(const vector<Triangle>& triangles)
{
	this->triangles = triangles;
}

//Deletes triangles belonging to node.
void BVHNode::ClearTriangles()
{
	if (!triangles.empty())
	{
		triangles.clear();
	}
}

//Compute the edges of all the triangles in this node
int32_t BVHNode::ComputeEdges()
{
	size_t s = triangles.size();
	if (s > 0)
	{
		if (edges.empty())
		{
			edges.reserve(s * 2);
			for (auto triangle = triangles.begin(); triangle != triangles.end(); ++triangle)
			{
				Point3D edge[2]
				{
					triangle->p[1] - triangle->p[0],  triangle->p[2] - triangle->p[0]
				};
				edges.push_back({ *triangle, {edge[0], edge[1]} });
			}
			edges.shrink_to_fit();
			//All edges computed return 1
			return 1;
		}
	}
	return -1;	//Failed to compute edges
}

bool BVHNode::IsLeafNode() const
{
	return (nullptr == left && nullptr == right);
}

const BoundingBox3D& BVHNode::Box() const noexcept
{
	return bbox;
}

vector<Triangle> BVHNode::GetTriangles() const noexcept
{
	return triangles;
}

vector<TriangleEdge> BVHNode::GetEdges() const noexcept
{
	return edges;
}

std::vector<Triangle>::iterator BVHNode::GetTriBegin()
{
	return triangles.begin();
}

std::vector<Triangle>::iterator BVHNode::GetTriEnd()
{
	return triangles.end();
}

std::vector<TriangleEdge>::iterator BVHNode::GetEdgeBegin()
{
	return edges.begin();
}

std::vector<TriangleEdge>::iterator BVHNode::GetEdgeEnd()
{
	return edges.end();
}

std::span<const Triangle> MIndices::BVHNode::triangleSpan()
{
	return triangles;
}

size_t BVHNode::GetNumOfTriangles() const
{
	return triangles.size();
}

bool BVHNode::HasComputedEdges() const
{
	return !edges.empty();
}
