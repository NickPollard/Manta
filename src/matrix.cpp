#include "src/common.h"
#include "src/matrix.h"
//--------------------------------------------------------
#include "src/vector.h"
#include <math.h>

void matrixIdent(matrix* m)
{
	// Rows at a time
	m->row[kX].x = 1.f;
	m->row[kX].y = 0.f;
	m->row[kX].z = 0.f;
	m->row[kX].w = 0.f;

	m->row[kY].x = 0.f;
	m->row[kY].y = 1.f;
	m->row[kY].z = 0.f;
	m->row[kY].w = 0.f;

	m->row[kZ].x = 0.f;
	m->row[kZ].y = 0.f;
	m->row[kZ].z = 1.f;
	m->row[kZ].w = 0.f;

	m->row[kW].x = 0.f;
	m->row[kW].y = 0.f;
	m->row[kW].z = 0.f;
	m->row[kW].w = 1.f;
}

void matrixVec3Rot(vec3* out, matrix* m, vec3* in)
{
	vec3 tmp;
	tmp.x = in->x * m->row[kX].x +
			in->y * m->row[kX].y +
			in->z * m->row[kX].z;
	tmp.y = in->x * m->row[kY].x +
			in->y * m->row[kY].y +
			in->z * m->row[kY].z;
	tmp.z = in->x * m->row[kZ].x +
			in->y * m->row[kZ].y +
			in->z * m->row[kZ].z;
	vec3Cpy(out, &tmp);
}

void matrixCpy(matrix* out, matrix* in)
{
    for (int i = 0; i < 4; i++)
    {
        out->row[i] = in->row[i];
    }

}

void rotMatrixFromVec3(matrix* out, vec3* dir, vec3* up)
{
	vec3 right;
	vec3Cross(&right, dir, up);

	out->row[0].x = right.x;
	out->row[1].x = right.y;
	out->row[2].x = right.z;
	out->row[0].y = up->x;
	out->row[1].y = up->y;
	out->row[2].y = up->z;
	out->row[0].z = dir->x;
	out->row[1].z = dir->y;
	out->row[2].z = dir->z;
}

void invRotMatrixFromVec3(matrix* out, vec3* dir, vec3* up)
{
	vec3 right;
	vec3Cross(&right, dir, up);
	vec3Cpy((vec3*)&out->row[0], &right);
	vec3Cpy((vec3*)&out->row[1], up);
	vec3Cpy((vec3*)&out->row[2], dir);
}

// Make a matrix to rotate around the Y axis
// Positive angle = clockwise rotation looking down
void matrixYAxisRot(matrix* m, float angle)
{
	float sinTh = sinf(angle);
	float cosTh = cosf(angle);
	matrixIdent(m);
	m->row[0].x = cosTh;
	m->row[1].x = 0.f;
	m->row[2].x = sinTh;
	m->row[0].y = 0.f;
	m->row[1].y = 1.f;
	m->row[2].y = 0.f;
	m->row[0].z = -sinTh;
	m->row[1].z = 0.f;
	m->row[2].z = cosTh;
}
