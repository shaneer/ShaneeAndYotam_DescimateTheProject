/*
    KDTree Source File
    Author: Shanee
*/
#include <stdbool.h>
#include <stdio.h>
#include<stdlib.h>
#include <time.h>
#include <limits.h>
#include <math.h>
#include <assert.h>
#include "SPPoint.h"
#include "SPKDTreeNNSearch.h"
#include "SPConfig.h"
#include "SPKDArray.h"
#include "SPBPQueue.h"
#include "SPListElement.h"
#include "SPList.h"

//TODO
struct sp_kd_node_t{
  int dim;
  int val;
  SPKDTreeNode left;
  SPKDTreeNode right;
  SPPoint data;
};

//CONSTANTS
const int INVALID_NUM  = -1;

//GETTERS FOR TREE NODE TYPE
/**
 * A getter for the left/right subtree of the target node.
 * @param data The target node
 * @assert(node isn't NULL)
 *
 * NULL if node==NULL
 * otherwise,
 * @return : pointer to the root node of the left/right subtree
 */
SPKDTreeNode SPKDTreeNodeGetLeft(SPKDTreeNode root){
  assert(root != NULL);
  if (root->left == NULL){
    return NULL;
  }
  return (root->left);
}
SPKDTreeNode SPKDTreeNodeGetRight(SPKDTreeNode root){
  assert(root != NULL);
  if (root->right == NULL){
    return NULL;
  }
  return (root->right);
}

/**
 * A getter for the value of the target node.
 * Meaning - the value which is the median value at the dim used to split it,
 * such that its left sub-tree will contain all those nodes with smaller values, and right larger
 * @param data The target node
 * @assert(node isn't NULL)
 *
 * NULL if node==NULL
 * otherwise,
 * @return : the value by which subtrees are split
 */
int SPKDTreeNodeGetVal(SPKDTreeNode root){
  assert(root != NULL);
  return root->val;
}
int SPKDTreeNodeGetDim(SPKDTreeNode root){
  assert(root != NULL);
  return root->dim;
}

//TODO
bool SPKDTreeNodeIsLeaf(SPKDTreeNode check){
  assert(check != NULL);
  if (check->data != NULL){
    return true;
  }
  return false;
}

SPPoint SPKDTreeNodeGetDataPoint(SPKDTreeNode root){
  assert(root != NULL);
  return root->data;
}

//TODO
void spKDTreeNodeDestroy(SPKDTreeNode node){
  assert(node != NULL);
  spKDTreeNodeDestroy(SPKDTreeNodeGetLeft(node));
  spKDTreeNodeDestroy(SPKDTreeNodeGetRight(node));
  spPointDestroy(SPKDTreeNodeGetDataPoint(node));
  free(node);
}

//TODO
SPKDTreeNode spKDTreeNodeCreate(int dimension, int value, SPKDTreeNode leftNode,
  SPKDTreeNode rightNode, SPPoint data, SP_NNSEARCH_MSG* msg){

  SPKDTreeNode node = (SPKDTreeNode) malloc(sizeof(*node));
  if (node==NULL){
    *msg = SP_NODE_ALLOC_FAIL;
    return NULL;
  }//else All memory allocated succesfully since no other struct members require an allocation

  node->dim = dimension;
  node->val = value;
  node->left = leftNode;
  node->right = rightNode;
  node->data = data;      //If data == NULL then this field is set to NULL
                          // and node is NOT a leaf
  *msg = SP_NODE_SUCCESS;
  return node;
}


