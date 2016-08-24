/*
 *  KDArray_unit_test test file for KDArray
 *  Author: Yotam
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "unit_test_util.h"
#include "../SPKDArray.h"
#include "../SPPoint.h"

const int DIM = 2;
const int SIZE = 5;

// TODO: Handle edge cases on all test functions.

SPPoint* GeneratePointArray() {
  SPPoint* arr = (SPPoint*)malloc(sizeof(SPPoint)*SIZE);
  double* data = (double*)malloc(DIM*sizeof(double));
  data[0] = 1;
  data[1] = 2;
  arr[0] = spPointCreate(data, DIM, 0);
  data[0] = 123;
  data[1] = 70;
  arr[1] = spPointCreate(data,DIM,1);
  data[0] = 2;
  data[1] = 7;
  arr[2] = spPointCreate(data,DIM,2);
  data[0] = 9;
  data[1] = 11;
  arr[3] = spPointCreate(data,DIM,3);
  data[0] = 3;
  data[1] = 4;
  arr[4] = spPointCreate(data,DIM,4);
  free(data);
  return arr;
}

int SPKDArrayGetPointsArrayTest(SPKDArray kdArr, SPPoint* arr, int size) {
    SPPoint* arr_cpy = spKDArrayGetPointsArray(kdArr);
    for (int i=0;i<size;i++) {
      ASSERT_TRUE(spPointL2SquaredDistance(arr[i],arr_cpy[i]) == 0);
      spPointDestroy(arr_cpy[i]);
    }
    ASSERT_FALSE(spKDArrayGetPointsArray(NULL));
    free(arr_cpy);
    return true;
}

int SPKDArrayGetIndexInKDArrayTest(SPKDArray kdArr, int* x_ind, int* y_ind, int size) {
  for (int i=0;i<size;i++) {
    ASSERT_TRUE(spKDArrayGetIndexInArray(kdArr, 0,i) == x_ind[i]);
    ASSERT_TRUE(spKDArrayGetIndexInArray(kdArr, 1,i) == y_ind[i]);
  }
  return true;
}

int SPKDArrayGetDimentionTest(SPKDArray kdArr, int dim) {
  ASSERT_TRUE(spKDArrayGetDimention(kdArr) == dim);
  return true;
}

int SPDKArrayGetNumberOfPointsTest(SPKDArray kdArr, int num) {
  ASSERT_TRUE(spKDArrayGetNumberOfPoints(kdArr) == num);
  return true;
}

int SPKDArrayInitTest() {
  SPKDArray kdArr;
  SPPoint* arr = GeneratePointArray();
  kdArr = Init(arr, SIZE);
  ASSERT_TRUE(kdArr != NULL);
  ASSERT_TRUE(SPKDArrayGetPointsArrayTest(kdArr, arr,SIZE));
  int* x_ind = (int*)malloc(SIZE*sizeof(int));
  x_ind[0] = 0;
  x_ind[1] = 2;
  x_ind[2] = 4;
  x_ind[3] = 3;
  x_ind[4] = 1;
  int* y_ind = (int*)malloc(SIZE*sizeof(int));
  y_ind[0] = 0;
  y_ind[1] = 4;
  y_ind[2] = 2;
  y_ind[3] = 3;
  y_ind[4] = 1;
  ASSERT_TRUE(SPKDArrayGetIndexInKDArrayTest(kdArr,x_ind,y_ind, 5));
  ASSERT_TRUE(SPKDArrayGetDimentionTest(kdArr, DIM));
  ASSERT_TRUE(SPDKArrayGetNumberOfPointsTest(kdArr,SIZE));

  for (int i=0;i<SIZE;i++) spPointDestroy(arr[i]);
  free(arr);
  free(x_ind);
  free(y_ind);
  spKDArrayDestroy(kdArr);
  return true;
}

int SPKDArraySplitTest() {
  SPKDArray kdArr;
  SPPoint* arr = GeneratePointArray();
  kdArr = Init(arr, SIZE);

  SPKDArray* kdarr_split = Split(kdArr, 0);
  ASSERT_TRUE(kdarr_split != NULL);
  SPKDArray kdLeft = kdarr_split[0];
  SPKDArray kdRight = kdarr_split[1];
  free(kdarr_split);
  ASSERT_TRUE(kdLeft != NULL);
  ASSERT_TRUE(kdRight != NULL);

  int* x_ind, *y_ind;
  // Test leftArr
  SPPoint* arrL = (SPPoint*)malloc(3*sizeof(SPPoint));
  arrL[0] = arr[0];
  arrL[1] = arr[2];
  arrL[2] = arr[4];
  ASSERT_TRUE(SPKDArrayGetPointsArrayTest(kdLeft, arrL,3));
  x_ind = (int*)malloc(3*sizeof(int));
  x_ind[0] = 0;
  x_ind[1] = 1;
  x_ind[2] = 2;
  y_ind = (int*)malloc(3*sizeof(int));
  y_ind[0] = 0;
  y_ind[1] = 2;
  y_ind[2] = 1;
  ASSERT_TRUE(SPKDArrayGetIndexInKDArrayTest(kdLeft,x_ind,y_ind, 3));
  ASSERT_TRUE(SPKDArrayGetDimentionTest(kdLeft, DIM));
  ASSERT_TRUE(SPDKArrayGetNumberOfPointsTest(kdLeft,3));
  free(x_ind);
  free(y_ind);
  free(arrL);
  spKDArrayDestroy(kdLeft);

  // Test rightArr
  SPPoint* arrR = (SPPoint*)malloc(2*sizeof(SPPoint));
  arrR[0] = arr[1];
  arrR[1] = arr[3];
  ASSERT_TRUE(SPKDArrayGetPointsArrayTest(kdRight, arrR,2));
  x_ind = (int*)malloc(sizeof(int)*2);
  x_ind[0] = 1;
  x_ind[1] = 0;
  y_ind = (int*)malloc(sizeof(int)*2);
  y_ind[0] = 1;
  y_ind[1] = 0;
  ASSERT_TRUE(SPKDArrayGetIndexInKDArrayTest(kdRight,x_ind,y_ind, 2));
  ASSERT_TRUE(SPKDArrayGetDimentionTest(kdRight, DIM));
  ASSERT_TRUE(SPDKArrayGetNumberOfPointsTest(kdRight,2));
  free(x_ind);
  free(y_ind);
  free(arrR);
  spKDArrayDestroy(kdRight);

  for (int i=0;i<SIZE;i++) spPointDestroy(arr[i]);
  free(arr);
  spKDArrayDestroy(kdArr);
  return true;
}

int main() {
  RUN_TEST(SPKDArrayInitTest);
  RUN_TEST(SPKDArraySplitTest);

  return 0;
}
