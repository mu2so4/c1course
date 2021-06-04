#include <stdbool.h>
#ifndef WORKS_ARR_H
#define WORKS_ARR_H

typedef struct Edge {
    int length;
    short startVertex, endVertex;
} Edge;

typedef struct EdgeArrays {
    Edge *edges;
    int arrLength, elementsCount;
} EdgeArrays;

bool createArray(EdgeArrays *array);
bool pushEdge(EdgeArrays *array, short vert1, short vert2, int length);
void destroyArrays(EdgeArrays *arrays, int count);
int compareEdges(const void *a, const void *b);
Edge *shortestEdge(EdgeArrays *arrays, int *iterators, int verticesCount);


#endif
