// Author Anastasios Kokkinidis 
//MarchingCubes.cpp
#include "MarchingCubes.h"

//Constructs an array of imaginary cubes using the coordinates of neighbooring voxels. 
MarchingCubes::MarchingCubes(Array3D* voxels, int DimX, int DimY, int DimZ)
{
	cubes.reserve((DimX - 1) * (DimY - 1) * (DimZ - 1));
	for (int k = 1; k < DimZ; k++)
	{
		for (int j = 1; j < DimY; j++)
		{
			for (int i = 1; i < DimX; i++)
			{
				Cube cube{ 0 };
				Point3D point[8]{ 0 };

				// x	y	z	v1
				point[0].x = i;
				point[0].y = j;
				point[0].z = k;

				// x + 1	y	z	v2
				point[1].x = i + 1;
				point[1].y = j;
				point[1].z = k;

				// x + 1	 y	z + 1	v3
				point[2].x = i + 1;
				point[2].y = j;
				point[2].z = k + 1;

				// x	y 	z + 1	v4
				point[3].x = i;
				point[3].y = j;
				point[3].z = k + 1;

				// x	y + 1	z	v5
				point[4].x = i;
				point[4].y = j + 1;
				point[4].z = k;

				// x + 1	y + 1	z	v6
				point[5].x = i + 1;
				point[5].y = j + 1;
				point[5].z = k;

				// x + 1	y + 1	z + 1	v7
				point[6].x = i + 1;
				point[6].y = j + 1;
				point[6].z = k + 1;

				// x	y + 1	z + 1	v8
				point[7].x = i;
				point[7].y = j + 1;
				point[7].z = k + 1;

				//asign points to the cube
				std::copy(std::begin(point), std::end(point), std::begin(cube.point));

				//asign values to the cube
				cube.val[0] = voxels->IsElementFilled((size_t)point[0].x - 1, (size_t)point[0].y - 1, (size_t)point[0].z - 1);
				cube.val[1] = voxels->IsElementFilled((size_t)point[1].x - 1, (size_t)point[1].y - 1, (size_t)point[1].z - 1);
				cube.val[2] = voxels->IsElementFilled((size_t)point[2].x - 1, (size_t)point[2].y - 1, (size_t)point[2].z - 1);
				cube.val[3] = voxels->IsElementFilled((size_t)point[3].x - 1, (size_t)point[3].y - 1, (size_t)point[3].z - 1);
				cube.val[4] = voxels->IsElementFilled((size_t)point[4].x - 1, (size_t)point[4].y - 1, (size_t)point[4].z - 1);
				cube.val[5] = voxels->IsElementFilled((size_t)point[5].x - 1, (size_t)point[5].y - 1, (size_t)point[5].z - 1);
				cube.val[6] = voxels->IsElementFilled((size_t)point[6].x - 1, (size_t)point[6].y - 1, (size_t)point[6].z - 1);
				cube.val[7] = voxels->IsElementFilled((size_t)point[7].x - 1, (size_t)point[7].y - 1, (size_t)point[7].z - 1);

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

//Triangulates cubes, extracts surface information from the cubes into an array of triangles.
void MarchingCubes::TriangulateCubes(std::vector<TRIANGLE>& outTriangles) noexcept
{
	outTriangles.reserve(cubes.size());
	for (std::vector<Cube>::iterator it = cubes.begin(); it != cubes.end(); ++it)
	{
		Point3D edges[12]{};
		int cubeindex;
		if (!CubeInOutSurface(*it, cubeindex))
		{
			if (edgeTable[cubeindex] & 1)
			{
				edges[0] = FindEdgeMidpoint(it->point[0], it->point[1]);
			}
			if (edgeTable[cubeindex] & 2)
			{
				edges[1] = FindEdgeMidpoint(it->point[1], it->point[2]);
			}
			if (edgeTable[cubeindex] & 4)
			{
				edges[2] = FindEdgeMidpoint(it->point[2], it->point[3]);
			}
			if (edgeTable[cubeindex] & 8)
			{
				edges[3] = FindEdgeMidpoint(it->point[3], it->point[0]);
			}
			if (edgeTable[cubeindex] & 16)
			{
				edges[4] = FindEdgeMidpoint(it->point[4], it->point[5]);
			}
			if (edgeTable[cubeindex] & 32)
			{
				edges[5] = FindEdgeMidpoint(it->point[5], it->point[6]);
			}
			if (edgeTable[cubeindex] & 64)
			{
				edges[6] = FindEdgeMidpoint(it->point[6], it->point[7]);
			}
			if (edgeTable[cubeindex] & 128)
			{
				edges[7] = FindEdgeMidpoint(it->point[7], it->point[4]);
			}
			if (edgeTable[cubeindex] & 256)
			{
				edges[8] = FindEdgeMidpoint(it->point[4], it->point[0]);
			}
			if (edgeTable[cubeindex] & 512)
			{
				edges[9] = FindEdgeMidpoint(it->point[1], it->point[5]);
			}
			if (edgeTable[cubeindex] & 1024)
			{
				edges[10] = FindEdgeMidpoint(it->point[2], it->point[6]);
			}
			if (edgeTable[cubeindex] & 2048)
			{
				edges[11] = FindEdgeMidpoint(it->point[3], it->point[7]);
			}
			for (size_t i = 0; triTable[cubeindex][i] != -1; i += 3)
			{
				TRIANGLE triangle = GenerateTriangle(edges, cubeindex, i);
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
inline bool MarchingCubes::CubeInOutSurface(const Cube& cube, int& cubeindex) noexcept
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
inline TRIANGLE MarchingCubes::GenerateTriangle(const Point3D(&edges)[12], size_t cubeIndx, size_t edgeIndx) noexcept
{
	Point3D p1 = edges[triTable[cubeIndx][edgeIndx]];
	Point3D p2 = edges[triTable[cubeIndx][edgeIndx + 1]];
	Point3D p3 = edges[triTable[cubeIndx][edgeIndx + 2]];
	return { p1, p2, p3 };
}

//Finds the midpoint of the given edge
inline Point3D MarchingCubes::FindEdgeMidpoint(const Point3D& p1, const Point3D& p2) noexcept
{
	Point3D midPoint{ 0 };
	midPoint.x = (p1.x + p2.x) * 0.5f;
	midPoint.y = (p1.y + p2.y) * 0.5f;
	midPoint.z = (p1.z + p2.z) * 0.5f;

	return midPoint;
}

//Prints triangles to a file
bool MarchingCubes::PrintTrianglesToFile(const vector<TRIANGLE>& vecTriangles, const std::string& fPath)
{
	int triCounter = 0;
	std::ofstream file(fPath, std::ios::out | std::ios::app);
	if (file.is_open())
	{
		std::cout << "Printing triangles into file " << fPath << std::endl;
		file << "	X	Y	Z" << std::endl;
		std::vector<TRIANGLE>::const_iterator const_it;
		for (const_it = vecTriangles.begin(); const_it != vecTriangles.end(); ++const_it)
		{
			file << "\n \t-------TRIANGLE " << ++triCounter << " -----\t" << std::endl;
			for (int i = 0; i < 3; i++)
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

//Prints triangles to a file using a buffer
bool MarchingCubes::PrintTrianglesToFile(const vector<TRIANGLE>& vecTriangles, const size_t bufferSize, const std::string& fPath)
{
	int triCounter = 0;
	std::ofstream file(fPath, std::ios::out | std::ios::app);
	if (file.is_open())
	{
		std::cout << "Printing triangles into file " << fPath << std::endl;
		file << "	X	Y	Z" << std::endl;
		std::vector<TRIANGLE>::const_iterator const_it;
		for (const_it = vecTriangles.begin(); const_it != vecTriangles.end(); ++const_it)
		{
			char* buffer = new char[bufferSize];
			sprintf(buffer, "\n \t-------TRIANGLE %d -----\n", ++triCounter);
			for (int i = 0; i < 3; i++)
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

bool MarchingCubes::SaveToFile(const vector<TRIANGLE>& triangles, const char* FileSpec)
{
	FILE* file = (FILE*)fopen(FileSpec, "w+");
	if (file == nullptr)
	{
		//show error
		std::cerr << "Can't open file " << FileSpec << std::endl;
		return false;
	}
	fprintf(file, "%I64u \n", triangles.size());
	for (int i = 0; i < triangles.size() - 1; i++)
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

//Prints the cubes to a file
bool MarchingCubes::PrintCubesToFile(const std::string& fPath)
{
	int cubeCounter = 0;
	std::ofstream file(fPath, std::ios::out | std::ios::app);
	if (file.is_open())
	{
		std::cout << "Printing cubes into file " << fPath << std::endl;
		file << "	X	Y	Z	ELEMENT Filled 0 | 1" << std::endl;
		std::vector<Cube>::iterator it;
		for (it = cubes.begin(); it != cubes.end(); ++it)
		{
			file << "\n \t-------Cube " << ++cubeCounter << " ---------\t" << std::endl;
			for (int i = 0; i < 8; i++)
			{
				file << "	" << it->point[i].x << "	" << it->point[i].y << "	" << it->point[i].z << "	" << it->val[i] << std::endl;
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
