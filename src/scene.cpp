#include "src/common.h"
#include "src/scene.h"
//--------------------------------------------------------
#include "src/geometry.h"
#include "src/kdtree.h"
#include "src/matrix.h"
#include "src/model.h"
#include "src/texture.h"
#include "src/vector.h"
#include <algorithm>
#include <math.h>
#include <stdio.h>

#define USE_KDTREE 1

//===========================================================================
Scene::Scene()
{
	init();
}

//===========================================================================
Scene::~Scene()
{
}

//===========================================================================
void Scene::init()
{
	vertexCount = 0;
	polyCount = 0;
	modelCount = 0;
	profileInit();
	texture = new Texture("assets/images/test.png");
	sceneTree = new kdTree();
}

//===========================================================================
void Scene::profileInit()
{
#if PROFILE_ENABLE == 1
	totalPolysTested = 0;
	raysTraced = 0;
	planesTested = 0;
#endif // PROFILE_ENABLE == 1
}

//===========================================================================
int Scene::addVertex(vec3* position)
{
	manta_assert(vertexCount < MAX_SCENE_VERTICES, "Error, attempting to add too many Vertices!\n");
	vec3Cpy(&sceneVertices[vertexCount++], position);
	return vertexCount-1;
}

//===========================================================================
int Scene::addPolygon(int* inVerts)
{
	manta_assert(polyCount < MAX_SCENE_POLYS, "Error, attempting to add too many Polygons!\n");
	color3f white;
	vec3Set(&white, 1.f, 1.f, 1.f);
	vec3 edgeA, edgeB, normal;
	vec3* verts[3];

	verts[0] = &sceneVertices[inVerts[0]];
	verts[1] = &sceneVertices[inVerts[1]];
	verts[2] = &sceneVertices[inVerts[2]];

	vec3Sub(&edgeA, verts[0], verts[1]);
	vec3Sub(&edgeB, verts[0], verts[2]);

	vec3Cross(&normal, &edgeB, &edgeA);
	vec3Unit(&normal);

	scenePolygons[polyCount].pointIndex[0] = inVerts[0];
	scenePolygons[polyCount].pointIndex[1] = inVerts[1];
	scenePolygons[polyCount].pointIndex[2] = inVerts[2];

	debugBuildPoly(&scenePolygons[polyCount++], 
					verts[0],
					verts[1],
					verts[2],
					&normal,
					&white);


	return polyCount-1;
}

//===========================================================================
model* Scene::addModel(vec3* position, int vertexIndex, int vertexCount, int polyIndex, int polyCount)
{
	manta_assert(modelCount < MAX_SCENE_MODELS, "Error, attempting to add too many Models!\n");
	model* newModel = &sceneModels[modelCount++];
	newModel->vertexIndex = vertexIndex;
	newModel->vertexCount = vertexCount;
	newModel->polyCount = polyCount;
	newModel->polyIndex = polyIndex;
	matrixIdent(&newModel->rotation);
	vec3 up, dir;
	vec3Set(&up, 1.f, 0.f, 0.f);
	vec3Set(&dir, 0.f, 0.f, 1.f);
	rotMatrixFromVec3(&newModel->rotation, &dir, &up);
	vec3Cpy(&newModel->position, position);
	return newModel;
}

//===========================================================================
color3f Scene::colorRayTrace(rayData& ray)
{
	intersectData intersect;
	sceneTree->TraverseTree(ray, intersect);
	return getRayColor(ray, intersect.poly, &intersect.location, intersect.u, intersect.v);
}

//===========================================================================
// Takes Ray and Intersect data and calculates the colour of that light ray
color3f Scene::getRayColor(rayData& ray, polygon* closestPoly, point3* closestPoint, float u, float v)
{
	color3f pixCol(0.f, 0.f, 0.f);
	PROFILE_BEGIN(PROFILE_LIGHT)
		if (closestPoly)
		{
#if LIGHTING_ENABLE
			color3f tmpColor;
			vec3Cpy(&tmpColor, &renderLight.color); 
			vec3 displacement;
			vec3Sub(&displacement, &renderLight.position, closestPoint);
			float scale = 1.f;
			{
#if LIGHTING_DISTANCE_BASED		
				scale = 1/(vec3LenSq(&displacement)+1.f);
#endif // LIGHTING_DISTANCE_BASED
#if LIGHTING_NORMAL_BASED
				scale *= vec3Dot(&closestPoly->normal, &displacement);
#endif // LIGHTING_NORMAL_BASED
#if LIGHTING_SPECULAR_ENABLED
				vec3 reflection;
				vec3Reflect(&reflection, &displacement, &closestPoly->normal);
				vec3Unit(&reflection);
				vec3Unit(closestPoint);
				float specular = vec3Dot(&reflection, closestPoint);
				specular = (-specular * SPECULAR_BRIGHTNESS) - SPECULAR_FALLOFF;
				specular = fmax(0.f, specular);
				scale += specular;
#endif // LIGHTING_SPECULAR_ENABLED
			}
#if LIGHTING_AMBIENT_ENABLED
			scale += AMBIENT_BRIGHTNESS;
#endif // LIGHTING_AMBIENT_ENABLED
			vec3Scale(&tmpColor, scale);
//			color3f* pColor = &closestPoly->color;
			color3f texColor;
			texture->Sample(u, v, texColor);
			tmpColor.x *= texColor.x;
			tmpColor.y *= texColor.y;
			tmpColor.z *= texColor.z;
			colorFloatTrunc(&tmpColor);
			pixCol = tmpColor;
#else
			pixCol = closestPoly->color;
#endif
		}
	PROFILE_END(PROFILE_LIGHT)
		return pixCol;
}

