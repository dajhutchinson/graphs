GCC = gcc -std=c99 -pedantic -Wall -O3

rgraph: graph.c graph.h
	$(GCC) -DgraphMain=main graph.c -o graph
	./graph

cgraph: graph.c graph.h
	$(GCC) -DgraphMain=main graph.c -o graph

rdij: dijkstra.c dijkstra.h graph.c graph.h
	$(GCC) -c graph.c
	$(GCC) -DmainDijk=main -c dijkstra.c
	$(GCC) graph.o dijkstra.o -o dij
	./dij

cdij: dijkstra.c dijkstra.h graph.c graph.h
	$(GCC) -c graph.c
	$(GCC) -DmainDijk=main -c dijkstra.c
	$(GCC) graph.o dijkstra.o -o dij
