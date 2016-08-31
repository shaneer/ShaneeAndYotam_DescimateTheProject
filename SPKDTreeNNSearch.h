/*
    KDTree Header File
    Author: Shanee
*/

#include "SPPoint.h"
#include "SPKDArray.h"
#include "SPPoint.h"
#ifndef SPKDTREENNSEARCH_H_
#define SPKDTREENNSEARCH_H_



/** Type used for error reporting in SPKDTreeNode*/
typedef enum SP_NNSEARCH_RESULT_t {
	//Messages set during node creation
	SP_NODE_SUCCESS,
	SP_NODE_INVALID_ARGUMENT,
	SP_NODE_ALLOC_FAIL,
	//Messages set during node creation
	SP_KDTREE_SUCCESS,
	SP_KDTREE_INVALID_ARGUMENT,
	SP_KDTREE_ALLOC_FAIL
	SP_KDTREE_POINT_ALLOC_FAIL
} SP_NNSEARCH_MSG;

/*
 * A data-structure used to represent a node  in a KD Tree
*/
typedef struct sp_kd_node_t* SPKDNode;

//TODO
void spKDTreeNodeDestroy(SPKDTreeNode node);
/**
 * Creates a new node TODO
 *
 * @param
 * @return
 * NULL in case of memory allocation fails.
 * A new KDNode
 *
 *
 */
SPKDNode spKDTreeNodeCreate(int dimension, int value, SPKDNode* leftNode, SPKDNode* rightNode, SPPoint* data, SP_NNSEARCH_MSG* msg);


/**
 * Creates a copy of target node.
 *
 * The new copy will contain the same TODO as original
 *
 * The comparison is made based on the following relation:
 *
 * @param data - The target node which will be copied
 * @return
 * NULL if a NULL was sent or memory allocation failed.
 * Otherwise the address of the new copy
 *
 **/
SPKDTreeNode spTreeNodeCopy(SPTreeNode original);

/**
 * Compares two nodes.
 *The function asserts that both nodes are not NULL pointers.
 *
 * @param n1 The first node to be compared
 * @param n2 The second node to be compared with
 * @assert e1!=NULL AND e2!=NULL
 * @return
 * 1  if n1 is bigger
 * -1 if n2 is larger
 * 0  if they are equal (datawise - not necesarilly same address)
 */
//int spKDTreeNodeCompare(SPKDTreeNode n1, SPKDTreeNode n2);

/**
 * Creates a KD tree
 *
 * @param
 * @assert
 *
 * NULL in case of memory allocation fails at any stage
 * @returns the root node of the tree built ( A newly created SPKDTreeNode)
 *
 */
SPKDArray spKDBuildTree(SPPoint* pnt, int prevDim, SPKDArray arr, SP_SPLIT_METHOD method, SP_NNSEARCH_MSG* msg);

/*
TODO
*/
void SPKDNNSearch(SPPoint queryPoint, SPKDTreeNode root, SPKDArray arr, SPBPQueue BPQ);

#endif // SPKDTREENNSEARCH_H_
