/*
    KDArray
    Author: Yotam
 */

#ifndef SPKDARRAY_H_
#define SPKDARRAY_H_

#include "SPPoint.h"

typedef struct sp_kd_array* SPKDArray;

/*
 *  Allocates the needed memory, copies the points
 *  and creates the KDArray from them, with the given size.
 *  The method assumes the size param (size) is valid for the points array (arr).
 *
 *  arr - the SPPoint array
 *  size - the amount of points in the array
 *
 *  @return
 *  NULL if error occured, otherwise returns the newly created SPKDArray.
 */
SPKDArray Init(SPPoint* arr, int size);

/*
 *  Splits the given SPKDArray based on a given coordinate.
 *  This function assumes that the coordinate param (coor) is valid for the given SPKDArray.
 *
 *  kdArr - the SPKDArray to split
 *  coor - the coordinate to split by
 *
 *  @return
 *  NULL if error occured , otherwise returns two SPKDArray (kdLeft, kdRight).
 */
SPKDArray* Split(SPKDArray spkdArr, int coor);

/*
 *  Returns the coor index with the max spread
 */
int spKDArrayFindMaxSpreadDim(SPKDArray arr);

/*
 *  Return the point at an index in the KDArray
 */
SPPoint spKDArrayGetPoint(SPKDArray arr, int index);

/*
 *  Getter for the image-index at kdArr[i,j].
 */
int spKDArrayGetIndexInArray(SPKDArray spkdArr, int i, int j);

/*
 *  Getter for the dimention of the SPKDArray.
 */
int spKDArrayGetDimention(SPKDArray spkdArr);

/*
 *  Getter for the amount of points in the SPKDArray.
 */
int spKDArrayGetSize(SPKDArray spkdArr);

/*
 *  Destroy the SPKDArray given and clear it's memory usage.
 */
void spKDArrayDestroy(SPKDArray spkdArr);

/*
 *  Fully print the array
 */
void spKDArrayFullPrint(SPKDArray spkdArr);

#endif // SPKDRAAY_H_
