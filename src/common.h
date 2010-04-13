#ifndef __COMMON_H__
#define __COMMON_H__

#include <float.h>
#include <cstdlib>
#include <math.h>
#include <vector>
#include <algorithm>
#include <stdio.h>

#define DONT_PRINTF 0

#if DONT_PRINTF
	#define printf(...)	{}
#endif // DONT_PRINTF

#define ENUM_ELEMENT(a)	a,
#define BUILD_ENUM(a)	enum { a(ENUM_ELEMENT) };
#define STRING_VERSION(a)	#a,
#define BUILD_STRINGS(a)	{ a(STRING_VERSION) };
#define	INC(a)				+1
#define COUNT(a)	(0 a(INC))

#define SQR(a)		((a) * (a))

#include "vlib/declarations.h"
#include "vlib/slice.h"
#include "commonTypes.h"
#include "profile.h"

#define THREEPHASE_RENDER			0
#define TWOPHASE_RENDER				0
#define MULTISAMPLE_ENABLE			0
#define LIGHTING_ENABLE				1
#define LIGHTING_NORMAL_BASED 		1
#define LIGHTING_DISTANCE_BASED 	1
#define LIGHTING_SPECULAR_ENABLED	1
#define LIGHTING_AMBIENT_ENABLED	1

#define RENDER_WIDTH	640
#define RENDER_HEIGHT	480

#define MAX_COLOR_VARIATION_SQUARE	30 

#define ForEachAxis()	for (int axis = 0; axis < 3; axis++)

enum
{
    kX = 0,
    kY = 1,
    kZ = 2,
    kW = 3
};

//--------------------------------------------------------
// Functions
//--------------------------------------------------------

int		max(int a, int b);
int		min(int a, int b);

void manta_assert(bool valid, const char* message);
bool floatEq(float f1, float f2);
unsigned long long rdtsc();

//--------------------------------------------------------
// === Color
//--------------------------------------------------------

void colorFloatToInt(color3i* intCol, color3f* floatCol);
void colorIntToFloat(color3f* floatCol, color3i* intCol);
int	 colorIntVariance(color3i* colorA, color3i* colorB);
void colorIntAdd(color3i* dst, color3i* src1, color3i* src2);
void colorIntDivide(color3i* color, int scale);
void colorFloatTrunc(color3f* color);

//--------------------------------------------------------
// === Profiling
//--------------------------------------------------------

void profileBegin(int index);
void profileEnd(int index);
void dumpProfileTimes();
void newFrame();

#endif // __COMMON_H__
