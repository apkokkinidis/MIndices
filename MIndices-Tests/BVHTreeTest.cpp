#include "pch.h"

#include "BVHTree.h"
#include "BoundingBox.h"

using TestTriangles = std::vector<std::vector<MIndices::Triangle>>;
using TestAABBoxes = std::vector<MIndices::BoundingBox3D>;

bool operator==(const MIndices::BoundingBox3D& lhs, const MIndices::BoundingBox3D& rhs) {
	return lhs.X1() == rhs.X1() && lhs.X2() == rhs.X2() &&
		lhs.Y1() == rhs.Y1() && lhs.Y2() == rhs.Y2() &&
		lhs.Z1() == rhs.Z1() && lhs.Z2() == rhs.Z2();
}

std::shared_ptr<MIndices::BVHTree> PrepareBVHTestData()
{
	TestTriangles testTriangles = {
		{ // Root-level (contains all triangle data)
			MIndices::Triangle{{MIndices::Point3D{0.0, 0.0, 0.0}, MIndices::Point3D{1.0, 0.0, 0.0}, MIndices::Point3D{0.0, 1.0, 0.0}}},
			MIndices::Triangle{{MIndices::Point3D{2.0, 2.0, 2.0}, MIndices::Point3D{3.0, 2.0, 2.0}, MIndices::Point3D{2.0, 3.0, 2.0}}}
		},
		{ // Left child
			MIndices::Triangle{{MIndices::Point3D{0.0, 0.0, 0.0}, MIndices::Point3D{1.0, 0.0, 0.0}, MIndices::Point3D{0.0, 1.0, 0.0}}}
		},
		{ // Right child
			MIndices::Triangle{{MIndices::Point3D{2.0, 2.0, 2.0}, MIndices::Point3D{3.0, 2.0, 2.0}, MIndices::Point3D{2.0, 3.0, 2.0}}}
		}
	};
	TestAABBoxes testBBoxes = {
		// Root AABB: union of the left and right triangles
		MIndices::BoundingBox3D(0, 0, 0, 3, 3, 2),

		// Left AABB (triangle at origin)
		MIndices::BoundingBox3D(0, 0, 0, 1, 1, 0),

		// Right AABB (translated triangle)
		MIndices::BoundingBox3D(2, 2, 2, 3, 3, 2),
	};

	std::shared_ptr<MIndices::BVHTree> testTree = std::make_shared<MIndices::BVHTree>();

	auto* root = new MIndices::BVHNode(testBBoxes[0], testTriangles[0]);
	auto* leftNode = new MIndices::BVHNode(testBBoxes[1], testTriangles[1]);
	auto* rightNode = new MIndices::BVHNode(testBBoxes[2], testTriangles[2]);

	root->left = leftNode;
	root->right = rightNode;

	testTree->SetRoot(root);

	return testTree;
}

bool CompareTreeData(std::shared_ptr<MIndices::BVHTree> testTree, std::shared_ptr<MIndices::BVHTree> expTree)
{
	return true;
}

TEST(BVHTreeTest, BVHTreeSmall)
{
	std::vector<MIndices::Triangle> triangles =
	{
		MIndices::Triangle{{MIndices::Point3D{0.0, 0.0, 0.0}, MIndices::Point3D{1.0, 0.0, 0.0}, MIndices::Point3D{0.0, 1.0, 0.0}}},
		MIndices::Triangle{{MIndices::Point3D{2.0, 2.0, 2.0}, MIndices::Point3D{3.0, 2.0, 2.0}, MIndices::Point3D{2.0, 3.0, 2.0}}}
	};

	std::shared_ptr<MIndices::BVHTree> expTree = PrepareBVHTestData();
	auto* expRoot = expTree->GetRoot();
	EXPECT_TRUE(expRoot);

	std::shared_ptr<MIndices::BVHTree> tree = std::make_shared<MIndices::BVHTree>();
	tree->TopDownBuildObjectMedian(triangles);
	auto* root = tree->GetRoot();
	EXPECT_TRUE(root);

	EXPECT_TRUE(CompareTreeData(tree, expTree));
}