#pragma once
#include <string>
#include <vector>

//custom headers
#include "Structures.h"

namespace MIndices
{
	class Vector3D
	{
	public:
		Vector3D(size_t dim_x, size_t dim_y, size_t dim_z);
		Vector3D(const Vector3D& other) = default;
		virtual ~Vector3D() = default;

		size_t GetIndex(size_t x, size_t y, size_t z) const noexcept;
		void SetElement(size_t x, size_t y, size_t z, ElementTypes elm) noexcept;
		ElementTypes GetElementType(size_t indx) const noexcept;
		ElementTypes GetElementType(size_t dim_x, size_t dim_y, size_t dim_z) const noexcept;
	
	protected:
		std::vector<ElementTypes> voxelArray;
	private:
		size_t dim_x, dim_y, dim_z, plane_xy;
	};

}
