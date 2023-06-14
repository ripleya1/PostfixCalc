/*
Aaron Ripley
4/26/23
A Queue Manager.
Allows someone to pass in the tail pointer (tp) queueNode (as a pointer) to functions 
to perform operations on the entire queue (which uses a linked list).
Each queueNode has a value, with next and previous pointers.

One can initialize a queueNode using makeEmptyQueue() and add to the back of the queue using enqueue(*tp, val).
One can determine if the queue is empty using queueEmpty(tp), and find the value at the front of the queue using front(tp).
One can remove from the front of the queue using dequeue(*tp), which also returns the value that was removed.*/

#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

// creates an empty queue by returning a pointer to a queueNode
// preconditions: none
// params: none
// returns: a queueNode *, the newly created node
// postconditions: none
queueNode *makeEmptyQueue(){
    return NULL;
}

// returns whether the queue is empty
// preconditions: none
// params: tail pointer
// returns: an int, 1 or 0 (ie true or false)
// postconditions: none
int queueEmpty(queueNode *tp){
    return tp == NULL;
}

// adds element to tail of queue, does not allow the pointer to be NULL
// preconditions: p is not NULL
// params: pointer to tail pointer, value to be added to queue
// returns: none
// postconditions: the tail pointer is replaced with a new tail containing the new value, 
// with the old value still being pointed to by other nodes
void enqueue(queueNode **tp, queueNode *p){
    if(p != NULL){
        if(queueEmpty(*tp)){ // 0 elements case, tail points to itself
            *tp = p;
            p -> next = p;
            p -> prev = p;
        }
        else{ // > 0 elements case set the tail pointer to the data in p, and rearrange other pointers
            p -> next = (*tp) -> next; // p -> next = head
            p -> prev = (*tp); // p -> prev = old tail

            (*tp) -> next -> prev = p; // head -> prev = p
            (*tp) -> next = p;
            (*tp) = p;
        }
    }
}

// removes and returns the value at the tail pointer's next (ie the head) of the queue, NULL if queue is empty
// preconditions: queue has at least one element (otherwise the queue won't be changed), and val is not the same as NULL
// params: pointer to tail pointer
// returns: the head pointer, or NULL if the queue is empty
// postconditions: the tail pointer's next (ie the head) is removed from the queue
queueNode *dequeue (queueNode **tp){
    if(!queueEmpty(*tp)){
        // temp variable of the value to be removed
        queueNode *head = (*tp) -> next;

        if((*tp) -> next == (*tp)){ // 1 element case, just nuke it
            *tp = NULL;
        }
        else{ // > 1 element case, set new head equal to current head's next, and new head's previous to tail
            (*tp) -> next = head -> next; // head = head -> next
            (*tp) -> next -> prev = (*tp); // head -> prev = (*tp)
        }

        // unlink from rest of queue
        head -> next = NULL;
        head -> prev = NULL;
        return head; // return dequeued value
    }
    return NULL; // 0 elements case
}

// returns the head of the queue, NULL if queue is empty
// preconditions: none
// params: tail pointer
// returns: a pointer to a queueNode, the head
// postconditions: none
queueNode *front(queueNode *tp){
    if(!queueEmpty(tp)){
        return tp -> next; // head
    }
    return NULL;
}

// removes and returns the value that p points to, NULL if the queue is empty or p is not in the queue
// preconditions: p is in queue and queue has at least one element (otherwise the queue won't be changed)
// params: pointer to tail pointer, pointer to the node to be removed
// returns: pointer to the node that was removed, or NULL if the queue is empty
// postconditions: p is removed from the queue
queueNode *outQueue(queueNode **tp, queueNode *p){
    // p not there, tp empty case
    if(queueEmpty(*tp)){
        return NULL;
    }

    queueNode *curr = (*tp); // start iterating at tail pointer

    if(curr == p){ // p is the tail cases
        if(curr -> next == curr){ // 1 element case, just nuke it
            return dequeue(&(*tp)); // nuke and return tp using dequeue (goes to the 1 element case)
        }
        else{ // > 1 element case
            queueNode *ret  = curr; // temp variable of the value to be removed

            queueNode *newTP = (*tp) -> prev; // temp variable that holds the new TP
            queueNode *newTPNext = (*tp) -> next; // temp variable that holds the new TP's next

            (*tp) = newTP; // tp = tp -> prev, ie the tp moves back one
            (*tp) -> next = newTPNext; // tp -> next = tp -> next, ie tp -> next is set to the old head

            // unlink from rest of queue
            ret -> prev = NULL;
            ret -> next = NULL;
            return ret;
        }
    }

    curr = curr -> next; // iterate curr to head

    // p is the head case
    if(curr == p){
        return dequeue(&(*tp)); // remove using dequeue (goes to the > 1 element case)
    }

    curr = curr -> next; // iterate curr to head -> next

    // p in the middle case, if it gets all the way through the while loop without finding p, it's not in the queue
    while(curr != (*tp)){
        if(curr == p){ // if p found, set curr's prev's next to curr's next, and set curr's next's prev to curr's prev
            queueNode *ret = curr; // temp variable of the value to be removed

            // temp variables of the prev and next pointers
            queueNode *oldPrev = curr -> prev;
            queueNode *oldNext = curr -> next;

            // bypass where curr used to be
            oldPrev -> next = oldNext; // curr -> prev -> next = curr -> next
            oldNext -> prev = oldPrev; // curr -> next -> prev = curr -> prev

            // unlink from rest of queue
            ret -> prev = NULL;
            ret -> next = NULL;
            return ret;
        }

        curr = curr -> next; // iterate curr if p not found
    }

    return NULL; // p not found, tp not empty case
}

// returns a newly allocated node with the given value
// preconditions: none
// params: value of new node
// returns: a pointer to a queueNode, the new node
// postconditions: a new node is allocated in memory
queueNode *newNode(double val){
    queueNode *ret = malloc(sizeof(queueNode));
    ret -> value = val;
    ret -> prev = NULL;
    ret -> next = NULL;
    return ret;
}