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
 SPKDArray* Split(SPKDArray kdArr, int coor);

/*
 *  Getter for the points array.
 */
 SPPoint* GetPointsArray(SPKDArray kdArr);

/*
 *  Getter for the image-index at kdArr[i,j].
 */
 int GetIndexInKDArray(SPKDArray kdArr, int i, int j);

/*
 *  Getter for the dimention of the SPKDArray.
 */
 int GetDimention(SPKDArray kdArr);

/*
 *  Getter for the amount of points in the SPKDArray.
 */
 int GetNumberOfPoints(SPKDArray kdArr);

/*
 *  Destroy the SPKDArray given and clear it's memory usage.
 */
 int spKDArrayDestroy(SPKDArray kdArr);

#endif // SPKDRAAY_H_
