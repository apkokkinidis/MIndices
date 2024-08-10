// Main.cpp : This file contains the 'main' function. Program execution begins and ends here.
// Author Anastasios Kokkinidis 

//In order to run the program, you to pass as input the path of the voxel file from the command line.

#include <stdint.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iterator>
#include <omp.h>
#include <chrono>

//custom headers
#include "Array3D.h"
#include "MarchingCubes.h"
#include "Ray.h"
#include "BVHTree.h"
#include "AnglePair.h"

using std::vector;
using namespace MIndices;

//Globals
constexpr uint32_t NUM_OF_THREADS_X_16 = 16;
constexpr uint32_t NUM_OF_THREADS_X_8 = 8;
constexpr uint32_t NUM_OF_THREADS_X_4 = 4;
constexpr uint32_t NUM_OF_THREADS_X_2 = 2;
bool parallel_flag = true;
bool has_precomputed_edges = false;

//Forward declarations
int32_t ComputeIndices(BVHTree* bvh, vector<Ray>& r, vector<AnglePair>& pairs);
int32_t ComputeIndicesParallel(BVHTree* bvh, vector<Ray>& r, vector<AnglePair>& pairs);
inline void RotateRays(double d, Axis a, vector<Ray>& ray);

int32_t Print32_tPairs(const std::string& fPath, const std::vector<AnglePair>& pairs, int32_t duration);
void SavePairs(const std::string& fPath, const std::vector<AnglePair>& pairs);

//utility functions
int32_t FindCharByEndOfString(const std::string& str, char c);

//sort method for std::sort
inline void Sortpoints(std::vector<Point3D>& points, const Ray& ray);

int32_t main(int32_t argc, char* argv[])
{
	if (argc < 1)
	{
		std::cerr << "Not enough command line arguements." << std::endl;
		return -1;
	}

	size_t DimX;
	size_t DimY;
	size_t DimZ;
	std::string filename;
	for (int32_t i = 1; i < argc; i++)
	{
		filename.append(argv[i]);
	}
	std::cout << "Input file dimensions" << std::endl << "X: ";
	std::cin >> DimX;
	std::cout << "Y: ";
	std::cin >> DimY;
	std::cout << "Z: ";
	std::cin >> DimZ;
	Array3D* array3D = new Array3D(DimX, DimY, DimZ);
	if (array3D->LoadFromFile(filename.c_str()))
	{
		std::cout << "Loaded file " << filename.c_str() << std::endl;
	}
	else
	{
		std::cout << "Failed to load file " << filename.c_str() << std::endl;
		return -1;
	}
	DimX = array3D->GetDimX();
	DimY = array3D->GetDimY();
	DimZ = array3D->GetDimZ();

	/*----------------Marching Cubes-------------------*/
	MarchingCubes cubes(array3D, DimX, DimY, DimZ);
	delete array3D;

	vector<Triangle> vecTriang;
	std::cout << "Running MC algorithm." << std::endl;
	cubes.TriangulateCubes(vecTriang);
	cubes.SaveToFile(vecTriang, "Triangles.txt");
	std::cout << "MC finished, resulting triangles number = " << vecTriang.size() << std::endl;

	/*----------------Bounding Volume Hierarchy construction-------------------*/
	std::cout << "Building Bounding Volume Hierarchy" << std::endl;
	std::chrono::high_resolution_clock::time_point start_f_bvh = std::chrono::high_resolution_clock::now();
	BVHTree* bvh = new BVHTree();
	bvh->TopDownBuildObjectMedian(vecTriang);
	vecTriang.clear();
	if (bvh->TreeIsEmpty())
	{
		std::cerr << "Failed to build BVH." << std::endl;
		return -1;
	}
	std::chrono::high_resolution_clock::time_point stop_f_bvh = std::chrono::high_resolution_clock::now();
	std::chrono::milliseconds dur_f_bvh = std::chrono::duration_cast<std::chrono::milliseconds>(stop_f_bvh - start_f_bvh);
	std::cout << "Hierarchy construction finished. Time : " << dur_f_bvh.count() << " milliseconds." << std::endl;

	BVHNode* tree = bvh->GetRoot();
	int32_t treeDepth = bvh->FindDepth(tree);
	int32_t numNodes = 0;
	int32_t numLeaves = 0;
	bvh->DFSTraverse(tree, numNodes, numLeaves);
	std::cout << "Tree depth : " << treeDepth << " No.Nodes : " << numNodes << " No.Leaves : " << numLeaves << std::endl;

	/*----------------Ray Generation -------------------*/
	size_t diagonal = tree->Box().Diagonal();
	RayGenerator rayGenerator(DimX, DimY, diagonal);
	vector<Ray> r = rayGenerator.Rays();

	//RayTraceVolume
	vector<AnglePair> pairs;
	vector<double> t;

	int32_t res = 0;
	std::cout << "Ray tracing volume." << std::endl;
	std::chrono::high_resolution_clock::time_point start_f = std::chrono::high_resolution_clock::now();
	if (parallel_flag)
	{
		res = ComputeIndicesParallel(bvh, r, pairs);
	}
	else
	{
		res = ComputeIndices(bvh, r, pairs);
	}
	std::chrono::high_resolution_clock::time_point stop_f = std::chrono::high_resolution_clock::now();
	std::chrono::minutes dur_f = std::chrono::duration_cast<std::chrono::minutes>(stop_f - start_f);
	int32_t durMinutes = dur_f.count();
	std::cout << "Total time : " << durMinutes << " minutes" << std::endl;
	delete bvh;

	//print32_t results int32_to file
	if (res == 1)
	{
		if (parallel_flag)
		{
			//sort angle pairs
			std::sort(pairs.begin(), pairs.end(), AnglePair::SortPairs);
			int32_t indx = FindCharByEndOfString(filename, '\\');
			std::string file = filename.substr(indx + 1, filename.length() - 3).append("NT_").append(std::to_string(NUM_OF_THREADS_X_16)).append("_Pairs_Parallel.txt");
			int32_t r = Print32_tPairs(file, pairs, durMinutes);
		}
		else
		{
			int32_t indx = FindCharByEndOfString(filename, '\\');
			std::string file = filename.substr(indx + 1, filename.length() - 3).append("_Pairs.txt");
			int32_t r = Print32_tPairs(file, pairs, durMinutes);
		}
	}
	return 0;
}

