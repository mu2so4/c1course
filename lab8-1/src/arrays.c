#include <malloc.h>
#include <stdbool.h>
#include "arr.h"

bool createArray(EdgeArrays *array) {
    array->edges = (Edge*) malloc(sizeof(Edge) * 6);
    if(!array->edges) {
        return false;
    }
    array->arrLength = 6;
    array->elementsCount = 0;
    return true;
}

bool pushEdge(EdgeArrays *array, short vert1, short vert2, int length) {
    if(array->arrLength == array->elementsCount) {
        array->arrLength = array->arrLength * 3 / 2;
        Edge *next = (Edge*) realloc(array->edges, sizeof(Edge) * array->arrLength);
        if(!next) {
            return false;
        }
        array->edges = next;
    }
    array->edges[array->elementsCount].startVertex = vert1;
    array->edges[array->elementsCount].endVertex = vert2;
    array->edges[array->elementsCount].length = length;
    array->elementsCount++;
    return true;
}

void destroyArrays(EdgeArrays *arrays, int count) {
    for(int index = 0; index < count; index++) {
        free(arrays[index].edges);
    }
    free(arrays);
    arrays = NULL;
}

int compareEdges(const void *a, const void *b) {
    return ((Edge*)a)->length - ((Edge*)b)->length;
}

Edge *shortestEdge(EdgeArrays *arrays, int *iterators, int verticesCount) {
    Edge *res = NULL;
    int pos = -1;
    for(int index = 0; index < verticesCount; index++) {
        if(iterators[index] == -1) {
            continue;
        }
        EdgeArrays *currentArray = arrays + index;
        while(iterators[index] < currentArray->elementsCount &&
                iterators[currentArray->edges[iterators[index]].endVertex] != -1) {
            iterators[index]++;
        }
        if(iterators[index] < currentArray->elementsCount) {
            if(!res || currentArray->edges[iterators[index]].length < res->length) {
                res = currentArray->edges + iterators[index];
                pos = index;
            }
        }
    }
    if(res) {
        iterators[res->endVertex] = 0;
        iterators[pos]++;
    }
    return res;
}
