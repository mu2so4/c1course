#include <stdio.h>
#include <stdlib.h>
#include "arr.h"

int main() {
    int verticesCount, edgesCount;
    if(scanf("%d", &verticesCount) < 1) {
        printf("bad number of lines\n");
        return 0;
    }
    if(verticesCount < 0 || verticesCount > 5000) {
        printf("bad number of vertices\n");
        return 0;
    }
    if(scanf("%d", &edgesCount) < 1) {
        printf("bad number of lines\n");
        return 0;
    }
    if(edgesCount < 0 || edgesCount > verticesCount * (verticesCount - 1) / 2) {
        printf("bad number of edges\n");
        return 0;
    }

    int *verticesStates = (int*) malloc(sizeof(int) * verticesCount);
    if(!verticesStates) {
        printf("out of memory, line %d\n", __LINE__ - 2);
        abort();
    }
    EdgeArrays *arrays = (EdgeArrays *) malloc(sizeof(EdgeArrays) * verticesCount);
    if(!arrays) {
        printf("out of memory, line %d\n", __LINE__ - 2);
        free(verticesStates);
        abort();
    }

    for(int index = 0; index < verticesCount; index++) {
        verticesStates[index] = index ? -1 : 0;
        if(!createArray(arrays + index)) {
            printf("out of memory, line %d\n", __LINE__ - 1);
            destroyArrays(arrays, index);
            free(arrays);
            free(verticesStates);
            abort();
        }
    }

    for(int index = 0; index < edgesCount; index++) {
        int vert1, vert2, length;
        if(scanf("%d %d %d", &vert1, &vert2, &length) < 3) {
            printf("bad number of lines\n");
            destroyArrays(arrays, verticesCount);
            free(arrays);
            free(verticesStates);
            return 0;
        }
        if(vert1 < 1 || vert1 > verticesCount || vert2 < 1 || vert2 > verticesCount) {
            printf("bad vertex\n");
            destroyArrays(arrays, verticesCount);
            free(arrays);
            free(verticesStates);
            return 0;
        }
        if(length < 0) {
            printf("bad length\n");
            destroyArrays(arrays, verticesCount);
            free(arrays);
            free(verticesStates);
            return 0;
        }
        vert1--; vert2--;
        if(!pushEdge(arrays + vert1, (short) vert1, (short) vert2, length) ||
           !pushEdge(arrays + vert2, (short) vert2, (short) vert1, length)) {
            printf("out of memory, line %d\n", __LINE__ - 1);
            destroyArrays(arrays, verticesCount);
            free(verticesStates);
            free(arrays);
            abort();
        }
    }

    if(!verticesCount) {
        printf("no spanning tree\n");
        destroyArrays(arrays, verticesCount);
        free(verticesStates);
        free(arrays);
        return 0;
    }

    short *takenEdges = (short*) malloc(sizeof(short) * 2 * (verticesCount - 1)), pos = 0;
    if(!takenEdges) {
        printf("out of memory, line %d\n", __LINE__ - 1);
        destroyArrays(arrays, verticesCount);
        free(verticesStates);
        free(arrays);
        abort();
    }
    for(int index = 0; index < verticesCount; index++) {
        qsort(arrays[index].edges, arrays[index].elementsCount, sizeof(Edge), compareEdges);
    }

    for(;;) {
        Edge *best = shortestEdge(arrays, verticesStates, verticesCount);
        if(!best) {
            break;
        }
        takenEdges[2 * pos] = best->startVertex;
        short vert = best->endVertex;
        takenEdges[2 * pos + 1] = vert;
        pos++;
    }

    if(pos != verticesCount - 1) {
        printf("no spanning tree\n");
    }
    else {
        for(int index = 0; index < verticesCount - 1; index++) {
            printf("%d %d\n", takenEdges[2 * index] + 1, takenEdges[2 * index + 1] + 1);
        }
    }

    destroyArrays(arrays, verticesCount);
    free(takenEdges);
    free(verticesStates);
    free(arrays);
    return 0;
}
