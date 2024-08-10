//BVHNode.h
#pragma once
#ifndef _BVHNode
#define _BVHNode
#include "BoundingBox.h"
#include "Structures.h"
#include <vector>

using std::vector;

namespace MIndices
{

	class BVHNode
	{
	public:
		BVHNode(BoundingBox3D bbox, vector<Triangle> triangles);
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
		vector<Triangle> GetTriangles() const noexcept;
		vector<TriangleEdge> GetEdges() const noexcept;
		std::vector<Triangle>::iterator GetTriBegin();
		std::vector<Triangle>::iterator GetTriEnd();
		std::vector<TriangleEdge>::iterator GetEdgeBegin();
		std::vector<TriangleEdge>::iterator GetEdgeEnd();

		//Set functions
		void SetBBox(const BoundingBox3D& newBBox);
		void SetTriangles(const vector<Triangle>& triangles);

		void ClearTriangles();

		//Compute the edges of all the triangles in this node
		int32_t ComputeEdges();

	private:
		BoundingBox3D bbox;
		vector<Triangle> triangles;
		vector<TriangleEdge> edges;

	};
}
#endif _BVHNode