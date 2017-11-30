#include <stdbool.h>

struct graph;
struct edge;
struct vertex;
typedef struct graph graph;
typedef struct edge edge;
typedef struct vertex vertex;

// Create a new graph, requires an initial node
graph* newGraph(char* name);

// Add a vertex to a graph
void addVertex(char* name, graph* g);

//Create an edge between two vertices
void connectVertices(int w, vertex* s, vertex* e);

// Output destination & weight of all edges from a given vertex
void outputEdges(vertex* v);

// Return pointer to vertex of a given name
vertex* fromName(char* s, graph* g);

// Output all vertices in a given graph
void outputVertices(graph* g);

// Create a graph from a file, see 'data/rules.txt' for formatting
graph* fromFile (char* file);
