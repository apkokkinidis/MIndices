// Author Anastasios Kokkinidis 
#pragma once

#include <vector>

namespace MIndices
{

	typedef double COORD_TYPE;

	enum class ParallelThreads
	{
		off,			// 0 - off
		enabled_auto,	// 1 - enabled auto
		enabled_2,		// 2 - enabled 2 threads
		enabled_4,		// 3 - enabled 4 threads
		enabled_6,		// 4 - enabled 6 threads
		enabled_8,		// 5 - enabled 8 threads
		enabled_10,		// 6 - enabled 10 threads
		enabled_12,		// 7 - enabled 12 threads
		count			// 8 - keep count of enums
	};

	enum class Axis { X, Y, Z };

	enum class ElementTypes : uint8_t
	{
		EMPTY,
		FILLED,
		VARIOUS,
	};

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

		bool operator==(const Point3D& other) const
		{
			return this->x == other.x &&
				this->y == other.y &&
				this->z == other.z;
		}
	}Point3D;

	typedef struct PointT
	{
		Point3D iPoint;
		double t;
	}PointT;

	typedef struct Edge3D {
		Point3D p1;
		Point3D p2;
	}Edge3D;

	typedef struct Triangle
	{
		Point3D p[3];

		bool operator==(const Triangle& other) const
		{
			return this->p[0] == other.p[0] &&
				this->p[1] == other.p[1] &&
				this->p[2] == other.p[2];
		}
	} Triangle;

	typedef struct MCube
	{
		Point3D Point[8];
		bool val[8];
	}Cube;

	typedef struct Voxel
	{
		Point3D point;
		bool val;
	}Voxel;

	typedef struct VoxelCube
	{
		Voxel vertex[8];
	}CubeV;

	typedef struct
	{
		Triangle triangle;
		Point3D edges[2];
	}TriangleEdge;

	using VecPoint3D = std::vector<Point3D>;
}
