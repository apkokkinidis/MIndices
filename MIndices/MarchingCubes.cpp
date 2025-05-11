// Author Anastasios Kokkinidis 
//MarchingCubes.cpp
#include "MarchingCubes.h"

using namespace MIndices;

//Constructs an array of imaginary cubes using the coordinates of neighbooring voxels. 
MarchingCubes::MarchingCubes(Array3D* voxels, int32_t DimX, int32_t DimY, int32_t DimZ)
{
	cubes.reserve((DimX - 1) * (DimY - 1) * (DimZ - 1));
	for (int32_t k = 1; k < DimZ; ++k)
	{
		for (int32_t j = 1; j < DimY; ++j)
		{
			for (int32_t i = 1; i < DimX; ++i)
			{
				MCube cube{ 0 };
				Point3D Point[8]{ 0 };

				// x	y	z	v1
				Point[0].x = static_cast<COORD_TYPE>(i);
				Point[0].y = static_cast<COORD_TYPE>(j);
				Point[0].z = static_cast<COORD_TYPE>(k);

				// x + 1	y	z	v2
				Point[1].x = static_cast<COORD_TYPE>(i + 1);
				Point[1].y = static_cast<COORD_TYPE>(j);
				Point[1].z = static_cast<COORD_TYPE>(k);

				// x + 1	 y	z + 1	v3
				Point[2].x = static_cast<COORD_TYPE>(i + 1);
				Point[2].y = static_cast<COORD_TYPE>(j);
				Point[2].z = static_cast<COORD_TYPE>(k + 1);

				// x	y 	z + 1	v4
				Point[3].x = static_cast<COORD_TYPE>(i);
				Point[3].y = static_cast<COORD_TYPE>(j);
				Point[3].z = static_cast<COORD_TYPE>(k + 1);

				// x	y + 1	z	v5
				Point[4].x = static_cast<COORD_TYPE>(i);
				Point[4].y = static_cast<COORD_TYPE>(j + 1);
				Point[4].z = static_cast<COORD_TYPE>(k);

				// x + 1	y + 1	z	v6
				Point[5].x = static_cast<COORD_TYPE>(i + 1);
				Point[5].y = static_cast<COORD_TYPE>(j + 1);
				Point[5].z = static_cast<COORD_TYPE>(k);

				// x + 1	y + 1	z + 1	v7
				Point[6].x = static_cast<COORD_TYPE>(i + 1);
				Point[6].y = static_cast<COORD_TYPE>(j + 1);
				Point[6].z = static_cast<COORD_TYPE>(k + 1);

				// x	y + 1	z + 1	v8
				Point[7].x = static_cast<COORD_TYPE>(i);
				Point[7].y = static_cast<COORD_TYPE>(j + 1);
				Point[7].z = static_cast<COORD_TYPE>(k + 1);

				//asign points to the cube
				std::copy(std::begin(Point), std::end(Point), std::begin(cube.Point));

				//asign values to the cube
				cube.val[0] = voxels->IsElementFilled((size_t)Point[0].x - 1, (size_t)Point[0].y - 1, (size_t)Point[0].z - 1);
				cube.val[1] = voxels->IsElementFilled((size_t)Point[1].x - 1, (size_t)Point[1].y - 1, (size_t)Point[1].z - 1);
				cube.val[2] = voxels->IsElementFilled((size_t)Point[2].x - 1, (size_t)Point[2].y - 1, (size_t)Point[2].z - 1);
				cube.val[3] = voxels->IsElementFilled((size_t)Point[3].x - 1, (size_t)Point[3].y - 1, (size_t)Point[3].z - 1);
				cube.val[4] = voxels->IsElementFilled((size_t)Point[4].x - 1, (size_t)Point[4].y - 1, (size_t)Point[4].z - 1);
				cube.val[5] = voxels->IsElementFilled((size_t)Point[5].x - 1, (size_t)Point[5].y - 1, (size_t)Point[5].z - 1);
				cube.val[6] = voxels->IsElementFilled((size_t)Point[6].x - 1, (size_t)Point[6].y - 1, (size_t)Point[6].z - 1);
				cube.val[7] = voxels->IsElementFilled((size_t)Point[7].x - 1, (size_t)Point[7].y - 1, (size_t)Point[7].z - 1);

				//assign cube to the cube vector
				cubes.push_back(cube);
			}//for X
		}//for Y
	}//for Z
	cubes.shrink_to_fit();
}

