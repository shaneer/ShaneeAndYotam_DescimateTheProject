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
  char* center = "\t\t\t\t\t";
  char* tabs5 = "\t\t\t";
  char* tabs3 = "\t";
  char* tabs1 = " ";

  double A[2] = {1,2};
  double B[2] = {123,70};
  double C[2] = {2,7};
  double D[2] = {9,11};
  double E[2] = {3,4};
  double QUERY[2] = {120,60};
  SPPoint a = spPointCreate(A, 2, 0);
  SPPoint b = spPointCreate(B, 2, 1);
  SPPoint c = spPointCreate(C, 2, 2);
  SPPoint d =spPointCreate(D, 2, 3);
  SPPoint e = spPointCreate(E, 2, 4);
  SPPoint queryPoint = spPointCreate(QUERY, 2, 5);
  SPPoint data;
  SPBPQueue q;
  SPListElement elem;
  SPKDTreeNode tempLeft;
  SPKDTreeNode tempRight;
  bool printTree=0;

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

  /*
  char* bar = "---------------";
  if (printTree){
      tempLeft = root;
      tempRight = SPKDTreeNodeGetRight(root);
      printf("\n%sRoot Node:", center);
      printf("\n %s%s" ,center, bar);
      printf("\n%s\t Dim: %d", center, SPKDTreeNodeGetDim(tempLeft));
      printf("\n%s\t Val: %d", center, SPKDTreeNodeGetVal(tempLeft));
      if (SPKDTreeNodeIsLeaf(root)){
        printf("Data Point:");
        SPPoint data;
        data = SPKDTreeNodeGetDataPoint(tempLeft);
        printf("\n%s\t Index: %d", center, spPointGetIndex(data));
      }
      printf("\n%s%s\n" ,center, bar);

      tempLeft = SPKDTreeNodeGetLeft(tempLeft);

      printf("\n %s%s%s%s" ,tabs5,bar,tabs5,bar);
      printf("\n%s\t Dim: %d %s Dim: %d", tabs5, SPKDTreeNodeGetDim(tempLeft), tabs5, SPKDTreeNodeGetDim(tempRight));
      printf("\n%s\t Val: %d %s Val: %d", tabs5, SPKDTreeNodeGetVal(tempLeft), tabs5, SPKDTreeNodeGetVal(tempRight));
      printf("\n%s%s%s%s\n" ,tabs5, bar, tabs5, bar);

      tempRight = SPKDTreeNodeGetRight(tempLeft);
      tempLeft = SPKDTreeNodeGetLeft(tempLeft);

      printf("\n %s%s%s%s" ,tabs3,bar,tabs3,bar);
      printf("\n%s\t Dim: %d %s Dim: %d", tabs3, SPKDTreeNodeGetDim(tempLeft), tabs3, SPKDTreeNodeGetDim(tempRight));
      printf("\n%s\t Val: %d %s Val: %d", tabs3, SPKDTreeNodeGetVal(tempLeft), tabs3, SPKDTreeNodeGetVal(tempRight));
      printf("\n\t%s%s\tData Point:", bar, tabs1);
      data = SPKDTreeNodeGetDataPoint(tempRight);
      printf("\n%s%s Index: %d", tabs5,tabs3, spPointGetIndex(data));
      printf("\n\t %s%s%s" ,tabs3,tabs3,bar);


      tempLeft = SPKDTreeNodeGetLeft(tempLeft);
      printf("\n %s%s" ,tabs1,bar);
      printf("\n%s\t Dim: %d", tabs1, SPKDTreeNodeGetDim(tempLeft));
      printf("\n%s\t Val: %d", tabs1, SPKDTreeNodeGetVal(tempLeft));
    //  if (SPKDTreeNodeIsLeaf(root)){
        printf("\nData Point:");
        data = SPKDTreeNodeGetDataPoint(tempLeft);
        printf("\n%s\t Index: %d", tabs1, spPointGetIndex(data));
    //  }
      printf("\n%s%s\n" ,tabs1, bar);

  }
  */

  //CHECK SEARCH
  q = SPSearchForNeighbors(queryPoint, root, 5, &message);

  //SPKDTreePrintTree(root);

  //elem = spListElementCopy(spBPQueuePeek(q));
  //printf("\nDeque smallest: point with index: %d\n",  spListElementGetIndex(elem));
  //spBPQueueDequeue(q);
  //elem = spListElementCopy(spBPQueuePeek(q));
  //printf("\nDeque smallest: point with index: %d\n", spListElementGetIndex(elem));
  //spBPQueueDequeue(q);
  //elem = spListElementCopy(spBPQueuePeek(q));
  //printf("\nDeque smallest: point with index: %d\n", spListElementGetIndex(elem));
  //spBPQueueDequeue(q);

  spBPQueueDestroy(q);
  spPointDestroy(pnts[0]);
  spPointDestroy(pnts[1]);
  spPointDestroy(pnts[2]);
  spPointDestroy(pnts[3]);
  spPointDestroy(pnts[4]);
  free(pnts);
  //spPointDestroy(queryPoint);   //DESTROYED IN FUNCTION!
  spKDTreeNodeDestroy(root);    //Destroyed in Function
  puts("Finished");
  return 0;
}
