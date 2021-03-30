#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
typedef struct Stack Stack;
typedef struct StackItem StackItem;
typedef struct Vertex Vertex;
typedef enum VertexColor VertexColor;

enum VertexColor {
    WHITE = 0, GREY = 1, BLACK = 2
};

struct StackItem {
    int vertexID;
    StackItem *next;
};

struct Stack {
    StackItem *begin;
};

struct Vertex {
    short *outgoingEdges;
    int outgoingEdgesCount, maxCount;
    VertexColor status;
};

Stack *createStack() {
    Stack *res = (Stack*) malloc(sizeof(Stack));
    res->begin = NULL;
    return res;
}

void pushBack(Stack *stack, int vertexID) {
    StackItem *item = (StackItem*) malloc(sizeof(StackItem));
    item->next = stack->begin;
    item->vertexID = vertexID;
    stack->begin = item;
}

void destroyStack(Stack *stack) {
    while(stack->begin) {
        StackItem *next = stack->begin->next;
        free(stack->begin);
        stack->begin = next;
    }
    free(stack);
}

bool topSort(int vertexID, Vertex *vertices, Stack *stack) {
    Vertex *current = vertices + vertexID - 1;
    switch(current->status) {
        case GREY: return false;
        case BLACK: return true;
        case WHITE: break;
    }
    bool status = true;
    current->status = GREY;
    for(int index = 0; index < current->outgoingEdgesCount && status; index++) {
        status &= topSort(current->outgoingEdges[index], vertices, stack);
    }
    if(!status) {
        return 0;
    }
    current->status = BLACK;
    pushBack(stack, vertexID);
    return 1;
}

void destroyEdges(Vertex *vertices, int count) {
    for(int index = 0; index < count; index++) {
        free(vertices[index].outgoingEdges);
    }
}


int main() {
    int verticesCount, edgesCount;
    if(scanf("%d", &verticesCount) < 1) {
        printf("bad number of lines\n");
        return 0;
    }
    if(verticesCount < 0 || verticesCount > 2000) {
        printf("bad number of vertices\n");
        return 0;
    }
    if(scanf("%d", &edgesCount) < 1) {
        printf("bad number of lines\n");
        return 0;
    }
    if(edgesCount < 0 || edgesCount > verticesCount * (verticesCount + 1) / 2) {
        printf("bad number of edges\n");
        return 0;
    }
    Vertex *vertices = (Vertex*) malloc(sizeof(Vertex) * verticesCount);
    if(!vertices) {
        printf("out of memory, line: %d\n", __LINE__);
        abort();
    }
    for(int index = 0; index < verticesCount; index++) {
        vertices[index].outgoingEdgesCount = vertices[index].status = WHITE;
        vertices[index].maxCount = 6;
        vertices[index].outgoingEdges = (short*) malloc(sizeof(short) * 6);
        if(!vertices[index].outgoingEdges) {
            printf("out of memomy on line %d\n", __LINE__);
            destroyEdges(vertices, index);
            free(vertices);
            abort();
        }
    }
    for(int index = 0; index < edgesCount; index++) {
        int ver1, ver2;
        if(scanf("%d %d", &ver1, &ver2) < 2) {
            printf("bad number of lines\n");
            destroyEdges(vertices, verticesCount);
            free(vertices);
            return 0;
        }
        if(ver1 < 1 || ver1 > verticesCount || ver2 < 1 || ver2 > verticesCount) {
            printf("bad vertex\n");
            destroyEdges(vertices, verticesCount);
            free(vertices);
            return 0;
        }
        Vertex *vert = vertices + ver1 - 1;
        if(vert->maxCount == vert->outgoingEdgesCount) {
            vert->maxCount = vert->maxCount * 3 / 2 + 1;
            short *prev = vert->outgoingEdges;
            vert->outgoingEdges =
                (short*) realloc(vert->outgoingEdges, sizeof(short) * vert->maxCount);
            if(!vert->outgoingEdges) {
                printf("out of memory on line %d\n", __LINE__);
                vert->outgoingEdges = prev;
                destroyEdges(vertices, verticesCount);
                free(vertices);
                abort();
            }
        }
        vert->outgoingEdges[vert->outgoingEdgesCount] = ver2;
        vert->outgoingEdgesCount++;
    }
    Stack *stack = createStack();
    bool status = true;
    for(int index = 0; index < verticesCount && status; index++) {
        status &= topSort(index + 1, vertices, stack);
    }
    if(status) {
        for(StackItem *item = stack->begin; item; item = item->next) {
            printf("%d ", (int) item->vertexID);
        }
        printf("\n");
    }
    else {
        printf("impossible to sort\n");
    }

    destroyEdges(vertices, verticesCount);
    destroyStack(stack);
    free(vertices);
    return 0;
}
