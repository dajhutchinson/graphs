#include "graph.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

struct edge;
typedef struct edge edge;

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
  if (int n < numOfVertices(g)) return g -> vertices[n];
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
graph* fromFile (char* file) {
  file[strlen(file)-1] = '\0';
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

int graphMain() {
  return 0;
}
