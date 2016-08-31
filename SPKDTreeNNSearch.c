/*
    KDTree Source File
    Author: Shanee
*/
#include <stdbool.h>
#include <stdio.h>
#include<stdlib.h>
#include<time.h>
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

/**TODO Delete
SPKDTreeNode spTreeNodeCopy(SPTreeNode original);
*/

//TODO
SPKDNode spKDBuildTree(SPPoint* pnt, int prevDim, SPKDArray arr, SP_SPLIT_METHOD method, SP_NNSEARCH_MSG* msg){
  assert (arr != NULL);//TODO
  assert (point!= NULL);

  SPKDNode node;
  SPKDNode tempLeft=NULL;
  SPKDNode tempRight=NULL;;
  int tempDim = -1;             //Both ins initialized to INVALID
  int tempMedian = -1;
  SPPoint* tempData;              //SHOULD IT BE A POINTER? TODO
  SPKDArray *arrayOfKDArrays;   //For use in the split method
  int median = 1+((spKDArrayGetSize(arr)-1)/2);       //TODO - CHECK : Find ciel(n/2)
  int coor;
  srand((unsigned int)time(NULL));

  //If size of our array is 1 build
  if (pKDArrayGetSize(arr)==1){
    *tempData = spKDArrayGetPoint(arr);
    if (tempData==NULL){
      *msg  = SP_KDTREE_POINT_ALLOC_FAIL;
      SPKDTreeNodestroy(node);              //TODO Necessary?
      return NULL;
    }
    node = spKDTreeNodeCreate(tempDim, tempMedian, tempLeft, tempRight, tempData, SOACDim msg);
    if (node==NULL){
      SPKDTreeNodestroy(node);
      return NULL;
    }
    return node;
  }

  else{             //Node is not a leaf
      switch(method) {
        case MAX_SPREAD:
            tempDim = spKDArrayFindMaxSpreadDim(arr); //TODO Check
            coor = spPointGetAxisCoor(pnt, tempDim);
            break;
        case INCREMENTAL:
            tempDim = (prevDim+1)%(spPointGetDimension(pnt));
            coor = spPointGetAxisCoor(pnt, tempDim);
            break;
        case RANDOM:
            tempDim = rand()%29+1;
            coor = spPointGetAxisCoor(pnt, tempDim);
            break;
    }
    arrayOfKDArrays = Split(arr, coor); //Creates 2 new arrays!
    SPKDArrayDestroy(arr);
    tempLeft = spKDBuildTree(pnt, tempDim, arrayOfKDArrays[0], method, msg); //FIX POINT AND SIZE TODO
    tempRight = spKDBuildTree(pnt, tempDim, arrayOfKDArrays[0], method, msg); //FIX POINT AND SIZE TODO
    node = spKDTreeNodeCreate(tempDim, median, tempLeft, tempRight, tempData, msg); //FIX DIM/VAL TODO
    return node;
}

void SPKDNNSearch(SPPoint queryPoint, SPKDTreeNode root, SPKDArray arr, SPBPQueue BPQ);
  assert(root != NULL);
  assert(BPQ != NULL);
  int index;
  int value = SPKDTreeNodeGetVal(root);
  SPKDTreeNode left = SPKDTreeNodeGetLeft(root);
  SPKDTreeNode right = SPKDTreeNodeGetRight(root);
  int maxDistance = spBPQueueMaxValue(BOQ);

  /*Allocate all memory - create BPQ and check
  BPQ = (SPBPQueue*) malloc(*BPQ);
  Iniitialize all variables
  BPQ = spBPQueueCreate(maxSize);
  Check if successful
  if (!BPQ || !curr){
    spBPQueueDestroy(BOQ);
    spPointDestroy(queryPoint);
    spKDTreeNodeDestroy(root);
    return NULL;
  }
  */

  if SPTreeNodeIsLeaf(root){
    //enqueue(index(current), distance(curr,p)) into bpq
    index = spKDArrayGetIndexInArray(arr, i, j); //TODO GET CORRECT INDEX
    spBPQueueEnqueue(spKDTreeNodeGetIndex(index, distance);   //TODO GET CORRECT DISTANCE
    //TODO Destroy necessary variables
    return;
  }
    //if(P[curr.dim] <= curr.val) then: recursively search the left subtree
    //else : recursively search the right subtree
    if (P[SPKDTreeNodeGetDim(root)]<= value){//TODO fix P
      SPKDNNSearch(queryPoint, left, arr);
      //|curr.val - P[curr.dim]| is less than the priority of themax-priority element of bpq
      if (spBPQueueIsFull(BPQ) || abs(value - maxDistance)){
        SPKDNNSearch(queryPoint, right, arr);
      }
    }else{
      SPKDNNSearch(queryPoint, right, arr);
      if (spBPQueueIsFull(BPQ) || abs(value - maxDistance)){
        SPKDNNSearch(queryPoint, left, arr);
      }
    }
}
