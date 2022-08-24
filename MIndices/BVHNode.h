//BVHNode.h
#pragma once
#ifndef _BVHNode
#define _BVHNode
#include "BoundingBox.h"
#include "Structures.h"
#include <vector>

using std::vector;

class BVHNode
{
private:
	BoundingBox3D bbox;
	vector<TRIANGLE> triangles;
	vector<TriangleEdge> edges;
public:
	BVHNode(BoundingBox3D bbox, vector<TRIANGLE> triangles);
	BVHNode(const BVHNode& other);
	~BVHNode();
	//public members
	BVHNode* left;
	BVHNode* right;

	//Utility functions
	size_t GetNumOfTriangles() const;
	bool IsLeafNode() const;
	bool HasComputedEdges() const;

	//Get functions
	BoundingBox3D Box() const noexcept;
	vector<TRIANGLE> GetTriangles() const noexcept;
	vector<TriangleEdge> GetEdges() const noexcept;
	std::vector<TRIANGLE>::iterator GetTriBegin();
	std::vector<TRIANGLE>::iterator GetTriEnd();
	std::vector<TriangleEdge>::iterator GetEdgeBegin();
	std::vector<TriangleEdge>::iterator GetEdgeEnd();

	//Set functions
	void SetBBox(const BoundingBox3D& newBBox);
	void SetTriangles(const vector<TRIANGLE>& triangles);

	void ClearTriangles()
	{
		if (!triangles.empty())
		{
			triangles.clear();
		}
	}

	//Compute the edges of all the triangles in this node
	inline int ComputeEdges()
	{
		size_t s = triangles.size();
		if (s > 0)
		{
			if (edges.empty())
			{
				edges.reserve(s * 2);
				for (std::vector<TRIANGLE>::iterator it = triangles.begin(); it != triangles.end(); ++it)
				{
					Point3D edge[2]{ 0 };
					edge[0] = it->p[1] - it->p[0];
					edge[1] = it->p[2] - it->p[0];
					edges.push_back({ *it, {edge[0], edge[1]} });
				}
				edges.shrink_to_fit();
				//All edges computed return 1
				return 1;
			}
		}
		return -1;	//Failed to compute edges
	}

};
#endif _BVHNode