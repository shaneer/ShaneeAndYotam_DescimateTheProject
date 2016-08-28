/*
    KDTree Header File
    Author: Shanee
*/

#include "SPPoint.h"
#include "SPKDArray.h"
#include "SPKDTreeNode.h"
#ifndef SPKDTREE_H_
#define SPKDTREE_H_

/*
 * A data-structure used to store info for nearest neighbor search
*/
typedef struct sp_kd_tree_t* SPKDTree;

/** Type used for returning error codes from KDTREE functions */
typedef enum sp_KDTREE_msg_t {
	SP_TreeNode_SUCCESS,
	SP_KDTREE_NULL_ARGUMENT,
	SP_KDTREE_OUT_OF_MEMORY,
	SP_KDTREE_INVALID_CURRENT,
} SP_KDTREE_MSG;

/**
 * Allocates a new KDTREE.
 *
 * This function creates a new empty KDTREE.
 * @return
 * 	NULL - If allocations failed.
 * 	A new KDTREE in case of success.
 */
SPKDTREE spKDTREECreate();

/**
 * Creates a copy of target KDTREE.
 *
 * The new copy will contain all the elements from the source KDTREE in the same
 * order. The internal iterator for both the new copy and the target KDTREE will not be
 * defined afterwards.
 *
 * @param KDTREE The target KDTREE to copy
 * @return
 * NULL if a NULL was sent or a memory allocation failed.
 * A KDTREE containing the same elements with same order as KDTREE otherwise.
 */
SPKDTREE spKDTREECopy(SPKDTREE KDTREE);

/**
 * Returns the number of elements in a KDTREE. the iterator state will not change.
 *
 * @param KDTREE The target KDTREE which size is requested.
 * @return
 * -1 if a NULL pointer was sent.
 * Otherwise the number of elements in the KDTREE.
 */
int spKDTREEGetSize(SPKDTREE KDTREE);

/**
 * Sets the internal iterator to the first element and retrieves it.
 *
 * The KDTREE has an internal iterator (also called current element) to allow
 * iteration over the KDTREE's elements. This function sets the iterator to point
 * to the first element in the KDTREE and return it.
 * Use this to start iterating over the KDTREE, searching from the beginning of
 * the KDTREE and/or get the first element in the KDTREE.
 * (To continue iteration use KDTREEGetNext)
 * @code
 * void f(SPKDTREE KDTREE) {
 *   KDTREEElement first = KDTREEGetFirst(KDTREE);
 *   printf("The first element is at address %x\n", first);
 * }
 * @endcode
 *
 * @param KDTREE The KDTREE for which to set the iterator and return the first
 * element.
 * @return
 * NULL is a NULL pointer was sent or the KDTREE is empty.
 * The first element of the KDTREE otherwise
 */
SPKDTREEElement spKDTREEGetFirst(SPKDTREE KDTREE);

/**
 * Sets the internal iterator to the last element and retrieves it.
 *
 * The KDTREE has an internal iterator (also called current element) to allow
 * iteration over the KDTREE's elements. This function sets the iterator to point
 * to the last element in the KDTREE and return it.
 * Use this to start iterating over the KDTREE, searching from the end of
 * the KDTREE and/or get the last element in the KDTREE.
 * (To continue iteration use KDTREEGetPrevious)
 * @code
 * void f(SPKDTREE KDTREE) {
 *   SPKDTREEElement first = spKDTREEGetLast(KDTREE);
 *   printf("The last element is at address %x\n", first);
 * }
 * @endcode
 *
 * @param KDTREE The KDTREE for which to set the iterator and return the last
 * element.
 * @return
 * NULL is a NULL pointer was sent or the KDTREE is empty.
 * The last element of the KDTREE otherwise
 */
SPKDTREEElement spKDTREEGetLast(SPKDTREE KDTREE);

/**
 * Advances the KDTREE's iterator to the next element and return it. In case
 * the return value is NULL, the state of the iterator will not be defined, otherwise
 * it will point to the next element in the KDTREE.
 *
 * @param KDTREE The KDTREE for which to advance the iterator
 * @return
 * NULL if reached the end of the KDTREE, the iterator is at an invalid state or
 * a NULL sent as argument
 * The next element on the KDTREE in case of success
 */
SPKDTREEElement spKDTREEGetNext(SPKDTREE KDTREE);

/**
 * Moves the KDTREE's iterator to the previous element and return it. In case
 * the return value is NULL, the state of the iterator will not be defined, otherwise
 * it will point to the previous element in the KDTREE.
 *
 * @param KDTREE The KDTREE for which to advance the iterator
 * @return
 * NULL if reached the head of the tail, the iterator is at an invalid state or
 * a NULL sent as argument
 * The previous element on the KDTREE in case of success
 */
SPKDTREEElement spKDTREEGetPrevious(SPKDTREE KDTREE);

/**
 * Returns the current element (pointed by the iterator)
 *
 * @param KDTREE The KDTREE for which to get the iterator
 * @return
 * NULL if the iterator is at an invalid state or a NULL sent as argument
 * The current element on the KDTREE in case of success
 */
SPKDTREEElement spKDTREEGetCurrent(SPKDTREE KDTREE);

/**
 * Adds a new element to the KDTREE, the new element will be the first element. The state
 * of the iterator will not be changed
 *
 * @param KDTREE The KDTREE for which to add an element in its start
 * @param element The element to insert. A copy of the element will be
 * inserted
 * @return
 * SP_KDTREE_NULL_ARGUMENT if a NULL was sent as KDTREE
 * SP_KDTREE_OUT_OF_MEMORY if an allocation failed
 * SP_KDTREE_SUCCESS the element has been inserted successfully
 */
