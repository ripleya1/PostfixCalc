#ifndef QUEUE
#define QUEUE

#include <stdint.h>

/************************** QUEUE.H ******************************
*
*  The externals declaration file for the Queue Manager Module
*
*  Written by Mikeyg and Aaron Ripley
*/


typedef struct queueNode {
	double value;
	struct queueNode *next;
    struct queueNode *prev;
} queueNode;

#define TRUE  1
#define FALSE 0
#define EMPTY -INT32_MAX


extern queueNode *makeEmptyQueue();
extern int queueEmpty(queueNode *tp);
extern void enqueue (queueNode **tp, queueNode *p);
extern queueNode *front (queueNode *tp);
extern queueNode *dequeue (queueNode **tp);
extern queueNode *outQueue (queueNode **tp, queueNode *p);
extern queueNode *newNode(double val);

/***************************************************************/

#endif
