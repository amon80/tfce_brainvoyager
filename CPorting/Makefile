all:
	gcc -c -g Queue.c Utilities.c Tfce.c
	gcc -o output.out -g main.c Queue.o Utilities.o Tfce.o -lm

clean:
	rm *.o
	rm output.out
	rm -r output.out.dSYM
