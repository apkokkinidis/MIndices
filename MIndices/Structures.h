// Author Anastasios Kokkinidis 
#pragma once
#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <vector>

typedef float COORD_TYPE;

enum class Axis { x, y, z };

/* ----------------------------- */
/* Struct definitions go in here */
/* ------------------------------*/
 typedef struct Point3D {
	 COORD_TYPE x;
	 COORD_TYPE y;
	 COORD_TYPE z;

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

 typedef struct Vec3f
 {
	 float v[3];
 }Vec3f;

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
} TRIANGLE;

typedef struct Cube
{
	Point3D point[8];
	bool val[8];
}Cube;

typedef struct Voxel
{
	Point3D p;
	bool val;
}Voxel;

typedef struct CubeV
{
	Voxel v[8];
}CubeV;

typedef struct
{
	TRIANGLE triangle;
	Point3D edges[2];
}TriangleEdge;

typedef struct
{
	std::vector<Point3D> p;
}VecPoint3D;

#endif STRUCTURES_H
