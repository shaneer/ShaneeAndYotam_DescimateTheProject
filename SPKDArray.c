/*
 *  SPKDArray code file
 *  Author: Yotam
 */

#include "SPKDArray.h"
#include "SPPoint.h"
#include <stdlib.h>
#include <assert.h>

struct sp_kd_array {
  SPPoint* points; // the copied points array
  int** kdArray; // the 2-dimentional matrix of indexes
  int dim; // the dimention of all points
  int size; // the number for points int array
};

SPKDArray Init(SPPoint* arr, int size) {
  if (size <= 0) return NULL; // incorrect size param

  int i, j;
  SPKDArray spkdArr;
  spkdArr = (SPKDArray) malloc(sizeof(SPKDArray*));
  if (spkdArr == NULL) return NULL;

  // copy the points
  spkdArr->points = (SPPoint*)malloc(sizeof(arr));
  for (i=0;i<size;i++) {
    spkdArr->points[i] = spPointCopy(arr[i]);
  }
  spkdArr->dim = spPointGetDimension(arr[0]);
  spkdArr->size = size;

  // initialize the kdArray matrix
  spkdArr->kdArray = (int**)malloc((spkdArr->dim)*sizeof(int*));
  for (i=0;i<spkdArr->dim;i++) {
    spkdArr->kdArray[i] = (int*)malloc(size*sizeof(int));
    for (j=0;j<size;j++) {
      spkdArr->kdArray[i][j] = j;
    }
  }

}
