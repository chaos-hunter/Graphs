a4: main.c graph.h graph_functions.c
	gcc -Wall -std=c99 -pedantic main.c graph_functions.c -o a4
clean:
	rm *.o a4