#include "src/vector.h"
#include "src/polygon.h"
#include "src/matrix.h"

typedef char byte;
typedef char* string;

struct rectangle
{
	vec2	min;
	vec2	max;
};

struct rayData
{
	point3	origin;
	vec3	direction;
};

struct light
{
	point3	position;
	color3f	color;
};

struct cameraData
{
	point3	position;
	vec3	dir;
	vec3	up;
	vec3	right;
};

struct model
{
	point3	position;
	matrix	rotation;
	int		vertexCount;
	int		vertexIndex;
	int		polyCount;
	int		polyIndex;
	std::vector<polygon>::iterator	polygons;
};

struct intersectData
{
	point3		location;
	polygon*	poly;
	float		u;
	float		v;

	intersectData()	:
		poly(NULL)
	{}
};

