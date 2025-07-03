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
		size_t TrianglesSize() const noexcept;
		bool IsLeafNode() const noexcept;

		//Get functions
		const BoundingBox3D& Box() const noexcept;
		std::vector<Triangle> GetTriangles() const noexcept;
		std::vector<TriangleEdge> GetEdges() const noexcept;
		std::span<const Triangle> triangleSpan() const noexcept;

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