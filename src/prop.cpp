#include "src/common.h"
#include "src/prop.h"
//--------------------------------------------------------
#include "src/matrix.h"
#include "src/vector.h"

Prop::Prop()
{
	init();
}

Prop::Prop(model* thisModel)
{
	propModel = thisModel;
	init();
}

Prop::~Prop()
{
}

void Prop::init()
{
	vec3Zero(&velocity);
}

void Prop::tick(float dt)
{
	if (propModel)
	{
		vec3Add(&propModel->position, &propModel->position, &velocity);
	}
}

void Prop::setPosition(vec3* newPosition)
{
	vec3Cpy(&propModel->position, newPosition);
}

void Prop::setVelocity(vec3* newVelocity)
{
	vec3Cpy(&velocity, newVelocity);
}

void Prop::setRotation(matrix* newRotation)
{
	matrixCpy(&propModel->rotation, newRotation);
}
