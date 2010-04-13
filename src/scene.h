#ifndef __SCENE_H__
#define __SCENE_H__

#include <vector>
#include <pthread.h>

#define MAX_SCENE_POLYS 1024
#define MAX_SCENE_VERTICES 512
#define	MAX_SCENE_MODELS 8

#define SPECULAR_BRIGHTNESS 0.2f
#define SPECULAR_FALLOFF	0.1f
#define AMBIENT_BRIGHTNESS	0.2f

// Forward Declarations
class kdTree;
class Texture;

class Scene
{
	public:
        Scene();
        virtual ~Scene();

        void	init();
        void	profileInit();

        int		addVertex(vec3* position);
        int		addPolygon(int* inVerts);
        model*	addModel(vec3* position, int vertexIndex, int vertexCount, int polyIndex, int polyCount);

        void	objectToCameraSpace(cameraData* viewCam);
        void	prepareModelRenderData(cameraData* viewCam);
        void	prepareForRender(cameraData* viewCam);

        // Calculate the color value of a pixel given by a ray
        color3f colorRayTrace(rayData& ray);

        // Get the colour of a particular ray intersection
        color3f getRayColor(rayData& ray, polygon* closestPoly, point3* closestPoint, float u, float v);

        void	PrintStats();

        // Getters'n'Setters
        int getVertexCount() { return vertexCount; }
        int getPolyCount()	 { return polyCount; }
        void setLight(vec3* position, color3f* color);

    private:
#if PROFILE_ENABLE == 1
        int totalPolysTested;
        int raysTraced;
        int planesTested;
#endif // PROFILE_ENABLE == 1

    protected:
        int polyCount;
        int vertexCount;
        int modelCount;

        // Scene Information
        polygon scenePolygons	    [MAX_SCENE_POLYS];
        point3	sceneVertices   	[MAX_SCENE_VERTICES];
        model	sceneModels			[MAX_SCENE_MODELS];

        // Frame Information
        std::vector<polygon>	    renderPolygons;
        std::vector<point3>		    renderVertices;
        std::vector<model>		    renderModels;

        // List of lights
        light sceneLight;
        light renderLight;

		// kdTree
		kdTree*		sceneTree;

		Texture*	texture;	// DEBUG: default texture for test
};

#endif // __SCENE_H__
