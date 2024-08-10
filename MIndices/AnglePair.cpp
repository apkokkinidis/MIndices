// Author Anastasios Kokkinidis 
//AnglePair.cpp

/*The distance formula states that the distance between two Points in xyz-space
is the square root of the sum of the squares of the differences between corresponding coordinates.
That is, given P1 = (x1,y1,z1) and P2 = (x2,y2,z2), the distance between P1 and P2 is given by
d(P1,P2) = srt(x2 - x1)^2 + (y2 - y1)^2 + (z2 - z1)^2.*/
#include <string>
#include <iostream>
#include <fstream>

#include "AnglePair.h"

using namespace MIndices;

AnglePair::AnglePair(int32_t a, int32_t e) : azimuth(a), elevation(e)
{
	tbTn = 0;
	tbTh = 0.0f;
}

AnglePair::AnglePair(const AnglePair& other)
{
	if (this != &other)
	{
		azimuth = other.azimuth;
		elevation = other.elevation;
		tbTn = other.tbTn;
		tbTh = other.tbTh;
	}
}

//The input vector must already be sorted to provide accurate results
int32_t AnglePair::CalculateIndices(const std::vector<VecPoint3D>& elements)
{
	if (elements.empty())
	{
		return 0;
	}
	COORD_TYPE sumM = (COORD_TYPE)0.0;
	size_t count = 0;
	for (size_t i = 0; i < elements.size() - 1; i++)
	{
		COORD_TYPE sum = (COORD_TYPE)0.0;
		size_t length = elements[i].size();
		//increase step by 2 taking each pair of 2 Points
		for (size_t j = 0; j < length; j += 2)
		{
			COORD_TYPE x1 = elements[i][j].x;
			COORD_TYPE y1 = elements[i][j].y;
			COORD_TYPE z1 = elements[i][j].z;
			COORD_TYPE x2 = elements[i][j + 1].x;
			COORD_TYPE y2 = elements[i][j + 1].y;
			COORD_TYPE z2 = elements[i][j + 1].z;

			double d = sqrt(((double)x2 - (double)x1) * ((double)x2 - (double)x1)
				+ ((double)y2 - (double)y1) * ((double)y2 - (double)y1)
				+ ((double)z2 - (double)z1) * ((double)z2 - (double)z1));
			sum += d;
			count++;
		}
		sumM += sum;
		//calculate distances between Points
	}
	//peform delayed division
	tbTh = sumM / elements.size();
	tbTn = count;
	return 1;
}
int32_t AnglePair::Azimuth() const
{
	return azimuth;
}
int32_t AnglePair::Elevation() const
{
	return elevation;
}
int32_t AnglePair::TbTn() const
{
	return static_cast<int32_t>(tbTn);
}
double AnglePair::TbTh() const
{
	return tbTh;
}

int32_t MIndices::PrintPairs(const std::string& fPath, const std::vector<AnglePair>& pairs, int32_t duration)
{
	std::ofstream file(fPath, std::ios::out | std::ios::app);
	if (file.is_open())
	{
		file << "Total runtime : " << duration << std::endl;
		for (size_t i = 0; i < pairs.size(); i++)
		{
			file << "Azimuth : " << pairs[i].Azimuth()
				<< " Elevation : " << pairs[i].Elevation()
				<< " Tabecular Number : " << pairs[i].TbTn()
				<< " Tabecular Thickness : " << pairs[i].TbTh()
				<< std::endl;;
		}
		file.close();
		return 1;
	}
	else
	{
		return 0;
	}
}

void MIndices::SavePairs(const std::string& fPath, const std::vector<AnglePair>& pairs)
{
	const int32_t step = 180;
	std::ofstream file(fPath, std::ios::out | std::ios::app);
	if (file.is_open())
	{
		for (size_t i = 0; i < pairs.size(); i += step)
		{
			double sum = 0.0f;
			double mean = 0.0f;
			for (size_t j = 0; j < step; j++)
			{
				sum += pairs[i + j].TbTh();
			}
			mean = sum / step;
			file << i / step << "\t"	//step
				<< mean << std::endl;	//tabecular thickness
		}
		file.close();
	}
}
