/*
 *  KDArray_unit_test test file for Config
 *  Author: Shanee
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "unit_test_util.h"
#include "SPPoint.h"
#include "SPKDTreeNNSearch.h"
#include "SPConfig.h"
#include "SPKDArray.h"
#include "SPBPQueue.h"


int main() {
  SPKDTreeNode root;
  SP_NNSEARCH_MSG success = SP_KDTREE_SUCCESS;

  double A[2] = {1,2};
  double B[2] = {123,70};
  double C[2] = {2,7};
  double D[2] = {9,11};
  double E[2] = {3,4};
  SPPoint a = spPointCreate(A, 2, -1);
  SPPoint b = spPointCreate(B, 2, -1);
  SPPoint c = spPointCreate(C, 2, -1);
  SPPoint d =spPointCreate(D, 2, -1);
  SPPoint e = spPointCreate(E, 2, -1);

  SPPoint pnts[5] = {a,b,c,d,e};
  SPKDArray arr = Init(pnts, 5);

  SP_NNSEARCH_MSG* message;
  SP_SPLIT_METHOD method = INCREMENTAL;
  root = spCreateKDTree(arr, method, message);


  if(*message == success){
    printf("Success buidling tree!");
  }
  return 0;
}
