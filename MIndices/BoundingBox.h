// Author Anastasios Kokkinidis 
//BoundingBox.h
#pragma once
#ifndef _BoundingBox_H
#define _BoundingBox_H
#include <iostream>
#include <cstdint>
#include <algorithm>
#include "Ray.h"
#include <string>
#include <math.h>

using std::min;
using std::max;
/* -------------------------------------------------------------------------- */
/* Bounding-box coordinate type.  Change to alter memory requirements.        */
/* -------------------------------------------------------------------------- */
typedef	int	COORD_TYPE_BB;

/* -------------------------------------------------------------------------- */
/* BoundingBox2D class.                                                       */
/* -------------------------------------------------------------------------- */
class BoundingBox2D
{
private:
	COORD_TYPE_BB	x1, y1, x2, y2;

public:
	inline BoundingBox2D()
		: x1(0), y1(0), x2(0), y2(0)

	{}	// Default constructor

	inline BoundingBox2D(const COORD_TYPE_BB _x1, const COORD_TYPE_BB _y1,
		const COORD_TYPE_BB _x2, const COORD_TYPE_BB _y2) noexcept
		: x1(min(_x1, _x2)), y1(min(_y1, _y2)), x2(max(_x1, _x2)), y2(max(_y1, _y2))
	{}	// Constructor

	inline BoundingBox2D(const size_t _x1, const size_t _y1,
		const size_t _x2, const size_t _y2) noexcept
	{
		x1 = (COORD_TYPE_BB)min(_x1, _x2);
		y1 = (COORD_TYPE_BB)min(_y1, _y2);
		x2 = (COORD_TYPE_BB)max(_x1, _x2);
		y2 = (COORD_TYPE_BB)max(_y1, _y2);
	}	// Constructor

	inline BoundingBox2D(const size_t DX, const size_t DY) noexcept
		: x1(0), y1(0), x2((COORD_TYPE_BB)DX - 1), y2((COORD_TYPE_BB)DY - 1)
	{
	}	// Constructor

	inline BoundingBox2D(const BoundingBox2D& AABB) noexcept
	{
		if (this != &AABB)
		{
			x1 = AABB.x1;
			y1 = AABB.y1;
			x2 = AABB.x2;
			y2 = AABB.y2;
		}
	}	// Copy constructor

	inline ~BoundingBox2D() noexcept
	{
		x1 = x2 = y1 = y2 = 0;
	}	// Destructor

	inline COORD_TYPE_BB X1() const noexcept { return x1; }
	inline COORD_TYPE_BB X2() const noexcept { return x2; }
	inline COORD_TYPE_BB Y1() const noexcept { return y1; }
	inline COORD_TYPE_BB Y2() const noexcept { return y2; }

	inline void Get(COORD_TYPE_BB* _x1, COORD_TYPE_BB* _y1,
		COORD_TYPE_BB* _x2, COORD_TYPE_BB* _y2) const noexcept
	{
		*_x1 = x1;
		*_y1 = y1;
		*_x2 = x2;
		*_y2 = y2;
	}	// Get

	inline void Get(size_t* _x1, size_t* _y1,
		size_t* _x2, size_t* _y2) const noexcept
	{
		*_x1 = x1;
		*_y1 = y1;
		*_x2 = x2;
		*_y2 = y2;
	}	// Get

	inline void Set(const COORD_TYPE_BB _x1, const COORD_TYPE_BB _y1,
		const COORD_TYPE_BB _x2, const COORD_TYPE_BB _y2) noexcept
	{
		x1 = min(_x1, _x2);
		y1 = min(_y1, _y2);
		x2 = max(_x1, _x2);
		y2 = max(_y1, _y2);
	}	// Set

	inline void Set(const size_t _x1, const size_t _y1,
		const size_t _x2, const size_t _y2) noexcept
	{
		x1 = (COORD_TYPE_BB)min(_x1, _x2);
		y1 = (COORD_TYPE_BB)min(_y1, _y2);
		x2 = (COORD_TYPE_BB)max(_x1, _x2);
		y2 = (COORD_TYPE_BB)max(_y1, _y2);
	}	// Set

