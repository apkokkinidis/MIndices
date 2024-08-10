// Author Anastasios Kokkinidis 
#pragma once
#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <vector>

namespace MIndices
{

	typedef double COORD_TYPE;

	enum class Axis { X, Y, Z };

	/* ----------------------------- */
	/* Struct definitions go in here */
	/* ------------------------------*/
	typedef struct Point3D {
		COORD_TYPE x, y, z;

		Point3D operator=(const Point3D& b)
		{
			x = b.x;
			y = b.y;
			z = b.z;
			return *this;
		}
		const Point3D operator-(const Point3D& b) const
		{
			COORD_TYPE x = this->x - b.x;
			COORD_TYPE y = this->y - b.y;
			COORD_TYPE z = this->z - b.z;
			Point3D p = { x, y, z };
			return p;
		}
		const Point3D operator+(const Point3D& b) const
		{
			COORD_TYPE x = this->x + b.x;
			COORD_TYPE y = this->y + b.y;
			COORD_TYPE z = this->z + b.z;
			Point3D p = { x, y, z };
			return p;
		}
	}Point3D;

	typedef struct PointT
	{
		Point3D iPoint;
		double t;
	}PointT;

	typedef struct {
		Point3D p1;
		Point3D p2;
	}Edge3D;

	typedef struct
	{
		Point3D p[3];
	} Triangle;

	typedef struct MCube
	{
		Point3D Point[8];
		bool val[8];
	}Cube;

	typedef struct Voxel
	{
		Point3D p;
		bool val;
	}Voxel;

	typedef struct VoxelCube
	{
		Voxel v[8];
	}CubeV;

	typedef struct
	{
		Triangle triangle;
		Point3D edges[2];
	}TriangleEdge;

	using VecPoint3D = std::vector<Point3D>;
}
#endif STRUCTURES_H
