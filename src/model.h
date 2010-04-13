#ifndef __MODEL_H__
#define __MODEL_H__

#define INIT_MEAN(a, x_, y_)	a.min.x = x_; a.max.x = x_; a.min.y = y_; a.max.y = y_;
#define MAX_KMEANS_ITERATIONS 2

/*
void	modelCreateBatches(model* mdl, model** batches, polygon** polygons);
float	polyDistanceFromMean(polygon* poly, rectangle* mean);
void	zSortPolygons(polygon* polygonList, int polygonCount);
void	zSortModels(model* modelList, int modelCount);
*/

#endif //__MODEL_H__