int32_t FindCharByEndOfString(const std::string& str, char c)
{
	for (auto i = str.length() - 1; i > 1; i--)
	{
		if (str[i] == c)
		{
			return i;
		}
	}
	//character not found return -1.
	return -1;
}

//Sort points based on their distance to the ray origin.
inline void Sortpoints(std::vector<Point3D>& points, const Ray& ray)
{
	std::sort(points.begin(), points.end(), [ray](const Point3D& p1, const Point3D& p2)
		{
			Point3D origin = ray.GetOrigin();
			COORD_TYPE dist1 = sqrt((p1.x - origin.x) * (p1.x - origin.x) + (p1.y - origin.y) * (p1.y - origin.y) + (p1.z - origin.z) * (p1.z - origin.z));
			COORD_TYPE dist2 = sqrt((p2.x - origin.x) * (p2.x - origin.x) + (p2.y - origin.y) * (p2.y - origin.y) + (p2.z - origin.z) * (p2.z - origin.z));
			return dist2 > dist1;
		});
}


//Rotates an array of rays
//@param degrees of rotation
//@param rotation axis
inline void RotateRays(double d, Axis a, vector<Ray>& ray)
{
	for (auto it = ray.begin(); it != ray.end(); ++it)
	{
		it->Rotate(d, a);
	}
}

int32_t Print32_tPairs(const std::string& fPath, const std::vector<AnglePair>& pairs, int32_t duration)
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

void SavePairs(const std::string& fPath, const std::vector<AnglePair>& pairs)
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

