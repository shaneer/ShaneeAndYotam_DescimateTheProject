/*
 *  SPKDArray code file
 *  Author: Yotam
 */

#include "SPKDArray.h"
#include "SPPoint.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
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
  bool failed = false;

  // memory allocation
  // ---------------------
  SPKDArray spkdArr = (SPKDArray)malloc(sizeof(struct sp_kd_array));

  // dim and size
  if (spkdArr) {
    spkdArr->dim = spPointGetDimension(arr[0]);
    spkdArr->size = size;

    spkdArr->points = (SPPoint*)calloc(spkdArr->size,sizeof(SPPoint));
    spkdArr->kdArray = (int**)calloc(spkdArr->dim,sizeof(int*));
    int* kdArrayV = (int*)calloc((spkdArr->dim)*(spkdArr->size), sizeof(int));
    double** rowTmp = (double**)calloc(spkdArr->size, sizeof(double*)); // freed
    double* rowTmpV = (double*)calloc(spkdArr->size*2, sizeof(double)); // freed
  }

  // copy the points
  if (spkdArr->points) {
    for (i=0;!failed && i<pkdArr->dim;i++) {
      spkdArr->points[i] = spPointCopy(arr[i]);
      // check for failure
      if (!spkdArr->points[i]) {
        for (j=0;j<i;j++) {
          spPointDestroy(spkdArr->points[j]);
        }
        failed = true;
      }
    }
  }

  // if any allocation failed free all and return NULL
  if (spkdArr || !spkdArr->points || !spkdArr->kdArray || !kdArrayV || !rowTmp || !rowTmpV || failed) {
    free(spkdArr->points);
    free(kdArrayV);
    free(spkdArr->kdArray);
    free(spkdArr);
    free(rowTmp);
    free(rowTmpV);
    return NULL;
  }

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

  // return
  return spkdArr;
}

SPPoint* GetPointsArray(SPKDArray spkdArr) {
  if (!spkdArr) return NULL;
  SPPoint* cpy = (SPPoint*)calloc(spkdArr->size, sizeof(SPPoint));
  for (int i=0; i<spkdArr->size; i++) {
    cpy[i] = spPointCopy((spkdArr->points)[i]);
    // check for failure
    if (!cpy[i]) {
      for (int j=0;j<i;j++) {
        spPointDestroy(cpy[j]);
      }
      free(cpy);
      retunr NULL;
    }
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
  free(*(spkdArr->kdArray));
  free(spkdArr->kdArray);
  free(spkdArr);
  return 0;
}

SPKDArray* Split(SPKDArray kdArr, int coor) {
  // check correct input
  if(!kdArr || coor < 0) return NULL;

  int i, j, k, l;
  int midP = (int)floor((kdArr->size)/2.0);
  // memory allocation
  // ---------------------
  int* xArr = (int*)calloc(kdArr->size, sizeof(int));
  int* mapL = (int*)calloc(kdArr->size, sizeof(int));
  int* mapR = (int*)calloc(kdArr->size, sizeof(int));
  SPKDArray* resArray = (SPKDArray*)calloc(2,sizeof(SPKDArray));
  // left array
  SPKDArray leftArr = (SPKDArray)malloc(sizeof(struct sp_kd_array));
  leftArr->size = midP+1;
  leftArr->dim = kdArr->dim;
  leftArr->points = (SPPoint*)calloc(leftArr->size,sizeof(SPPoint));
  leftArr->kdArray = (int**)calloc(leftArr->dim,sizeof(int*));
  int* kdArrayVL = (int*)calloc((leftArr->size)*(leftArr->dim), sizeof(int))
  // right array
  SPKDArray rightArr = (SPKDArray)malloc(sizeof(struct sp_kd_array));
  rightArr->size = kdArr->size-(midP+1)
  rightArr->dim = kdArr->dim;
  rightArr->points = (SPPoint*)calloc(rightArr->size,sizeof(SPPoint));
  rightArr->kdArray = (int**)calloc(rightArr->dim,sizeof(int*));
  int* kdArrayVR = (int*)calloc((rightArr->size)*(rightArr->dim), sizeof(int))

  // use points copy temp
  SPPoint* pointTmp = GetPointsArray(kdArr);

  // exit on allocation error
  if (!resArray || !pointTmp || !xArr || !mapL || !mapR || !leftArr || !leftArr->points || !leftArr->kdArray || !kdArrayVL || !rightArr || !rightArr->points || !rightArr->kdArray || !kdArrayVR) {
    free(xArr);
    free(mapL);
    free(mapR);
    free(leftArr->points);
    free(kdArrayVL);
    free(leftArr->kdArray);
    free(leftArr);
    free(rightArr->points);
    free(kdArrayVR);
    free(rightArr->kdArray);
    free(rightArr);
    return NULL;
  }

  j=0;
  k=0;
  for (i=0;i<kdArr->size;i++) {
    if (i <= midP)
      xArr[(kdArr->kdArray)[coor][i]] = 0;
      leftArr->points[j] = pointTmp[i];
      mapL[i] = j;
      mapR[i] = -1;
      j++
    else
      xArr[(kdArr->kdArray)[coor][i]] = 1;
      rightArr->points[k] = pointTmp[i];
      mapL[i] = -1;
      mapR[i] = k;
      k++
  }

  for (i=0;i<kdArr->dim;i++) {
    k = 0;
    l = 0;
    for (j=0;j<kdArr->size;j++) {
      if (xArr[(kdArr->kdArray)[i][j]] == 0) {
        // in leftArr
        (leftArr->kdArray)[i][k] = mapL[(kdArr->kdArray)[i][j]];
        k++;
      } else {
        // in rightArr
        (rightArr->kdArray)[i][l] = mapR[(kdArr->kdArray)[i][j]];
        l++;
      }
    }
  }
  resArray[0] = leftArr;
  resArray[1] = rightArr;

  // return
  return resArray;
}

void spKDArrayPrint2D(SPKDArray spkdArr) {
  char* m1 = "SPKDArray\n--------------\n";
  char* m2 = "- Points:\n";
  char* m3 = "- Size:\t";
  char* m4 = "- Dim:\t";
  char* m5 = "- Matrix:";

  printf("%s", m1);
  printf("%s%d\n", m3, spkdArr->size);
  printf("%s%d\n", m4, spkdArr->dim);
  printf("%s", m2);
  for (int i=0; i<spkdArr->size; i++) {
    printf("\t%d\t-\t(%.0f, %.0f)\n", i, spPointGetAxisCoor((spkdArr->points)[i],0),
        spPointGetAxisCoor((spkdArr->points)[i],1));
  }
  printf("%s", m5);
  for (int i=0;i<spkdArr->dim;i++) {
    printf("\n\t");
    for (int j=0;j<spkdArr->size; j++) {
      printf("%d\t", (spkdArr->kdArray)[i][j]);
    }
  }
  printf("\n");
  return;
}
