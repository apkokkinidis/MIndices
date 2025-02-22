// Author Anastasios Kokkinidis 

#pragma once
#ifndef _ARRAY_3D_H
#define _ARRAY_3D_H
#pragma warning(disable : 4996)
#include <assert.h>
#include <fstream>
#include "BoundingBox.h"
#include "Structures.h"

namespace MIndices
{

	typedef unsigned char ELEMENT;
	/* -------------------------------------------------------------------------- */
	/* Basic element types.                                                       */
	/* -------------------------------------------------------------------------- */
	const unsigned char ELEMENT_EMPTY = 0;
	const unsigned char ELEMENT_FILLED = 1;
	const unsigned char ELEMENT_VARIOUS = 2;
	const unsigned char ELEMENT_NUMTYPES = 3;

	/* -------------------------------------------------------------------------- */
	/* Array3D class.                                                             */
	/* -------------------------------------------------------------------------- */
	class Array3D
	{
	private:
		ELEMENT* Array;
		size_t      DimX, DimY, DimZ, PlaneXY;

	public:
		inline Array3D(const size_t DX, const size_t DY, const size_t DZ)
			: DimX(DX), DimY(DY), DimZ(DZ), PlaneXY(DX* DY)
		{
			Array = new ELEMENT[DimX * DimY * DimZ];
			assert(Array != nullptr);

			for (size_t k = 0; k < DimZ; k++)   // DimZ
			{
				for (size_t j = 0; j < DimY; j++)   // DimY
				{
					for (size_t i = 0; i < DimX; i++)   // DimX
					{
						Array[i + j * DimX + k * DimX * DimY] = ELEMENT_EMPTY;
						//Array[k * PlaneXY + j * DimX + i] = ELEMENT_EMPTY;
					}   // for x
				}   // for y
			}   // for z
		}   // Constructor

		inline Array3D(const Array3D& other)
		{
			if (this != &other)
			{
				Array = new ELEMENT[DimX * DimY * DimZ];
				assert(Array != nullptr);

				DimX = other.DimX;
				DimY = other.DimY;
				DimZ = other.DimZ;
				PlaneXY = other.PlaneXY;

				for (size_t count = 0; count < DimX * DimY * DimZ; ++count)
				{
					Array[count] = other.Array[count];
				}   // for, copy values
			}   // if
		}   // Copy constructor

		inline Array3D(Array3D&& other) noexcept
		{
			if (this != &other)
			{
				Array = other.Array;
				DimX = other.DimX;
				DimY = other.DimY;
				DimZ = other.DimZ;
				PlaneXY = other.PlaneXY;

				other.Array = nullptr;
			}	//if
		}	//Move constructor

		inline ~Array3D() noexcept
		{
			delete[] Array;
			Array = nullptr;
			DimX = DimY = DimZ = PlaneXY = 0;
		}   // Destructor

		inline size_t GetDimX() const noexcept { return DimX; }
		inline size_t GetDimY() const noexcept { return DimY; }
		inline size_t GetDimZ() const noexcept { return DimZ; }

		inline ELEMENT GetElement(const size_t x, const size_t y, const size_t z) const noexcept
		{
			return Array[z * PlaneXY + y * DimX + x];
		}   // GetElement

		inline bool IsElementFilled(const size_t x, const size_t y, const size_t z)const noexcept
		{
			ELEMENT elm = Array[z * PlaneXY + y * DimX + x];
			if (elm == ELEMENT_FILLED)
			{
				return true;
			}
			else
			{
				return false;
			}
		}

		inline void SetElement(const size_t x, const size_t y, const size_t z, const ELEMENT elm) noexcept
		{
			Array[z * PlaneXY + y * DimX + x] = elm;
		}   // SetElement

