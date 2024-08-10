// Author Anastasios Kokkinidis 
//MarchingCubes.cpp
#include "MarchingCubes.h"

using namespace MIndices;

//Constructs an array of imaginary cubes using the coordinates of neighbooring voxels. 
MarchingCubes::MarchingCubes(Array3D* voxels, int32_t DimX, int32_t DimY, int32_t DimZ)
{
	cubes.reserve((DimX - 1) * (DimY - 1) * (DimZ - 1));
	for (int32_t k = 1; k < DimZ; k++)
	{
		for (int32_t j = 1; j < DimY; j++)
		{
			for (int32_t i = 1; i < DimX; i++)
			{
				MCube cube{ 0 };
				Point3D point32_t[8]{ 0 };

				// x	y	z	v1
				point32_t[0].x = i;
				point32_t[0].y = j;
				point32_t[0].z = k;

				// x + 1	y	z	v2
				point32_t[1].x = i + 1;
				point32_t[1].y = j;
				point32_t[1].z = k;

				// x + 1	 y	z + 1	v3
				point32_t[2].x = i + 1;
				point32_t[2].y = j;
				point32_t[2].z = k + 1;

				// x	y 	z + 1	v4
				point32_t[3].x = i;
				point32_t[3].y = j;
				point32_t[3].z = k + 1;

				// x	y + 1	z	v5
				point32_t[4].x = i;
				point32_t[4].y = j + 1;
				point32_t[4].z = k;

				// x + 1	y + 1	z	v6
				point32_t[5].x = i + 1;
				point32_t[5].y = j + 1;
				point32_t[5].z = k;

				// x + 1	y + 1	z + 1	v7
				point32_t[6].x = i + 1;
				point32_t[6].y = j + 1;
				point32_t[6].z = k + 1;

				// x	y + 1	z + 1	v8
				point32_t[7].x = i;
				point32_t[7].y = j + 1;
				point32_t[7].z = k + 1;

				//asign points to the cube
				std::copy(std::begin(point32_t), std::end(point32_t), std::begin(cube.point32_t));

				//asign values to the cube
				cube.val[0] = voxels->IsElementFilled((size_t)point32_t[0].x - 1, (size_t)point32_t[0].y - 1, (size_t)point32_t[0].z - 1);
				cube.val[1] = voxels->IsElementFilled((size_t)point32_t[1].x - 1, (size_t)point32_t[1].y - 1, (size_t)point32_t[1].z - 1);
				cube.val[2] = voxels->IsElementFilled((size_t)point32_t[2].x - 1, (size_t)point32_t[2].y - 1, (size_t)point32_t[2].z - 1);
				cube.val[3] = voxels->IsElementFilled((size_t)point32_t[3].x - 1, (size_t)point32_t[3].y - 1, (size_t)point32_t[3].z - 1);
				cube.val[4] = voxels->IsElementFilled((size_t)point32_t[4].x - 1, (size_t)point32_t[4].y - 1, (size_t)point32_t[4].z - 1);
				cube.val[5] = voxels->IsElementFilled((size_t)point32_t[5].x - 1, (size_t)point32_t[5].y - 1, (size_t)point32_t[5].z - 1);
				cube.val[6] = voxels->IsElementFilled((size_t)point32_t[6].x - 1, (size_t)point32_t[6].y - 1, (size_t)point32_t[6].z - 1);
				cube.val[7] = voxels->IsElementFilled((size_t)point32_t[7].x - 1, (size_t)point32_t[7].y - 1, (size_t)point32_t[7].z - 1);

				//assign cube to the cube vector
				cubes.push_back(cube);
			}//for X
		}//for Y
	}//for Z
	cubes.shrink_to_fit();
}

MarchingCubes::~MarchingCubes()
{
	if (!this->cubes.empty())
	{
		this->cubes.clear();
	}
}

