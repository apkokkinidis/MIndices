#pragma once
#include "Structures.h"

namespace MIndices
{
	inline const Point3D CrossProduct(const Point3D& A, const Point3D& B) noexcept
	{
		Point3D C = { A.y * B.z - A.z * B.y,
					A.z * B.x - A.x * B.z,
					A.x * B.y - A.y * B.x };
		return C;
	}

	inline const double DotProduct(const Point3D& A, const Point3D& B) noexcept
	{
		return static_cast<double>(A.x * B.x + A.y * B.y + A.z * B.z);
	}

	inline const Point3D Subpoints(const Point3D& p1, const Point3D& p2) noexcept
	{
		Point3D s = { p1.x - p2.x, p1.y - p2.y, p1.z - p2.z };
		return s;
	}

	inline const Point3D ComputeNormal(Triangle const& triangle) noexcept
	{
		Point3D v0 = triangle.p[0];
		Point3D v1 = triangle.p[1];
		Point3D v2 = triangle.p[2];
		Point3D A = { v1.x - v0.x, v1.y - v0.y, v1.z - v0.z };
		Point3D B = { v2.x - v0.x, v2.y - v0.y, v2.z - v0.z };
		Point3D C = CrossProduct(A, B);

		return C;
	}
}