		inline size_t CountElements(const ELEMENT& elm) const noexcept
		{
			size_t	Counter = 0, i, j, k;

			for (k = 0; k < DimZ; k++)
			{
				for (j = 0; j < DimY; j++)
				{
					for (i = 0; i < DimX; i++)
					{
						if (Array[k * PlaneXY + j * DimX + i] == elm)
						{
							Counter++;
						}   // if
					}   // for x
				}   // for y
			}   // for z

			return Counter;
		}   // CountElements

		inline ELEMENT FindElementTypes() const noexcept
		{
			size_t	i, j, k;
			ELEMENT	Elm;
			bool	HasEmpty = false, HasFilled = false;

			for (k = 0; k < DimZ; k++)
			{
				for (j = 0; j < DimY; j++)
				{
					for (i = 0; i < DimX; i++)
					{
						Elm = GetElement(i, j, k);
						HasEmpty = HasEmpty || (Elm == ELEMENT_EMPTY);
						HasFilled = HasFilled || (Elm == ELEMENT_FILLED);

						// Terminate prematurely of there is more than one element type.
						if (HasFilled && HasEmpty) return ELEMENT_VARIOUS;
					}   // for x
				}   // for y
			}   // for z

			if (!HasFilled)
			{
				return ELEMENT_EMPTY;
			}
			return ELEMENT_FILLED;
		}   // FindElementTypes

		inline size_t CountElements(const ELEMENT& elm, const size_t z) const noexcept
		{
			size_t	Counter = 0, i, j;

			for (j = 0; j < DimY; j++)
			{
				for (i = 0; i < DimX; i++)
				{
					if (Array[z * PlaneXY + j * DimX + i] == elm)
					{
						Counter++;
					}   // if
				}   // for x
			}   // for y

			return Counter;
		}	// CountElements

		inline ELEMENT FindElementTypes(const size_t z) const noexcept
		{
			size_t	i, j;
			ELEMENT	Elm;
			bool	HasEmpty = false, HasFilled = false;

			for (j = 0; j < DimY; j++)
			{
				for (i = 0; i < DimX; i++)
				{
					Elm = GetElement(i, j, z);
					HasEmpty = HasEmpty || (Elm == ELEMENT_EMPTY);
					HasFilled = HasFilled || (Elm == ELEMENT_FILLED);

					// Terminate prematurely of there is more than one element type.
					if (HasFilled && HasEmpty) return ELEMENT_VARIOUS;
				}   // for x
			}   // for y

			if (!HasFilled) return ELEMENT_EMPTY;
			/*if ( !HasEmpty )*/ return ELEMENT_FILLED;
		}   // FindElementTypes

		inline void SetElements(const ELEMENT& elm, const size_t z) noexcept
		{
			size_t	i, j;

			for (j = 0; j < DimY; j++)
			{
				for (i = 0; i < DimX; i++)
				{
					Array[z * PlaneXY + j * DimX + i] = elm;
				}   // for x
			}   // for y
		}   // SetElements

		inline size_t CountElements(const ELEMENT& elm, const BoundingBox2D& BB, const size_t z) const noexcept
		{
			size_t	Counter = 0, x, y;

			for (y = BB.Y1(); y <= BB.Y2(); y++)
			{
				for (x = BB.X1(); x <= BB.X2(); x++)
				{
					if (GetElement(x, y, z) == elm)
					{
						Counter++;
					}   // if
				}   // for x
			}   // for y

			return Counter;
		}   // CountElements

		inline ELEMENT FindElementTypes(const BoundingBox2D& BB, const size_t z) const noexcept
		{
			size_t	i, j;
			ELEMENT	Elm;
			bool	HasEmpty = false, HasFilled = false;

			for (j = BB.Y1(); j <= BB.Y2(); j++)
			{
				for (i = BB.X1(); i <= BB.X2(); i++)
				{
					Elm = GetElement(i, j, z);
					HasEmpty = HasEmpty || (Elm == ELEMENT_EMPTY);
					HasFilled = HasFilled || (Elm == ELEMENT_FILLED);

					// Terminate prematurely of there is more than one element type.
					if (HasFilled && HasEmpty) return ELEMENT_VARIOUS;
				}   // for x
			}   // for y

			if (!HasFilled) return ELEMENT_EMPTY;
			/*if ( !HasEmpty )*/ return ELEMENT_FILLED;
		}   // FindElementTypes