//Triangulates cubes, extracts surface information from the cubes int32_to an array of triangles.
void MarchingCubes::TriangulateCubes(std::vector<Triangle>& outTriangles) noexcept
{
	outTriangles.reserve(cubes.size());
	for (std::vector<MCube>::iterator it = cubes.begin(); it != cubes.end(); ++it)
	{
		Point3D edges[12]{};
		int32_t cubeindex;
		if (!CubeInOutSurface(*it, cubeindex))
		{
			if (edgeTable[cubeindex] & 1)
			{
				edges[0] = FindEdgeMidpoint32_t(it->point32_t[0], it->point32_t[1]);
			}
			if (edgeTable[cubeindex] & 2)
			{
				edges[1] = FindEdgeMidpoint32_t(it->point32_t[1], it->point32_t[2]);
			}
			if (edgeTable[cubeindex] & 4)
			{
				edges[2] = FindEdgeMidpoint32_t(it->point32_t[2], it->point32_t[3]);
			}
			if (edgeTable[cubeindex] & 8)
			{
				edges[3] = FindEdgeMidpoint32_t(it->point32_t[3], it->point32_t[0]);
			}
			if (edgeTable[cubeindex] & 16)
			{
				edges[4] = FindEdgeMidpoint32_t(it->point32_t[4], it->point32_t[5]);
			}
			if (edgeTable[cubeindex] & 32)
			{
				edges[5] = FindEdgeMidpoint32_t(it->point32_t[5], it->point32_t[6]);
			}
			if (edgeTable[cubeindex] & 64)
			{
				edges[6] = FindEdgeMidpoint32_t(it->point32_t[6], it->point32_t[7]);
			}
			if (edgeTable[cubeindex] & 128)
			{
				edges[7] = FindEdgeMidpoint32_t(it->point32_t[7], it->point32_t[4]);
			}
			if (edgeTable[cubeindex] & 256)
			{
				edges[8] = FindEdgeMidpoint32_t(it->point32_t[4], it->point32_t[0]);
			}
			if (edgeTable[cubeindex] & 512)
			{
				edges[9] = FindEdgeMidpoint32_t(it->point32_t[1], it->point32_t[5]);
			}
			if (edgeTable[cubeindex] & 1024)
			{
				edges[10] = FindEdgeMidpoint32_t(it->point32_t[2], it->point32_t[6]);
			}
			if (edgeTable[cubeindex] & 2048)
			{
				edges[11] = FindEdgeMidpoint32_t(it->point32_t[3], it->point32_t[7]);
			}
			for (size_t i = 0; triTable[cubeindex][i] != -1; i += 3)
			{
				Triangle triangle = GenerateTriangle(edges, cubeindex, i);
				outTriangles.push_back(triangle);
			}
		}
	}
	outTriangles.shrink_to_fit();
}

/*
	Determine if the cube is entirely in/out of the surface.
	@param	a constant reference to an existing Cube.
	@return true if the cube is entirely in/out of the surface.
*/
inline bool MarchingCubes::CubeInOutSurface(const MCube& cube, int32_t& cubeindex) const noexcept
{
	cubeindex = 0;
	if (cube.val[0]) cubeindex |= 1;
	if (cube.val[1]) cubeindex |= 2;
	if (cube.val[2]) cubeindex |= 4;
	if (cube.val[3]) cubeindex |= 8;
	if (cube.val[4]) cubeindex |= 16;
	if (cube.val[5]) cubeindex |= 32;
	if (cube.val[6]) cubeindex |= 64;
	if (cube.val[7]) cubeindex |= 128;

	return edgeTable[cubeindex] == 0;
}
/*
	Generates a triangle.
	@param	a constant reference of the edges of the cube.
	@return the generated triangle.
*/
inline Triangle MarchingCubes::GenerateTriangle(const Point3D(&edges)[12], int32_t cubeIndx, int32_t edgeIndx) const noexcept
{
	Point3D p1 = edges[triTable[cubeIndx][edgeIndx]];
	Point3D p2 = edges[triTable[cubeIndx][edgeIndx + 1]];
	Point3D p3 = edges[triTable[cubeIndx][edgeIndx + 2]];
	return { p1, p2, p3 };
}

//Finds the midpoint32_t of the given edge
inline Point3D MarchingCubes::FindEdgeMidpoint32_t(const Point3D& p1, const Point3D& p2) const noexcept
{
	Point3D midPoint32_t{ 0 };
	midPoint32_t.x = (p1.x + p2.x) * 0.5f;
	midPoint32_t.y = (p1.y + p2.y) * 0.5f;
	midPoint32_t.z = (p1.z + p2.z) * 0.5f;

	return midPoint32_t;
}

//Print32_ts triangles to a file
bool MarchingCubes::Print32_tTrianglesToFile(const vector<Triangle>& vecTriangles, const std::string& fPath)
{
	int32_t triCounter = 0;
	std::ofstream file(fPath, std::ios::out | std::ios::app);
	if (file.is_open())
	{
		std::cout << "Print32_ting triangles int32_to file " << fPath << std::endl;
		file << "	X	Y	Z" << std::endl;
		std::vector<Triangle>::const_iterator const_it;
		for (const_it = vecTriangles.begin(); const_it != vecTriangles.end(); ++const_it)
		{
			file << "\n \t-------TRIANGLE " << ++triCounter << " -----\t" << std::endl;
			for (int32_t i = 0; i < 3; i++)
			{
				file << "	" << const_it->p[i].x << "	" << const_it->p[i].y << "	" << const_it->p[i].z << std::endl;
			}
		}
		std::cout << "Done!" << std::endl;
		file.close();
		return true;
	}
	else
	{
		return false;
	}
}

