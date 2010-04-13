
// PROFILE_ENABLE: disabled (0), enabled (1), master (2)
#define PROFILE_ENABLE 1

#if PROFILE_ENABLE
//#define MAX_PROFILES 8
#define	profiles(f) \
		f(PROFILE_RENDER) \
		f(PROFILE_RENDER_PREPARE) \
		f(PROFILE_RAYTRACE) \
		f(PROFILE_PROJECTRAY) \
		f(PROFILE_INTERSECT) \
		f(PROFILE_TESTPOLY) \
		f(PROFILE_RENDER_PHASE_1) \
		f(PROFILE_RENDER_PHASE_2) \
		f(PROFILE_RENDER_PHASE_3) \
		f(PROFILE_LIGHT) \
		f(PROFILE_SETPIXEL) \
		f(PROFILE_LHS_SLOW) \
		f(PROFILE_LHS_FAST) \
		f(PROFILE_TRAVERSE) \
		f(PROFILE_BUILD_KDTREE) \
		f(PROFILE_SAH) \
		f(PROFILE_BUILD_EVENTS)

#define MAX_PROFILES COUNT(profiles)

BUILD_ENUM(profiles)
#if PROFILE_ENABLE == 1
#define PROFILE_BEGIN(a)		profileBegin(a);
#define PROFILE_END(a)			profileEnd(a);
#define PROFILE_COMMAND(a)		a
#else
#define PROFILE_BEGIN(a)		/* disabled profile */
#define PROFILE_END(a)			/* disabled profile */
#define PROFILE_COMMAND(a)		/* disabled profile */
#endif // PROFILE_ENABLE==1
#define PROFILE_MASTER_BEGIN(a)	profileBegin(a);
#define PROFILE_MASTER_END(a)	profileEnd(a);
#else // PROFILE_ENABLE
#define PROFILE_BEGIN(a)		/* disabled profile */
#define PROFILE_END(a)			/* disabled profile */
#define PROFILE_COMMAND(a)		/* disabled profile */

#define PROFILE_MASTER_BEGIN(a)	/* disabled profile */
#define PROFILE_MASTER_END(a)	/* disabled profile */
#endif // PROFILE_ENABLE

