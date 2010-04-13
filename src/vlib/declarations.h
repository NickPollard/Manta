// Includes common declarations, macros etc. for the V lib

//
// Float values
//
#define FLOAT_EPSILON	0.000001f
#define FLOAT_MAX		FLT_MAX 

//
// Syntactic sugar
//
#define repeat(n)		for (int i = 0; i < (n); i++)
#define repeati(i, n)	for (int i = 0; i < (n); i++)

float	fmin3(float a, float b, float c);
float	fmax3(float a, float b, float c);
float	fmin4(float a, float b, float c, float d);
float	fmax4(float a, float b, float c, float d);
