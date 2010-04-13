#include "src/common.h"
#include "src/vector.h"
//--------------------------------------------------------
#include <math.h>
#include <stdio.h>

const vec3 kVec3Up (0.f, 0.f, 1.f);

void vec3::Normalize()
{
	float l = vec3Len(this);
	vec3Scale(this, 1 / l);
}

void vec3Zero(vec3* vec)
{
	vec->x = 0.f;
	vec->y = 0.f;
	vec->z = 0.f;
}

bool vec3IsUnit(vec3* vec)
{
	if (floatEq( ((vec->x*vec->x) + (vec->y*vec->y) + (vec->z*vec->z)), 1.f))
	{
		return true;
	}
	return false;
}

void vec3Set(vec3* vec, float x, float y, float z)
{
	vec->x = x;
	vec->y = y;
	vec->z = z;
}

float vec3Dot(vec3& v1, vec3& v2)
{
	return (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z);
}

float vec3Dot(vec3* v1, vec3* v2)
{
	return (v1->x * v2->x) + (v1->y * v2->y) + (v1->z * v2->z);
}

void vec3Cross(vec3* dst, const vec3* v1, const vec3* v2)
{
	dst->x = (v1->y * v2->z) - (v2->y * v1->z);
	dst->y = (v1->z * v2->x) - (v2->z * v1->x);
	dst->z = (v1->x * v2->y) - (v2->x * v1->y);
}

void vec3Cpy(vec3* dst, vec3* src)
{
	dst->x = src->x;
	dst->y = src->y;
	dst->z = src->z;
}

void vec3Scale(vec3* v, float f)
{
	v->x = v->x * f;
	v->y = v->y * f;
	v->z = v->z * f;
}

void vec3Add(vec3* dst, vec3* src1, vec3* src2)
{
	dst->x = src1->x + src2->x;
	dst->y = src1->y + src2->y;
	dst->z = src1->z + src2->z;
}

void vec3AddScale(vec3* dst, vec3* src, float scale)
{
	dst->x = dst->x + src->x * scale;
	dst->y = dst->y + src->y * scale;
	dst->z = dst->z + src->z * scale;
}

void vec3Sub(vec3* dst, vec3* src1, vec3* src2)
{
	dst->x = src1->x - src2->x;
	dst->y = src1->y - src2->y;
	dst->z = src1->z - src2->z;
}

float vec3::LengthSq()
{
	return (x*x + y*y +	z*z);
}

float vec3LenSq(vec3* v)
{
	return (v->x*v->x + 
			v->y*v->y +
			v->z*v->z);
}

float vec3::Length()
{
	return sqrt(x*x + y*y +	z*z);
}

float vec3Len(vec3* v)
{
	return sqrt(v->x*v->x + 
				v->y*v->y +
				v->z*v->z);
}

float vec3Manhattan(vec3* v)
{
	return abs(v->x) + abs(v->y) + abs(v->z);
}

void vec3Unit(vec3* v)
{
	float l = vec3Len(v);
	vec3Scale(v, 1/l);
}

// Reflect a vector in an arbitrary plane, eg. bounce a light ray off a poly
void vec3Reflect(vec3* reflectRay, vec3* inRay, vec3* normal)
{
	float dot = vec3Dot(inRay, normal);
	dot *= 2;
	vec3Cpy(reflectRay, normal);
	vec3Scale(reflectRay, dot);
	vec3Sub(reflectRay, reflectRay, inRay);
}

void vec3::Print()
{
	printf("Vector (%.2f, %.2f, %.2f)\n", x, y, z);
}

color3i ColorFromG8(guint8* pixels) {
	return color3i(*pixels, *(pixels + 1), *(pixels + 2)); }
