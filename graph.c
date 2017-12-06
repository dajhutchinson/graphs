#include "graph.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

// A collection of vertices
struct graph {
  int n; //Number of vertices in a graph
  vertex** vertices;
};

// A point on the graph
struct vertex {
  char* name;
  int n; // Number of edges from this graph
  edge** edges;
};

// Connect to vertices, one way
struct edge {
  int weight;
  vertex* start;
  vertex* end;
};

// Convert a string to an integer
int intFromString(int l, char* s) {
  int n = 0;
  for (int k = 0; k < l; k++) {
    int p = (int) s[k] - '0';
    for (int i = 0; i < (l - (k +1)); i++) p = p * 10;
    n += p;
  }
  return n;
}

// Create a new vertex, requires a name
vertex* newVertex(char* string) {
  vertex* new = malloc(sizeof(vertex));
  new -> name = string;
  new -> n = 0;
  new -> edges = malloc(sizeof(edge) * new -> n);
  return new;
}

// Create a new graph, requires name for first vertex.
graph* newGraph(char* name) {
  vertex* v = newVertex(name);
  graph* new = malloc(sizeof(graph));
  new -> n = 1;
  new -> vertices = malloc(sizeof(vertex) * new -> n);
  new -> vertices[0] = v;
  return new;
}

// Create a connection between two vertices
// w is the weighting on the edge, s is starting vertex, e is the end vertex
edge* newEdge(int w, vertex* s, vertex* e) {
  edge* new = malloc(sizeof(edge));
  new -> weight = w;
  new -> start = s;
  new -> end = e;
  return new;
}

// Add a vertex to a graph
void addVertex(char* name, graph* g) {
  vertex* v = newVertex(name);
  g -> n ++;
  g -> vertices = realloc(g -> vertices, sizeof(vertex) * g -> n);
  g -> vertices[(g -> n) - 1] = v;
}

// Create a connection between two vertices
// NOTE: w = weighting of edge; s = start vertex; e = end vertex.
void connectVertices(int w, vertex* s, vertex* e) {
  edge* ed = newEdge(w, s, e);
  s -> n++;
  s -> edges = realloc(s -> edges, sizeof(edge) * s -> n);
  s -> edges[(s -> n) - 1] = ed;
}

// Returns an array of pointers to all the vertices in a given graph.
vertex** arrayOfVertices(graph* g) {
  return g -> vertices;
}

// Returns an array of pointers to all the edges from a given vertex
edge** arrayOfEdges(vertex* v) {
  return v -> edges;
}

// Returns the number of vertices in a given graph.
int numOfVertices(graph* g) {
  return g -> n;
}

// Returns true if there is an edge between two given vertices, false otherwise.
// NOTE: s = start vertex; e = end vertex.
bool isConnection(vertex* s, vertex* e) {
  for (int k = 0; k < s -> n; k++) if (s -> edges[k] -> end == e) return true;
  return false;
}

// Return the weighting on an edge between two given vertices.
// Returns 9999 if there is no such edge.
// NOTE: s = start vertex; e = end vertex.
int cost(vertex* s, vertex* e) {
  if (!isConnection(s, e)) return 9999;
  for (int k = 0; k < s -> n; k++) if (s -> edges[k] -> end == e) return s -> edges[k] -> weight;
  return 9999;
}

// Returns the number of edges away from a given vertex.
int numberConnections(vertex* s, graph* g) {
  int n = 0;
  for (int k = 0; k < numOfVertices(g); k++) if (isConnection(s, g -> vertices[k])) n++;
  return n;
}

// Returns name of a given vertex.
char* name(vertex* v) {
  return v -> name;
}

// Returns pointer to vertex with a given name.
// Returns NULL is no such vertex.
vertex* fromName(char* s, graph* g) {
  for (int k = 0; k < g -> n; k++) if (strcmp(g -> vertices[k] -> name, s) == 0) return g -> vertices[k];
  return NULL;
}

