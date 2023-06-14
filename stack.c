/*
Aaron Ripley
2/25/23
A Stack Manager. 
Allows someone using it to pass in the top node (as a pointer) to functions to perform operations on the entire stack (using a linked list).*/

#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

// creates an empty stack by returning a pointer to a stackNode
stackNode *makeEmptyStack(){
    return NULL;
}

// returns whether the stack is empty
int stackEmpty(stackNode * top){
    return top == NULL;
}

// adds element to top of stack
// does not allow EMPTY as a value
void push(stackNode **top, double value){
    if(!stackEmpty(*top)){ // > 0 elements case, set a temp variable to current top and set new top next to that, new top value set to param value
        if(value != EMPTY){ // make sure that value is valid, do nothing if it isn't
            stackNode *temp = malloc(sizeof(stackNode *));
            temp -> value = (*top) -> value;
            temp -> next = (*top) -> next;

            (*top) -> next = temp;
            (*top) -> value = value;       
        }
    }
    else{ // 0 elements case, only set top value, top next is null
        stackNode *temp = malloc(sizeof(stackNode *));
        temp -> value = value;
        temp -> next  = NULL;
        *top = temp;
    }
}

// removes and returns the value of the top of the stack
// returns EMPTY if stack is empty
double pop(stackNode **top){
    if(!stackEmpty(*top)){
        double topVal = (*top) -> value;
        if((*top) -> next == NULL){ // 1 element case, just nuke it
            free(*top);
            *top = NULL;
        }
        else{ // > 1 element case, set new top equal to current top's next
            stackNode *topNext = malloc(sizeof(stackNode *));
            topNext = (*top) -> next;

            (*top) -> value = topNext -> value; 
            (*top) -> next = topNext -> next;
        }
        return topVal;
    }
    return EMPTY; // 0 elements case
}

// returns the value of the top of the stack
// returns EMPTY if stack is empty
double top(stackNode *top){
    if(!stackEmpty(top)){
        return top -> value;
    }
    return EMPTY;
}