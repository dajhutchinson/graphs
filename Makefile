GCC = gcc -std=c99 -pedantic -Wall -O3

rgraph: graph.c graph.h
	$(GCC) -DgraphMain=main graph.c -o graph
	./graph

cgraph: graph.c graph.h
	$(GCC) -DgraphMain=main graph.c -o graph
