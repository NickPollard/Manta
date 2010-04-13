#include "src/common.h"
#include "src/camera.h"
//--------------------------------------------------------
#include "src/matrix.h"
#include "src/vector.h"

#define CAMERA_ANIM 1

Camera::Camera()
{
    init();
}

Camera::~Camera()
{
}

//-------------------------------------------------------------------------------
// Initialise the Camera
void Camera::init()
{
	// Test Stuff
#if CAMERA_ANIM
    camData.position =  vec3(0.f, 7.f, -9.f);
    camData.right =     vec3(1.f, 0.f, 0.f);
    camData.up =        vec3(0.f, 1.f, 1.f);
    camData.dir =       vec3(0.f, -1.f, 1.f);
#else
    camData.position =  vec3(0.f, 0.f, 0.f);
    camData.right =     vec3(1.f, 0.f, 0.f);
    camData.up =        vec3(0.f, 1.f, 0.f);
    camData.dir =       vec3(0.f, 0.f, 1.f);
#endif

	camData.dir.Normalize();
	camData.up.Normalize();
	camData.right.Normalize();

	manta_assert(vec3IsUnit(&camData.dir), 	"camData direction is not a unit vector.\n");
	manta_assert(vec3IsUnit(&camData.up), 	"camData up is not a unit vector.\n");
	manta_assert(vec3IsUnit(&camData.right),	"camData right is not a unit vector.\n");
}

//-------------------------------------------------------------------------------
void Camera::tick(float dt)
{
#if CAMERA_ANIM
	float rotation = 0.5f * dt;
	vec3 origin(0.f, 0.f, 0.f);
	matrix rotMatrix;
	matrixYAxisRot(&rotMatrix, rotation);
	matrixVec3Rot(&camData.dir,   &rotMatrix, &camData.dir);
	matrixVec3Rot(&camData.up,	  &rotMatrix, &camData.up);
	matrixVec3Rot(&camData.right, &rotMatrix, &camData.right);
	
    camData.dir.Normalize();
    camData.up.Normalize();
	camData.right.Normalize();

	lookAt_Track(&origin, 8.f);
#endif
}

//-------------------------------------------------------------------------------
void Camera::lookAt_Track(vec3* target, float distance)
{
	vec3Cpy(&camData.position, &camData.dir);
	vec3Scale(&camData.position, -distance);
	vec3Add(&camData.position, &camData.position, target);
}

//-------------------------------------------------------------------------------
void Camera::lookAt_Pan(vec3* target)
{
	vec3Sub(&camData.dir, target, &camData.position);
	vec3Cross(&camData.right, &camData.dir, &kVec3Up);
	vec3Cross(&camData.up, &camData.dir, &camData.right);
    
    camData.dir.Normalize();
    camData.up.Normalize();
	camData.right.Normalize();
}