MarchingCubes::MarchingCubes(std::unique_ptr<Vector3D> voxels, size_t dim_x, size_t dim_y, size_t dim_z)
{
	vCubes.reserve((dim_x - 1) * (dim_y - 1) * (dim_z - 1));
	for (int32_t k = 1; k < dim_z; ++k)
	{
		for (int32_t j = 1; j < dim_y; ++j)
		{
			for (int32_t i = 1; i < dim_x; ++i)
			{
				VoxelCube vCube{ 0 };

				// x	y	z	v1
				vCube.vertex[0].point.x = static_cast<COORD_TYPE>(i);
				vCube.vertex[0].point.y = static_cast<COORD_TYPE>(j);
				vCube.vertex[0].point.z = static_cast<COORD_TYPE>(k);

				// x + 1	y	z	v2
				vCube.vertex[1].point.x = static_cast<COORD_TYPE>(i + 1);
				vCube.vertex[1].point.y = static_cast<COORD_TYPE>(j);
				vCube.vertex[1].point.z = static_cast<COORD_TYPE>(k);

				// x + 1	 y	z + 1	v3
				vCube.vertex[2].point.x = static_cast<COORD_TYPE>(i + 1);
				vCube.vertex[2].point.y = static_cast<COORD_TYPE>(j);
				vCube.vertex[2].point.z = static_cast<COORD_TYPE>(k + 1);

				// x	y 	z + 1	v4
				vCube.vertex[3].point.x = static_cast<COORD_TYPE>(i);
				vCube.vertex[3].point.y = static_cast<COORD_TYPE>(j);
				vCube.vertex[3].point.z = static_cast<COORD_TYPE>(k + 1);

				// x	y + 1	z	v5
				vCube.vertex[4].point.x = static_cast<COORD_TYPE>(i);
				vCube.vertex[4].point.y = static_cast<COORD_TYPE>(j + 1);
				vCube.vertex[4].point.z = static_cast<COORD_TYPE>(k);

				// x + 1	y + 1	z	v6
				vCube.vertex[5].point.x = static_cast<COORD_TYPE>(i + 1);
				vCube.vertex[5].point.y = static_cast<COORD_TYPE>(j + 1);
				vCube.vertex[5].point.z = static_cast<COORD_TYPE>(k);

				// x + 1	y + 1	z + 1	v7
				vCube.vertex[6].point.x = static_cast<COORD_TYPE>(i + 1);
				vCube.vertex[6].point.y = static_cast<COORD_TYPE>(j + 1);
				vCube.vertex[6].point.z = static_cast<COORD_TYPE>(k + 1);

				// x	y + 1	z + 1	v8
				vCube.vertex[7].point.x = static_cast<COORD_TYPE>(i);
				vCube.vertex[7].point.y = static_cast<COORD_TYPE>(j + 1);
				vCube.vertex[7].point.z = static_cast<COORD_TYPE>(k + 1);

				//ToDo refactor the below assignments as I understand it is hard to read and maintain.
				vCube.vertex[0].val = (bool)voxels->GetElementType((size_t)vCube.vertex[0].point.x - 1, (size_t)vCube.vertex[0].point.y - 1, (size_t)vCube.vertex[0].point.z - 1);
				vCube.vertex[1].val = (bool)voxels->GetElementType((size_t)vCube.vertex[1].point.x - 1, (size_t)vCube.vertex[1].point.y - 1, (size_t)vCube.vertex[1].point.z - 1);
				vCube.vertex[2].val = (bool)voxels->GetElementType((size_t)vCube.vertex[2].point.x - 1, (size_t)vCube.vertex[2].point.y - 1, (size_t)vCube.vertex[2].point.z - 1);
				vCube.vertex[3].val = (bool)voxels->GetElementType((size_t)vCube.vertex[3].point.x - 1, (size_t)vCube.vertex[3].point.y - 1, (size_t)vCube.vertex[3].point.z - 1);
				vCube.vertex[4].val = (bool)voxels->GetElementType((size_t)vCube.vertex[4].point.x - 1, (size_t)vCube.vertex[4].point.y - 1, (size_t)vCube.vertex[4].point.z - 1);
				vCube.vertex[5].val = (bool)voxels->GetElementType((size_t)vCube.vertex[5].point.x - 1, (size_t)vCube.vertex[5].point.y - 1, (size_t)vCube.vertex[5].point.z - 1);
				vCube.vertex[6].val = (bool)voxels->GetElementType((size_t)vCube.vertex[6].point.x - 1, (size_t)vCube.vertex[6].point.y - 1, (size_t)vCube.vertex[6].point.z - 1);
				vCube.vertex[7].val = (bool)voxels->GetElementType((size_t)vCube.vertex[7].point.x - 1, (size_t)vCube.vertex[7].point.y - 1, (size_t)vCube.vertex[7].point.z - 1);

				//assign cube to the cube vector
				vCubes.push_back(vCube);
			}//for X
		}//for Y
	}//for Z
	vCubes.shrink_to_fit();
}

