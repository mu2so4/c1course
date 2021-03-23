#include <stdio.h>
#include <stdlib.h>
typedef struct Stack Stack;
typedef struct StackItem StackItem;
typedef struct Vertex Vertex;

struct StackItem {
    int vertexID;
    StackItem *next;
};

struct Stack {
    StackItem *begin;
};

struct Vertex {
    short *outgoingEdges;
    int outgoingEdgesCount, maxCount, status;
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

int topSort(int vertexID, Vertex *vertices, Stack *stack) {
    Vertex *current = vertices + vertexID - 1;
    switch(current->status) {
        case 1: return 0;
        case 2: return 1;
    }
    current->status = 1;
    int status = 1;
    for(int index = 0; index < current->outgoingEdgesCount && status; index++) {
        status *= topSort(current->outgoingEdges[index], vertices, stack);
    }
    if(!status) {
        return 0;
    }
    current->status = 2;
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
    Vertex vertices[verticesCount];
    for(int index = 0; index < verticesCount; index++) {
        vertices[index].outgoingEdgesCount = vertices[index].status = 0;
        vertices[index].maxCount = 6;
        vertices[index].outgoingEdges = (short*) malloc(sizeof(short) * 6);
    }
    for(int index = 0; index < edgesCount; index++) {
        int ver1, ver2;
        if(scanf("%d %d", &ver1, &ver2) < 2) {
            printf("bad number of lines\n");
            destroyEdges(vertices, verticesCount);
            return 0;
        }
        if(ver1 < 1 || ver1 > verticesCount || ver2 < 1 || ver2 > verticesCount) {
            printf("bad vertex\n");
            destroyEdges(vertices, verticesCount);
            return 0;
        }
        Vertex *vert = vertices + ver1 - 1;
        if(vert->maxCount == vert->outgoingEdgesCount) {
            vert->maxCount = vert->maxCount * 3 / 2 + 1;
            vert->outgoingEdges =
                (short*) realloc(vert->outgoingEdges, sizeof(short) * vert->maxCount);
        }
        vert->outgoingEdges[vert->outgoingEdgesCount] = ver2;
        vert->outgoingEdgesCount++;
    }
    Stack *stack = createStack();
    int status = 1;
    for(int index = 0; index < verticesCount && status; index++) {
        status *= topSort(index + 1, vertices, stack);
    }
    if(status) {
        for(StackItem *item = stack->begin; item; item = item->next) {
            printf("%hd ", item->vertexID);
        }
    }
    else {
        printf("impossible to sort\n");
    }

    destroyEdges(vertices, verticesCount);
    destroyStack(stack);
    return 0;
}