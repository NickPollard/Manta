#include "src/common.h"
//--------------------------------------------------------
#include <math.h>
#include <stdio.h>
#include <cassert>

#if PROFILE_ENABLE
unsigned long long profileTimers[MAX_PROFILES];
unsigned long long profileDurations[MAX_PROFILES];

int frameCount;

char profileStrings[MAX_PROFILES][32] = BUILD_STRINGS(profiles)
#endif // PROFILE_ENABLE

int		max(int a, int b)	{	return a > b ? a : b;	}
int		min(int a, int b)	{	return a < b ? a : b;	}

void manta_assert(bool valid, const char* message) {
	if (!valid) printf("%s", message);
	assert(valid); }

bool floatEq(float f1, float f2) {
	return ((f1 < (f2 + FLOAT_EPSILON)) && (f1 > (f2 - FLOAT_EPSILON))); }

// Color

void colorFloatToInt(color3i* intCol, color3f* floatCol)
{
	intCol->red   = (int) (255.f * floatCol->red);
	intCol->green = (int) (255.f * floatCol->green);
	intCol->blue  = (int) (255.f * floatCol->blue);
}

void colorIntToFloat(color3f* floatCol, color3i* intCol)
{
	floatCol->red   = ((float)intCol->red / 255.f);
	floatCol->green = ((float)intCol->green / 255.f);
	floatCol->blue  = ((float)intCol->blue / 255.f);
}

int	colorIntVariance(color3i* colorA, color3i* colorB)
{
	color3i diff;
	diff.red   = colorA->red   - colorB->red;
	diff.green = colorA->green - colorB->green;
	diff.blue  = colorA->blue  - colorB->blue;
	return  diff.red   * diff.red +
			diff.green * diff.green +
			diff.blue  * diff.blue;
}

void colorIntAdd(color3i* dst, color3i* src1, color3i* src2)
{
	dst->red   = src1->red   + src2->red;
	dst->green = src1->green + src2->green;
	dst->blue  = src1->blue  + src2->blue;
}

void colorIntDivide(color3i* color, int scale)
{
	color->red	 /= scale;
	color->green /= scale;
	color->blue  /= scale;
}

void colorFloatTrunc(color3f* color)
{
	color->x = fmax(0.f, fmin(color->x, 1.f));
	color->y = fmax(0.f, fmin(color->y, 1.f));
	color->z = fmax(0.f, fmin(color->z, 1.f));
}

unsigned long long rdtsc()
{
  #define rdtsc(low, high) \
         __asm__ __volatile__("rdtsc" : "=a" (low), "=d" (high))

  unsigned long low, high;
  rdtsc(low, high);
  return ((unsigned long long)high << 32) | low;
	#undef rdtsc
}

#if PROFILE_ENABLE
void profileBegin(int index)
{
	unsigned long long timer = rdtsc();
	profileTimers[index] = timer;
}

void profileEnd(int index)
{
	unsigned long long timer = rdtsc();
	profileDurations[index] += (timer - profileTimers[index]);
}

void initProfiles()
{
	for (int i=0; i<MAX_PROFILES; i++)
	{
		profileTimers[i] = 0;
		profileDurations[i] = 0;
	}	
}

void dumpProfileTimes()
{
	for (int i=0; i<MAX_PROFILES; i++)
	{
		printf("%.1f%%	%llu	(%s)\n", 100.f * (float)profileDurations[i]/(float)profileDurations[0], profileDurations[i], profileStrings[i]);
	}

	unsigned long long frameTime = profileDurations[0]/frameCount;
	float fps = 1000000000.f / (float)frameTime;
	printf("AVG FRAMES PER SECOND: %.2f\n", fps);
}

void newFrame()
{
	frameCount++;
}
#endif // PROFILE_ENABLE