SPKDTreeNode spKDBuildTree(int prevDim, SPKDArray arr, SP_SPLIT_METHOD method, int d, SP_NNSEARCH_MSG* msg){
  assert (arr != NULL);//TODO
  SPKDTreeNode node= NULL;
  SPKDTreeNode tempLeft=NULL;
  SPKDTreeNode tempRight=NULL;
  int tempDim;                             //Both ints initialized to -1
  int tempMedianValue = INVALID_NUM;
  SPPoint tempData = NULL;
  SPKDArray *arrayOfKDArrays;                       //For use in the split method
  //int medianIndex = (spKDArrayGetSize(arr)-1)/2;       // Find ciel(n/2) index
  srand((unsigned int)time(NULL));                  //TODO Does this need to be closed?

  //If size of our array is 1 we have reached a leaf: create and return a node with value of point in array
  if (spKDArrayGetSize(arr)==1){
    tempData = spPointCopy(spKDArrayGetPoint(arr, 0));

    if (!tempData){
      *msg  = SP_KDTREE_GETPOINT_FAIL;
      spKDTreeNodeDestroy(node);
      spKDArrayDestroy(arr);
      return NULL;
    }
    node = spKDTreeNodeCreate(INVALID_NUM, tempMedianValue, tempLeft, tempRight, tempData, msg);

    spKDArrayDestroy(arr);
    //Check if creation succeeded:
    if (!node){
      return NULL;
    }//else return node created after having destroyed array used to create it and temps used
      printf("\n Returning Leaf Node %d \n", spPointGetIndex(SPKDTreeNodeGetDataPoint(node))); //TODO REMOVE PRINT
    return node;
  }
  else{             //Node is not a leaf
      switch(method) {
        case MAX_SPREAD:
            tempDim = spKDArrayFindMaxSpreadDim(arr); //TODO Check w/Yotam
            break;
        case INCREMENTAL:
            tempDim = (prevDim+1)%(d);
            break;
        case RANDOM:
            tempDim = rand()%29+1;
            break;
        case INVALID:
            break;
    }
  }
    //We now split the array according to the SPREAD defined, with the median value found
    arrayOfKDArrays = Split(arr, tempDim);                                       //Creates 2 new arrays
    spKDArrayDestroy(arr);
    tempMedianValue = spKDArrayGetHighestPointValueInDim(arrayOfKDArrays[0], tempDim);

    tempLeft = spKDBuildTree(tempDim, arrayOfKDArrays[0], method, d, msg);
    printf("Left sub tree built successfully \n"); //TODO REMOVE PRINT
    tempRight = spKDBuildTree(tempDim, arrayOfKDArrays[1], method, d, msg);
    printf("Right sub tree built successfully \n"); //TODO REMOVE PRINT
    if (!tempLeft || !tempRight){
        spKDTreeNodeDestroy(node);
        spKDArrayDestroy(arrayOfKDArrays[0]);
        spKDArrayDestroy(arrayOfKDArrays[1]);
        spPointDestroy(tempData);
        spKDTreeNodeDestroy(tempLeft);
        spKDTreeNodeDestroy(tempRight);
        return NULL;
    }
    printf("\ntempDim: %d and tempMedianValue: %d\n", tempDim, tempMedianValue);
    node = spKDTreeNodeCreate(tempDim, tempMedianValue, tempLeft, tempRight, tempData, msg);    //Creates node of this level of the tree
    printf("Created point w/median value: %d and dim: %d \n", SPKDTreeNodeGetVal(node), SPKDTreeNodeGetDim(node)); //TODO REMOVE PRINT
    spPointDestroy(tempData);
    if (!node){
      return NULL;
    }
    *msg = SP_KDTREE_SUCCESS;
    return node;
}

//TODO
SPKDTreeNode spCreateKDTree(SPKDArray arr, SP_SPLIT_METHOD method, SP_NNSEARCH_MSG* msg){
  int dimension = spKDArrayGetDimention(arr);
  SPKDTreeNode resultNode;
  printf("\n about to build tree: \n"); //TODO REMOVE PRINT
  resultNode = spKDBuildTree(-1, arr, method, dimension, msg);
  return resultNode;
}

