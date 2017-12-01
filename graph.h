#include <stdbool.h>

struct graph;
struct vertex;
typedef struct graph graph;
typedef struct vertex vertex;

// Create a new graph, requires an initial node
graph* newGraph(char* name);

// Add a vertex to a graph
void addVertex(char* name, graph* g);

//Create an edge between two vertices
void connectVertices(int w, vertex* s, vertex* e);

// Output all vertices in a given graph
void outputVertices(graph* g);

// Output destination & weight of all edges from a given vertex
void outputEdges(vertex* v);

// Return pointer to vertex of a given name
vertex* fromName(char* s, graph* g);
vertex* fromPos(int n, graph* g);

// Create a graph from a file, see 'data/rules.txt' for formatting
graph* fromFile (char* file);

// Get details of graph
int numOfVertices(graph* g);
vertex** arrayOfVertices(graph* g);

// Get details of edges
int cost(vertex* s, vertex* e);
bool isConnection(vertex* s, vertex* e);

// Get details of vertices
char* name(vertex* v);
vertex** dests(vertex* v, graph* g);
int numberConnections(vertex* s, graph* g);
