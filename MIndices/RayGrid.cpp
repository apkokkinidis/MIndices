#include "RayGrid.h"

using namespace MIndices;

RayGrid::RayGrid(size_t DimX, size_t DimY, size_t diagonal)
{
	//Point in direction of z
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

//Rotates an array of rays
//@param degrees of rotation
//@param rotation axis
void RayGrid::RotateRays(double d, Axis axis)
{
	for (auto &ray : rays)
	{
		ray.Rotate(d, axis);
	}
}

const std::vector<Ray>& MIndices::RayGrid::getRays() const noexcept
{
	return rays;
}

std::span<const Ray> MIndices::RayGrid::getRaySpan() const noexcept
{
	return rays;
}

const bool MIndices::RayGrid::empty() const noexcept
{
	return rays.empty();
}
