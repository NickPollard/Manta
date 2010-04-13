#ifndef __VECTOR_H__
#define __VECTOR_H__

#include "gdkmm/pixbuf.h"

// TODO - this should really be smaller, 32 bits for one color in 8bit r/g/b/a values
// Then can just load in place from pixbuf
// Maybe use gdk color if there is a good one?
struct color3i
{
	int red;
	int green;
	int blue;

	color3i() :
		red(0),
		green(0),
		blue(0)
	{}
	color3i(guint8 r, guint8 g, guint8 b) {
		red = r;
		green = g;
		blue = b;
	}
};

union vec2
{
	struct
	{
		float x;
		float y;
	};
	float	val[2];
};


union vec3
{
    struct
    {
        float x;
        float y;
        float z;
    };
    struct
    {
        float red;
        float green;
        float blue;
    };
    float val[3];
    
    vec3()  {   x = 0.f; y = 0.f; z = 0.f;  }
    vec3(float xIn, float yIn, float zIn)   {   x = xIn; y = yIn; z = zIn;  }

    public:
        void Normalize();

		float& operator [] (int i) {
			return val[i];
		}

        vec3 operator * (float s)  {
            return vec3(x * s, y * s, z * s);
        }

        vec3 operator + (vec3 v) {
            return vec3(x + v.x, y + v.y, z + v.z);
        }

        vec3 operator - (vec3 v) {
            return vec3(x - v.x, y - v.y, z - v.z);
        }

		void operator = (const color3i& color) {
			red = ((float)color.red) / 255.f;
			green = ((float)color.green) / 255.f;
			blue = ((float)color.blue) / 255.f;
		}

        float Length();
        float LengthSq();

        void Print();
};

extern const vec3 kVec3Up;

void	vec3Zero(vec3* vec);
bool	vec3IsUnit(vec3* vec);
void	vec3Set(vec3* vec, float x, float y, float z);
void	vec3Cpy(vec3* dst, vec3* src);
void	vec3Cross(vec3* dst, const vec3* v1, const vec3* v2);
float	vec3Dot(vec3& v1, vec3& v2);
float	vec3Dot(vec3* v1, vec3* v2);
void	vec3Scale(vec3* v, float f);
void	vec3Add(vec3* dst, vec3* src1, vec3* src2);
void	vec3AddScale(vec3* dst, vec3* src, float scale);
void	vec3Sub(vec3* dst, vec3* src1, vec3* src2);
float	vec3Len(vec3* v);
float	vec3LenSq(vec3* v);
float	vec3Manhattan(vec3* v);
void	vec3Unit(vec3* v);
void	vec3Reflect(vec3* reflectRay, vec3* inRay, vec3* normal);

union vec4
{
	struct
	{
		float x;
		float y;
		float z;
		float w;
	};
	struct
	{
		float red;
		float green;
		float blue;
		float alpha;
	};
	float	val[4];

    vec4()  {   x = 0.f; y = 0.f; z = 0.f; w = 0.f; }
    vec4(float xIn, float yIn, float zIn, float wIn = 1.f)   {   x = xIn; y = yIn; z = zIn; w = wIn; }

    vec4 operator * (float s) 
    {
        return vec4(x * s, y * s, z * s, w);
    }
};

typedef vec2 point2;
typedef vec3 point3;

//
// Color
//

typedef vec3 color3f;

color3i ColorFromG8(guint8* pixels);

#endif // __VECTOR_H__
