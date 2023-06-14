/*
Aaron Ripley
4/26/23
A postfix calculator. Given a user inputted mathematical expression in infix notation, it outputs and calculates the expression in postfix notation.
It supports addition, subtraction, multiplication, division, modular division, and parenthesis. 
Negative numbers are not natively supported. In order to input a negative value, you can use subtraction. For example, 0 - 42.
Basic error checking is done, checking for basic syntax errors, such as operators or numbers being inputted directly after one another, and non-matching parentheses.
Operators are treated as numbers within the program in order to avoid having to frequently convert strings.
Their number representations are calculated by subtracting its ASCII value, as found on the below webpage, from INT32_MAX. For example, + is represented as INT32_MAX - 43.
https://www.cs.cmu.edu/~pattis/15-1XX/common/handouts/ascii.html
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "postfixCalc.h"
#include "queue.h"
#include "stack.h"

int main(){
    queueNode *infixQueue = getInput();
    queueNode *postfixQueue = infixToPostfix(infixQueue);
    printf("%s", "\nEquation in postfix: ");
    queueToString(postfixQueue);
    printf("%s%f%s", "\nAnswer: ", calculatePostfix(postfixQueue), "\n");
}

// gets the user input of the infix expression, tokenizes it, puts each token into a queue, and does some basic error checking on it,
// which checks for basic syntax errors, such as operators or numbers being inputted directly after one another, and non-matching parentheses
// preconditions: none
// params: none
// returns: a queue, with each token of the (hopefully valid) user input being an individual element
// postconditions: none
queueNode* getInput(){
    char *inputStr;
    inputStr = malloc(200 * sizeof(char));
    char *whitespace = " \t\f\r\v\n";
    queueNode *retQueue = makeEmptyQueue();
    int cont = TRUE;

    printf("%s", "Enter an arithmetic expression. Put one space between each operator and operand (including parenthesis), ex. ( 5 + 5 ) * 5.\n");
    printf("%s", "Supported operators: +, -, *, /, %, (, ).\n\n");
    
    while(cont){ // continue until the user does a valid input
        cont = FALSE; // ititially assume that the input is valid so we don't keep going
        printf("%s", "> ");

        inputStr = fgets(inputStr, 200 * sizeof(char), stdin); // get user input

        // tokenize and enqueue each token
        char *tokenizedStr = strtok(inputStr, whitespace);
        int prevToken = EMPTY, leftParenCount = 0, rightParenCount = 0;
        while(tokenizedStr != NULL){            
            int invalid = FALSE;
            int toBeEnqueued = toModifiedASCII(tokenizedStr); // convert the token to a modified ascii value

            // count parens
            if(getTokenType(toBeEnqueued) == 2){
                leftParenCount++;
            }
            else if(getTokenType(toBeEnqueued) == 3){
                rightParenCount++;
            }

            // basic error checking
            if(toBeEnqueued == -1){
                invalid = TRUE;
                printf("%s", "Unrecognized token, ");
            }
            else if(getTokenType(prevToken) == getTokenType(toBeEnqueued)){ // if the token type of the previous token was the same
                if(getTokenType(prevToken) != 2 && getTokenType(prevToken) != 3){ // if they weren't parens
                    invalid = TRUE;
                    printf("%s", "Invalid syntax, ");
                }
            }
            else if((getTokenType(prevToken) == 3 && (getTokenType(toBeEnqueued) == 2) || getTokenType(prevToken) == 2 && getTokenType(toBeEnqueued) == 3)){ // if the parens were back to back ie ) ( or open with nothing in them ie ( )
                invalid = TRUE;
                printf("%s", "Invalid syntax, ");
            }

            if(invalid){
                printf("%s", "reinput expression.\n");
                // clear queue
                while(!queueEmpty(retQueue)){
                    dequeue(&retQueue);
                }
                cont = TRUE; // keep getting user input
                break; // start over
            }

            enqueue(&retQueue, newNode(toBeEnqueued)); // enqueue it since it's valid
            tokenizedStr = strtok(NULL, whitespace); // get the next token
            prevToken = toBeEnqueued;
        }
        // parens don't match case
        if(leftParenCount != rightParenCount){
            printf("%s", "Parentheses do not match, reinput expression.\n");
            // clear queue
            while(!queueEmpty(retQueue)){
                dequeue(&retQueue);
            }
            cont = TRUE; // keep getting user input
        }
    }

    free(inputStr);
    return retQueue;
}

// given the "in program" representation a token, returns what type of token it is
// preconditions: none
// params: the "in program" representation of the token whose type is being checked
// returns: -1 for unrecognized, 0 for number, 1 for operator, 2 for left paren, 3 for right paren
// postconditions: none
int getTokenType(double token){
    if(isNumber(token)){ // number case
        return 0;
    }
    if(token == toModifiedASCII("(")){ // left paren case
        return 2;
    }
    if(token == toModifiedASCII(")")){ // right paren case
        return 3;
    }
    if(toChar(token) != ""){ // operator (not paren) case
        return 1;
    }
    // unrecognized token case
    return -1;
}

// prints all elements of a queue, starting at the tail's next, with some basic reformatting for floats
// preconditions: the parameter queue is not null ie has at least one element
// params: the queue to be printed
// returns: none
// postconditions: the queue is printed
void queueToString(queueNode* aQueue){
    // print the tail's next (ie head)
    if(toChar(aQueue -> next -> value) == "0"){ // number case
        if(fmod(aQueue -> next -> value, 1) == 0){ // x.0 case
            printf("%d ", (int)(aQueue -> next -> value)); // pretend it's an int
        }
        else{ // decimal case
            printf("%f ", aQueue -> next -> value);
        }
    }
    else{ // operator case
        printf("%s ", toChar(aQueue -> next -> value));
    }

    queueNode *temp = aQueue -> next -> next; // start at the head's next

    while(TRUE){ // keep going until break statement
        // print temp's value
        if(toChar(temp -> value) == "0"){ // number case
            if(fmod(temp -> value, 1) == 0){ // x.0 case
                printf("%d ", (int)(temp -> value)); // pretend it's an int
            }
            else{ // decimal case
                printf("%f ", temp -> value);
            }
        }
        else{ // operator case
            printf("%s ", toChar(temp -> value)); 
        }

        temp = temp -> next; // iterate temp
        if(temp == aQueue -> next){ // end the loop if it comes back to the head
            break;
        }
    }
}

// converts the user inputted infix expression into a postfix expression
// using a queue for the postfix expression, and a stack for the operators
// if we encounter a number, enqueue it, and if we encounter an operator, 
// we keep adding operators to the queue until the stack is empty or we hit a non-first priority operator (ie not * or /),
// at which point we add the operator at our current element to the stack
// for parens, we push the right paren to the stack, and keep adding operators to the queue until we get to the left one
// preconditions: the parameter queue is not null, ie it has at least one element,
// and is a representation of a postfix expression, with each element being an "in program" representation of a token of the expression
// params: the user inputted infix expression, as a queue
// returns: a queue with the postfix representation of the infix expression
// postconditions: none
queueNode* infixToPostfix(queueNode* infixQueue){
    queueNode* postfixQueue = makeEmptyQueue();
    stackNode* opStack = makeEmptyStack();

    queueNode *temp = infixQueue -> next; // start at the head of the queue
    while(TRUE){ // iterate through the queue until break statement
        // number case
        if(isNumber(temp -> value)){
            // negative number case
            if(temp -> value < 0){
                enqueue(&postfixQueue, newNode(0));
                
                double val = temp -> value * -1;
                enqueue(&postfixQueue, newNode(val));
                
                push(&opStack, toModifiedASCII("-"));
            }
            // positive number case
            else{
                enqueue(&postfixQueue, newNode(temp -> value));
            }
        }
        // non number case
        else{
            // left parenthesis case
            if(temp -> value == toModifiedASCII("(")){
                push(&opStack, temp -> value);
            }            
            else if(temp -> value == toModifiedASCII(")")){
                // keep adding operators to the queue until we get to the left parenthesis
                while(!stackEmpty(opStack) && top(opStack) != toModifiedASCII("(")){
                    enqueue(&postfixQueue, newNode(pop(&opStack)));
                }
                pop(&opStack);
            }
            // normal operator case
            else{
                // keep adding operators to the queue until the stack is empty or we hit a non first priority operator
                while(!stackEmpty(opStack) && isFirstPriority(top(opStack))){
                    enqueue(&postfixQueue, newNode(pop(&opStack)));
                }
                // add the operator at our current element to the stack
                push(&opStack, temp -> value);
            }
        }

        temp = temp -> next; // iterate temp
        if(temp == infixQueue -> next){ // end the loop if it comes back to the head
            break;
        }
    }
    // put all the remaining operators in the postfix queue
    while(!stackEmpty(opStack)){
        enqueue(&postfixQueue, newNode(pop(&opStack)));
    }
    return postfixQueue;
}

// calculates a mathematical expression in postfix notation using a stack
// push to the stack when we see numbers, and pop from it twice when we see operators,
// push the result of doing the operator on the two values that we poppped off,
// do this until we get to the end of the postfix expression, and the answer is the top of the stack
// preconditions: the parameter queue is not null, ie it has at least one element,
// and is a representation of a postfix expression, with each element being an "in program" representation of a token of the expression
// params: a queue with an expression in postfix notation
// returns: the result at the top of the stack at the end, ie the answer
// postconditions: none 
double calculatePostfix(queueNode* postfixQueue){
    stackNode *calcStack = makeEmptyStack();
    
    queueNode *temp = postfixQueue -> next; // start at head of queue

    while(TRUE){ // keep going until break statement
        if(isNumber(temp -> value)){ // number case
            push(&calcStack, temp -> value); // push numbers to the stack until we find an operator
        }
        else{ // operator case
            double num1 = pop(&calcStack);
            double num2 = pop(&calcStack);

            if(temp -> value == toModifiedASCII("+")){push(&calcStack, num2 + num1);}
            if(temp -> value == toModifiedASCII("-")){push(&calcStack, num2 - num1);}
            if(temp -> value == toModifiedASCII("*")){push(&calcStack, num2 * num1);}
            if(temp -> value == toModifiedASCII("/")){push(&calcStack, num2 / num1);}
            if(temp -> value == toModifiedASCII("%")){push(&calcStack, fmod(num2, num1));}
        }        
        temp = (temp -> next); // iterate temp
        if(temp == postfixQueue -> next){ // end the loop if it comes back to the head
            break;
        }
    }
    return top(calcStack);
}

// returns true if i represents a number, ie is not an operator or paren
// preconditions: none
// params: the number whose representation is being checked
// returns: TRUE or FALSE
// postconditions: none
int isNumber(double i){
    return(i < INT32_MAX - 47); 
}

// returns true if i represents a first priority operator, ie * or /
// preconditions: none
// params: the number whose representation is being checked
// returns: TRUE or FALSE
// postconditions: none
int isFirstPriority(double i){
    return(i == INT32_MAX - 42 || i == INT32_MAX - 47);
}

// converts the "in program" representation of a number (i) to a character representation,
// in the case of i representing a number, the conversion is handled outside of the function so "0" is returned,
// in the case of i representing an operator, the operator it corresponds to is returned,
// in the case of an unrecognized token, "" is returned
// preconditions: none
// params: the number which is being converted 
// returns: "0" if number, the operator if operator, "" otherwise
// postconditions: none
char* toChar(double i){
    if(isNumber(i)){ // number case
        return "0";
    }
    // operator cases
    if(i == INT32_MAX - 37){return "%";}
    if(i == INT32_MAX - 40){return "(";}
    if(i == INT32_MAX - 41){return ")";}
    if(i == INT32_MAX - 42){return "*";}
    if(i == INT32_MAX - 43){return "+";}
    if(i == INT32_MAX - 45){return "-";}
    if(i == INT32_MAX - 46){return ".";}
    if(i == INT32_MAX - 47){return "/";}
    // unrecognized token case
    return "";
}

// converts characters to their "in program" representation, 
// which is done by subtracting the ascii value of the symbol from INT32_MAX if it's an operator
// and using the built in atoi function if it's a number
// preconditions: c is not null
// params: the character which is being converted
// returns: the number if c is a number, 
// the "in program" representation of the number if c is an operator, 
// -1 if not a recognized symbol
// postconditions: none
double toModifiedASCII(char *c){
    if(atoi(c) != 0){ // number case
        return atoi(c);
    }
    else{ // operator & 0 cases
        if(strcmp(c, "0") == 0){return 0;}
        if(strcmp(c, "%") == 0){return INT32_MAX - 37;}
        if(strcmp(c, "(") == 0){return INT32_MAX - 40;}
        if(strcmp(c, ")") == 0){return INT32_MAX - 41;}
        if(strcmp(c, "*") == 0){return INT32_MAX - 42;}
        if(strcmp(c, "+") == 0){return INT32_MAX - 43;}
        if(strcmp(c, "-") == 0){return INT32_MAX - 45;}
        if(strcmp(c, ".") == 0){return INT32_MAX - 46;}
        if(strcmp(c, "/") == 0){return INT32_MAX - 47;}
    }
    // unrecognized token case
    return -1;
}
