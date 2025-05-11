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

//Computes the rotational grid for all rays for given elevation & azimuth pairs.
//@param value of elevation to rotate the rays
//@param value of azimuth to rotate the rays
RotationGrid MIndices::RayGrid::ComputeRotationGrid(int32_t elevation, int32_t azimuth)
{
	RotationGrid rotationGrid;
	for (int i = 0; i < elevation; ++i)
	{
		for (int32_t j = 0; j < azimuth; ++j)
		{
			RotateRays(DEGREE, Axis::X, i, j, rotationGrid);
		}
		RotateRays(DEGREE, Axis::Z, i, 0, rotationGrid);
	}
	return rotationGrid;
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

void MIndices::RayGrid::RotateRays(double d, Axis axis, int32_t elevation, int32_t azimuth, RotationGrid& rotationGrid)
{
	auto tmp_rays = rays;
	for (auto& ray : tmp_rays)
	{
		ray.Rotate(d, Axis::X);
	}
	rotationGrid.emplace_back(AzthElevPair{elevation, azimuth}, std::move(tmp_rays));
}