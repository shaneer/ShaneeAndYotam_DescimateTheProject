
/*
 *  SPPoint
 *  Author: Yotam
 */

#include "SPPoint.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

struct sp_point_t {
    double* p_itms;
    int p_dim;
    int p_index;
} Point;

/**
 * Allocates memory to a new point based on the dimension.
 * If allocation fails, frees allocated memory and returns NULL.
 *
 * @param dim - The dimension of the point.
 * @return
 * A pointer to the newly allocated point.
 * NULL if allocation failure occurred.
 */
SPPoint assignMemForNewPoint(int dim) {
	SPPoint p = (SPPoint)malloc(sizeof(Point));
	if (p == NULL) {
		return NULL;
	}
	p->p_itms = (double*)malloc(sizeof(double) * dim);
	if (p->p_itms == NULL) {
		free(p);
		return NULL;
	}
	return p;
}

SPPoint spPointCreate(double* data, int dim, int index) {
    if (data == NULL || dim <= 0 || index < 0)  return NULL;
	SPPoint p = assignMemForNewPoint(dim);
	if (p == NULL) return NULL;
    for (int i=0; i<dim; i++) p->p_itms[i] = data[i];
    p->p_dim = dim;
    p->p_index = index;
    return p;
}

SPPoint spPointCopy(SPPoint source) {
	assert(source != NULL);
    SPPoint p = spPointCreate(source->p_itms, source->p_dim, source->p_index);
    if (p == NULL) return NULL;
    else return p;
}

void spPointDestroy(SPPoint point) {
	if (point != NULL) {
		free(point->p_itms);
    	free(point);
	}
}

int spPointGetDimension(SPPoint point) {
	assert(point != NULL);
    return point->p_dim;
}

int spPointGetIndex(SPPoint point) {
	assert(point != NULL);
    return point->p_index;
}

double spPointGetAxisCoor(SPPoint point, int axis) {
	assert(point != NULL && axis < point->p_dim);
    return point->p_itms[axis];
}

double spPointL2SquaredDistance(SPPoint p, SPPoint q) {
	assert(p != NULL && q != NULL && p->p_dim == q->p_dim);
    double dist = 0.0;
    for (int i=0; i<p->p_dim;i++) {
    	// dist += pow((p->p_itms[i] - q->p_itms[i]), 2);
    	dist += (p->p_itms[i] - q->p_itms[i])*(p->p_itms[i] - q->p_itms[i]);
    }
    return dist;
}
