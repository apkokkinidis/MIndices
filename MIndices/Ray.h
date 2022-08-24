// Author Anastasios Kokkinidis 
#pragma once
#ifndef RAY_H
#define RAY_H

#include "Structures.h"
#include <math.h>
#include <vector>

constexpr double pi = 3.14159265359;;
constexpr double rad = pi / 180;
constexpr float EPSILON = 0.0000001;
class Ray
{
private:
	Point3D origin;
	Point3D direction;
public:
	Ray();
	Ray(Point3D _origin, Point3D direction);
	Ray(const Ray& r);

	Ray& operator=(const Ray& r);

	inline const Point3D& GetOrigin() const noexcept { return origin; };
	inline const Point3D& GetDirection() const noexcept { return direction; };
	inline void SetDirection(const Point3D& direction) { this->direction = direction; };

	//rotate ray by degrees
	void Rotate(double d, Axis axis);
	inline COORD_TYPE RayOriginDistFromPoint3D(const Point3D& tri) const noexcept;

	//intersection tests
	bool IntersectsTriangle(const TRIANGLE& triangle, Point3D &P) const noexcept;
	//MT algorithm approach
	bool FastRayTriangleIntersection(const TRIANGLE& triangle, Point3D& iPoint, double& t) const noexcept;
	bool FastRayTriangleIntersection(const TRIANGLE& triangle, const Point3D& edge1, const Point3D& edge2, Point3D& iPoint, double& t) const noexcept;
};

inline const Point3D CrossProduct(const Point3D& A, const Point3D& B) noexcept
{
	Point3D C = { A.y * B.z - A.z * B.y,
				A.z * B.x - A.x * B.z,
				A.x * B.y - A.y * B.x };
	return C;
}

inline const COORD_TYPE DotProduct(const Point3D& A, const Point3D& B) noexcept
{
	return A.x * B.x + A.y * B.y + A.z * B.z;
}

inline const Point3D SubPoints(const Point3D& p1, const Point3D& p2) noexcept
{
	Point3D s = { p1.x - p2.x, p1.y - p2.y, p1.z - p2.z };
	return s;
}

inline const Point3D ComputeNormal(TRIANGLE const& triangle) noexcept
{
	Point3D v0 = triangle.p[0];
	Point3D v1 = triangle.p[1];
	Point3D v2 = triangle.p[2];
	Point3D A = { v1.x - v0.x, v1.y - v0.y, v1.z - v0.z };
	Point3D B = { v2.x - v0.x, v2.y - v0.y, v2.z - v0.z };
	Point3D C = CrossProduct(A, B);

	return C;
}
#endif RAY_H//  RAY_H