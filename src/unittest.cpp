#include "src/common.h"
#include "src/unittest.h"
//--------------------------------------------------------
#include "src/vector.h"

#include <stdio.h>

UnitTest::UnitTest()
{
}

UnitTest::~UnitTest()
{
}

void UnitTest::TestVectors()
{
	vec3 OneTwoThree(1.f, -2.f, -3.f);
	vec3 FourFiveSix(4.f, -5.f, 6.f);
	vec3 resultv;
	float resultf;

	vec3Zero(&OneTwoThree);
	manta_assert(floatEq(OneTwoThree.x, 0.f), "Unit Test Error (vec3Zero): X value not 0.\n");
	manta_assert(floatEq(OneTwoThree.y, 0.f), "Unit Test Error (vec3Zero): Y value not 0.\n");
	manta_assert(floatEq(OneTwoThree.z, 0.f), "Unit Test Error (vec3Zero): Z value not 0.\n");

	vec3Set(&OneTwoThree, 1.f, -2.f, -3.f);
	manta_assert(floatEq(OneTwoThree.x, 1.f), "Unit Test Error (vec3Set): X value not 1.\n");
	manta_assert(floatEq(OneTwoThree.y, -2.f), "Unit Test Error (vec3Set): Y value not -2.\n");
	manta_assert(floatEq(OneTwoThree.z, -3.f), "Unit Test Error (vec3Set): Z value not -3.\n");

	resultf = vec3Dot(&OneTwoThree, &FourFiveSix);
	manta_assert(floatEq(resultf, -4.f), "Unit Test Error (vec3Dot): Dot product not -3.\n");
//	printf("dot is %f.\n", resultf);

	vec3Cross(&resultv, &OneTwoThree, &FourFiveSix);
	manta_assert(floatEq(resultv.x, -27.f), "Unit Test Error (vec3Cross): X value not -27.\n");
	manta_assert(floatEq(resultv.y, -18.f), "Unit Test Error (vec3Cross): Y value not -18.\n");
	manta_assert(floatEq(resultv.z, 3.f), "Unit Test Error (vec3Cross): Z value not 3.\n");

	vec3Add(&resultv, &OneTwoThree, &FourFiveSix);
	manta_assert(floatEq(resultv.x, 5.f), "Unit Test Error (vec3Add): X value not 5.\n");
	manta_assert(floatEq(resultv.y, -7.f), "Unit Test Error (vec3Add): Y value not -7.\n");
	manta_assert(floatEq(resultv.z, 3.f), "Unit Test Error (vec3Add): Z value not 3.\n");

	vec3Sub(&resultv, &OneTwoThree, &FourFiveSix);
	manta_assert(floatEq(resultv.x, -3.f), "Unit Test Error (vec3Sub): X value not -3.\n");
	manta_assert(floatEq(resultv.y, 3.f), "Unit Test Error (vec3Sub): Y value not 3.\n");
	manta_assert(floatEq(resultv.z, -9.f), "Unit Test Error (vec3Sub): Z value not -9.\n");

	resultf = vec3Len(&FourFiveSix);

	vec3Unit(&FourFiveSix);
	manta_assert(vec3IsUnit(&FourFiveSix), "Unit Test Error (vec3Unit/vec3IsUnit) - vector not Unit!.\n");
	vec3Scale(&FourFiveSix, resultf);
	manta_assert(floatEq(FourFiveSix.x, 4.f), "Unit Test Error (vec3Scale): X value not 4.\n");
	manta_assert(floatEq(FourFiveSix.y, -5.f), "Unit Test Error (vec3Scale): Y value not -5.\n");
	manta_assert(floatEq(FourFiveSix.z, 6.f), "Unit Test Error (vec3Scale): Z value not 6.\n");
}

int slow(int *a, int *b)
{
   *a = 5;
   *b = 7;
   return *a + *b; // LHS stall: the compiler doesn't
                  // know whether a == b, so it has to
                  //  reload both before the add
}

//Whereas this code will run quickly:
int fast(int * __restrict a, int * __restrict b)
{
   *a = 5;
   *b = 7; // RESTRICT promises that a != b
   return *a + *b; // no stall; the compiler hangs onto
                         // 5 and 7 in the registers.
}

void UnitTest::TestSpeed()
{
	int x, y, z;
	// Load-hit-store testing
	PROFILE_BEGIN(PROFILE_LHS_SLOW);
	for (int i = 0; i > 1000000000; i++)
	{
		z = slow(&x, &y);
	}
	PROFILE_END(PROFILE_LHS_SLOW);

	PROFILE_BEGIN(PROFILE_LHS_FAST);
	for (int i = 0; i > 1000000000; i++)
	{
		z = fast(&x, &y);
	}
	PROFILE_END(PROFILE_LHS_FAST);
}

