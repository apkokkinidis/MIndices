#pragma once
#include <string>
#include <vector>

//custom headers
#include "Structures.h"

namespace MIndices
{
	class Array
	{
	public:
		Array(size_t dim_x, size_t dim_y, size_t dim_z);
		Array(const Array& other) = default;
		~Array() = default;

		size_t GetIndex(size_t x, size_t y, size_t z) const noexcept;
		void SetElement(size_t x, size_t y, size_t z, ElementTypes elm) noexcept;
		ElementTypes GetElementType(size_t indx) const noexcept;
		ElementTypes GetElementType(size_t dim_x, size_t dim_y, size_t dim_z) const noexcept;

	private:
		std::vector<ElementTypes> voxelArray;
		size_t dim_x, dim_y, dim_z, plane_xy;
	};

}
