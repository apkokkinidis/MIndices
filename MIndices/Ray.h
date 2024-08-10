// Author Anastasios Kokkinidis 
#pragma once
#ifndef RAY_H
#define RAY_H

#include "Structures.h"
#include <math.h>
#include <vector>

namespace MIndices
{

	constexpr double PI = 3.14159265359;;
	constexpr double RAD = PI / 180;
	constexpr double EPSILON = 0.0000001;

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
		inline COORD_TYPE RayOriginDistFromPoint32_t3D(const Point3D& tri) const noexcept;

		//int32_tersection tests
		bool intersectsTriangle(const Triangle& triangle, Point3D& P) const noexcept;
		//MT algorithm approach
		bool FastRayTriangleint32_tersection(const Triangle& triangle, Point3D& iPoint32_t, double& t) const noexcept;
		bool FastRayTriangleint32_tersection(const Triangle& triangle, const Point3D& edge1, const Point3D& edge2, Point3D& iPoint32_t, double& t) const noexcept;
	};

	//Creates an array of rays of X * Y size
	class RayGenerator
	{
	public:
		RayGenerator() = default;
		~RayGenerator() = default;
		RayGenerator(size_t DimX, size_t DimY, size_t diagonal)
		{
			//point32_t in direction of z
			COORD_TYPE z = 0.0f;

			rays.reserve(DimX * DimY);
			for (size_t x = 0; x < DimX; x++)
			{
				for (size_t y = 0; y < DimY; y++)
				{
					Point3D origin = { (COORD_TYPE)x, (COORD_TYPE)y, (COORD_TYPE)z };
					Point3D dest = { (COORD_TYPE)x, (COORD_TYPE)y, (COORD_TYPE)diagonal };
					Ray r = Ray(origin, dest);
					rays.push_back(r);
				}
			}
		}
		inline std::vector<Ray> Rays()
		{
			return rays;
		}
	private:
		std::vector<Ray> rays;
	};
}
#endif RAY_H//  RAY_H