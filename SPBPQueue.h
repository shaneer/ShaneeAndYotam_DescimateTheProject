#ifndef SPBPQUEUE_H_
#define SPBPQUEUE_H_
#include "SPListElement.h"
#include <stdbool.h>
/**
 * SP Bounded Priority Queue summary
 *
 * TODO Complete documentation
 */


/** type used to define Bounded priority queue **/
typedef struct sp_bp_queue_t* SPBPQueue;

/** type for error reporting **/
typedef enum sp_bp_queue_msg_t {
	SP_BPQUEUE_OUT_OF_MEMORY,
	SP_BPQUEUE_FULL,
	SP_BPQUEUE_EMPTY,
	SP_BPQUEUE_INVALID_ARGUMENT,
	SP_BPQUEUE_SUCCESS
} SP_BPQUEUE_MSG;

/**
 * Allocates a new point in memory.
 * Given the desired max capacity of the queue
 * The new queue will have an integer signifying its max and a list
 * of elements that are held in sorted order, meaning:
 * The element with the smallest value is always on the left.
 *
 * @param maxSize an integer serving as the max capacity
 * @return
 * NULL in case allocation failure occurred OR data is NULL OR maxSize <=0
 * Otherwise, a new priority queue is created.
 */
SPBPQueue spBPQueueCreate(int maxSize);

/**
 * Allocates a copy of the given queue.
 *
 * Given the queue source, the function returns a new queue
 * which maintains the same traits as the one in source meaning
 * It has the same capacity and list that the source queue has at the time the function is called.
 *
 * @param source - The source queue to be copied
 * @return
 * NULL in case memory allocation occurs
 * Otherwise a copy of source is returned.
 */
SPBPQueue spBPQueueCopy(SPBPQueue source);

/**
 * Frees all memory taht had been allocated for or associated
 * with the source queue.
 *
 * If  the source queue is NULL, nothing happens.
 *
 * @param source - The source queue to be destroyed.
 */
void spBPQueueDestroy(SPBPQueue source);

/**
 * Removes all elements from the source queue's list, leaving it an empty queue.
 *
 * The elements themselves are deallocated using SPList's destroy function on the list elements.
 *
 *@param source -  Target queue to remove all elements from.
 */
void spBPQueueClear(SPBPQueue source);

/**
 * Returns the current number of elements inside the queue.
 * Determine the size of the source queueu's list which is in turn
 * the number of elements currently in it.
 *
 * The iterator of the list's state will not change.
 *
 * @param source - the queue whose size is requested.
 * @return
 * -1 if a NULL pointer was sent.
 * Otherwise the number of elements currently in the queue.
 */
int spBPQueueSize(SPBPQueue source);

/**
 * Returns the maximal number of elements the queue can hold,
 * meaning, the max capacity given when the queue was created and saved as a member of queue.
 *
 * @param source - the queue whose max capacity is requested.
 * @return
 * -1 if a NULL pointer was sent.
 * Otherwise the max capacity of elements currently in the queue.
 */
int spBPQueueGetMaxSize(SPBPQueue source);

/**
 * Adds a new element to the queue, in it's correct place in the lists order.
 * Checks  both if the element need be inserted at all, and then finds its correct place of inserting it,
 * adding it to the source queue's list.
 * If the source was full beforehand the largest element is first removed and then the insertion takes place.
 *
 * If the value to be inserted is larger than all others then it is immediately inserted at the end.
 * A element is created then inserted if its value is smaller than the largest element.
 *
 * The insertion is done by iterating over the elements in queue's list in the list until the correct
 * place for our new element is found. This happens when are value is larger than the previous
 * and smaller than the next value checked -  we then insert between the two.
 *
 * @param source - The source queue to which we add the new element.
 * @param element - The data of the element we want to insert into the queue. A copy of
 * the element will be inserted, created when creating the element to insert into source's list.
 *
 * @return
 * SSP_BPQUEUE_INVALID_ARGUMENT if a NULL was sent as queue
 * P_BPQUEUE_OUT_OF_MEMORY if an allocation failed
 * SP_BPQUEUE_SUCCESS the element has been inserted successfully
 */
SP_BPQUEUE_MSG spBPQueueEnqueue(SPBPQueue source, SPListElement element);

/**
 * Removes the smallest (first) element from the queue.
 * Removes by using SPList's iterator set to first element
 * and then using the RemoveCurrent function to return it and disconnect it from our queue's list.
 *
 * @param source - the queue from which we want to remove the first element.
 * @return
 * SSP_BPQUEUE_INVALID_ARGUMENT if a NULL was sent as queue or its list is NULL
 * SP_BPQUEUE_SUCCESS the element has been inserted successfully
 *
 */
SP_BPQUEUE_MSG spBPQueueDequeue(SPBPQueue source);

/**
 * Returns the smallest (first) element from the queue.
 * The state of the queue remains unchanged.
 *
 * @param source - the queue from which we want to return the first element.
 * @return
 * NULL if a NULL was sent as queue or its list is NULL or size is negative
 * Otherwise returns a copy of the first element in the queue.
 *
 */
SPListElement spBPQueuePeek(SPBPQueue source);

/**
 * Returns the largest (last) element from the queue.
 * The state of the queue remains unchanged.
 *
 * @param source - the queue from which we want to return the last element.
 * @return
 * NULL if a NULL was sent as queue or its list is NULL or size is negative
 * Otherwise returns a copy of the last element in the queue.
 *
 */
SPListElement spBPQueuePeekLast(SPBPQueue source);

/**
 * Returns the value of the minimal element (the first element)
 * Gets the first element of the list of the given source queue which is the smallest as we
 * maintain a sorted list with each Enqueue.
 *
 * @ param source - the queue given whose min value we want.
 * @ return
 * -1 if NULL was sent as queue or its list is NULL or size of queue is 0
 * Otherwise double - the minimal value of all elements in the queue
 */
double spBPQueueMinValue(SPBPQueue source);

/**
 * Returns the value of the maximal element (the last element)
 * Gets the last element of the list of the given source queue which is the largest as we
 * maintain a sorted list with each Enqueue.
 *
 * @ param source - the queue given whose max value we want.
 * @ return
 * -1 if NULL was sent as queue or its list is NULL or size of queue is 0
 * Otherwise double - the maximal value of all elements in the queue
 */
double spBPQueueMaxValue(SPBPQueue source);

/**
 * Checks if the queue is empty - meaning has no elements
 * by checking if the given source queue's list is empty- contains no elements.
 *
 * @param source - the queue given which we want to check
 * @assert source != NULL && source->list != NULL
 * @return
 *  True if the queue is empty (no elements in its list)
 *  Otherwise False
 */
bool spBPQueueIsEmpty(SPBPQueue source);

/**
 * Checks if the queue is full -
 * meaning it has the same number of elements a its given capacity
 * we do so by checking the size of the source queue's list.
 *
 * @param source - the queue given which we want to check
 * @assert source != NULL && source->list != NULL
 * @return
 *  True if the queue is full (size of list == capacity)
 *  Otherwise False
 */
bool spBPQueueIsFull(SPBPQueue source);

#endif
