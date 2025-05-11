//BVHNode.h
#pragma once
#include "BoundingBox.h"
#include <span>
#include "Structures.h"
#include <vector>

namespace MIndices
{

	class BVHNode
	{
	public:
		BVHNode(BoundingBox3D bbox, std::vector<Triangle> triangles);
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
		const BoundingBox3D& Box() const noexcept;
		std::vector<Triangle> GetTriangles() const noexcept;
		std::vector<TriangleEdge> GetEdges() const noexcept;
		std::vector<Triangle>::iterator GetTriBegin();
		std::vector<Triangle>::iterator GetTriEnd();
		std::vector<TriangleEdge>::iterator GetEdgeBegin();
		std::vector<TriangleEdge>::iterator GetEdgeEnd();
		std::span<const Triangle> triangleSpan();

		//Set functions
		void SetBBox(const BoundingBox3D& newBBox);
		void SetTriangles(const std::vector<Triangle>& triangles);

		void ClearTriangles();

		int32_t ComputeEdges();

	private:
		BoundingBox3D bbox;
		std::vector<Triangle> triangles;
		std::vector<TriangleEdge> edges;

	};
}