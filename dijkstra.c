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
void startCost(list *l, graph* g) {
  for (int k = 0; k < numOfVertices(g); k++) {
    if (l -> vertices[k] -> current == l -> start) {
      l -> vertices[k] -> cost = 0;
      return;
}}}

// Add items to list
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
item* itemFromVertex(vertex* v, list* l, graph* g) {
  for (int k = 0; k < numOfVertices(g); k++) {
    if (strcmp(name(l -> vertices[k] -> current), name(v)) == 0) return l -> vertices[k];
  }
  return NULL;
}

// Compare the cost of given routes
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

int routeLength(vertex** r, list* l) {
  int n = 1;
  while(strcmp(name(r[n-1]),name(l->start)) != 0) n++;
  return n;
}

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

void outputRoute(vertex** r, list* l) {
  printf("------------------------------------\nBest route:\n  ");
  int n = routeLength(r, l);
  for (int k = 0; k < n; k++) {
    if (k != n-1) printf("%s -> ",name(r[n-(k+1)]));
    else printf("%s\n",name(r[n-(k+1)]));
  }
  printf("------------------------------------\n");
}

// Perform dijkstra algorithm
void dijkstra(vertex* s, vertex* t, graph* g) {
  list* l = iniateList(s, t, g);
  for (int k = 0; k < numOfVertices(g); k++) {
    dijCompare(k, l, g);
  }
  vertex** r = route(l, g);
  outputRoute(r, l);
}

int mainDijk() {
  int max = 100; bool run = true;
  char *s = malloc(sizeof(char) * max); char *i = malloc(sizeof(char) * max); char *t = malloc(sizeof(char) * max);
  printf("Enter graph to load:\n>  ");
  if(fgets(s, max, stdin) != NULL) {
    if(strcmp(s, ".\n") == 0) return 0;
    graph* g = fromFile(s);
    while(run) {
      printf("Locations:\n");
      outputVertices(g);
      printf("\nWhere do you want to start:\n>  ");
      if(fgets(i,max,stdin) != NULL);
      if(strcmp(i, ".\n") == 0) return 0;
      printf("\nWhere do you want to finish:\n>  ");
      if(fgets(t,max,stdin) != NULL);
      if(strcmp(i, ".\n") == 0) return 0;
      i[strlen(i)-1] = '\0'; t[strlen(t)-1] = '\0';
      dijkstra(fromName(i, g), fromName(t, g), g);
    }
    return 0;
  }
  return 0;
}
