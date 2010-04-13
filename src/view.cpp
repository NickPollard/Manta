#include "src/common.h"
#include "src/view.h"
//--------------------------------------------------------
#include "src/camera.h"
#include "src/scene.h"
#include "src/vector.h"

//===========================================================================
// Constructor
View::View()
{
	init();
}

//===========================================================================
// Constructor
View::View(Scene* inScene)
{
	viewScene = inScene;
	init();
}

//===========================================================================
// Destructor
View::~View()
{
}

//===========================================================================
// Initialise
void View::init()
{
	imageWidth = 10.f;
	aspectRatio = 0.75f;
	imageHeight = imageWidth * aspectRatio;
	imageDepth = 5.f;
	viewCamera = new Camera();
}

//===========================================================================
void View::tick(float dt)
{
	viewCamera->tick(dt);
}

//===========================================================================
inline void View::projectRay(rayData* ray, float x, float y)
{
	float ix = x * imageWidth;
	float iy = y * imageHeight;

	// Scene is rotated so camera faces 0,0,1
	vec3Set(&ray->direction, ix, iy, imageDepth);

	// Scene is altered so camera is at 0,0,0
	vec3Zero(&ray->origin);
}

//===========================================================================
// rayTrace - Trace a ray to calculate the colour of a given pixel
void View::rayTrace(color3f& pixCol, float sx, float sy)
{
	rayData ray;

	PROFILE_BEGIN(PROFILE_PROJECTRAY);
    {
		projectRay(&ray, sx, sy);
    }
	PROFILE_END(PROFILE_PROJECTRAY);

	PROFILE_BEGIN(PROFILE_RAYTRACE);
    {
		pixCol = viewScene->colorRayTrace(ray);
    }
	PROFILE_END(PROFILE_RAYTRACE);
}

//===========================================================================
// Do scene related preparations before raytracing
void View::prepareForRender()
{
	viewScene->prepareForRender(viewCamera->getCameraData());
}

#if PROFILE_ENABLE == 1
void View::PrintStats()
{
	viewScene->PrintStats();
}
#endif // PROFILE_ENABLE == 1
