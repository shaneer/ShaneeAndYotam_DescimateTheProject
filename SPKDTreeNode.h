/*
    KDTree Header File
    Author: Shanee
*/

#include "SPPoint.h"
#include "SPKDArray.h"
#ifndef SPKDNODE_H_
#define SPKDNODE_H_



/** Type used for error reporting in SPKDTreeNode*/
typedef enum SP_TREE_NODE_RESULT_t {
	SP_NODE_SUCCESS,
	SP_NODE_INVALID_ARGUMENT,
	SP_NODE_ALLOC_FAIL,
} SP_NODE_MSG;

/*
 * A data-structure used to represent a node  in a KD Tree
*/
typedef struct sp_kd_node_t* SPKDNode;

/**
 * Creates a new node
 *
 * @param
 * @return
 * NULL in case of memory allocation fails.
 * A new KDNode
 *
 *
 */
SPKDNode spKDTreeNodeCreate();

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
 * Destroys the given node.
 * All memory allocation associated with the node is freed
 *
 * @param node the target node to be be freed.
 *
 * If data is NULL, then nothing is done
 */
void spKDTreeNodeDestroy(SPKDTreeNode node);

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
int spKDTreeNodeCompare(SPKDTreeNode n1, SPKDTreeNode n2);

/**
 * 	A setter for TODO
 *  Constraints:
 *
 *  @param node   The target node
 *  @param  TODO
 *  @return
 *  SP_NODE_INVALID_ARGUMENT in case data==NULL || index<0
 *  SP_NODE_ALLOC_FAIL in case memory allocation fails
 *  SP_NODE_SUCCESS otherwise
 */
SP_NODE_MSG spKDTreeNodeSet(SPTreeNode node, int data);

/**
 * A getter for the TODO of the target node.
 *
 * @param data The target node
 * @return
 * -1 in case data==NULL or node==NULL
 * otherwise, returns TODO of the target node
 */
int spKDTreeNodeGet(SPTreeNode node);



#endif // SPKDNODE_H_
