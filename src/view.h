#ifndef __VIEW_H__
#define __VIEW_H__

class Camera;
class Scene;

class View
{
	protected:
		// Camera
		Camera*	viewCamera;
		Scene*	viewScene;
		float	imageWidth;
		float	imageHeight;
		float	aspectRatio;
		float	imageDepth;

		void projectRay(rayData* ray, float x, float y);
	public:
		View();
		View(Scene* inScene);
		virtual ~View();

		void init();
		void tick(float dt);

		void prepareForRender();
		void rayTrace(color3f& pixCol, float sx, float sy);

#if PROFILE_ENABLE == 1
		void PrintStats();
#endif // PROFILE_ENABLE == 1
};

#endif // __VIEW_H__