int32_t ComputeIndices(BVHTree* bvh, vector<Ray>& rays, vector<AnglePair>& pairs)
{
	BVHNode* root = bvh->GetRoot();
	if (rays.empty())
	{
		//print32_t error and exit early
		std::cerr << "The ray vector is empty." << std::endl;
		return -1;
	}
	pairs.reserve(90 * 180);
	//Test rays for any int32_tersections with the structure, then rotate them 
	for (int32_t e = 0; e < 90; e++)
	{
		std::chrono::high_resolution_clock::time_point start_f = std::chrono::high_resolution_clock::now();
		for (int32_t a = 0; a < 180; a++)
		{
			uint32_t count = 0;
			std::vector<VecPoint3D> points;
			for (size_t i = 0; i < rays.size() - 1; i++)
			{
				vector<Point3D> p;
				bvh->RayTraceNodes(root, rays[i], p);
				if (!p.empty() && p.size() % 2 == 0)
				{
					//sort points based on the distance from the origin of the ray
					Sortpoints(p, rays[i]);
					points.push_back({ p });
				}
				else if (p.size() == 1)
				{
					count++;
				}
			}
			//Rotate Rays by 1 degree
			RotateRays(1.0, Axis::X, rays);
			AnglePair newPair(a, e);
			newPair.CalculateIndices(points);
			pairs.push_back(newPair);
		}
		std::chrono::high_resolution_clock::time_point stop_f = std::chrono::high_resolution_clock::now();
		std::chrono::seconds dur = std::chrono::duration_cast<std::chrono::seconds>(stop_f - start_f);
		RotateRays(1.0, Axis::Z, rays);
		std::cout << "Cycle " << e + 1 << " out of 90 duration: " << dur.count() << " sec" << std::endl;
	}
	return 1;
}

int32_t ComputeIndicesParallel(BVHTree* bvh, vector<Ray>& r, vector<AnglePair>& pairs)
{
	//Test rays for int32_tersections then rotate them 
	for (int32_t e = 0; e < 90; e++)
	{
		std::chrono::high_resolution_clock::time_point start_f = std::chrono::high_resolution_clock::now();
		for (int32_t a = 0; a < 180; a += NUM_OF_THREADS_X_16)
		{
			vector<Ray> rParallel[NUM_OF_THREADS_X_16];
			size_t aParallel[NUM_OF_THREADS_X_16];
			rParallel[0] = r;
			aParallel[0] = a;
			for (size_t i = 1; i < NUM_OF_THREADS_X_16; i++)
			{
				aParallel[i] = a + i;
				rParallel[i] = rParallel[i - 1];
				RotateRays(1.0, Axis::X, rParallel[i]);
			}
			omp_set_num_threads(NUM_OF_THREADS_X_16);
			//Parallel region
#pragma omp parallel
			{
				int32_t id = omp_get_thread_num();
				BVHNode* root = bvh->GetRoot();
				vector<Ray> tmpray = rParallel[id];;
				std::vector<VecPoint3D> points;
				int32_t tmpa = aParallel[id];
				for (size_t i = 0; i < tmpray.size() - 1; i++)
				{
					vector<Point3D> p;
					bvh->RayTraceNodes(root, tmpray[i], p);
					if (!p.empty() && p.size() % 2 == 0)
					{
						//sort points based on the distance from the origin of the ray
						Sortpoints(p, tmpray[i]);
						points.push_back({ p });
					}
				}
				AnglePair newPair(tmpa, e);
				newPair.CalculateIndices(points);
#pragma omp critical
				{
					pairs.push_back(newPair);
				}
			}
			//update the original ray vector
			RotateRays(1.0, Axis::X, rParallel[NUM_OF_THREADS_X_16 - 1]);
			r = rParallel[NUM_OF_THREADS_X_16 - 1];
		}
		std::chrono::high_resolution_clock::time_point stop_f = std::chrono::high_resolution_clock::now();
		std::chrono::milliseconds dur = std::chrono::duration_cast<std::chrono::milliseconds>(stop_f - start_f);
		RotateRays(1.0, Axis::Z, r);
		std::cout << "Cycle " << e + 1 << " out of 90 duration: " << dur.count() << " sec" << std::endl;
	}
	return 1;
}