// Returns pointer to vertex at a given index a given graph's list of vertices.
// Returns NULL is no such vertex.
vertex* fromPos(int n, graph* g) {
  if (n < numOfVertices(g)) return g -> vertices[n];
  return NULL;
}

// Prints list of all edges leaving a given vertex, including name of destination & weighting of edge.
// If a vertex has no edges leaving it "No edges from <name>." is printed.
void outputEdges(vertex* v) {
  if (v -> n == 0) {printf("No edges from %s.\n", v -> name); return;}
  for (int k = 0; k < v -> n; k++) {
    printf("%s -> %s <%i>\n", v -> name, v -> edges[k] -> end -> name, v -> edges[k] -> weight);
}}

// Returns an array of pointers to all the vertices which can be travelled to from a given vertex.
// Returns an array of NULLs if there are no such vertices.
vertex** dests(vertex* v, graph* g) {
  vertex** dest = malloc(sizeof(vertex) * numOfVertices(g));
  for (int k = 0; k < numOfVertices(g); k++) dest[k] = NULL;
  int n = 0;
  for (int k = 0; k < numOfVertices(g); k++) if (isConnection(v, fromPos(k, g))) {dest[n] = fromPos(k,g); n++;}
  return dest;
}

// Prints name of all vertices in a given graph.
void outputVertices(graph* g) {
  printf("  {");
  for (int k = 0; k < g -> n; k++) {
    if (k != (g -> n)-1) printf("%s, ", g -> vertices[k] -> name);
    else printf("%s", g -> vertices[k] -> name);
  }
  printf("}\n");
}

// Begins creating graph from file
graph* newGraphFromFile (char* line) {
  char* name = malloc(sizeof(char) * strlen(line));
  for (int k = 0; k < strlen(line) + 1; k++) name[k] = '\0';
  int n = 0;
  while(line[n] != ';') {name[n] = line[n]; n++;}
  graph* g = newGraph(name);
  return g;
}

// Adds vertices to graph from file
graph* nextVertex (char* line, int n, graph* g) {
  int pos = n;
  for (int k = 0; k < n; k++) pos += strlen(g -> vertices[k] -> name);
  char* name = malloc(sizeof(char) * strlen(line));
  for (int k = 0; k < strlen(line) + 1; k++) name[k] = '\0';
  n = 0;
  while((line[pos] != ';') && (line[pos] != '.')) {name[n] = line[pos]; n++; pos++;}
  addVertex(name, g);
  return g;
}

// Add vertices from the file
graph* verticesFromFile(char* line) {
  graph* g = newGraphFromFile(line);
  int count = 1;
  int pos = strlen(g -> vertices[0] -> name);
  while (line[pos] != '.') {
    g = nextVertex(line, count, g);
    count++;
    pos = count - 1;
    for (int k = 0; k < count; k++) pos += strlen(g -> vertices[k] -> name);
  }
  return g;
}

// Add edges from file
graph* edgesFromFile(FILE* in, graph* g) {
  int max = 1000;
  char* line = malloc(sizeof(char) * max);
  if (fgets(line, max, in) != NULL) {
    char* weight = malloc(sizeof(char) * 200);
    char* start = malloc(sizeof(char) * 200);
    char* end = malloc(sizeof(char) * 200);
    while (! feof(in)) {
      int n = 0; int pos = 0; int place = 0; // 0 = start node, 1 = end node; 2 = weight.
      for (int i = 0; i < 200; i++) {weight[i] = '\0'; start[i] = '\0'; end[i] = '\0';}
      while (line[pos] != ';') {
        if (line[pos] == '-') {
          pos++; place = 1; n = pos + 1;
        } else if (line[pos] == '<') {
          place = 2; n = pos + 1;
        } else if (line[pos] == '>') {
          break;
        } else if (place == 0) start[pos-n] = line[pos];
        else if (place == 1) end[pos-n] = line[pos];
        else if (place == 2) weight[pos-n] = line[pos];
        pos++;
      }
      connectVertices(intFromString(strlen(weight), weight), fromName(start, g), fromName(end, g));
      if (fgets(line, max, in) != NULL);
    }
  }
  return g;
}

