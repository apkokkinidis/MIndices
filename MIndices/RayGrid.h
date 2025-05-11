#pragma once
#include <vector>
#include <span>
#include "Ray.h"
#include "Structures.h"

namespace MIndices
{
	using RotationGrid = std::vector<std::pair<AzthElevPair, std::vector<Ray>>>;
	//Creates an array of rays of X * Y size
	class RayGrid
	{
	public:
		RayGrid() = default;
		~RayGrid() = default;
		RayGrid(size_t DimX, size_t DimY, size_t diagonal);

		void RotateRays(double d, Axis axis);
		RotationGrid ComputeRotationGrid(int32_t elevation, int32_t azimuth);
		std::span<const std::vector<Ray>> ComputeAzimuthRays(int32_t steps = AZIMUTH);
		const std::vector<Ray> &getRays() const noexcept;
		std::span<const Ray> getRaySpan() const noexcept;
		const bool empty() const noexcept;
	private:
		std::vector<Ray> rays;
		std::array<std::vector<Ray>, AZIMUTH> azimuthRays;
		void RotateRays(double d, Axis axis, int32_t elevation, int32_t azimuth, RotationGrid& rotationGrid);
	};

}
