/*
 * SPBPQueue.c
 *
 *  Created on: May 29, 2016
 *      Author: shaneer
 */

#include "SPBPQueue.h"
#include "SPListElement.h"
#include "SPList.h"
#include <stdlib.h>
#include <assert.h>

struct sp_bp_queue_t {
    int capacity;
    SPList list;
} Queue;

SPBPQueue spBPQueueCreate(int maxSize){
    if (maxSize<1) {
        return NULL;
    }

    SPBPQueue queue = (SPBPQueue) malloc(sizeof(Queue));
    if (queue == NULL) {
        return NULL;
    } else {
        queue->list = spListCreate();
        if (queue->list == NULL) {
            queue->capacity = 0;
            free(queue);
            return NULL;
        }
        queue->capacity = maxSize;
        return queue;
    }
}

SPBPQueue spBPQueueCopy(SPBPQueue source){
    if (source == NULL) {
        return NULL;
    }
    int copyCapacity = spBPQueueGetMaxSize(source);
    SPBPQueue copyQueue = spBPQueueCreate(copyCapacity);
    if (copyQueue == NULL) {
        return NULL;
    }
    spListDestroy(copyQueue->list);
    copyQueue->list = spListCopy(source->list);
    if (copyQueue->list == NULL) {
        spBPQueueDestroy(copyQueue);
        return NULL;
    }
    copyQueue->capacity = copyCapacity;
    return copyQueue;
}

void spBPQueueDestroy(SPBPQueue source){
    if (source == NULL) {
        return;
    }
    spBPQueueClear(source);
    spListDestroy(source->list);
    free(source);
    return;
}

void spBPQueueClear(SPBPQueue source){
    if (source == NULL) {
        return;
    }
    spListClear(source->list);
    return;
}

int spBPQueueSize(SPBPQueue source){
    return source == NULL ? -1 : spListGetSize(source->list);
}

int spBPQueueGetMaxSize(SPBPQueue source){
    return source == NULL ? -1 : source->capacity;
}

double spBPQueueMinValue(SPBPQueue source){
    if (source == NULL || spBPQueueSize(source)==0 || spListGetFirst(source->list) == NULL) {
        return -1;
    }
    return spListElementGetValue(spListGetFirst(source->list));
}

double spBPQueueMaxValue(SPBPQueue source){
    if (source == NULL || spBPQueueSize(source)==0 || spListGetLast(source->list) == NULL) {
        return -1;
    }
    return spListElementGetValue(spListGetLast(source->list));
}

SP_BPQUEUE_MSG queueListMessageConvert(SP_LIST_MSG message) {
    switch (message) {
    case SP_LIST_NULL_ARGUMENT:
        return SP_BPQUEUE_INVALID_ARGUMENT;
    case SP_LIST_OUT_OF_MEMORY:
        return SP_BPQUEUE_OUT_OF_MEMORY;
    default:
        return SP_BPQUEUE_SUCCESS;
    }
}

SP_BPQUEUE_MSG insertElementToQueue(SPBPQueue source, SPListElement element) {
    double elementValue = spListElementGetValue(element);
    if (spListElementGetValue(spListGetFirst(source->list)) > elementValue) {
        // Adding element to the beginning of list since value(first) > value(element)
        return queueListMessageConvert(spListInsertFirst(source->list, element));
    }
    if (spListElementGetValue(spListGetLast(source->list)) < elementValue) {
        // Adding element to the end of list since value(last) < value(element)
        return queueListMessageConvert(spListInsertLast(source->list, element));
    }
    SP_LIST_FOREACH(SPListElement, e, source->list) {
        double eVal = spListElementGetValue(e);
        if (eVal > elementValue) {
            return queueListMessageConvert(spListInsertBeforeCurrent(source->list, element));
        }
    }
    return SP_BPQUEUE_SUCCESS;
}

SP_BPQUEUE_MSG spBPQueueEnqueue(SPBPQueue source, SPListElement element){
    // TODO: Is 'source->list == NULL' needed?
    if (source == NULL || element == NULL) {
        return SP_BPQUEUE_INVALID_ARGUMENT;
    }
    // Check for empty queue
    if (spBPQueueIsEmpty(source)) {
        return queueListMessageConvert(spListInsertFirst(source->list, element));
    }
    int value = spListElementGetValue(element);
    // Checks for full queue
    if (spBPQueueIsFull(source)) {
        if ( spBPQueueMaxValue(source)<value) {
            return SP_BPQUEUE_FULL;             // No need to add element
        }
        // Removing last element since value(last) > value(element) and queue is full
        spListGetLast(source->list);
        spListRemoveCurrent(source->list);
    }
    return insertElementToQueue(source, element);
}

SP_BPQUEUE_MSG spBPQueueDequeue(SPBPQueue source){
    if (source == NULL ||  source->list == NULL ) {
        return SP_BPQUEUE_INVALID_ARGUMENT;
    }
    if (spListGetFirst(source->list) == NULL) {
        return SP_BPQUEUE_EMPTY;
    }
    return queueListMessageConvert(spListRemoveCurrent(source->list));
}

SPListElement spBPQueuePeek(SPBPQueue source){
    if (source == NULL || spBPQueueSize(source)== 0 || spListGetFirst(source->list) == NULL) {
        return NULL;
    }
    return spListElementCopy(spListGetFirst(source->list));
}

SPListElement spBPQueuePeekLast(SPBPQueue source){
    if (source == NULL || spBPQueueSize(source)==0 || spListGetLast(source->list) == NULL) {
        return NULL;
    }
    return spListElementCopy(spListGetLast(source->list));
}

bool spBPQueueIsEmpty(SPBPQueue source){
    assert(source != NULL && source->list != NULL);
    if (spBPQueueSize(source) == 0) {
        return true;
    }
    return false;

}

bool spBPQueueIsFull(SPBPQueue source){
    assert(source != NULL && source->list != NULL);
    if (spBPQueueSize(source) == spBPQueueGetMaxSize(source)) {
        return true;
    }
    return false;
}
