#ifndef __PROP_H__
#define __PROP_H__

class Prop
{
	protected:
		model*	propModel;
		vec3	velocity;

	public:
		Prop();
		Prop(model* thisModel);
		virtual ~Prop();

		void init();
		void tick(float dt);

		// Getters and Setters
		void setPosition(vec3* newPosition);
		void setVelocity(vec3* newVelocity);
		void setRotation(matrix* newRotation);
};

#endif
