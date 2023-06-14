#ifndef STACK
#define STACK

#include <stdint.h>

/************************** STACK.H ******************************
*
*  The externals declaration file for the Stack Manager Module
*
*  Written by Mikeyg
*/


typedef struct stackNode {
	double value;
	struct stackNode *next;
} stackNode;

#define TRUE  1
#define FALSE 0
#define EMPTY -INT32_MAX


extern stackNode *makeEmptyStack();
extern int stackEmpty(stackNode *top);
extern void push (stackNode **top, double newValue);
extern double top (stackNode *top);
extern double pop (stackNode **top);

/***************************************************************/

#endif
