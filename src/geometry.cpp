#include "src/common.h"
#include "src/geometry.h"
//--------------------------------------------------------
#include "src/vector.h"

#include <math.h>
#include <stdio.h>

#define OPTIMIZE_USE_BACKFACE_CULLING 1

//===========================================================================
// Calculate Surface Area
float volume3::SurfaceArea()
{
	// Assume max > min ?
	float w = max.x - min.x;
	float h = max.y - min.y;
	float d = max.z - min.z;

	return (2 * w * h) + (2 * w * d) + (2 * h * d);
}

//===========================================================================
// Find the point at which a ray intersects a plane, based on a poly
bool intersect(polygon& __restrict poly, rayData& __restrict ray, point3& intersection, float& distance, float& u, float& v)
{
    float B = vec3Dot(ray.direction, poly.normal);

#if OPTIMIZE_USE_BACKFACE_CULLING
    if (B > 0.f)
        return false;
#endif // OPTIMIZE_USE_BACKFACE_CULLING

    float A = vec3Dot(ray.origin, poly.normal); // Can do this test after backface cull

    // Find the depth of the poly from the ray origin, in terms of ray direction
    float depth = (poly.d - A) / B;

    if ((depth < 0.f) || (depth > distance))
        return false;

	distance = depth;

    // Calculate the vector from ray origin to intersection point
    intersection = ray.origin + (ray.direction * distance);

	vec3* __restrict triA = poly.A();

    vec3 AB = *poly.B() - *triA;
    vec3 AC = *poly.C() - *triA;
    vec3 AI = intersection - *triA;

	// TODO - delay calculation or do all immediately?
	// Calulate UV coordinates on the triangle
	u = (AC.y * AI.x - AC.x * AI.y) / (AC.y * AB.x - AC.x * AB.y);

	if ((u < 0.f) || ( u > 1.f))
		return false;

	v = (AB.y * AI.x - AB.x * AI.y) / (AB.y * AC.x - AB.x * AC.y);

	if ((v < 0.f) || (v > 1.f) || (u + v > 1.f)) {
		return false;
	}

	return true;
}

//===========================================================================
float   distanceBetweenRays(rayData& first, rayData& second)
{
    vec3 normal;
    vec3Cross(&normal, &first.direction, &second.direction);
    normal.Normalize();

    float d = vec3Dot(normal, first.origin);
    float c = d - vec3Dot(normal, second.origin);

    return c;
}

/*
//===========================================================================
// Test whether a 3d point lies inside an axis-aligned cuboidal volume
bool PointInVolume(point3* point, volume3* volume)
{
	if	((point->x > volume->max.x) ||
		(point->y > volume->max.y) ||
		(point->z > volume->max.z) ||
		(point->x < volume->min.x) ||
		(point->y < volume->min.y) ||
		(point->z < volume->min.z))
		return false;

	return true;
}
*/

//===========================================================================
// Does a plane-box intersection (TODO - triangle, not plane)
bool overlap(polygon& poly, volume3& volume)
{
	float aZ = -((volume.min.x * poly.normal.x + volume.min.y * poly.normal.y) - poly.d) / poly.normal.z;
	float bZ = -((volume.min.x * poly.normal.x + volume.max.y * poly.normal.y) - poly.d) / poly.normal.z;
	float cZ = -((volume.max.x * poly.normal.x + volume.min.y * poly.normal.y) - poly.d) / poly.normal.z;
	float dZ = -((volume.max.x * poly.normal.x + volume.max.y * poly.normal.y) - poly.d) / poly.normal.z;

	float maxZ = fmax4(aZ, bZ, cZ, dZ);
	float minZ = fmin4(aZ, bZ, cZ, dZ);

	return ((minZ < volume.max.z) && (maxZ > volume.min.z));
}

//===========================================================================
// Debug
void	debugBuildPoly(polygon* poly, point3* p0, point3* p1, point3* p2, vec3* normal, color3f* color)
{
	// Points
	poly->point[0] = p0;
	poly->point[1] = p1;
	poly->point[2] = p2;
	// Normal
	vec3Cpy(&poly->normal, normal);
	// Color
	vec3Cpy(&poly->color, color);
	// D
	poly->d = vec3Dot(&poly->normal, poly->point[0]);
}

//===========================================================================
bool	pointCloser(point3* base, point3* first, point3* second)
{
	vec3 firstDisp, secondDisp;
	float firstDist, secondDist;

	vec3Sub(&firstDisp, first, base);
	vec3Sub(&secondDisp, second, base);

	firstDist = vec3Len(&firstDisp);
	secondDist = vec3Len(&secondDisp);

//	printf("FirstPoint: %.2f, %.2f, %.2f, SecondPoint: %.2f, %.2f, %.2f.\n", first->x, first->y, first->z, second->x, second->y, second->z);
//	printf("FirstDist: %.2f, SecondDist: %.2f.\n", firstDist, secondDist);

	return (firstDist <= secondDist);
}
