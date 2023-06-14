#ifndef POSTFIXCALC
#define POSTFIXCALC

#include <stdint.h>
#include "queue.h"

/************************ POSTFIXCALC.H ******************************
*
*  The externals declaration file for the postfix calculator
*
*  Written by Mikeyg and Aaron Ripley
*/


#define TRUE  1
#define FALSE 0
#define EMPTY -INT32_MAX


extern queueNode* getInput();
extern int getTokenType(double token);
extern queueNode* infixToPostfix(queueNode* infixQueue);
extern void queueToString(queueNode* aQueue);
extern double calculatePostfix(queueNode* postfixQueue);
extern int isNumber(double i);
extern int isFirstPriority(double i);
extern char* toChar(double i);
extern double toModifiedASCII(char* c);

/***************************************************************/

#endif