// Create a graph from a file, see 'data/rules.txt' for formatting.
graph* fromFile(char* file) {
  if (file[strlen(file)-1] == '\n') file[strlen(file)-1] = '\0';
  int max = 200;
  FILE *in = fopen(file, "r");
  char* line = malloc(sizeof(char) * max);
  if (fgets(line, max, in) != NULL) {
    graph* g = verticesFromFile(line);
    g = edgesFromFile(in, g);
    return g;
  }
  return NULL;
}

// Remove an edge from a vertex
void deleteEdge(edge* e, vertex* v) {
  int en = 0;
  for (int k = 0; k < v -> n; k++) if (v -> edges[k] == e) {en = k; break;}
  free(v -> edges[en]);
  for (int k = en; k < v -> n - 1; k++) v -> edges[k] = v -> edges[k+1];
  v -> n--;
  v -> edges = realloc(v -> edges, sizeof(edge) * v -> n);
}

// Remove a vertex from a graph
void deleteVertex(vertex* v, graph* g) {
  int vn = 0;
  for (int k = 0; k < g -> n; k++) if (g -> vertices[k] == v) {vn = k; break;}
  for (int k = 0; k < v -> n; k++) deleteEdge(v -> edges[k], v);
  free(g -> vertices[vn]);
  for (int k = vn; k < g -> n - 1; k++) g -> vertices[k] = g -> vertices[k+1];
  g -> n--;
  g -> vertices = realloc(g -> vertices, sizeof(vertex) * g -> n);
}

// Delete a graphs
void deleteGraph(graph* g) {
  int i = g -> n;
  for (int k = 0; k < i; k++) deleteVertex(fromPos(0, g), g);
  free(g);
}

// Test intFromString()
void testConversion() {
  assert(intFromString(1, "0") == 0);
  assert(intFromString(6, "999999") == 999999);
}

// Test that pointers returned from vertex return functions work
void testPointers() {
  graph* g = newGraph("Start");
  addVertex("Middle", g);
  assert(g -> vertices[0] == fromName("Start", g));
  assert(g -> vertices[0] == fromPos(0, g));
  assert(g -> vertices[(g -> n) - 1] == fromName("Middle", g));
  assert(g -> vertices[(g -> n) - 1] == fromPos(1, g));
  assert(arrayOfVertices(g)[0] == fromPos(0, g));
  assert(arrayOfVertices(g)[1] == fromPos(1, g));
}

// Test creation of graphs, vertices & edges
void testCreation() {
  graph* g = newGraph("Start");
  assert(strcmp(g -> vertices[(g -> n) - 1] -> name, "Start") == 0);
  assert(g -> n == 1);
  addVertex("Middle", g);
  assert(strcmp(g -> vertices[(g -> n) - 1] -> name, "Middle") == 0);
  assert(g -> n == 2);
  connectVertices(10, fromName("Start", g), fromName("Middle", g));
  assert(g -> vertices[0] -> n == 1);
  assert(g -> vertices[1] -> n == 0);
  assert(g -> vertices[0] -> edges[0] -> start == fromName("Start", g));
  assert(g -> vertices[0] -> edges[0] -> weight == 10);
  assert(g -> vertices[0] -> edges[0] -> end == fromName("Middle", g));
  connectVertices(50, fromName("Middle", g), fromName("Start", g));
  assert(g -> vertices[0] -> n == 1);
  assert(g -> vertices[1] -> n == 1);
  assert(g -> vertices[1] -> edges[0] -> start == fromName("Middle", g));
  assert(g -> vertices[1] -> edges[0] -> weight == 50);
  assert(g -> vertices[1] -> edges[0] -> end == fromName("Start", g));
}

