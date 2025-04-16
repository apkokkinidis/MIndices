#include "pch.h"
#include "Vector3D.h"
#include "MarchingCubes.h"

using TYPE = MIndices::ElementTypes;

class TestVector3D : public MIndices::Vector3D
{
public:
	TestVector3D(size_t dim_x, size_t dim_y, size_t dim_z, const std::vector<MIndices::ElementTypes>& elements) : Vector3D(dim_x, dim_y, dim_z)
	{
		if (dim_x * dim_y * dim_z != elements.size())
		{
			std::cerr << "Bad test dimenssions." << std::endl;
			assert(false);
		}
		voxelArray = elements;
	}
	~TestVector3D() override = default;
};

TEST(MarchingCubes, MarchingCubesSmall)
{
	const int32_t dimX = 3, dimY = 3, dimZ = 3;

	std::vector<MIndices::ElementTypes> testSmall = {
			TYPE::EMPTY, TYPE::EMPTY, TYPE::FILLED, TYPE::EMPTY,
			TYPE::EMPTY, TYPE::FILLED, TYPE::EMPTY, TYPE::FILLED,
			TYPE::FILLED, TYPE::EMPTY, TYPE::EMPTY, TYPE::EMPTY,
			TYPE::EMPTY, TYPE::EMPTY, TYPE::FILLED, TYPE::EMPTY,
			TYPE::EMPTY, TYPE::FILLED, TYPE::EMPTY, TYPE::EMPTY,
			TYPE::FILLED, TYPE::EMPTY, TYPE::EMPTY, TYPE::EMPTY,
			TYPE::FILLED, TYPE::EMPTY, TYPE::FILLED };

	const std::vector<MIndices::Triangle> expTriangles = { 
		MIndices::Triangle({{3.0, 1.5, 2.0}, {2.0, 1.0, 1.0}, {3.0, 1.0, 1.5}}),
		MIndices::Triangle({{3.0, 1.5, 2.0}, {2.5, 2.0, 2.0}, {2.0, 1.0, 1.0}}),
		MIndices::Triangle({{2.5, 2.0, 2.0}, {2.5, 2.0, 1.0},{2.0, 1.0, 1.0}}),
		MIndices::Triangle({{2.0, 2.5, 1.0}, {2.0, 3.0, 1.5},{1.5, 3.0, 1.0}}),
		MIndices::Triangle({{2.0, 3.0, 1.5}, {2.0, 2.5, 1.0},{2.0, 2.0, 1.0}}),
		MIndices::Triangle({{2.0, 3.0, 1.5}, {2.0, 2.0, 1.0},{2.5, 3.0, 2.0}}), 
		MIndices::Triangle({{2.5, 3.0, 2.0}, {2.0, 2.0, 1.0},{2.5, 2.0, 2.0}}),
		MIndices::Triangle({{3.0, 1.0, 2.5}, {2.5, 1.0, 3.0},{3.0, 1.5, 3.0}}), 
		MIndices::Triangle({{3.0, 1.5, 2.0}, {3.0, 2.0, 2.5},{2.5, 2.0, 2.0}}),
		MIndices::Triangle({{1.0, 3.0, 2.5}, {1.5, 3.0, 3.0},{1.0, 2.5, 3.0}}),
		MIndices::Triangle({{3.0, 2.5, 3.0}, {2.0, 2.0, 2.0},{3.0, 2.0, 2.5}}),
		MIndices::Triangle({{3.0, 2.5, 3.0}, {2.5, 3.0, 3.0},{2.0, 2.0, 2.0}}),
		MIndices::Triangle({{2.5, 3.0, 3.0}, {2.5, 3.0, 2.0},{2.0, 2.0, 2.0}}) };

	std::vector<MIndices::Triangle> testTriangles;

	std::unique_ptr<TestVector3D> testData = std::make_unique<TestVector3D>(dimX, dimY, dimZ, testSmall);

	try
	{
		//Initialize Marching cubes class
		MIndices::MarchingCubes mCubes(std::move(testData), dimX, dimY, dimZ);
		mCubes.TriangulateVCubes(testTriangles);
	}
	catch (const std::out_of_range& e)
	{
		std::cerr << "Incorect dimessions or voxel array size." << std::endl;
		assert(false);
	}

	EXPECT_EQ(expTriangles.size(), testTriangles.size());
	try
	{
		for (int32_t i = 0; i < testTriangles.size(); ++i)
		{
			EXPECT_TRUE(testTriangles[i] == expTriangles[i]);
		}
	}
	catch (const std::out_of_range& e)
	{
		std::cerr << "Vector out of range." << std::endl;
		assert(false);
	}
}

TEST(MarchingCubes, MarchingCubesEmpty)
{
	const int32_t dimX = 3, dimY = 3, dimZ = 3;

	std::vector<MIndices::ElementTypes> testEmpty = {
			TYPE::EMPTY, TYPE::EMPTY, TYPE::EMPTY, TYPE::EMPTY,
			TYPE::EMPTY, TYPE::EMPTY, TYPE::EMPTY, TYPE::EMPTY,
			TYPE::EMPTY, TYPE::EMPTY, TYPE::EMPTY, TYPE::EMPTY,
			TYPE::EMPTY, TYPE::EMPTY, TYPE::EMPTY, TYPE::EMPTY,
			TYPE::EMPTY, TYPE::EMPTY, TYPE::EMPTY, TYPE::EMPTY,
			TYPE::EMPTY, TYPE::EMPTY, TYPE::EMPTY, TYPE::EMPTY,
			TYPE::EMPTY, TYPE::EMPTY, TYPE::EMPTY };

	std::vector<MIndices::Triangle> testTriangles;

	std::unique_ptr<TestVector3D> testData = std::make_unique<TestVector3D>(dimX, dimY, dimZ, testEmpty);

	//Initialize Marching cubes class
	MIndices::MarchingCubes mCubes(std::move(testData), dimX, dimY, dimZ);

	mCubes.TriangulateVCubes(testTriangles);
	//Cubes are enterily outside the surface, expect no triangulation.
	EXPECT_EQ(0, testTriangles.size());
}
TEST(MarchingCubes, MarchingCubesFilled)
{
	const int32_t dimX = 3, dimY = 3, dimZ = 3;

	std::vector<MIndices::ElementTypes> testFilled = {
			TYPE::FILLED, TYPE::FILLED, TYPE::FILLED, TYPE::FILLED,
			TYPE::FILLED, TYPE::FILLED, TYPE::FILLED, TYPE::FILLED,
			TYPE::FILLED, TYPE::FILLED, TYPE::FILLED, TYPE::FILLED,
			TYPE::FILLED, TYPE::FILLED, TYPE::FILLED, TYPE::FILLED,
			TYPE::FILLED, TYPE::FILLED, TYPE::FILLED, TYPE::FILLED,
			TYPE::FILLED, TYPE::FILLED, TYPE::FILLED, TYPE::FILLED,
			TYPE::FILLED, TYPE::FILLED, TYPE::FILLED };

	std::vector<MIndices::Triangle> testTriangles;

	std::unique_ptr<TestVector3D> testData = std::make_unique<TestVector3D>(dimX, dimY, dimZ, testFilled);

	//Initialize Marching cubes class
	MIndices::MarchingCubes mCubes(std::move(testData), dimX, dimY, dimZ);

	mCubes.TriangulateVCubes(testTriangles);
	//Cubes are enterily inside the surface, expect no triangulation.
	EXPECT_EQ(0, testTriangles.size());
}