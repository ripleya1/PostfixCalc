# Postfix Calc

A [postfix calculator](postfixCalc.c) written in C that given a user inputted mathematical expression in infix notation, calculates and outputs the expression in postfix notation.

Supports addition, subtraction, multiplication, division, modular division, and parenthesis.
Negative numbers are not natively supported. In order to input a negative value, you can use subtraction. For example, `0 - 42`.
Basic error checking is done, checking for basic syntax errors, such as operators or numbers being inputted directly after one another, and non-matching parentheses.

## How it works
### Getting User Input
[`getInput()`](https://github.com/ripleya1/PostfixCalc/blob/master/postfixCalc.c#L35)

Gets input using `fgets()`, tokenizes it, puts each token into a queue, and does some basic error checking on it. This error checking checks for basic syntax errors, such as operators or numbers being inputted directly after one another, and non-matching parentheses. If there is invalid syntax, the rest of the input is not tokenized and an error message is printed based on the kind of syntax error that was enocuntered.

### Converting from Infix to Postfix
[`infixToPostfix()`](https://github.com/ripleya1/PostfixCalc/blob/master/postfixCalc.c#L186)

Uses a [queue](queue.c) for the postfix expression, and a [stack](stack.c) for the operators. If we encounter a number, enqueue it, and if we encounter an operator, we keep adding operators to the queue until the stack is empty or we hit a non-first priority operator (ie not `*` or `/`), at which point we add the operator at our current element to the stack. For parentheses, we push the right paren to the stack, and keep adding operators to the queue until we get to the left one.

### Calculating using Postfix
[`calculatePostfix()`](https://github.com/ripleya1/PostfixCalc/blob/master/postfixCalc.c#L253)

Uses a [stack](stack.c). Push to the stack when we see numbers, and pop from it twice when we see operators. Push the result of doing the operator on the two values that we poppped off. Do this until we get to the end of the postfix expression, and the answer is the top of the stack.

### Printing Postfix Expression
[`queueToString()`](https://github.com/ripleya1/PostfixCalc/blob/master/postfixCalc.c#L138)

Uses the [queue](queue.c) from the Infix to Postfix function and basically just iterates through each element and prints it.

### Operators
[`toChar()`](https://github.com/ripleya1/PostfixCalc/blob/master/postfixCalc.c#L306), [`toModifiedASCII()`](https://github.com/ripleya1/PostfixCalc/blob/master/postfixCalc.c#L332)

Operators are handled by treating them as numbers in order to avoid having to frequently convert strings. Number representations of operators are calculated by subtracting its ASCII value, as found on the below webpage, from `INT32_MAX`. For example, `+` is represented as `INT32_MAX - 43`.
