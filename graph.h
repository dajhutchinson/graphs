/*
 *  This is the header file for graph.c
 *  graph.c is an API for graph theory in c
 *  Note: A vertex is a location;
 *        An edge is a connection between two vertices;
 *        Edges an uni-directional.
 *
 *  COMPILE: <graph.c, graph.h>
 *  gcc -std=c99 -pedantic -Wall -O3 -DgraphMain=main graph.c -o graph
 *
 *  RUN:
 *  ./graph
 */

#include <stdbool.h>

struct graph;
struct vertex;
typedef struct graph graph;
typedef struct vertex vertex;

// Create a new graph, requires name for first vertex.
graph* newGraph(char* name);

// Add a vertex to a graph.
void addVertex(char* name, graph* g);

// Create a connection between two vertices
// NOTE: w = weighting of edge; s = start vertex; e = end vertex.
void connectVertices(int w, vertex* s, vertex* e);

// Prints name of all vertices in a given graph.
void outputVertices(graph* g);

// Prints list of all edges leaving a given vertex, including name of destination & weighting of edge.
// If a vertex has no edges leaving it "No edges from <name>." is printed.
void outputEdges(vertex* v);

// Returns pointer to vertex with a given name, or in a given index a given graph's list of vertices.
// Returns NULL is no such vertex.
vertex* fromName(char* s, graph* g);
vertex* fromPos(int n, graph* g);

// Create a graph from a file, see 'data/rules.txt' for formatting.
graph* fromFile (char* file);

// Returns the number of vertices in a given graph.
int numOfVertices(graph* g);

// Returns an array of pointers to all the vertices in a given graph.
vertex** arrayOfVertices(graph* g);

// Return the weighting on an edge between two given vertices.
// Returns 9999 if there is no such edge.
// NOTE: s = start vertex; e = end vertex.
int cost(vertex* s, vertex* e);

// Returns true if there is an edge between two given vertices, false otherwise.
// NOTE: s = start vertex; e = end vertex.
bool isConnection(vertex* s, vertex* e);

// Returns name of a given vertex.
char* name(vertex* v);

// Returns an array of pointers to all the vertices which can be travelled to from a given vertex.
// Returns an array of NULLs if there are no such vertices.
vertex** dests(vertex* v, graph* g);

// Returns the number of edges away from a given vertex.
int numberConnections(vertex* s, graph* g);
