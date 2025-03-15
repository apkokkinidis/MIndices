#pragma once
#include <vector>
#include <span>
#include "Ray.h"
#include "Structures.h"

namespace MIndices
{
	constexpr double DEGREE = 1.0;

	//Creates an array of rays of X * Y size
	class RayGrid
	{
	public:
		RayGrid() = default;
		~RayGrid() = default;
		RayGrid(size_t DimX, size_t DimY, size_t diagonal);

		void RotateRays(double d, Axis axis);
		const std::vector<Ray> &getRays() const noexcept;
		std::span<const Ray> getRaySpan() const noexcept;
		const bool empty() const noexcept;
	private:
		std::vector<Ray> rays;
	};

}
