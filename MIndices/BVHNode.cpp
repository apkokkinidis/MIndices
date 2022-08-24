// Author Anastasios Kokkinidis 
//BVHNode.cpp
#include "BVHNode.h"

BVHNode::BVHNode(BoundingBox3D b, vector<TRIANGLE> tri) : bbox(b), triangles(tri), right(nullptr), left(nullptr)
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

bool HasEvenIntersections(const vector<TRIANGLE>& t)
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

void BVHNode::SetTriangles(const vector<TRIANGLE>& triangles)
{
	this->triangles = triangles;
}

bool BVHNode::IsLeafNode() const
{
	return (left == NULL && right == NULL);
}

BoundingBox3D BVHNode::Box() const noexcept
{
	return bbox;
}

vector<TRIANGLE> BVHNode::GetTriangles() const noexcept
{
	return triangles;
}

vector<TriangleEdge> BVHNode::GetEdges() const noexcept
{
	return edges;
}

std::vector<TRIANGLE>::iterator BVHNode::GetTriBegin()
{
	return triangles.begin();
}

std::vector<TRIANGLE>::iterator BVHNode::GetTriEnd()
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

size_t BVHNode::GetNumOfTriangles() const
{
	return triangles.size();
}

bool BVHNode::HasComputedEdges() const
{
	return !edges.empty();
}
