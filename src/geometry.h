#ifndef __GEOMETRY_H__
#define __GEOMETRY_H__

// Calculate the intersection point of a ray and a polygon
bool	intersect(polygon& poly, rayData& ray, point3& intersectPoint, float& distance, float& u, float& v);

// Calculate the distance between two rays at the closest point
float   distanceBetweenRays(rayData& first, rayData& second);

// Return whether a given point is outside a given AABB
bool	pointOutsideBox(vec2* point, rectangle* box);

// Return whether point B is closer than point C to point A
bool	pointCloser(point3* base, point3* first, point3* second);

// Return whether a polygon is in an AABB
bool	overlap(polygon& poly, volume3& volume);
//debug

// Build a polygon from input data
void	debugBuildPoly(polygon* poly, point3* p0, point3* p1, point3* p2, vec3* normal, color3f* color);

#endif // __GEOMETRY_H__
