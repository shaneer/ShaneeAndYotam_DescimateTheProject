/*
    KDArray Header File
    Author: Yotam
*/

#ifndef KDARRAY_H_
#define KDARRAY_H_

typedef struct kd_array* KDArray;

/*
 *  Allocates the needed memory, copies the points
 *  and creates the KDArray from them, with the given size.
 *
 *  arr - the SPPoint array
 *  size - the amount of points in the array
 *
 *  @return
 *  NULL if error occured, otherwise returns the newly created KDArray.
 */
KDArray Init(SPPoint* arr, int size);

/*
 *  Splits the given SPKDArray based on a given coordinate.
 *
 *  kdArr - the SPKDArray to split
 *  coor - the coordinate to split by
 *
 *  @return
 *  NULL if error occured , otherwise returns two SPKDArray (kdLeft, kdRight).
 */

#endif // KDRAAY_H
