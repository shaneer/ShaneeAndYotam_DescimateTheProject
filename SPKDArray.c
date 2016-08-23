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

int compareCoorValues(const void* a, const void* b) {
    const double* pa = *(const double**)a;
    const double* pb = *(const double**)b;
    return (int)(pa[1]-pb[1]);
}

SPKDArray Init(SPPoint* arr, int size) {
  if (size <= 0) return NULL; // incorrect size param

  int i, j;
  SPKDArray spkdArr = (SPKDArray)malloc(sizeof(SPKDArray*));
  spkdArr->points = (SPPoint*)malloc(sizeof(arr));
  spkdArr->kdArray = (int**)malloc((spkdArr->dim)*sizeof(int*));
  int* kdArrayV = (int*)malloc((spkdArr->dim)*size*sizeof(int));
  double** rowTmp = (double**)malloc(size*sizeof(double*)); // freed
  double* rowTmpV = (double*)malloc(size*2*sizeof(double)); // freed

  // if any allocation failed free all and return NULL
  if (!spkdArr || !spkdArr->points || !spkdArr->kdArray || !kdArrayV || !rowTmp || !rowTmpV) {
    free(spkdArr->points);
    free(spkdArr->kdArray);
    free(spkdArr);
    free(kdArrayV);
    free(rowTmp);
    free(rowTmpV);
    return NULL;
  }

  // copy the points
  for (i=0;i<size;i++) {
    spkdArr->points[i] = spPointCopy(arr[i]);
    // check for failure
    if (!spkdArr->points[i]) {
      for (j=0;j<i;j++) {
        spPointDestroy(spkdArr->points[j]);
      }
      free(spkdArr->points);
      free(spkdArr->kdArray);
      free(spkdArr);
      free(kdArrayV);
      free(rowTmp);
      free(rowTmpV);
      return NULL;
    }
  }
  // dim and size
  spkdArr->dim = spPointGetDimension(arr[0]);
  spkdArr->size = size;

  // fix temp row pointer placement
  for (j=0;j<size;j++) {
    rowTmp[j] = rowTmpV + 2*j;
  }

  // initialize the kdArray matrix
  for (i=0;i<spkdArr->dim;i++) {
    // calc sorted indexes by coor
    for (j=0;j<size;j++) {
      rowTmp[j][0] = (double)j;
      rowTmp[j][1] = spPointGetAxisCoor(arr[j], i);
    }
    qsort(rowTmp, size, sizeof(double*), compareCoorValues);
    // compile row
    spkdArr->kdArray[i] = kdArrayV + i*size;
    for (j=0;j<size;j++) {
      spkdArr->kdArray[i][j] = (int)rowTmp[j][0];
    }
  }

  // free the temporary row
  free(rowTmpV);
  free(rowTmp);
  return spkdArr;
}

SPPoint* GetPointsArray(SPKDArray kdArr) {
  SPPoint* cpy = (SPPoint*)malloc(sizeof(kdArr->points));
  for (int i=0; i<kdArr->size; i++) {
    cpy[i] = spPointCopy(kdArr->points[i]);
  }
  return cpy;
}

int GetIndexInKDArray(SPKDArray kdArr, int i, int j) {
  return kdArr->kdArray[i][j];
}

int GetDimention(SPKDArray kdArr) {
  return kdArr->dim;
}

int GetNumberOfPoints(SPKDArray kdArr) {
  return kdArr->size;
}

int spKDArrayDestroy(SPKDArray spkdArr){
  for (int i=0;i<spkdArr->size;i++) spPointDestroy(spkdArr->points[i]);
  free(spkdArr->points);
  free(*spkdArr->kdArray);
  free(spkdArr->kdArray);
  free(spkdArr);
  return 0;
}

SPKDArray* Split(SPKDArray kdArr, int coor) {
  return NULL;
}
