/*
    KDArray Header File
    Author: Yotam
*/

#include "SPPoint.h"
#ifndef SPKDARRAY_H_
#define SPKDARRAY_H_

typedef struct sp_kd_array* SPKDArray;

/*
 *  Allocates the needed memory, copies the points
 *  and creates the KDArray from them, with the given size.
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
 *
 *  kdArr - the SPKDArray to split
 *  coor - the coordinate to split by
 *
 *  @return
 *  NULL if error occured , otherwise returns two SPKDArray (kdLeft, kdRight).
 */
 SPKDArray* Split(SPKDArray spkdArr, int coor);

/*
 *  Return a copy of the point array
 */
 SPPoint* GetPointsArray(SPKDArray spkdArr);

/*
 *  Getter for the image-index at kdArr[i,j].
 */
 int GetIndexInKDArray(SPKDArray spkdArr, int i, int j);

/*
 *  Getter for the dimention of the SPKDArray.
 */
 int GetDimention(SPKDArray spkdArr);

/*
 *  Getter for the amount of points in the SPKDArray.
 */
 int GetNumberOfPoints(SPKDArray spkdArr);

/*
 *  Destroy the SPKDArray given and clear it's memory usage.
 */
 int spKDArrayDestroy(SPKDArray spkdArr);

/*
 *  Prints the SPKDAraay given
 */
 void spKDArrayPrint2D(SPKDArray spkdArr);

#endif // SPKDRAAY_H_
