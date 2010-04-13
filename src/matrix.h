#ifndef __MATRIX_H__
#define __MATRIX_H__

class matrix
{
    public:
	vec4	row[4];

    matrix()    {}
};

// Set the matrix to the identity matrix
void matrixIdent(matrix* m);

// Multiply vec3 by matrix, store the result in out
void matrixVec3Rot(vec3* out, matrix* m, vec3* in);

// Copy one matrix to another
void matrixCpy(matrix* out, matrix* in);

// create a matrix from look and up vectors
void rotMatrixFromVec3(matrix* out, vec3* dir, vec3* up);

// create an inverse matrix from look and up vectors
void invRotMatrixFromVec3(matrix* out, vec3* dir, vec3* up);

// Create a y axis rotation matrix from a float
void matrixYAxisRot(matrix* m, float angle);

#endif // __MATRIX_H__