	inline size_t Width() const noexcept 
	{
		return (size_t)x2 - (size_t)x1 + 1; 
	}
	inline size_t Height() const noexcept 
	{ 
		return (size_t)y2 - (size_t)y1 + 1; 
	}
	inline size_t Size() const noexcept 
	{ return Width() * Height(); 
	}

	inline void Write(const std::string& Msg) const noexcept
	{
		std::cout << "BoundingBox2D object 0x" << this << ": " <<
			Msg.c_str() << "(" << x1 << ", " << y1 << ") -> (" << x2 << ", " << y2 << ")" << std::endl;
	}	// Write

	inline std::string ToString() const noexcept
	{
		return "(" + std::to_string(x1) + ", " + std::to_string(y1) + ") -> (" +
			std::to_string(x2) + ", " + std::to_string(y2) + ")";
	}	// ToString
};

/* -------------------------------------------------------------------------- */
/* BoundingBox3D class.                                                       */
/* -------------------------------------------------------------------------- */
class BoundingBox3D
{
private:
	COORD_TYPE_BB	x1, y1, z1, x2, y2, z2;

public:
	inline BoundingBox3D()
		: x1(0), y1(0), z1(0), x2(0), y2(0), z2(0)
	{}	// Default constructor

	inline BoundingBox3D(const COORD_TYPE_BB _x1, const COORD_TYPE_BB _y1, const COORD_TYPE_BB _z1,
		const COORD_TYPE_BB _x2, const COORD_TYPE_BB _y2, const COORD_TYPE_BB _z2) noexcept
		: x1(min(_x1, _x2)), y1(min(_y1, _y2)), z1(min(_z1, _z2)),
		x2(max(_x1, _x2)), y2(max(_y1, _y2)), z2(max(_z1, _z2))
	{}	// Constructor

	inline BoundingBox3D(const size_t _x1, const size_t _y1, const size_t _z1,
		const size_t _x2, const size_t _y2, const size_t _z2) noexcept
	{
		x1 = (COORD_TYPE_BB)min(_x1, _x2);
		y1 = (COORD_TYPE_BB)min(_y1, _y2);
		z1 = (COORD_TYPE_BB)min(_z1, _z2);

		x2 = (COORD_TYPE_BB)max(_x1, _x2);
		y2 = (COORD_TYPE_BB)max(_y1, _y2);
		z2 = (COORD_TYPE_BB)max(_z1, _z2);
	}	// Constructor

	inline BoundingBox3D(const size_t DX, const size_t DY, const size_t DZ) noexcept
		: x1(0), y1(0), z1(0),
		x2((COORD_TYPE_BB)DX - 1), y2((COORD_TYPE_BB)DY - 1), z2((COORD_TYPE_BB)DZ - 1)
	{
		//x1 = 0;
		//y1 = 0;
		//z1 = 0;

		//x2 = (COORD_TYPE)DX - 1;
		//y2 = (COORD_TYPE)DY - 1;
		//z2 = (COORD_TYPE)DZ - 1;
	}	// Constructor

	inline BoundingBox3D(const BoundingBox3D& AABB) noexcept
	{
		if (this != &AABB)
		{
			x1 = AABB.x1;
			y1 = AABB.y1;
			z1 = AABB.z1;
			x2 = AABB.x2;
			y2 = AABB.y2;
			z2 = AABB.z2;
		}	// if
	}	// Copy constructor

	inline ~BoundingBox3D() noexcept
	{
		x1 = y1 = z1 = x2 = y2 = z2 = 0;
	}	// Destructor

	inline COORD_TYPE_BB X1() const noexcept
	{
		return x1;
	}
	inline COORD_TYPE_BB X2() const noexcept
	{
		return x2;
	}
	inline COORD_TYPE_BB Y1() const noexcept
	{
		return y1;
	}
	inline COORD_TYPE_BB Y2() const noexcept
	{
		return y2;
	}
	inline COORD_TYPE_BB Z1() const noexcept
	{
		return z1;
	}
	inline COORD_TYPE_BB Z2() const noexcept
	{
		return z2;
	}

