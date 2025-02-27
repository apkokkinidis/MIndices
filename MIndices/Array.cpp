#include "Array.h"

using namespace MIndices;

Array::Array(size_t dim_x, size_t dim_y, size_t dim_z) : dim_x(dim_x), dim_y(dim_y), dim_z(dim_z), voxelArray(dim_x* dim_y* dim_z, ElementTypes::EMPTY)
{
	plane_xy = dim_x * dim_y;
}

size_t Array::GetIndex(size_t x, size_t y, size_t z) const noexcept
{
	return z * plane_xy + y * dim_x + x;
}

void Array::SetElement(size_t x, size_t y, size_t z, ElementTypes elm) noexcept
{
	voxelArray[GetIndex(x, y, z)] = elm;
}

ElementTypes MIndices::Array::GetElementType(size_t indx) const noexcept
{
	return voxelArray[indx];
}

ElementTypes MIndices::Array::GetElementType(size_t dim_x, size_t dim_y, size_t dim_z) const noexcept
{
	return voxelArray[GetIndex(dim_x, dim_y, dim_z)];
}
