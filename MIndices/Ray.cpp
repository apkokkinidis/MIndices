// Author Anastasios Kokkinidis 
#include "Ray.h"
#include "Products.h"

using namespace MIndices;

Ray::Ray()
{
	origin = { 0, 0, 0 };
	direction = { 0, 0, 0 };
};

Ray::Ray(Point3D _origin, Point3D _direction)
{
	origin = _origin;
	direction = _direction;
}

Ray::Ray(const Ray& r)
{
	if (this != &r)
	{
		this->origin = r.origin;
		this->direction = r.direction;
	}
}

Ray& Ray::operator=(const Ray& r)
{
	if (this != &r)
	{
		this->origin = r.origin;
		this->direction = r.direction;
	}
	return *this;
}

void Ray::Rotate(double d, Axis a)
{
	double r = d * RAD;
	COORD_TYPE x2 = direction.x;
	COORD_TYPE y2 = direction.y;
	COORD_TYPE z2 = direction.z;
	switch (a)
	{
	case Axis::X:
	{	
		Point3D dirT = { x2,
						cos(r) * y2 - sin(r) * z2,
						sin(r) * y2 + cos(r) * z2 };
		this->direction = dirT;
		break;
	}
	case Axis::Y:
	{
		Point3D dirT = { cos(r) * x2 + sin(r) * z2,
						y2,
						-sin(r) * x2 + cos(r) * z2 };
		this->direction = dirT;
		break;
	}
	case Axis::Z:
	{
		Point3D dirT = { cos(r) * x2 - sin(r) * y2,
						sin(r) * x2 + cos(r) * y2,
						z2 };
		this->direction = dirT;
		break;
	}
	default: 
		break;
	}
}

inline COORD_TYPE Ray::RayOriginDistFromPoint3D(const Point3D& tri) const noexcept
{
	COORD_TYPE dist = sqrt((tri.x - origin.x) * (tri.x - origin.x) + (tri.y - origin.y) * (tri.y - origin.y) + (tri.z - origin.z) * (tri.z - origin.z));
	return dist;
}

//Geometric solution based on scratchpixel.com
bool Ray::intersectsTriangle(const Triangle& triangle, Point3D &P) const noexcept
{
	
	Point3D v0 = triangle.p[0];
	Point3D v1 = triangle.p[1];
	Point3D v2 = triangle.p[2];

	//compute plane's normal
	Point3D N = ComputeNormal(triangle);
	float area = sqrtf(N.x * N.x + N.y * N.y + N.z * N.z);

	//check if ray is paraller to triangle
	float NdotDir = DotProduct(N, direction);
	if (fabs(NdotDir < EPSILON))
	{
		//they are parralel so they don't int32_tersect
		return false;
	}

	//compute D parameter
	float d = DotProduct(N, v0);

	//compute t
	float t = (DotProduct(N, origin) + d) / NdotDir;
	// check if the triangle is in behind the ray
	//if (t < 0) return false; 
	

	//compute intersection Point P using the equation P = O + t*R
	Point3D tR = { t * direction.x, t * direction.y, t * direction.z };
	 P = origin + tR;
	
	Point3D C; //vector perpendicular to triangle plane
	//edge0
	Point3D edge0 = v1 - v0;
	Point3D vp0 = P - v0;
	C = CrossProduct(edge0, vp0);
	if (DotProduct(N, C) < 0)
	{
		//P is on the right side
		return false; 
	}

	//edge1
	Point3D edge1 = v2 - v1;
	Point3D vp1 = P - v1;
	C = CrossProduct(edge1, vp1);
	if (DotProduct(N, C) < 0)
	{
		//P is on the right side
		return false;
	}

	//edge2
	Point3D edge2 = v0 - v2;
	Point3D vp2 = P - v2;
	C = CrossProduct(edge2, vp2);
	if (DotProduct(N, C) < 0)
	{
		//P is on the right side
		return false;
	}
	//The ray hits the triangle

	return true;
}

//No culling
bool Ray::FastRayTriangleIntersection(const Triangle& triangle, Point3D& outPoint, double &t) const noexcept
{
	Point3D edge1, edge2, pvec, tvec, qvec;
	edge1 = triangle.p[1] - triangle.p[0];
	edge2 = triangle.p[2] - triangle.p[0];

	//calculate determinant also used for u parameter
	pvec = CrossProduct(this->direction, edge2);
	COORD_TYPE det = DotProduct(edge1, pvec);

	// if the determinant is negative the triangle is backfacing yet we do not cull backfacing triangles
	if (det > -EPSILON && det < EPSILON)
	{
		//Ray is parallel to the triangle when the determinant is very close to zero
		//exit
		return false;
	}
	COORD_TYPE inv_det = 1.0f / det;
	tvec = Subpoints(this->origin, triangle.p[0]);

	//calculate u
	double u = DotProduct(tvec, pvec) * inv_det;

	if (u < 0.0f || u > 1.0f )
	{
		return false;
	}

	//calculate v
	qvec = CrossProduct(tvec, edge1);
	COORD_TYPE v = DotProduct(direction, qvec) * inv_det;

	if (v < 0.0f || v + u  > 1.0f)
	{
		return false;
	}

	//find intersection Point
	t = abs(DotProduct(edge2, qvec));
	COORD_TYPE t_inv_det = abs(t * inv_det);
	outPoint = { origin.x + direction.x * t_inv_det, origin.y + direction.y * t_inv_det, origin.z + direction.z * t_inv_det };
	return true;
}

bool Ray::FastRayTriangleIntersection(const Triangle& triangle, const Point3D& edge1, const Point3D& edge2, Point3D& PointI, double& t) const noexcept
{
	Point3D pvec, tvec, qvec;

	//calculate determinant also used for u parameter
	pvec = CrossProduct(edge2, this->direction);
	double det = DotProduct(edge1, pvec);
	// if the determinant is negative the triangle is backfacing

	if (det > -EPSILON && det < EPSILON)
	{
		//Ray is parallel to triangle, so exit
		return false;
	}
	double inv_det = 1 / det;
	tvec = Subpoints(this->origin, triangle.p[0]);

	//calculate u
	double u = DotProduct(tvec, pvec) * inv_det;

	if (u < 0.0f )
	{
		return false;
	}

	//calculate v
	qvec = CrossProduct(tvec, edge1);
	double v = DotProduct(direction, qvec) * inv_det;

	if (v < 0.0f || v + u  > 1)
	{
		return false;
	}

	//find intersection Point
	t = abs(DotProduct(edge2, qvec));
	double t_inv_det = abs(t * inv_det);
	PointI = { origin.x + direction.x * (COORD_TYPE)t_inv_det, origin.y + direction.y * (COORD_TYPE)t_inv_det, origin.z + direction.z * (COORD_TYPE)t_inv_det };
	return true;
}

//Rotates an array of rays
//@param degrees of rotation
//@param rotation axis
void MIndices::RotateRays(double d, Axis a, std::vector<Ray>& ray)
{
	for (auto it = ray.begin(); it != ray.end(); ++it)
	{
		it->Rotate(d, a);
	}
}