	inline void Get(COORD_TYPE_BB* xNear, COORD_TYPE_BB* yNear, COORD_TYPE_BB* zNear,
		COORD_TYPE_BB* xFar, COORD_TYPE_BB* yFar, COORD_TYPE_BB* zFar) const noexcept
	{

		*xNear = x1;
		*yNear = y1;
		*zNear = z1;

		*xFar = x2;
		*yFar = y2;
		*zFar = z2;
	}	// Get

	inline void Get(size_t* _x1, size_t* _y1, size_t* _z1,
		size_t* _x2, size_t* _y2, size_t* _z2) const noexcept
	{
		*_x1 = x1;
		*_y1 = y1;
		*_z1 = z1;

		*_x2 = x2;
		*_y2 = y2;
		*_z2 = z2;
	}	// Get

	inline void Set(const COORD_TYPE_BB xNear, const COORD_TYPE_BB yNear, const COORD_TYPE_BB zNear,
		const COORD_TYPE_BB xFar, const COORD_TYPE_BB yFar, const COORD_TYPE_BB zFar) noexcept
	{
		x1 = min(xNear, xFar);
		y1 = min(yNear, yFar);
		z1 = min(zNear, zFar);

		x2 = max(xNear, xFar);
		y2 = max(yNear, yFar);
		z2 = max(zNear, zFar);
	}	// Set

	inline void Set(const size_t _x1, const size_t _y1, const size_t _z1,
		const size_t _x2, const size_t _y2, const size_t _z2) noexcept
	{
		x1 = (COORD_TYPE_BB)min(_x1, _x2);
		y1 = (COORD_TYPE_BB)min(_y1, _y2);
		z1 = (COORD_TYPE_BB)min(_z1, _z2);

		x2 = (COORD_TYPE_BB)max(_x1, _x2);
		y2 = (COORD_TYPE_BB)max(_y1, _y2);
		z2 = (COORD_TYPE_BB)max(_z1, _z2);
	}	// Set

	inline size_t Width() const noexcept { return (size_t)x2 - (size_t)x1 + 1; }
	inline size_t Height() const noexcept { return (size_t)y2 - (size_t)y1 + 1; }
	inline size_t Depth() const noexcept { return (size_t)z2 - (size_t)z1 + 1; }
	inline size_t Size() const noexcept { return Width() * Height() * Depth(); }

	//Diagonal of 3D bounding box
	inline size_t Diagonal() const
	{
		COORD_TYPE x = (COORD_TYPE)this->Width();
		COORD_TYPE y = (COORD_TYPE)this->Height();
		COORD_TYPE z = (COORD_TYPE)this->Depth();
		return (size_t)sqrt(x * x + y * y + z * z);
	}

	inline bool TestOverlap(const BoundingBox3D& other) const noexcept
	{
		//test all axes for overlap
		if (this->x2 < other.x1 || this->x1 > other.x2) return false;
		if (this->y2 < other.y1 || this->y1 > other.y2) return false;
		if (this->z2 < other.z1 || this->z1 > other.z2) return false;
		//if all axes intersect then it overlaps
		return true;
	}

	inline bool RayBoxIntersect(const Ray& r) const noexcept
	{
		COORD_TYPE inv_dirX = 1 / r.GetDirection().x;
		COORD_TYPE inv_dirY = 1 / r.GetDirection().y;
		COORD_TYPE inv_dirZ = 1 / r.GetDirection().z;

		COORD_TYPE tMinX = (x1 - r.GetOrigin().x) * inv_dirX;
		COORD_TYPE tMinY = (y1 - r.GetOrigin().y) * inv_dirY;
		COORD_TYPE tMinZ = (z1 - r.GetOrigin().z) * inv_dirZ;
		COORD_TYPE tMaxX = (x2 - r.GetOrigin().x) * inv_dirX;
		COORD_TYPE tMaxY = (y2 - r.GetOrigin().y) * inv_dirY;
		COORD_TYPE tMaxZ = (z2 - r.GetOrigin().z) * inv_dirZ;

		if (tMinX > tMaxX) std::swap(tMinX, tMaxX);
		if (tMinY > tMaxY) std::swap(tMinY, tMaxY);
		if (tMinZ > tMaxZ) std::swap(tMinZ, tMaxZ);

		if (tMinX > tMaxY || tMinY > tMaxX)
		{
			return false;
		}

		if (tMinY > tMinX)
		{
			tMinX = tMinY;
		}

		if (tMaxY < tMaxX)
		{
			tMaxX = tMaxY;
		}

		if (tMinX > tMaxZ || tMinZ > tMaxX)
		{
			return false;
		}

		if (tMinZ > tMinX)
		{
			tMinX = tMinZ;
		}

		if (tMaxZ < tMaxX)
		{
			tMaxX = tMaxZ;
		}
		return true;
	}

