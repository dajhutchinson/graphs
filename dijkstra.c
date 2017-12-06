#include "graph.h"
#include "dijkstra.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

struct list {
  vertex* start;
  item** vertices;
  vertex* end;
};

struct item {
  vertex* current;
  item* previous;
  int cost;
};

// Set cost of start position to be 0
// NOTE: l = list of items to change; g = graph that list references
void startCost(list *l, graph* g) {
  for (int k = 0; k < numOfVertices(g); k++) {
    if (l -> vertices[k] -> current == l -> start) {
      l -> vertices[k] -> cost = 0;
      return;
}}}

// Add items to list
// NOTE: l = list to add items to; g = graph that list references
list* verticesToList(list* l, graph* g) {
  for (int k = 0; k < numOfVertices(g); k++) {
    item* new = malloc(sizeof(item));
    new -> current = fromPos(k, g);
    new -> previous = NULL;
    new -> cost = 9999;
    l -> vertices[k] = new;
  }
  return l;
}

// Create list to store details
// NOTE: s = starting vertex; t = target vertex; g = graph
list* iniateList(vertex* s, vertex* t, graph* g) {
  list* new = malloc(sizeof(list));
  new -> start = s;
  new -> end = t;
  new -> vertices = malloc(sizeof(item) * numOfVertices(g));
  new = verticesToList(new, g);
  startCost(new, g);
  return new;
}

// Return the item which records details about a given vertex
// NOTE: v = vertex to find item for; l = list to find item in; g = graph that vertex is in
item* itemFromVertex(vertex* v, list* l, graph* g) {
  for (int k = 0; k < numOfVertices(g); k++) {
    if (strcmp(name(l -> vertices[k] -> current), name(v)) == 0) return l -> vertices[k];
  }
  return NULL;
}

// Compare the cost of given routes
// NOTE: n = index of item being read from
void dijCompare(int n, list* l, graph* g) {
  item* cur = l -> vertices[n];
  if (strcmp(name(cur -> current),name(l -> end)) == 0) return;
  vertex** dest = dests(cur -> current, g);
  if (dest[0] == NULL) return;
  for (int k = 0; k < numberConnections(cur -> current, g); k++) {
    item* target = itemFromVertex(dest[k], l, g);
    if (target -> cost > (cost(cur -> current, dest[k]) + cur -> cost)) {
      target -> previous = cur;
      target -> cost = cost(cur -> current, dest[k]) + cur -> cost;
}}}

// Returns length of route
int routeLength(vertex** r, list* l) {
  int n = 1;
  while(strcmp(name(r[n-1]),name(l->start)) != 0) n++;
  return n;
}

// Reverse path to find shortest route
vertex** route(list* l, graph* g) {
  vertex** r = malloc(sizeof(l -> start) * numOfVertices(g));
  item* cur = itemFromVertex(l -> end, l ,g);
  int n = 0;
  while(cur -> previous != NULL) {
    r[n] = cur -> current;
    cur = cur -> previous;
    n++;
  }
  r[n] = cur -> current;
  n++;
  return r;
}

// Print out route to user
void outputRoute(vertex** r, list* l) {
  printf("------------------------------------\nBest route:\n  ");
  int n = routeLength(r, l);
  for (int k = 0; k < n; k++) {
    if (k != n-1) printf("%s -> ",name(r[n-(k+1)]));
    else printf("%s\n",name(r[n-(k+1)]));
  }
  printf("------------------------------------\n");
}

// Delete used list
void deleteList(graph* g, list* l) {
  for (int k = 0; k < numOfVertices(g); k++) free(l -> vertices[k]);
  free(l);
}

// Test correct route is outputted
// Assumes route from "Start" -> "End"
void testRoute(vertex** r, graph* g, list* l) {
  assert(routeLength(r, l) == 2);
  assert(r[0] == fromName("End", g));
  assert(r[1] == fromName("Start", g));
}

// Perform dijkstra algorithm
void dijkstra(vertex* s, vertex* t, graph* g) {
  list* l = iniateList(s, t, g);
  for (int k = 0; k < numOfVertices(g); k++) {
    dijCompare(k, l, g);
  }
  vertex** r = route(l, g);
  outputRoute(r, l);
  //deleteList(g, l);
}

// Test list is created properly
void testList(list* l, vertex* s, vertex* t, graph* g) {
  assert(l -> start == s);
  assert(l -> end == t);
  assert(l -> vertices[0] -> current == fromName("End", g));
  assert(l -> vertices[1] -> current == fromName("Middle", g));
  assert(l -> vertices[2] -> current == fromName("Start", g));
  assert(l -> vertices[0] -> previous == NULL);
  assert(l -> vertices[1] -> previous == NULL);
  assert(l -> vertices[2] -> previous == NULL);
  assert(l -> vertices[2] -> cost == 0);
  assert(l -> vertices[1] -> cost == 9999);
  assert(l -> vertices[0] -> cost == 9999);
}

// Run tests
void dijkTest() {
  graph* g = fromFile("data/test.txt");
  list* l = iniateList(fromName("Start", g), fromName("End", g), g);
  testList(l, fromName("Start", g), fromName("End", g), g);
  for (int k = 0; k < numOfVertices(g); k++) {
    dijCompare(k, l, g);
  }
  vertex** r = route(l, g);
  testRoute(r, g, l);
  printf("All tests passed. :)\n");
}

// Run program, UI pieces
int mainDijk() {
  int max = 100; bool run = true;
  char *s = malloc(sizeof(char) * max); char *i = malloc(sizeof(char) * max); char *t = malloc(sizeof(char) * max);
  printf("Enter graph to load:\n>  ");
  if(fgets(s, max, stdin) != NULL) {
    if(strcmp(s, ",\n") == 0) {dijkTest(); return 0;}
    graph* g = fromFile(s);
    if(strcmp(s, ".\n") == 0) {deleteGraph(g); return 0;}
    while(run) {
      printf("Locations:\n");
      outputVertices(g);
      printf("\nWhere do you want to start:\n>  ");
      if(fgets(i,max,stdin) != NULL);
      if(strcmp(i, ".\n") == 0) return 0;
      else if(strcmp(i, ",\n") == 0) {dijkTest(); deleteGraph(g); return 0;}
      if(strcmp(i, ".\n") == 0) {deleteGraph(g); return 0;}
      printf("\nWhere do you want to finish:\n>  ");
      if(fgets(t,max,stdin) != NULL);
      if(strcmp(t, ".\n") == 0) {deleteGraph(g); return 0;}
      else if(strcmp(t, ",\n") == 0) {dijkTest(); deleteGraph(g); return 0;}
      i[strlen(i)-1] = '\0'; t[strlen(t)-1] = '\0';
      dijkstra(fromName(i, g), fromName(t, g), g);
    }
    return 0;
  }
  return 0;
}
