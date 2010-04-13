#ifndef __CAMERA_H__
#define __CAMERA_H__

class Camera
{
	protected:
		cameraData	camData;

	public:
		Camera();
		virtual ~Camera();

		void init();
		void tick(float dt);

		void lookAt_Track(vec3* target, float distance);
		void lookAt_Pan(vec3* target);

		// Getters and Setters
		cameraData*	getCameraData()		{ return &camData; }
};

#endif // __CAMERA_H__
