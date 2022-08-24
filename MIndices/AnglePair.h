// Author Anastasios Kokkinidis 
//AnglePair.h
#pragma once
#ifndef _AnglePair
#define _AnglePair
#include "Structures.h"
#include "Ray.h"
#include <vector>
#include <map>
//tabecular seperation method requires to ray trace the structure and find the points where there is empty space (reverse operation of tabecular thickness)
//tabecular number is the number of beams the structure has, it is found by finding the entry and exit points between triangles.
class AnglePair
{
private:
	int azimuth, elevation;
	size_t tbTn;
	double tbTh;
public:
	AnglePair(int a, int e);
	AnglePair(const AnglePair& other);
	~AnglePair();

	int Azimuth() const;
	int Elevation() const;
	int TbTn() const;
	double TbTh() const;

	//old way of calculating indices
	int CalculateIndices(const std::vector<Point3D>& iPoints);

	//new way of calculating indices the map has to be sorted first
	int CalculateIndices(const std::vector<VecPoint3D>& elements);

	//Definition of the search method to be used by std::sort
	static inline bool SortPairs(const AnglePair& pair1, const AnglePair& pair2)
	{
		if (pair1.Elevation() == pair2.Elevation())
		{
			return pair1.Azimuth() < pair2.Azimuth();
		}
		else
		{
			return pair1.Elevation() < pair2.Elevation();
		}
	}
};
#endif _AnglePair
