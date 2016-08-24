/*
 *  SPKDArray code file
 *  Author: Yotam
 */

#include "SPKDArray.h"
#include "SPPoint.h"
#include <stdlib.h>
#include <stdio.h>
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

int GetIndexInKDArray(SPKDArray kdArr, int i, int j) {
  assert(kdArr != NULL && i >= 0 && j >= 0);
  return kdArr->kdArray[i][j];
}

int GetDimention(SPKDArray kdArr) {
  assert(kdArr != NULL);
  return kdArr->dim;
}

int GetNumberOfPoints(SPKDArray kdArr) {
  assert(kdArr != NULL);
  return kdArr->size;
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
      return NULL;
    }
  }
  return cpy;
}

SPKDArray Init(SPPoint* arr, int size) {
  if (size <= 0) return NULL; // incorrect size param

  int i, j;

  // memory allocation
  SPKDArray spkdArr = (SPKDArray)malloc(sizeof(struct sp_kd_array));
  if (!spkdArr) return NULL; // exit on SPKDArray allocation error

  // dim and size
  spkdArr->dim = spPointGetDimension(arr[0]);
  spkdArr->size = size;

  spkdArr->points = (SPPoint*)calloc(spkdArr->size,sizeof(SPPoint));
  spkdArr->kdArray = (int**)calloc(spkdArr->dim,sizeof(int*));
  int* kdArrayV = (int*)calloc((spkdArr->dim)*spkdArr->size, sizeof(int));
  double** rowTmp = (double**)calloc(spkdArr->size, sizeof(double*)); // freed
  double* rowTmpV = (double*)calloc(spkdArr->size*2, sizeof(double)); // freed

  // if any allocation failed free all and return NULL
  if (!spkdArr->points || !spkdArr->kdArray || !kdArrayV || !rowTmp || !rowTmpV) {
    free(spkdArr->points);
    free(kdArrayV);
    free(spkdArr->kdArray);
    free(spkdArr);
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
      free(kdArrayV);
      free(spkdArr->kdArray);
      free(spkdArr);
      free(rowTmp);
      free(rowTmpV);
      return NULL;
    }
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

SPKDArray* Split(SPKDArray kdArr, int coor) {
  // check correct input
  if(!kdArr || coor < 0) return NULL;

  int i, j, k, l;
  int midP = (int)((kdArr->size)/2.0);
  // memory allocation
  // ---------------------
  SPKDArray* resArray = (SPKDArray*)calloc(2,sizeof(SPKDArray));
  if (!resArray) return NULL;
  //
  SPKDArray leftArr = (SPKDArray)malloc(sizeof(struct sp_kd_array));
  SPKDArray rightArr = (SPKDArray)malloc(sizeof(struct sp_kd_array));
  if (!leftArr || !rightArr) {
    free(leftArr);
    free(rightArr);
    free(resArray);
    return NULL;
  }
  leftArr->size = midP+1; // left
  leftArr->dim = kdArr->dim; // left
  rightArr->size = (kdArr->size)-(midP+1); // right
  rightArr->dim = kdArr->dim; // right
  leftArr->points = (SPPoint*)calloc(leftArr->size,sizeof(SPPoint)); // left
  leftArr->kdArray = (int**)calloc(leftArr->dim,sizeof(int*)); // left
  int* kdArrayVL = (int*)calloc((leftArr->size)*(leftArr->dim), sizeof(int)); // left
  rightArr->points = (SPPoint*)calloc(rightArr->size,sizeof(SPPoint)); // right
  rightArr->kdArray = (int**)calloc(rightArr->dim,sizeof(int*)); // right
  int* kdArrayVR = (int*)calloc((rightArr->size)*(rightArr->dim), sizeof(int)); // right
  // general allocation
  int* xArr = (int*)calloc(kdArr->size, sizeof(int)); // freed
  int* mapL = (int*)calloc(kdArr->size, sizeof(int)); // freed
  int* mapR = (int*)calloc(kdArr->size, sizeof(int)); // freed
  // copy the points from the kdArr
  SPPoint* pointTmp = GetPointsArray(kdArr); // freed

  // check that no allocation errors occured
  if (!leftArr->points || !leftArr->kdArray || !kdArrayVL || !rightArr->points || !rightArr->kdArray || !kdArrayVR || !xArr || !mapL || !mapR || !pointTmp) {
    free(leftArr->points);
    free(leftArr->kdArray);
    free(kdArrayVL);
    free(rightArr->points);
    free(rightArr->kdArray);
    free(kdArrayVR);
    free(leftArr);
    free(rightArr);
    free(resArray);
    free(xArr);
    free(mapL);
    free(mapR);
    free(pointTmp);
    return NULL;
  }

  // update X-Array
  for (i=0;i<kdArr->size;i++) {
    if (i <= midP) {
      xArr[(kdArr->kdArray)[coor][i]] = 0;
    }
    else {
      xArr[(kdArr->kdArray)[coor][i]] = 1;
    }
  }

  // update the maps and add point to correct list
  j=0;
  k=0;
  for (i=0;i<kdArr->size;i++) {
	  if (xArr[i] == 0) {
		  // left point
		  leftArr->points[j] = pointTmp[i];
		  mapL[i] = j;
		  mapR[i] = -1;
		  j++;
	  } else {
      // right point
      rightArr->points[k] = pointTmp[i];
      mapL[i] = -1;
      mapR[i] = k;
      k++;
	  }
  }

  // fix placement for kdArrays
  for(i=0;i<leftArr->dim;i++) {
	  leftArr->kdArray[i] = kdArrayVL + i*leftArr->size;
  }
  for(i=0;i<rightArr->dim;i++) {
	  rightArr->kdArray[i] = kdArrayVR + i*rightArr->size;
  }

  // recreate sorted matrix
  for (i=0;i<kdArr->dim;i++) {
    k = 0;
    l = 0;
    for (j=0;j<kdArr->size;j++) {
      if (xArr[(kdArr->kdArray)[i][j]] == 0) {
        // in leftArr
        leftArr->kdArray[i][k] = mapL[(kdArr->kdArray)[i][j]];
        k++;
      } else {
        // in rightArr
        (rightArr->kdArray)[i][l] = mapR[(kdArr->kdArray)[i][j]];
        l++;
      }
    }
  }

  // place results
  resArray[0] = leftArr;
  resArray[1] = rightArr;

  free(xArr);
  free(mapL);
  free(mapR);
  free(pointTmp);
  // return
  return resArray;
}

void spKDArrayDestroy(SPKDArray spkdArr){
  if (!spkdArr) {
    return;
  }
  for (int i=0;i<spkdArr->size;i++) spPointDestroy(spkdArr->points[i]);
  free(spkdArr->points);
  free(*(spkdArr->kdArray));
  free(spkdArr->kdArray);
  free(spkdArr);
  return;
}

void spKDArrayFullPrint(SPKDArray spkdArr) {
  char* m1 = "\nSPKDArray\n--------------\n";
  char* m2 = "- Points:\n";
  char* m3 = "- Size:\t";
  char* m4 = "- Dim:\t";
  char* m5 = "- Matrix:";

  printf("%s", m1);
  printf("%s%d\n", m3, spkdArr->size);
  printf("%s%d\n", m4, spkdArr->dim);
  printf("%s", m2);
  for (int i=0; i<spkdArr->size; i++) {
    printf("\t%d\t-\t(");
    for (int j=0;j<spkdArr->dim;j++) {
      printf("%.0f", i, spPointGetAxisCoor((spkdArr->points)[i],j));
      if (j < spkdArr->dim - 1) printf(", \n", );
    }
    printf(")\n", );
  }
  printf("%s", m5);
  for (int i=0;i<spkdArr->dim;i++) {
    printf("\n\t");
    for (int j=0;j<spkdArr->size; j++) {
      printf("%d\t", (spkdArr->kdArray)[i][j]);
    }
  }
  printf("\n\n");
  return;
}
