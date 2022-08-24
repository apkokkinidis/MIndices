#include "RayIntersection.h"
#include <vector>
template<typename T>
void RayIntersection::crossProduct(std::vector<T> v1, std::vector<T> v2, std::vector<T> &c_v)
{
	c_v.reserve(3);
	c_v.push_back(v1.at(1) * v2.at(2) - v1.at(2) * v2.at(1));
	c_v.push_back(v1.at(0) * v2.at(2) - v1.at(2) * v2.at(0));
	c_v.push_back(v1.at(0)* v2.at(1) - v1.at(1) * v2.at(0));
}

template<typename T>
T RayIntersection::dotProduct(std::vector<T> v1, std::vector<T> v2)
{
	return v1.at(0) * v2.at(0) + v1.at(1) * v2.at(1) + v1.at(2) * v2.at(2);
}

template<typename T>
void RayIntersection::sub(std::vector<T> v1, std::vector<T> v2, std::vector<T> &s_v)
{
	s_v.reserve(3);
	s_v.push_back(v1.at(0) - v2.at(0));
	s_v.push_back(v1.at(1) - v2.at(1));
	s_v.push_back(v1.at(2) - v2.at(2));
}

template<typename T>
int RayIntersection::IntersectsTriangle(std::vector<T> origin, std::vector<T> dest, 
										std::vector<T> v1, std::vector<T> v2, std::vector<T> v3,
										T* t, T* u, T* v)
{
	std::vector<T> edge1(3), edge2(3),tvec(3), pvec(3), qvec(3);
	T det;

	//find vector for two edges sharing v1
	sub(v2, v1, edge1);
	sub(v3, v1, edge2);

	//begin calculating determinant also used for calculating u parameter
	crossProduct(dest, edge1, pvec);
	
	//if determinant is near zero then the ray is parralel to the triangle
#ifdef TEST_CULL	//define test cull if culling is desired
	det = dotProduct(edge1, pvec);
	if (det < EPSILON)
	{
		return 0;
	}

	//calculate distance between vertex1 and ray origin
	sub(tvec, origin, v1);

	//calculate u parameter and test bounds
	*u = dotProduct(tvec, pvec);
	if (*u < 0.0 || *u > det)
	{
		return 0;
	}

	//calculate v parameter and test bounds
	crossProduct(tvec, edge1, qvec);
	*v = dotProduct(dest, qvec);
	if (*v < 0.0 || *v + *u > det)
	{
		return 0;
	}

	//calculate t, scale parameters 
	*t = dot(edge2, qvec);
	*t *= 1 / det;
	*u *= 1 / det;
	*v *= 1 / det;
#else	// NON CULLING
	if (det < EPSILON && det > -EPSILON)
	{
		return 0;

	}
	//calculate distance between vertex1 and ray origin
	sub(origin, v1, tvec);

	//calculate u parameter and test bounds
	*u = dotProduct(tvec, pvec) * (1/det);
	if (*u < 0.0 || *u > 1.0)
	{
		return 0;
	}
	//calculate v parameter and test bounds
	crossProduct(tvec, edge1, qvec);
	*v = dotProduct(dest, qvec) * (1/det);
	if (*v < 0.0 || *v + *u > 1.0)
	{
		return 0;
	}
	*t = dotProduct(edge2, qvec) * (1/det);

#endif // TEST_CULL
	return 1;
}