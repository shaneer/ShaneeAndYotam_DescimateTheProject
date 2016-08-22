/*
 *  KDArray_unit_test test file for KDArray
 *  Author: Yotam
 */

#include <stdio.h>
#include <stdlib.h>
#include "unit_test_util.h"
#include "../SPKDArray.h"
#include "../SPPoint.h"

const int DIM = 2;

// TODO: Handle edge cases on all test functions.

SPPoint* GeneratePointArray() {
  SPPoint* arr = (SPPoint*)malloc(sizeof(SPPoint)*5);
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

int SPKDArrayGetPointsArrayTest(SPKDArray kdArr, SPPoint* arr, int mx) {
    SPPoint* arr_cpy = GetPointsArray(kdArr);
    for (int i=0;i<mx;i++) {
      ASSERT_TRUE(spPointL2SquaredDistance(arr[i],arr_cpy[i]));
    }
    ASSERT_FALSE(GetPointsArray(NULL));
    return 0;
}

int SPKDArrayGetIndexInKDArrayTest(SPKDArray kdArr, int* x_ind, int* y_ind) {
  for (int i=0;i<5;i++) {
    ASSERT_TRUE(GetIndexInKDArray(kdArr, 0,i) == x_ind[i]);
    ASSERT_TRUE(GetIndexInKDArray(kdArr, 1,i) == y_ind[i]);
  }
  return 0;
}

int SPKDArrayGetDimentionTest(SPKDArray kdArr, int dim) {
  ASSERT_TRUE(GetDimention(kdArr) == dim);
  return 0;
}

int SPDKArrayGetNumberOfPointsTest(SPKDArray kdArr, int num) {
  ASSERT_TRUE(GetNumberOfPoints(kdArr) == num);
  return 0;
}

int SPKDArrayInitTest() {
  SPKDArray kdArr;
  SPPoint* arr = GeneratePointArray();
  kdArr = Init(arr, 5);
  ASSERT_TRUE(kdArr != NULL);
  ASSERT_TRUE(SPKDArrayGetPointsArrayTest(kdArr, arr,5) == 0);
  int* x_ind = {0,2,4,3,1};
  int* y_ind = {0,4,2,3,1};
  ASSERT_TRUE(SPKDArrayGetIndexInKDArrayTest(kdArr,x_ind,y_ind) == 0);
  ASSERT_TRUE(SPKDArrayGetDimentionTest(kdArr, 2) == 0);
  ASSERT_TRUE(SPDKArrayGetNumberOfPointsTest(kdArr,5) == 0);

  for (int i=0;i<5;i++) spPointDestroy(arr[i]);
  free(arr);
  spKDArrayDestroy(kdArr);
  return 0;
}

int SPKDArraySplitTest() {
  SPKDArray kdArr;
  SPPoint* arr = GeneratePointArray();
  kdArr = Init(arr, 5);

  SPKDArray* kdarr_split = Split(kdArr, 0);
  ASSERT_TRUE(kdarr_split != NULL);
  SPKDArray kdLeft = kdarr_split[0];
  SPKDArray kdRight = kdarr_split[1];
  ASSERT_TRUE(kdLeft != NULL);
  ASSERT_TRUE(kdRight != NULL);

  int* x_ind, y_ind;
  // Test leftArr
  SPPoint* arrL = (SPPoint*)malloc(3*sizeof(SPPoint));
  arrL[0] = arr[0];
  arrL[1] = arr[2];
  arrL[2] = arr[4];
  ASSERT_TRUE(SPKDArrayGetPointsArrayTest(kdLeft, arrL,3) == 0);
  x_ind = {0,1,2};
  y_ind = {0,2,1};
  ASSERT_TRUE(SPKDArrayGetIndexInKDArrayTest(kdLeft,x_ind,y_ind) == 0);
  ASSERT_TRUE(SPKDArrayGetDimentionTest(kdLeft, 2) == 0);
  ASSERT_TRUE(SPDKArrayGetNumberOfPointsTest(kdLeft,3) == 0);
  free(arrL);

  // Test rightArr
  SPPoint* arrR = (SPPoint*)malloc(2*sizeof(SPPoint));
  arrR[0] = arr[1];
  arrR[1] = arr[3];
  ASSERT_TRUE(SPKDArrayGetPointsArrayTest(kdRight, arrR,2) == 0);
  x_ind = {1,0};
  y_ind = {1,0};
  ASSERT_TRUE(SPKDArrayGetIndexInKDArrayTest(kdRight,x_ind,y_ind) == 0);
  ASSERT_TRUE(SPKDArrayGetDimentionTest(kdRight, 2) == 0);
  ASSERT_TRUE(SPDKArrayGetNumberOfPointsTest(kdRight,2) == 0);
  free(arrR);

  for (int i=0;i<5;i++) spPointDestroy(arr[i]);
  free(arr);
  spKDArrayDestroy(kdArr);
  return 0;
}

int main(int argc, char const *argv[]) {
  RUN_TEST(SPKDArrayInitTest);
  RUN_TEST(SPKDArraySplitTest);

  return 0;
}
