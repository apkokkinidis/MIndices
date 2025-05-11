// Author Anastasios Kokkinidis 
//AnglePair.h
#pragma once

#include <stdint.h>
#include <vector>
#include <map>

//Custom headers
#include "Structures.h"
#include "Ray.h"


//tabecular seperation method requires to ray trace the structure and find the points where there is empty space (reverse operation of tabecular thickness)
//tabecular number is the number of beams the structure has, it is found by finding the entry and exit points between triangles.

namespace MIndices
{

	class AnglePair
	{
	public:
		AnglePair(int32_t a, int32_t e);
		AnglePair(const AnglePair& other);
		~AnglePair() = default;
		AnglePair(AnglePair&&) = default;
		AnglePair& operator=(AnglePair&&) = default;
		AnglePair& operator=(const AnglePair& other) = default;

		int32_t Azimuth() const;
		int32_t Elevation() const;
		int32_t TbTn() const;
		double TbTh() const;

		//new way of calculating indices the map has to be sorted first
		int32_t CalculateIndices(const std::vector<VecPoint3D>& elements);

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
	private:
		int32_t azimuth, elevation;
		size_t tbTn;
		double tbTh;
	};

	int32_t PrintPairs(const std::string& fPath, const std::vector<AnglePair>& pairs, int32_t duration);
	void SavePairs(const std::string& fPath, const std::vector<AnglePair>& pairs);
}