//Print32_ts triangles to a file using a buffer
bool MarchingCubes::Print32_tTrianglesToFile(const vector<Triangle>& vecTriangles, const size_t bufferSize, const std::string& fPath)
{
	int32_t triCounter = 0;
	std::ofstream file(fPath, std::ios::out | std::ios::app);
	if (file.is_open())
	{
		std::cout << "Print32_ting triangles int32_to file " << fPath << std::endl;
		file << "	X	Y	Z" << std::endl;
		std::vector<Triangle>::const_iterator const_it;
		for (const_it = vecTriangles.begin(); const_it != vecTriangles.end(); ++const_it)
		{
			char* buffer = new char[bufferSize];
			sprintf(buffer, "\n \t-------TRIANGLE %d -----\n", ++triCounter);
			for (int32_t i = 0; i < 3; i++)
			{
				sprintf(buffer + std::strlen(buffer), "\t%.1f\t%.1f\t%.1f\n", const_it->p[i].x, const_it->p[i].y, const_it->p[i].z);
			}
			/* This section removes the unneeded N's from final string but adds further junk character during copying*/
			size_t indx = 0;
			while (buffer[indx] != -51 && indx < bufferSize * sizeof(char))
			{
				++indx;
			}
			char* newBuffer = new char[indx];
			memcpy(newBuffer, buffer, sizeof(*newBuffer) * indx);
			//toDo resize the array to remove NULL values.
			file.write(newBuffer, bufferSize);
			file.flush();
			delete[] buffer;
			delete[] newBuffer;
		}
		std::cout << "Done!" << std::endl;
		file.close();
		return true;
	}
	else
	{
		return false;
	}
}

bool MarchingCubes::SaveToFile(const vector<Triangle>& triangles, const char* FileSpec) const
{
	FILE* file = (FILE*)fopen(FileSpec, "w+");
	if (file == nullptr)
	{
		//show error
		std::cerr << "Can't open file " << FileSpec << std::endl;
		return false;
	}
	fprintf(file, "%I64u \n", triangles.size());
	for (int32_t i = 0; i < triangles.size() - 1; i++)
	{
		COORD_TYPE x1 = triangles[i].p[0].x;
		COORD_TYPE y1 = triangles[i].p[0].y;
		COORD_TYPE z1 = triangles[i].p[0].z;
		COORD_TYPE x2 = triangles[i].p[1].x;
		COORD_TYPE y2 = triangles[i].p[1].y;
		COORD_TYPE z2 = triangles[i].p[1].z;
		COORD_TYPE x3 = triangles[i].p[2].x;
		COORD_TYPE y3 = triangles[i].p[2].y;
		COORD_TYPE z3 = triangles[i].p[2].z;
		fprintf(file, "%f\t%f\t%f\t,%f\t%f\t%f\t,%f\t%f\t%f\n", (float)x1, (float)y1, (float)z1,
			(float)x2, (float)y2, (float)z2,
			(float)x3, (float)y3, (float)z3);
	}
	fclose(file);
	return true;
}

//Print32_ts the cubes to a file
bool MarchingCubes::Print32_tCubesToFile(const std::string& fPath)
{
	int32_t cubeCounter = 0;
	std::ofstream file(fPath, std::ios::out | std::ios::app);
	if (file.is_open())
	{
		std::cout << "Print32_ting cubes int32_to file " << fPath << std::endl;
		file << "	X	Y	Z	ELEMENT Filled 0 | 1" << std::endl;
		std::vector<MCube>::iterator it;
		for (it = cubes.begin(); it != cubes.end(); ++it)
		{
			file << "\n \t-------Cube " << ++cubeCounter << " ---------\t" << std::endl;
			for (int32_t i = 0; i < 8; i++)
			{
				file << "	" << it->point32_t[i].x << "	" << it->point32_t[i].y << "	" << it->point32_t[i].z << "	" << it->val[i] << std::endl;
			}
		}
		std::cout << "Done!" << std::endl;
		file.close();
		return true;
	}
	else
	{
		return false;
	}
}
