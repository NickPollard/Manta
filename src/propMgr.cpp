#include "src/common.h"
#include "src/propMgr.h"
//--------------------------------------------------------
#include "src/mantaMgr.h"
#include "src/prop.h"
#include "src/vector.h"

PropMgr::PropMgr()
{
	init();
}

PropMgr::~PropMgr()
{
}

void PropMgr::init()
{
	propCount = 0;
}

void PropMgr::tick(float dt)
{
	for (int i = 0; i < propCount; i++)
	{
		propList[i]->tick(dt);
	}
}

Prop* PropMgr::createProp(string modelFile)
{
	vec3 position;
	vec3Zero(&position);
	model* thisModel = modelManager->LoadObj(modelFile, &position);
	propList[propCount] = new Prop(thisModel);
	Prop* thisProp = propList[propCount++];
	return thisProp;
}