void SPKDNNSearch(SPPoint queryPoint, SPKDTreeNode root, SPBPQueue BPQ){ //arr is P
  assert(BPQ != NULL);
  int index;
  int value = SPKDTreeNodeGetVal(root);
  double distance;
  SPKDTreeNode left = SPKDTreeNodeGetLeft(root);
  SPKDTreeNode right = SPKDTreeNodeGetRight(root);
  int maxDistance = INT_MAX;
  int currDim;
  int queryValueAtDim;
  SPPoint tempPnt;
  SPListElement elem;
  bool searchLeft = false;

  if (spBPQueueSize(BPQ)>1){
    maxDistance = spBPQueueMaxValue(BPQ);
  }

  if (SPKDTreeNodeIsLeaf(root)){
    //enqueue the index of our given node into bpq
    tempPnt = spPointCopy(SPKDTreeNodeGetDataPoint(root));
    index = spPointGetIndex(tempPnt);
    distance = spPointL2SquaredDistance(queryPoint, tempPnt);
    elem = spListElementCreate(index, distance);
    if (!elem){
      spBPQueueDestroy(BPQ);
      return;
    }
    spBPQueueEnqueue(BPQ, elem);
    return;
  }
  currDim = SPKDTreeNodeGetDim(root);
  queryValueAtDim = spPointGetAxisCoor(queryPoint, currDim);

  //if the point's relevant coordinate value <= current node's' value, then recursively search the left subtree
  if (queryValueAtDim <= value){
      searchLeft = true;
      SPKDNNSearch(queryPoint, left, BPQ);
  }
  else{             //else : recursively search the right subtree
      SPKDNNSearch(queryPoint, right, BPQ);
  }
  //distance between point's relevant coordinate and root's value is less than the max distance in the queue
  if (spBPQueueIsFull(BPQ) || (value - queryValueAtDim)*(value - queryValueAtDim) < maxDistance){
      if (searchLeft){
        SPKDNNSearch(queryPoint, right, BPQ);
      }
      else{
        SPKDNNSearch(queryPoint, left, BPQ);
      }
  }
}

SPBPQueue SPSearchForNeighbors(SPPoint queryPoint, SPKDTreeNode root, int maxSize, SP_NNSEARCH_MSG* msg){
  if (!root || !queryPoint || maxSize<0){
    spKDTreeNodeDestroy(root);
    spPointDestroy(queryPoint);
    *msg = SP_SEARCH_INVALID_ARGUMENT;
    return NULL;
  }
  SPBPQueue BPQ;                    //Allocated in Create function
  BPQ = spBPQueueCreate(maxSize);
  if (!BPQ ){
    *msg = SP_KDTREE_QUEUE_FAILURE;
    spKDTreeNodeDestroy(root);

    spPointDestroy(queryPoint);
    return NULL;
  }
  else SPKDNNSearch(queryPoint, root, BPQ);
  //spKDTreeNodeDestroy(root);              //LEAVE FOR FURTHER QUERIES
  spPointDestroy(queryPoint);
  *msg = SP_SEARCH_SUCCESS;
  if (!BPQ){
    spKDTreeNodeDestroy(root);
    free(root);
    *msg = SP_SEARCH_FAIL;
    return NULL;
  }
  return BPQ;
}
void SPKDTreePrintNode(SPKDTreeNode root){
  char* bar = "---------------";

  printf("\n %s" , bar);
  printf("\n Dim: %d", SPKDTreeNodeGetDim(root));
  printf("\n Val: %d", SPKDTreeNodeGetVal(root));
  if (SPKDTreeNodeIsLeaf(root)){
      printf("\n Index: %d", (int) spPointGetAxisCoor(SPKDTreeNodeGetDataPoint(root),SPKDTreeNodeGetDim(root)));
  }
  printf("\n%s" ,bar);

  return;
}

void SPKDTreePrintTree(SPKDTreeNode root){
  //FILE* fp = fopen(filepath, "w");
  char* branchLeft = "\n/";
  char* branchRight = "\n\\";


  printf("\nKDD Tree:\n");
  SPKDTreePrintNode(root);
  if (!SPKDTreeNodeIsLeaf(root)){
    if (SPKDTreeNodeGetLeft(root) != NULL){
      puts(branchLeft);
      SPKDTreePrintTree(SPKDTreeNodeGetLeft(root));
    }
    if (SPKDTreeNodeGetRight(root) != NULL){
      puts(branchRight);
      SPKDTreePrintTree(SPKDTreeNodeGetRight(root));
    }
  }
  //fclose(fp);
  return;
}