// Test functions which return data about graphs
void testGraphDetails() {
  graph* g = newGraph("Start");
  addVertex("Middle", g);
  assert(arrayOfVertices(g)[0] == fromPos(0, g));
  assert(arrayOfVertices(g)[1] == fromPos(1, g));
  assert(numOfVertices(g) == 2);
}

// Test functions which return data about vertices
void testVertexDetails() {
  graph* g = newGraph("Start");
  addVertex("End", g);
  connectVertices(10, fromName("Start", g), fromName("End", g));
  assert(numberConnections(fromName("Start", g), g) == 1);
  assert(numberConnections(fromName("End", g), g) == 0);
  assert(strcmp(name(fromName("End", g)), "End") == 0);
  assert(dests(fromName("Start", g), g)[0] == fromName("Start", g) -> edges[0] -> end);
  assert(dests(fromName("End", g), g)[0] == NULL);

}

// Test functions which return data about edges
void testEdgeDetails() {
  graph* g = newGraph("Start");
  addVertex("End", g);
  connectVertices(10, fromName("Start", g), fromName("End", g));
  assert(isConnection(fromName("Start", g), fromName("End", g)) == true);
  assert(isConnection(fromName("End", g), fromName("Start", g)) == false);
  assert(cost(fromName("Start", g), fromName("End", g)) == 10);
  assert(cost(fromName("End", g), fromName("Start", g)) == 9999); //i.e. there is no connection
}

// Test correct graph is created from file.
// NOTE: Testing with file data/test.txt
void testFile() {
  graph* g = fromFile("data/test.txt");
  assert(numOfVertices(g) == 3);
  assert(arrayOfVertices(g)[0] == fromName("End", g));
  assert(arrayOfVertices(g)[1] == fromName("Middle", g));
  assert(arrayOfVertices(g)[2] == fromName("Start", g));
  assert(numberConnections(fromName("Start", g), g) == 2);
  assert(numberConnections(fromName("Middle", g), g) == 2);
  assert(numberConnections(fromName("End", g), g) == 0);
  assert(isConnection(fromName("Start", g), fromName("Middle", g)) == true);
  assert(isConnection(fromName("Start", g), fromName("End", g)) == true);
  assert(isConnection(fromName("Middle", g), fromName("Start", g)) == true);
  assert(isConnection(fromName("Middle", g), fromName("End", g)) == true);
  assert(isConnection(fromName("End", g), fromName("Start", g)) == false);
  assert(isConnection(fromName("End", g), fromName("Middle", g)) == false);
  assert(cost(fromName("Start", g), fromName("Middle", g)) == 20);
  assert(cost(fromName("Start", g), fromName("End", g)) == 10);
  assert(cost(fromName("Middle", g), fromName("Start", g)) == 10);
  assert(cost(fromName("Middle", g), fromName("End", g)) == 50);
}

// Test deleteEdge(), deleteVertex()
void testDeletion() {
  graph* g = fromFile("data/test.txt");
  vertex* v = fromPos(1,g); // "Middle"
  edge* e = arrayOfEdges(v)[0]; // "Middle->Start"
  assert(v -> n == 2);
  assert(v -> edges[0] == e);
  deleteEdge(e, v);
  assert(v -> n == 1);
  assert(v -> edges[0] != e);
  assert(g -> n == 3);
  assert(g -> vertices[1] == v);
  deleteVertex(v, g);
  assert(g -> n == 2);
  assert(g -> vertices[1] !=   v);
}

// Run tests
void test() {
  testConversion();
  testCreation();
  testPointers();
  testGraphDetails();
  testVertexDetails();
  testEdgeDetails();
  testFile();
  testDeletion();
  printf("All test passed. :)\n");
}

int graphMain() {
  test();
  return 0;
}
