#ifndef __PROPMGR_H__
#define __PROPMGR_H__

class Prop;

#define MAX_PROPS	64

class PropMgr
{
	protected:
		int		propCount;
		Prop*	propList[MAX_PROPS];

	public:
		PropMgr();
		virtual ~PropMgr();

		void init();
		void tick(float dt);
		
		Prop* createProp(string modelFile);
};

#endif
