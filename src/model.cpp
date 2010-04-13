#include "src/common.h"
#include "src/model.h"
//--------------------------------------------------------
#include "src/vector.h"
#include <math.h>
#include <stdio.h>
/*
void	modelCreateBatches(model* mdl, model** batches, polygon** polygons)
{
	const int k = 4;
	// K(4) means clustering
	rectangle means[k];
	// Estimate 4 initial points
	// Use the .25 and .75 quartiles
	float x25 = ((3.f * mdl->gradients.min.x) + (mdl->gradients.max.x)) * 0.25f;
	float x75 = ((mdl->gradients.min.x) + (3.f * mdl->gradients.max.x)) * 0.25f;
	float y25 = ((3.f * mdl->gradients.min.y) + (mdl->gradients.max.y)) * 0.25f;
	float y75 = ((mdl->gradients.min.y) + (3.f * mdl->gradients.max.y)) * 0.25f;

	INIT_MEAN(means[0], x25, y25)
	INIT_MEAN(means[1], x75, y25)
	INIT_MEAN(means[2], x25, y75)
	INIT_MEAN(means[3], x75, y75)

	// iterative K-means clustering
	for (int iterations = 0; iterations < MAX_KMEANS_ITERATIONS; iterations++)
	{
		// Average the means Extents
		for (int i = 0; i < k; i++)
		{
			means[i].min.x = (means[i].min.x + means[i].max.x) * 0.5;
			means[i].max.x = means[i].min.x;
			means[i].min.y = (means[i].min.y + means[i].max.y) * 0.5;
			means[i].max.y = means[i].min.y;
		}
		// Loop through every polygon in the model
		for (int i = 0; i < mdl->polyCount; i++)
		{
			polygon* thePolygon = &mdl->polygons[i];
			int		closestMean = 0;
			float	closestDistance = FLOAT_MAX;
			// Get the max distance from that polygons extents to each mean's extents
			for (int j = 0; j < k; j++)
			{
				float distance = polyDistanceFromMean(thePolygon, &means[j]);
				if (distance < closestDistance)
				{
					closestDistance = distance;
					closestMean = j;
				}
			}
			thePolygon->kmean = closestMean;
			// Update Extents
			*/
			/*
			means[closestMean].maxX = (thePolygon->gradients.max.x > means[closestMean].maxX) ? thePolygon->gradients.max.x : means[closestMean].maxX;
			means[closestMean].minX = (thePolygon->gradients.min.x < means[closestMean].minX) ? thePolygon->gradients.min.x : means[closestMean].minX;
			means[closestMean].maxY = (thePolygon->gradients.max.y > means[closestMean].maxY) ? thePolygon->gradients.max.y : means[closestMean].maxY;
			means[closestMean].minY = (thePolygon->gradients.min.y < means[closestMean].minY) ? thePolygon->gradients.min.y : means[closestMean].minY;
			*/
/*
		}
	}
	// Build the batches
	int n = 0;
	for (int i = 0; i < k; i++)
	{
		(**batches) = *mdl;
		(**batches).polyCount = 0;
		(**batches).polyIndex = n;
		(**batches).polygons = (*polygons);
		for (int j = 0; j < mdl->polyCount; j++)
		{
			if (mdl->polygons[j].kmean == i)
			{
				(**batches).polyCount++;
				(**polygons) = mdl->polygons[j];
				n++;
				(*polygons)++;
			}
		}
		(*batches)++;
	}
}

float polyDistanceFromMean(polygon* poly, rectangle* mean)
{
	float xdistance, ydistance;
	xdistance = fmax((poly->gradients.max.x - mean->max.x), (mean->min.x - poly->gradients.min.x));
	ydistance = fmax((poly->gradients.max.y - mean->max.y), (mean->min.y - poly->gradients.min.y));
	return xdistance + ydistance;
}

void zSortPolygons(polygon* polygonList, int polygonCount)
{
	// TODO Currently a bubblesort, change to something more efficient!
	for (int i = 0; (i + 1) < polygonCount; i++)
	{
		for (int j = 0; (j + 1) < (polygonCount - i); j++)
		{
			// Test j and j+1, maybe swap
			if (polygonList[j].minZ > polygonList[j+1].minZ)
			{
				//Swap
				polygon slot;
				slot = polygonList[j+1];
				polygonList[j+1] = polygonList[j];
				polygonList[j] = slot;
			}
		}
	}
}

void zSortModels(model* modelList, int modelCount)
{
	// TODO Currently a bubblesort, change to something more efficient!
	for (int i = 0; (i + 1) < modelCount; i++)
	{
		for (int j = 0; (j + 1) < (modelCount - i); j++)
		{
			// Test j and j+1, maybe swap
			if (modelList[j].minZ > modelList[j+1].minZ)
			{
				//Swap
				model slot;
				slot = modelList[j+1];
				modelList[j+1] = modelList[j];
				modelList[j] = slot;
			}
		}
	}
}
*/
