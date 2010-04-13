#ifndef __POLYGON_H__
#define __POLYGON_H__

struct volume3
{
	point3	min;
	point3	max;

	float SurfaceArea();
};

//
// Polygons
//

struct polygon2d
{
	point2	point[3];
};

struct polygon
{
	point3*	point[3];
	int		pointIndex[3];	// TODO - separate into different classes? Result polys don't need index
	vec3	normal;			// TODO - vertex normals
	color3f	color;
	float	d;

    point3* A() {   return point[0];    }
    point3* B() {   return point[1];    }
    point3* C() {   return point[2];    }
	point3& GetA() { return *point[0];	}
	point3& GetB() { return *point[1];	}
	point3& GetC() { return *point[2];	}

	volume3 getBounds();

	float	getMin(int axis) {
		return fmin3((*point[0])[axis],(*point[1])[axis],(*point[2])[axis]);	}

	float	getMax(int axis) {
		return fmax3((*point[0])[axis],(*point[1])[axis],(*point[2])[axis]);	}
};

#endif // __POLYGON_H__
