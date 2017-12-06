# GRAPHS

So this is a very rudimentary graph API in c.

The graph.c & graph.h files form the API. The functions work well, as long as you pass them valid data.

dijkstra.c & dijkstra.h use this API to perform the [Dijkstra Path Finding Algorithm](https://en.wikipedia.org/wiki/Dijkstra%27s_algorithm).
  Again this program works well, as long as you pass it valid data.

USAGE:
creating graph.txt files
- Check out 'data/rules.txt'.
graph.c
- Look at graph.h to see what functions are avaiable and their parameters;
- Running graph.c will run automatic tests on file.
dijkstra.c
- Running this program you will be asked to enter the name of the graph you wish to load (the root path of this file is best);
- Then you will be asked to enter start and end locations;
- Enter '.' to end the program;
- Enter ',' to run tests.

KNOWN ISSUES:
(There are many, but the most notework)
- If you request to find the path between two locations between which no path exists, then the program will crash;
- Spelling errors cause the program to crash.

RECOMMENDED COMPILATION COMMANDS:
graph.c
- gcc -std=c99 -DgraphMain=main graph.c -o graph
dijkstra.c
- gcc -std=c99 -c graph.c
- gcc -std=c99 -DmainDijk=main -c dijkstra.c
- gcc -std=c99 graph.o dijkstra.o -o dij