SP_KDTREE_MSG spKDTREEInsertFirst(SPKDTREE KDTREE, SPKDTREEElement element);

/**
 * Adds a new element to the KDTREE, the new element will be the last element. The state
 * of the iterator will not be changed.
 *
 * @param KDTREE The KDTREE for which to add an element in its end
 * @param element The element to insert. A copy of the element will be
 * inserted
 * @return
 * SP_KDTREE_NULL_ARGUMENT if a NULL was sent as KDTREE
 * SP_KDTREE_OUT_OF_MEMORY if an allocation failed
 * SP_KDTREE_SUCCESS the element has been inserted successfully
 */
SP_KDTREE_MSG spKDTREEInsertLast(SPKDTREE KDTREE, SPKDTREEElement element);

/**
 * Adds a new element to the KDTREE, the new element will be placed right before
 * the current element (As pointed by the inner iterator of the KDTREE). The state
 * of the iterator will not be changed
 *
 * @param KDTREE The KDTREE for which to add an element before its current element
 * @param element The element to insert. A copy of the element will be
 * inserted
 * @return
 * SP_KDTREE_NULL_ARGUMENT if a NULL was sent as KDTREE or element
 * SP_KDTREE_INVALID_CURRENT if the KDTREE's iterator is in an invalid state (Does
 * not point to a legal element in the KDTREE)
 * SP_KDTREE_OUT_OF_MEMORY if an allocation failed
 * SP_KDTREE_SUCCESS the element has been inserted successfully
 */
SP_KDTREE_MSG spKDTREEInsertBeforeCurrent(SPKDTREE KDTREE, SPKDTREEElement element);

/**
 * Adds a new element to the KDTREE, the new element will be place right after
 * the current element (As pointed by the inner iterator be of the KDTREE). The state
 * of the iterator will not be changed.
 *
 * @param KDTREE The KDTREE for which to add an element after its current element
 * @param element The element to insert. A copy of the element will be
 * inserted
 * @return
 * SP_KDTREE_NULL_ARGUMENT if a NULL was sent as KDTREE
 * SP_KDTREE_INVALID_CURRENT if the KDTREE's iterator is in an invalid state (Does
 * not point to a legal element in the KDTREE)
 * SP_KDTREE_OUT_OF_MEMORY if an allocation failed
 * SP_KDTREE_SUCCESS the element has been inserted successfully
 */
SP_KDTREE_MSG spKDTREEInsertAfterCurrent(SPKDTREE KDTREE, SPKDTREEElement element);

/**
 * Removes the currently pointed element of the KDTREE using the stored freeing
 * function. The state of the current element will not be defined afterwards.
 *
 * @param KDTREE The KDTREE for which the current element will be removed
 * @return
 * SP_KDTREE_NULL_ARGUMENT if KDTREE is NULL
 * SP_KDTREE_INVALID_CURRENT if the current pointer of the KDTREE is in invalid state
 * SP_KDTREE_SUCCESS the current element was removed successfully
 */
SP_KDTREE_MSG spKDTREERemoveCurrent(SPKDTREE KDTREE);

/**
 * Removes all elements from target KDTREE. The state of the current element will not be defined afterwards.
 *
 * The elements are deallocated using the stored freeing function
 * @param KDTREE Target KDTREE to remove all element from
 * @return
 * SP_KDTREE_NULL_ARGUMENT - if a NULL pointer was sent.
 * SP_KDTREE_SUCCESS - Otherwise.
 */
SP_KDTREE_MSG spKDTREEClear(SPKDTREE KDTREE);

/**
 * KDTREEDestroy: Deallocates an existing KDTREE. Clears all elements by using the
 * stored free function.
 *
 * @param KDTREE Target KDTREE to be deallocated. If KDTREE is NULL nothing will be
 * done
 */
void spKDTREEDestroy(SPKDTREE KDTREE);

/**
 * Macro for iterating over a KDTREE.
 *
 * Declares a new variable to hold each element of the KDTREE.
 * Use this macro for iterating through the KDTREE conveniently.
 * Note that this mcaro modifies the internal iterator.
 *
 * For example, the following code will go through all elements
 * of the KDTREE and returns the minimum value
 *
 * @code
 * double minElementValue(SPKDTREE KDTREE) {
 *   double minVal = -1.0;//Elements in KDTREE has non-negative values
 *   if(KDTREE==NULL){
 *   	return -1.0;
 *   }
 *   SP_KDTREE_FOREACH(SPKDTREEElement, e, KDTREE) {
 *     double eVal = spKDTREEElementGetValue(e)
 *     if(minVal == -1.0 || eVal<minVal){ //Updates minimum
 *     		minVal = eVal;
 *     }
 *   }
 *   return minVal;
 * }
 *
 * @endcode
 *
 * @param type The type of the elements in the KDTREE
 * @param iterator The name of the variable to hold the next KDTREE element
 * @param KDTREE the KDTREE to iterate over
 */
#define SP_KDTREE_FOREACH(type,iterator,KDTREE) \
	for(type iterator = spKDTREEGetFirst(KDTREE) ; \
		iterator ;\
		iterator = spKDTREEGetNext(KDTREE))















#endif // SPKDTREE_H_
