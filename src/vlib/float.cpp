#include "declarations.h"

#include <float.h>
#include <math.h>

float	fmin3(float a, float b, float c)	{	return fmin(a, fmin(b, c));	}
float	fmax3(float a, float b, float c)	{	return fmax(a, fmax(b, c));	}

float	fmin4(float a, float b, float c, float d)	{	return fmin(fmin(a, b), fmin(c, d));	}
float	fmax4(float a, float b, float c, float d)	{	return fmax(fmax(a, b), fmax(c, d));	}
