/*
    KDTree Source File
    Author: Shanee
*/
#include <stdbool.h>
#include <stdio.h>
#include<stdlib.h>
#include<time.h>
#include <limits.h>
#include <math.h>
#include"SPPoint.h"
#include "SPKDTreeNNSearch.h"
#include "SPConfig.h"

//TODO
struct sp_kd_node_t*{
  int dim;
  int val;
  SPKDNode* left;
  SPKDNode* right;
  SPPoint* data;
};

//TODO
void spKDTreeNodeDestroy(SPKDTreeNode node){
  assert(node != NULL);
  spKDTreeNodeDestroy(left);
  spKDTreeNodeDestroy(right);
  spPointDestroy(data);
  free(node);

}

//TODO
SPKDNode spKDTreeNodeCreate(int dimension, int value, SPKDNode* leftNode, SPKDNode* rightNode, SPPoint* data, SP_NNSEARCH_MSG* msg){

  SPKDNode node = (SPKDNode) malloc (sizeof(*node));
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

//TODO
bool SPTreeNodeIsLeaf(SPTreeNode check){
  assert(check != NULL);
  if (check->data == NULL){
    return false;
  }
  return true;
}

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
  return root->left;
}
SPKDTreeNode SPKDTreeNodeGetRight(SPKDTreeNode root){
  assert(root != NULL);
  if (root->right == NULL){
    return NULL;
  }
  return root->right;
}

//TODO Getter for Value
int SPKDTreeNodeGetVal(SPKDTreeNode root){
  assert(root != NULL);
  if (root->val == NULL){ //TODO Check
    return -1;
  }
  return root->val;
}
int SPKDTreeNodeGetDim(SPKDTreeNode root){
  assert(root != NULL);
  if (root->dim == NULL){ //TODO Check
    return -1;
  }
  return root->dim;
}

SPPoint SPKDTreeNodeGetDataPoint(SPKDTreeNode root){
  assert(root != NULL);
  if (root->data == NULL || SPTreeNodeIsLeaf(root)){ //TODO Check
    return NULL;
  }
  return root->data;
}

//TODO
SPKDNode spKDBuildTree(int prevDim, SPKDArray arr, SP_SPLIT_METHOD method, SP_NNSEARCH_MSG* msg){
  assert (arr != NULL);//TODO
  SPKDNode node;
  SPKDNode tempLeft=NULL;
  SPKDNode tempRight=NULL;
  int tempDim = INVALID;                             //Both ints initialized to -1
  int tempMedianValue = INVALID;
  SPPoint tempData;
  SPKDArray *arrayOfKDArrays;                       //For use in the split method
  int medianIndex = 1+((spKDArrayGetSize(arr)-1)/2);       // Find ciel(n/2) index
  srand((unsigned int)time(NULL));                  //TODO Does this need to be closed?

  //If size of our array is 1 we have reached a leaf: create and return a node with value of point in array
  if (pKDArrayGetSize(arr)==1){
    *tempData = spPointCopy(spKDArrayGetPoint(arr, 0));
    if (!tempData){
      *msg  = SP_KDTREE_GETPOINT_FAIL;
      spKDTreeNodeDestroy(node);
      spKDArrayDestroy(arr);
      return NULL;
    }
    node = spKDTreeNodeCreate(tempDim, tempMedianValue, tempLeft, tempRight, tempData, msg);

    spKDArrayDestroy(arr);
    spPointDestroy(tempData);
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
            tempDim = (prevDim+1)%(spPointGetDimension(pnt));
            break;
        case RANDOM:
            tempDim = rand()%29+1;
            break;
    }
    //We find the apropriate value of the point that is the median by dimension calculated
    tempMedianValue = spPointGetAxisCoor(spKDArrayGetPoint(arr, medianIndex), tempDim);  //TODO Check w/Yotam
    //We now split the array according to the SPREAD defined, with the median value found above

    arrayOfKDArrays = Split(arr, tempDim);                                       //Creates 2 new arrays
    SPKDArrayDestroy(arr);
    tempLeft = spKDBuildTree(pnt, tempDim, arrayOfKDArrays[0], method, msg);            //FIX POINT AND SIZE TODO
    tempRight = spKDBuildTree(pnt, tempDim, arrayOfKDArrays[0], method, msg);           //FIX POINT AND SIZE TODO
    if (!tempLeft || !tempRight){
      spKDTreeNodeDestroy(node);
      spKDArrayDestroy(arr);
      spPointDestroy(tempData);
      spKDTreeNodeDestroy(tempLeft);
      spKDTreeNodeDestroy(tempRight);
      return NULL;
    }
    node = spKDTreeNodeCreate(tempDim, tempMedianValue, tempLeft, tempRight, tempData, msg);    //Creates node of this level of the tree
    spKDArrayDestroy(arr);
    spPointDestroy(tempData);
    spKDTreeNodeDestroy(tempLeft);
    spKDTreeNodeDestroy(tempRight);
    if (!node){
      return NULL;
    }
    return node;
}

void SPKDNNSearch(SPPoint queryPoint, SPKDTreeNode root, SPBPQueue BPQ){ //arr is P
  assert(BPQ != NULL);
  int index;
  int value = SPKDTreeNodeGetVal(root);
  double distance;
  SPPoint pnt;
  SPKDTreeNode left = SPKDTreeNodeGetLeft(root);
  SPKDTreeNode right = SPKDTreeNodeGetRight(root);
  int maxDistance = INT_MAX;
  int currDim;
  int queryValueAtDim;
  bool searchLeft = false;

  if (spBPQueueSize(BOQ)>1){
    maxDistance = spBPQueueMaxValue(BPQ);
  }

  if SPTreeNodeIsLeaf(root){
    //enqueue the index of our given node into bpq
    pnt = root->data;
    index = spPointGetIndex(pnt);
    distance = spPointL2SquaredDistance(queryPoint, pnt);
    spBPQueueEnqueue(index, distance);
    return;
  }
  currDim = root->dim;
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
  if (spBPQueueIsFull(BPQ) || pow(abs(value - queryValueAtDim),2) < maxDistance)){
      if (searchLeft){
        SPKDNNSearch(queryPoint, right, BOQ);
      }
      else{
        SPKDNNSearch(queryPoint, left, BOQ);
      }
  }
}

SPBPQueue SPSearch(SPPoint queryPoint, SPKDTreeNode root, int maxSize, SP_NNSEARCH_MSG* msg){
  if (!root || !queryPoint || maxSize<0){
    spKDTreeNodeDestroy(root);
    spPointDestroy(queryPoint);
    *msg = SP_SEARCH_INVALID_ARGUMENT;
    return NULL;
  }

  SPBPQueue BPQ;
  BPQ = (SPBPQueue*) malloc(sizeof(*BPQ));
  spBPQueueCreate(maxSize);
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
  return BPQ;
}
