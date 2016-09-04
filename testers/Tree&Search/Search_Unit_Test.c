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
#include "SPList.h"
#include "SPListElement.h"


int main() {
  SPKDTreeNode root;
  SP_NNSEARCH_MSG success = SP_KDTREE_SUCCESS;

  double A[2] = {1,2};
  double B[2] = {123,70};
  double C[2] = {2,7};
  double D[2] = {9,11};
  double E[2] = {3,4};
  SPPoint a = spPointCreate(A, 2, 0);
  SPPoint b = spPointCreate(B, 2, 1);
  SPPoint c = spPointCreate(C, 2, 2);
  SPPoint d =spPointCreate(D, 2, 3);
  SPPoint e = spPointCreate(E, 2, 4);

  SPPoint* pnts;
  pnts = (SPPoint*) malloc(sizeof(*pnts) * 5);
  pnts[0] = a;
  pnts[1] = b;
  pnts[2] = c;
  pnts[3] = d;
  pnts[4] = e;

  SPKDArray arr = Init(pnts, 5);
  SP_NNSEARCH_MSG message = SP_NODE_SUCCESS;

  root = spCreateKDTree(arr, INCREMENTAL, &message);
  if(message == success){
    printf("Success buidling tree!\n");
  }
  char* bar = "---------------";

  printf("\n   Root Node:");
  printf("\n %s" ,bar);
  printf("\n\t Dim: %d", SPKDTreeNodeGetDim(root));
  printf("\n\t Val: %d", SPKDTreeNodeGetVal(root));
  if (SPKDTreeNodeIsLeaf(root)){
    printf("Data Point:");
    SPPoint data;
    data = SPKDTreeNodeGetDataPoint(root);
    printf("\n\t Index: %d", spPointGetIndex(data));
  }
  printf("\n%s\n" ,bar);

  char* filepath = "./testOut.txt";
  FILE* fp = fopen(filepath, "w");
  SPKDTreePrintTree(root, fp, 0, "\t\t\t\t\t\t\t\t\t\t\t\t\t");
  spPointDestroy(pnts[0]);
  spPointDestroy(pnts[1]);
  spPointDestroy(pnts[2]);
  spPointDestroy(pnts[3]);
  spPointDestroy(pnts[4]);
  free(pnts);
  return 0;
}
