#include "Vector3D.h"

using namespace MIndices;

Vector3D::Vector3D(size_t dim_x, size_t dim_y, size_t dim_z) : dim_x(dim_x), dim_y(dim_y), dim_z(dim_z), voxelArray(dim_x* dim_y* dim_z, ElementTypes::EMPTY)
{
	plane_xy = dim_x * dim_y;
}

size_t Vector3D::GetIndex(size_t x, size_t y, size_t z) const noexcept
{
	return z * plane_xy + y * dim_x + x;
}

void Vector3D::SetElement(size_t x, size_t y, size_t z, ElementTypes elm) noexcept
{
	voxelArray[GetIndex(x, y, z)] = elm;
}

ElementTypes MIndices::Vector3D::GetElementType(size_t indx) const noexcept
{
	return voxelArray[indx];
}

ElementTypes MIndices::Vector3D::GetElementType(size_t dim_x, size_t dim_y, size_t dim_z) const noexcept
{
	return voxelArray[GetIndex(dim_x, dim_y, dim_z)];
}
