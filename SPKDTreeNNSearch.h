/*
    KDTree Header File
    Author: Shanee
*/
#include "SPPoint.h"
#include "SPKDArray.h"
#include "SPPoint.h"
#include "SPConfig.h"
#include "SPBPQueue.h"
#ifndef SPKDTREENNSEARCH_H_
#define SPKDTREENNSEARCH_H_
/*
 * KD Tree and NN Search Summary
 *
 *
 *
 *
 *
 *
 */

/** Type used for error reporting in SPKDTreeNode*/
typedef enum SP_NNSEARCH_RESULT_t {
	//Messages set during node creation
	SP_NODE_SUCCESS,
	SP_NODE_INVALID_ARGUMENT,
	SP_NODE_ALLOC_FAIL,
	//Messages set during node creation
	SP_KDTREE_SUCCESS,
	SP_KDTREE_INVALID_ARGUMENT,
	SP_KDTREE_ALLOC_FAIL,
	SP_KDTREE_GETPOINT_FAIL,
	//Failure building queue
	SP_KDTREE_QUEUE_FAILURE,
	//During Search:
	SP_SEARCH_INVALID_ARGUMENT,
	SP_SEARCH_SUCCESS,
	SP_SEARCH_FAIL
	//
} SP_NNSEARCH_MSG;

/*
 * A data-structure used to represent a node  in a KD Tree
*/
typedef struct sp_kd_node_t* SPKDTreeNode;

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
SPKDTreeNode spKDTreeNodeCreate(int dimension, int value, SPKDTreeNode leftNode, SPKDTreeNode rightNode, SPPoint data, SP_NNSEARCH_MSG* msg);


//TODO
SPKDTreeNode SPKDTreeNodeGetLeft(SPKDTreeNode root);
SPKDTreeNode SPKDTreeNodeGetRight(SPKDTreeNode root);
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
SPKDTreeNode spCreateKDTree(SPKDArray arr, SP_SPLIT_METHOD method, SP_NNSEARCH_MSG* msg);

/*
TODO
*/
SPBPQueue SPSearchForNeighbors(SPPoint queryPoint, SPKDTreeNode root, int maxSize, SP_NNSEARCH_MSG* msg);

/*
TODO
*/
void SPKDTreePrintNode(SPKDTreeNode node);

#endif // SPKDTREENNSEARCH_H_