//Triangulates cubes, extracts surface information from the cubes into an array of triangles.
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
				edges[0] = FindEdgeMidPoint(it->Point[0], it->Point[1]);
			}
			if (edgeTable[cubeindex] & 2)
			{
				edges[1] = FindEdgeMidPoint(it->Point[1], it->Point[2]);
			}
			if (edgeTable[cubeindex] & 4)
			{
				edges[2] = FindEdgeMidPoint(it->Point[2], it->Point[3]);
			}
			if (edgeTable[cubeindex] & 8)
			{
				edges[3] = FindEdgeMidPoint(it->Point[3], it->Point[0]);
			}
			if (edgeTable[cubeindex] & 16)
			{
				edges[4] = FindEdgeMidPoint(it->Point[4], it->Point[5]);
			}
			if (edgeTable[cubeindex] & 32)
			{
				edges[5] = FindEdgeMidPoint(it->Point[5], it->Point[6]);
			}
			if (edgeTable[cubeindex] & 64)
			{
				edges[6] = FindEdgeMidPoint(it->Point[6], it->Point[7]);
			}
			if (edgeTable[cubeindex] & 128)
			{
				edges[7] = FindEdgeMidPoint(it->Point[7], it->Point[4]);
			}
			if (edgeTable[cubeindex] & 256)
			{
				edges[8] = FindEdgeMidPoint(it->Point[4], it->Point[0]);
			}
			if (edgeTable[cubeindex] & 512)
			{
				edges[9] = FindEdgeMidPoint(it->Point[1], it->Point[5]);
			}
			if (edgeTable[cubeindex] & 1024)
			{
				edges[10] = FindEdgeMidPoint(it->Point[2], it->Point[6]);
			}
			if (edgeTable[cubeindex] & 2048)
			{
				edges[11] = FindEdgeMidPoint(it->Point[3], it->Point[7]);
			}
			for (size_t i = 0; triTable[cubeindex][i] != -1; i += 3)
			{
				Triangle triangle = GenerateTriangle(edges, cubeindex, static_cast<int32_t>(i));
				outTriangles.push_back(triangle);
			}
		}
	}
	outTriangles.shrink_to_fit();
}