	inline void Write(const std::string& Msg) const noexcept
	{
		std::cout << "BoundingBox3D object 0x" << this << ": " <<
			Msg.c_str() << "(" << x1 << ", " << y1 << ", " << z1 << ") -> (" <<
			x2 << ", " << y2 << ", " << z2 << ")" << std::endl;
	}	// Write

	inline std::string ToString() const noexcept
	{
		return "(" + std::to_string(x1) + ", " +
			std::to_string(y1) + ", " +
			std::to_string(z1) + ") -> (" +
			std::to_string(x2) + ", " +
			std::to_string(y2) +
			std::to_string(z2) + ")";
	}	// ToString

	static BoundingBox3D ComputeBounds(const std::vector<TRIANGLE>& triangles)
	{
		BoundingBox3D* bbox = NULL;
		Point3D minTriPoint, maxTriPoint;
		minTriPoint = triangles[0].p[0];
		maxTriPoint = triangles[0].p[0];
		for (size_t i = 0; i < triangles.size() - 1; i++)
		{
			for (size_t j = 1; j <= 2; j++)
			{
				//find min
				if (triangles[i].p[j].x < minTriPoint.x)
				{
					minTriPoint.x = triangles[i].p[j].x;
				}
				if (triangles[i].p[j].y < minTriPoint.y)
				{
					minTriPoint.y = triangles[i].p[j].y;
				}
				if (triangles[i].p[j].z < minTriPoint.z)
				{
					minTriPoint.z = triangles[i].p[j].z;
				}
				//find max
				if (triangles[i].p[j].x > maxTriPoint.x)
				{
					maxTriPoint.x = triangles[i].p[j].x;
				}
				if (triangles[i].p[j].y > maxTriPoint.y)
				{
					maxTriPoint.y = triangles[i].p[j].y;
				}
				if (triangles[i].p[j].z > maxTriPoint.z)
				{
					maxTriPoint.z = triangles[i].p[j].z;
				}
			}
		}
		bbox = new BoundingBox3D((size_t)minTriPoint.x,
			(size_t)minTriPoint.y,
			(size_t)minTriPoint.z,
			(size_t)maxTriPoint.x,
			(size_t)maxTriPoint.y,
			(size_t)maxTriPoint.z);
		return *bbox;
	}

	//Merge this AABB with another
	inline void MergeAABB(const BoundingBox3D& other)
	{
		this->x1 = std::min(x1, other.X1());
		this->y1 = std::min(y1, other.Y1());
		this->z1 = std::min(z1, other.Z1());

		this->x2 = std::max(x2, other.X2());
		this->y2 = std::max(y2, other.Y2());
		this->z2 = std::max(z2, other.Z2());
	}

	//Merge two AABB's and return a boundingbox3D object
	inline static BoundingBox3D MergeAABB(const BoundingBox3D& bbox1, const BoundingBox3D& bbox2)
	{
		size_t minx, miny, minz, maxx, maxy, maxz;
		minx = std::min(bbox1.X1(), bbox2.X1());
		miny = std::min(bbox1.Y1(), bbox2.Y1());
		minz = std::min(bbox1.Z1(), bbox2.Z1());

		maxx = std::max(bbox1.X2(), bbox2.X2());
		maxy = std::max(bbox1.Y2(), bbox2.Y2());
		maxz = std::max(bbox1.Z2(), bbox2.Z2());

		return BoundingBox3D(minx, miny, minz, maxx, maxy, maxz);
	}
};
#endif _BoundingBox_H