		inline size_t CountElements(const ELEMENT& elm, const BoundingBox3D& BB) const noexcept
		{
			size_t	Counter = 0, x, y, z;

			for (z = BB.Z1(); z <= BB.Z2(); z++)
			{
				for (y = BB.Y1(); y <= BB.Y2(); y++)
				{
					for (x = BB.X1(); x <= BB.X2(); x++)
					{
						if (GetElement(x, y, z) == elm)
						{
							Counter++;
						}   // if
					}   // for x
				}   // for y
			}   // for z

			return Counter;
		}   // CountElements

		inline ELEMENT FindElementTypes(const BoundingBox3D& BB) const noexcept
		{
			size_t	i, j, k;
			ELEMENT	Elm;
			bool	HasEmpty = false, HasFilled = false;

			for (k = BB.Z1(); k <= BB.Z2(); k++)
			{
				for (j = BB.Y1(); j <= BB.Y2(); j++)
				{
					for (i = BB.X1(); i <= BB.X2(); i++)
					{
						Elm = GetElement(i, j, k);
						HasEmpty = HasEmpty || (Elm == ELEMENT_EMPTY);
						HasFilled = HasFilled || (Elm == ELEMENT_FILLED);

						// Terminate prematurely of there is more than one element type.
						if (HasFilled && HasEmpty) return ELEMENT_VARIOUS;
					}   // for x
				}   // for y
			}   // for z

			if (!HasFilled) return ELEMENT_EMPTY;
			return ELEMENT_FILLED;
		}   // FindElementTypes

		inline size_t GetTotalElements() const noexcept
		{
			return DimX * DimY * DimZ;
		}   // GetTotalElements

		inline BoundingBox3D GetBoundingBox3D() const noexcept
		{
			return BoundingBox3D((COORD_TYPE_BB)0, (COORD_TYPE_BB)0, (COORD_TYPE_BB)0,
				(COORD_TYPE_BB)(DimX - 1), (COORD_TYPE_BB)(DimY - 1), (COORD_TYPE_BB)(DimZ - 1));
		}   // GetBoundingBox3D

		void Print32_tArray2D(const BoundingBox2D& BB, const size_t z) const noexcept
		{
			size_t	i, j;

			for (j = BB.Y1(); j <= BB.Y2(); j++)
			{
				for (i = BB.X1(); i <= BB.X2(); i++)
				{
					if (GetElement(i, j, z) == ELEMENT_EMPTY)
						std::cout << "0";
					else
						std::cout << "1";
				}   // for
				std::cout << std::endl;
			}   // for
		}   // Print32_tArray2D

		//void Print32_tArray3D(const BoundingBox3D &BB) const noexcept;
		//void Print32_tArray3D(const BoundingBox3D &BB, const size_t z) const noexcept;

		bool LoadFromFile(const char* FileSpec) noexcept
		{
			FILE* pFile;
			size_t	i, j, k, TotalElements, ElementNumber;

			// 1. Open file.
			pFile = (FILE*)fopen(FileSpec, "r");
			if (pFile == nullptr) // If file doesn't open show an ERROR.
			{
				std::cerr << "Can't open file " << FileSpec << std::endl;
				return false;
			}   // if

			// 2. Load data.
			//fscanf(pFile, "%u", &TotalElements);
			fscanf(pFile, "%llu", &TotalElements);
			while (!feof(pFile))
			{
				fscanf(pFile, "%llu%llu%llu%llu", &ElementNumber, &i, &j, &k);
				SetElement(i - 1, j - 1, k - 1, ELEMENT_FILLED);
			}   // while

			// 3. Close file.
			std::fclose(pFile);
			return true;
		}   // LoadFromFile