void MIndices::MarchingCubes::TriangulateVCubes(std::vector<Triangle>& outTriangles) noexcept
{
	outTriangles.reserve(vCubes.size());
	for (auto& cube : vCubes)
	{
		Point3D edges[12]{};
		int32_t cubeindex;
		if (!CubeInOutSurface(cube, cubeindex))
		{
			if (edgeTable[cubeindex] & 1)
			{
				edges[0] = FindEdgeMidPoint(cube.vertex[0].point, cube.vertex[0].point);
			}
			if (edgeTable[cubeindex] & 2)
			{
				edges[1] = FindEdgeMidPoint(cube.vertex[1].point, cube.vertex[2].point);
			}
			if (edgeTable[cubeindex] & 4)
			{
				edges[2] = FindEdgeMidPoint(cube.vertex[2].point, cube.vertex[3].point);
			}
			if (edgeTable[cubeindex] & 8)
			{
				edges[3] = FindEdgeMidPoint(cube.vertex[3].point, cube.vertex[0].point);
			}
			if (edgeTable[cubeindex] & 16)
			{
				edges[4] = FindEdgeMidPoint(cube.vertex[4].point, cube.vertex[5].point);
			}
			if (edgeTable[cubeindex] & 32)
			{
				edges[5] = FindEdgeMidPoint(cube.vertex[5].point, cube.vertex[6].point);
			}
			if (edgeTable[cubeindex] & 64)
			{
				edges[6] = FindEdgeMidPoint(cube.vertex[6].point, cube.vertex[7].point);
			}
			if (edgeTable[cubeindex] & 128)
			{
				edges[7] = FindEdgeMidPoint(cube.vertex[7].point, cube.vertex[4].point);
			}
			if (edgeTable[cubeindex] & 256)
			{
				edges[8] = FindEdgeMidPoint(cube.vertex[4].point, cube.vertex[0].point);
			}
			if (edgeTable[cubeindex] & 512)
			{
				edges[9] = FindEdgeMidPoint(cube.vertex[1].point, cube.vertex[5].point);
			}
			if (edgeTable[cubeindex] & 1024)
			{
				edges[10] = FindEdgeMidPoint(cube.vertex[2].point, cube.vertex[6].point);
			}
			if (edgeTable[cubeindex] & 2048)
			{
				edges[11] = FindEdgeMidPoint(cube.vertex[3].point, cube.vertex[7].point);
			}
			for (int32_t i = 0; triTable[cubeindex][i] != -1; i += 3)
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

inline bool MIndices::MarchingCubes::CubeInOutSurface(const VoxelCube& cube, int32_t& cubeindex) const noexcept
{
	cubeindex = 0;
	if (cube.vertex[0].val) cubeindex |= 1;
	if (cube.vertex[1].val) cubeindex |= 2;
	if (cube.vertex[2].val) cubeindex |= 4;
	if (cube.vertex[3].val) cubeindex |= 8;
	if (cube.vertex[4].val) cubeindex |= 16;
	if (cube.vertex[5].val) cubeindex |= 32;
	if (cube.vertex[6].val) cubeindex |= 64;
	if (cube.vertex[7].val) cubeindex |= 128;

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

//Finds the midPoint of the given edge
inline Point3D MarchingCubes::FindEdgeMidPoint(const Point3D& p1, const Point3D& p2) const noexcept
{
	return { (p1.x + p2.x) * static_cast<COORD_TYPE>(0.5),
		(p1.y + p2.y) * static_cast<COORD_TYPE>(0.5),
		(p1.z + p2.z) * static_cast<COORD_TYPE>(0.5) };
}

//Prints triangles to a file
bool MarchingCubes::PrintTrianglesToFile(const std::vector<Triangle>& vecTriangles, const std::string& fPath)
{
	int32_t triCounter = 0;
	std::ofstream file(fPath, std::ios::out | std::ios::app);
	if (file.is_open())
	{
		std::cout << "Printing triangles into file " << fPath << std::endl;
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

//Prints triangles to a file using a buffer
bool MarchingCubes::PrintTrianglesToFile(const std::vector<Triangle>& vecTriangles, const size_t bufferSize, const std::string& fPath)
{
	int32_t triCounter = 0;
	std::ofstream file(fPath, std::ios::out | std::ios::app);
	if (file.is_open())
	{
		std::cout << "Printing triangles into file " << fPath << std::endl;
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

bool MarchingCubes::SaveToFile(const std::vector<Triangle>& triangles, const char* FileSpec) const
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

//Prints the cubes to a file
bool MarchingCubes::PrintCubesToFile(const std::string& fPath)
{
	int32_t cubeCounter = 0;
	std::ofstream file(fPath, std::ios::out | std::ios::app);
	if (file.is_open())
	{
		std::cout << "Printing cubes into file " << fPath << std::endl;
		file << "	X	Y	Z	ELEMENT Filled 0 | 1" << std::endl;
		std::vector<MCube>::iterator it;
		for (it = cubes.begin(); it != cubes.end(); ++it)
		{
			file << "\n \t-------Cube " << ++cubeCounter << " ---------\t" << std::endl;
			for (int32_t i = 0; i < 8; i++)
			{
				file << "	" << it->Point[i].x << "	" << it->Point[i].y << "	" << it->Point[i].z << "	" << it->val[i] << std::endl;
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