//===========================================================================
// Transforms vertices + lights to Camera Space for this frame
void Scene::objectToCameraSpace(cameraData* viewCam)
{
	matrix rotMatrix;
	matrixIdent(&rotMatrix);
	invRotMatrixFromVec3(&rotMatrix, &viewCam->dir, &viewCam->up);

	renderVertices.clear();
	for (int i = 0; i < modelCount; i++)
	{
		model* currentModel = &sceneModels[i];
		for (int j = 0; j < currentModel->vertexCount; j++)
		{
			int n = currentModel->vertexIndex + j;
			renderVertices.push_back(sceneVertices[n]);

			// Transform verts from model space to world space
			matrixVec3Rot(&renderVertices[n], &currentModel->rotation, &sceneVertices[n]);
			renderVertices[n] = renderVertices[n] + currentModel->position;

			// Transform verts from world space to camera space
			renderVertices[n] = renderVertices[n] - viewCam->position;
			matrixVec3Rot(&renderVertices[n], &rotMatrix, &renderVertices[n]);
		}
	}
	// Transform Lights from world space to camera space
	vec3Sub(&renderLight.position, &sceneLight.position, &viewCam->position);
	matrixVec3Rot(&renderLight.position, &rotMatrix, &renderLight.position);
	vec3Cpy(&renderLight.color, &sceneLight.color);
}

//===========================================================================
void Scene::prepareModelRenderData(cameraData* viewCam)
{
	matrix rotMatrix;
	matrixIdent(&rotMatrix);
	invRotMatrixFromVec3(&rotMatrix, &viewCam->dir, &viewCam->up);

	int newPolyIndex = 0;
	renderModels.clear();
	renderPolygons.clear();
	for (int i = 0; i < modelCount; i++)
	{
		renderModels.push_back(sceneModels[i]);
		model* mdl = &renderModels.back();
		int newPolyCount = 0;
		// For every polygon in each model
		for (int j = 0; j < mdl->polyCount; j++)
		{
			int n = j + mdl->polyIndex;

			// Copy scene polygon to render polygons
			renderPolygons.push_back(scenePolygons[n]);
			polygon* poly = &renderPolygons.back();

			// Transform normals to camera space
			matrixVec3Rot(&poly->normal, &mdl->rotation, &poly->normal);
			matrixVec3Rot(&poly->normal, &rotMatrix, &poly->normal);
            poly->normal.Normalize();

			poly->point[0] = &renderVertices[poly->pointIndex[0]];
			poly->point[1] = &renderVertices[poly->pointIndex[1]];
			poly->point[2] = &renderVertices[poly->pointIndex[2]];

			// TODO -  Store points B and C as vectors from A

			poly->d = vec3Dot(&poly->normal, poly->point[0]);

			newPolyCount++;
		}
		mdl->polyIndex = newPolyIndex;
		mdl->polygons = renderPolygons.begin() += newPolyIndex;
		mdl->polyCount = newPolyCount;
		newPolyIndex += newPolyCount;
	}
	polyCount = newPolyIndex;
}

//===========================================================================
void Scene::prepareForRender(cameraData* viewCam)
{
	// Transform scene to Camera Space
	objectToCameraSpace(viewCam);
	// Calculate model render data
	prepareModelRenderData(viewCam);

	PROFILE_BEGIN(PROFILE_BUILD_KDTREE)
	{
		sceneTree->Rebuild(&(*renderPolygons.begin()), (int)renderPolygons.size());
	}
	PROFILE_END(PROFILE_BUILD_KDTREE)

//	printf("Scene has %d tris\n.", polyCount);
}

//===========================================================================
void Scene::PrintStats()
{
#if PROFILE_ENABLE == 1
	sceneTree->PrintStats();
#endif // PROFILE_ENABLE == 1
}

void Scene::setLight(vec3* position, color3f* color)
{
	vec3Cpy(&sceneLight.position, position);
	vec3Cpy(&sceneLight.color, color);
}
