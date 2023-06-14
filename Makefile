CC = gcc

all: calculatorStuff

calculatorStuff: postfixCalc.o queue.o stack.o
	$(CC) postfixCalc.o queue.o stack.o -o myCalc -lm

clean:
	rm *.o
    
.c.o:
	$(CC) -c $< -o $@

