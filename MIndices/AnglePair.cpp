// Author Anastasios Kokkinidis 
//AnglePair.cpp
#include "AnglePair.h"

AnglePair::AnglePair(int a, int e) : azimuth(a), elevation(e)
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

AnglePair::~AnglePair()
{
	azimuth = elevation = 0;
	tbTh = 0;
	tbTn = 0;
}

//calculates the average distance between the intersection points
//Note that the points vector must already be sorted based on the distance of the points to the origin of the ray
int AnglePair::CalculateIndices(const std::vector<Point3D>& iPoints)
{
	if (iPoints.size() > 0)
	{
		size_t length = iPoints.size();
		double sum = 0.0;
		for (size_t i = 1; i < length; i++)
		{
			//calculate dist from 0 to n - 1 + n
			COORD_TYPE x1 = iPoints[i - 1].x;
			COORD_TYPE y1 = iPoints[i - 1].y;
			COORD_TYPE z1 = iPoints[i - 1].z;
			COORD_TYPE x2 = iPoints[i].x;
			COORD_TYPE y2 = iPoints[i].y;
			COORD_TYPE z2 = iPoints[i].z;

			double d = sqrt(((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1) + (z2 - z1) * (z2 - z1)));
			sum += d;
		}
		//divide sum by number of points to determine average value
		tbTh = sum / (double)length;
		return 1;
	}
	else
	{
		return 0;
	}
}
//The input vector must already be sorted to provide accurate results
int AnglePair::CalculateIndices(const std::vector<VecPoint3D>& elements)
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
		size_t length = elements[i].p.size();
		//increase step by 2 taking each pair of 2 points
		for (size_t j = 0; j < length; j += 2)
		{
			COORD_TYPE x1 = elements[i].p[j].x;
			COORD_TYPE y1 = elements[i].p[j].y;
			COORD_TYPE z1 = elements[i].p[j].z;
			COORD_TYPE x2 = elements[i].p[j + 1].x;
			COORD_TYPE y2 = elements[i].p[j + 1].y;
			COORD_TYPE z2 = elements[i].p[j + 1].z;

			double d = sqrt(((double)x2 - (double)x1) * ((double)x2 - (double)x1)
				+ ((double)y2 - (double)y1) * ((double)y2 - (double)y1)
				+ ((double)z2 - (double)z1) * ((double)z2 - (double)z1));
			sum += d;
			count++;
		}
		sumM += sum;
		//calculate distances between points
	}
	//peform delayed division
	tbTh = sumM / elements.size();
	tbTn = count;
	return 1;
}
int AnglePair::Azimuth() const
{
	return azimuth;
}
int AnglePair::Elevation() const
{
	return elevation;
}
int AnglePair::TbTn() const
{
	return tbTn;
}
double AnglePair::TbTh() const
{
	return tbTh;
}
/*The distance formula states that the distance between two points in xyz-space
is the square root of the sum of the squares of the differences between corresponding coordinates.
That is, given P1 = (x1,y1,z1) and P2 = (x2,y2,z2), the distance between P1 and P2 is given by
d(P1,P2) = srt(x2 - x1)^2 + (y2 - y1)^2 + (z2 - z1)^2.*/