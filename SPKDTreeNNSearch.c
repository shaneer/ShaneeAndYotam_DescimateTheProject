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
  SPKDTreeNode* left;
  SPKDTreeNode* right;
  SPPoint* data;
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
SPKDTreeNode* SPKDTreeNodeGetLeft(SPKDTreeNode root){
  assert(root != NULL);
  if (root->left == NULL){
    return NULL;
  }
  return (root->left);
}
SPKDTreeNode* SPKDTreeNodeGetRight(SPKDTreeNode root){
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
  if (!root->val){ //TODO Check
    return -1;
  }
  return root->val;
}
int SPKDTreeNodeGetDim(SPKDTreeNode root){
  assert(root != NULL);
  if (!root->dim){ //TODO Check
    return -1;
  }
  return root->dim;
}

//TODO
bool SPKDTreeNodeIsLeaf(SPKDTreeNode check){
  assert(check != NULL);
  if (check->data == NULL){
    return false;
  }
  return true;
}

SPPoint* SPKDTreeNodeGetDataPoint(SPKDTreeNode root){
  assert(root != NULL);
  if (root->data == NULL || SPKDTreeNodeIsLeaf(root)){ //TODO Check
    return NULL;
  }
  return (root->data);
}

//TODO
void spKDTreeNodeDestroy(SPKDTreeNode node){
  assert(node != NULL);
  spKDTreeNodeDestroy(*SPKDTreeNodeGetLeft(node));
  spKDTreeNodeDestroy(*SPKDTreeNodeGetRight(node));
  spPointDestroy(*SPKDTreeNodeGetDataPoint(node));
  free(node);
}

//TODO
SPKDTreeNode spKDTreeNodeCreate(int dimension, int value, SPKDTreeNode* leftNode,
  SPKDTreeNode* rightNode, SPPoint* data, SP_NNSEARCH_MSG* msg){

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
  SPKDTreeNode* tempLeft=NULL;
  SPKDTreeNode* tempRight=NULL;
  int tempDim = INVALID_NUM;                             //Both ints initialized to -1
  int tempMedianValue = INVALID_NUM;
  SPPoint* tempData = NULL;
  SPKDArray *arrayOfKDArrays;                       //For use in the split method
  int medianIndex = 1+((spKDArrayGetSize(arr)-1)/2);       // Find ciel(n/2) index
  srand((unsigned int)time(NULL));                  //TODO Does this need to be closed?

  //If size of our array is 1 we have reached a leaf: create and return a node with value of point in array
  if (spKDArrayGetSize(arr)==1){
    *tempData = spPointCopy(spKDArrayGetPoint(arr, 0));
    if (!tempData){
      *msg  = SP_KDTREE_GETPOINT_FAIL;
      spKDTreeNodeDestroy(node);
      spKDArrayDestroy(arr);
      return NULL;
    }
    node = spKDTreeNodeCreate(tempDim, tempMedianValue, tempLeft, tempRight, tempData, msg);

    spKDArrayDestroy(arr);
    spPointDestroy(*tempData);
    //Check if creation succeeded:
    if (!node){
      return NULL;
    }//else return node created after having destroyed array used to create it and temps used
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
    //We find the apropriate value of the point that is the median by dimension calculated
    tempMedianValue = spPointGetAxisCoor(spKDArrayGetPoint(arr, medianIndex), tempDim);  //TODO Check w/Yotam
    //We now split the array according to the SPREAD defined, with the median value found above

    arrayOfKDArrays = Split(arr, tempDim);                                       //Creates 2 new arrays
    spKDArrayDestroy(arr);
    *tempLeft = spKDBuildTree(prevDim+1, arrayOfKDArrays[0], method, d, msg);            //FIX POINT AND SIZE TODO
    *tempRight = spKDBuildTree(prevDim+1, arrayOfKDArrays[1], method, d, msg);           //FIX POINT AND SIZE TODO
    if (!tempLeft || !tempRight){
      spKDTreeNodeDestroy(node);
      spKDArrayDestroy(arr);
      spPointDestroy(*tempData);
      spKDTreeNodeDestroy(*tempLeft);
      spKDTreeNodeDestroy(*tempRight);
      return NULL;
    }
    node = spKDTreeNodeCreate(tempDim, tempMedianValue, tempLeft, tempRight, tempData, msg);    //Creates node of this level of the tree
    spKDArrayDestroy(arr);
    spPointDestroy(*tempData);
    spKDTreeNodeDestroy(*tempLeft);
    spKDTreeNodeDestroy(*tempRight);
    if (!node){
      return NULL;
    }
    *msg = SP_KDTREE_SUCCESS;
    return node;
}
//TODO
SPKDTreeNode spCreateKDTree(SPKDArray arr, SP_SPLIT_METHOD method, SP_NNSEARCH_MSG* msg){
  int dimension = spKDArrayGetDimention(arr);
  return spKDBuildTree(0, arr, method, dimension, msg);
}

void SPKDNNSearch(SPPoint queryPoint, SPKDTreeNode* root, SPBPQueue BPQ){ //arr is P
  assert(BPQ != NULL);
  int index;
  int value = SPKDTreeNodeGetVal(*root);
  double distance;
  SPKDTreeNode* left = SPKDTreeNodeGetLeft(*root);
  SPKDTreeNode* right = SPKDTreeNodeGetRight(*root);
  int maxDistance = INT_MAX;
  int currDim;
  int queryValueAtDim;
  SPPoint tempPnt;
  SPListElement elem;
  bool searchLeft = false;

  if (spBPQueueSize(BPQ)>1){
    maxDistance = spBPQueueMaxValue(BPQ);
  }

  if (SPKDTreeNodeIsLeaf(*root)){
    //enqueue the index of our given node into bpq
    tempPnt = spPointCopy(*SPKDTreeNodeGetDataPoint(*root));
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
  currDim = SPKDTreeNodeGetDim(*root);
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
  if (spBPQueueIsFull(BPQ) || pow(abs(value - queryValueAtDim),2) < maxDistance){
      if (searchLeft){
        SPKDNNSearch(queryPoint, right, BPQ);
      }
      else{
        SPKDNNSearch(queryPoint, left, BPQ);
      }
  }
}

SPBPQueue SPSearchForNeighbors(SPPoint queryPoint, SPKDTreeNode* root, int maxSize, SP_NNSEARCH_MSG* msg){
  if (!root || !queryPoint || maxSize<0){
    spKDTreeNodeDestroy(*root);
    spPointDestroy(queryPoint);
    *msg = SP_SEARCH_INVALID_ARGUMENT;
    return NULL;
  }
  SPBPQueue BPQ;                    //Allocated in Create function
  BPQ = spBPQueueCreate(maxSize);
  if (!BPQ ){
    *msg = SP_KDTREE_QUEUE_FAILURE;
    spKDTreeNodeDestroy(*root);

    spPointDestroy(queryPoint);
    return NULL;
  }
  else SPKDNNSearch(queryPoint, root, BPQ);
  //spKDTreeNodeDestroy(root);              //LEAVE FOR FURTHER QUERIES
  spPointDestroy(queryPoint);
  *msg = SP_SEARCH_SUCCESS;
  if (!BPQ){
    spKDTreeNodeDestroy(*root);
    free(root);
    *msg = SP_SEARCH_FAIL;
    return NULL;
  }
  return BPQ;
}