		bool SaveToFile(const char* FileSpec) noexcept
		{
			FILE* pFile;
			size_t	x, y, z, TotalElements, ElementNumber = 1;

			// 1. Open File.
			pFile = (FILE*)fopen(FileSpec, "wt");
			if (pFile == nullptr) // If file doesn't open show an ERROR.
			{
				std::cerr << "Can't open file " << FileSpec << std::endl;
				return false;
			}   // if

			// 2. Save data.
			TotalElements = CountElements(ELEMENT_FILLED);
			fprintf(pFile, "%llu", TotalElements);
			std::cout << "Total elements = " << TotalElements << std::endl;
			for (z = 0; z < DimZ; z++)
			{
				for (y = 0; y < DimY; y++)
				{
					for (x = 0; x < DimX; x++)
					{
						if (GetElement(x, y, z) == ELEMENT_FILLED)
						{
							fprintf(pFile, "%llu\t%llu\t%llu\t%llu\n", ElementNumber, x + 1, y + 1, z + 1);
							ElementNumber++;
						}   // if
					}   // for x
				}   // for y
			}   // for z

			// 3. Close file.
			fclose(pFile);
			return true;
		}   // SaveToFile

		inline void InitArray3D(const ELEMENT& elm) noexcept
		{
			for (size_t k = 0; k < DimZ; k++)   // DimZ
			{
				for (size_t j = 0; j < DimY; j++)   // DimY
				{
					for (size_t i = 0; i < DimX; i++)   // DimX
					{
						Array[k * PlaneXY + j * DimX + i] = elm;
					}   // for x
				}   // for y
			}   // for z
		}   // InitArray3D

		inline void InitArray3D(const BoundingBox3D& BB, const ELEMENT& elm) noexcept
		{
			for (size_t k = BB.Z1(); k <= BB.Z2(); k++)   // DimZ
			{
				for (size_t j = BB.Y1(); j <= BB.Y2(); j++)   // DimY
				{
					for (size_t i = BB.X1(); i <= BB.X2(); i++)   // DimX
					{
						Array[k * PlaneXY + j * DimX + i] = elm;
					}   // for x
				}   // for y
			}   // for z
		}   // InitArray3D

		//---------------------OPERATOR = --------------------------//

		inline Array3D& operator=(const Array3D& Other)
		{
			if (this == &Other) return *this;

			delete[] Array;
			Array = new ELEMENT[DimX * DimY * DimZ];
			DimX = Other.DimX;
			DimY = Other.DimY;
			DimZ = Other.DimZ;
			PlaneXY = Other.PlaneXY;

			for (size_t count = 0; count < DimX * DimY * DimZ; ++count)
			{
				Array[count] = Other.Array[count];
			}   // for, copy values

			return *this;
		}   // operator=

		//---------------------OPERATOR == ------------------------//

		inline bool operator==(const Array3D& Other) const noexcept
		{
			if (DimX != Other.DimX || DimY != Other.DimY || DimZ != Other.DimZ)
			{
				return false;
			}   // if

			for (size_t count = 0; count < (DimX * DimY * DimZ); ++count)
			{
				if (Array[count] != Other.Array[count]) return false;
			}   // for

			return true;
		}   // operator==

		inline void BinarySave(const char* Filename) const noexcept
		{
			std::ofstream outArray(Filename, std::ios::out | std::ios::binary);

			if (!outArray)
			{
				std::cerr << "connection failure";
				exit(1);
			}   // if

			for (size_t k = 0; k < DimZ; k++)
			{
				for (size_t j = 0; j < DimY; j++)
				{
					for (size_t i = 0; i < DimX; i++)
					{
						outArray << (int32_t)GetElement(i, j, k) << " ";
					}   // for
				}   // for
			}   // for
			outArray.close();
		} //Binary Save
	};
}
#endif _ARRAY_3D_H