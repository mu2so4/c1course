#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>
#include <string.h>
typedef struct List List;
typedef struct Item Item;

struct Item {
    int isNum;
    int value;
    char oper;
    Item * next, * prev;
};

struct List {
     Item * begin, * end;
     int isBroken;
};

void pushBack(List * list, Item * item) {
    item->next = NULL;
    item->prev = list->end;
    if(list->begin == NULL) {
        fprintf(stderr, "source code line number %d\n", __LINE__); fflush(0);
        list->begin = list->end = item;
    }
    else {
        fprintf(stderr, "source code line number %d\n", __LINE__); fflush(0);
        list->end->next = item;
        list->end = item;
    }
}

void pushBackNum(List * list, int num) {
    fprintf(stderr, "source code line number %d\n", __LINE__); fflush(0);
    Item * item = (Item *) malloc(sizeof(Item));
    assert(item != NULL);
    item->isNum = 1;
    item->value = num;
    pushBack(list, item);
}

void pushBackOper(List * list, char oper) {
    fprintf(stderr, "source code line number %d\n", __LINE__); fflush(0);
    Item * item = (Item *) malloc(sizeof(Item));
    assert(item != NULL);
    item->isNum = 0;
    item->oper = oper;
    pushBack(list, item);
}

int popBack(List * list) {
    fprintf(stderr, "source code line number %d\n", __LINE__); fflush(0);
    if(list->end == NULL) {
        fprintf(stderr, "source code line number %d\n", __LINE__); fflush(0);
        return 0;
    }
    fprintf(stderr, "source code line number %d\n", __LINE__); fflush(0);
    Item * prev = list->end->prev;
    free(list->end);
    if(prev != NULL) {
        fprintf(stderr, "source code line number %d\n", __LINE__); fflush(0);
        prev->next = NULL;
    }
    else {
        fprintf(stderr, "source code line number %d\n", __LINE__); fflush(0);
        list->begin = NULL;
    }
    fprintf(stderr, "source code line number %d\n", __LINE__); fflush(0);
    list->end = prev;
    return 1;
}

void clear(List * list) {
    fprintf(stderr, "source code line number %d\n", __LINE__); fflush(0);
    while(popBack(list));
}

int move(List * from, List * to) {
    if(from->end == NULL) {
        fprintf(stderr, "source code line number %d\n", __LINE__); fflush(0);
        return 0;
    }
    fprintf(stderr, "source code line number %d\n", __LINE__); fflush(0);
    Item * item = from->end;
    from->end = from->end->prev;
    if(from->end != NULL) {
        fprintf(stderr, "source code line number %d\n", __LINE__); fflush(0);
        from->end->next = NULL;
    }
    else {
        fprintf(stderr, "source code line number %d\n", __LINE__); fflush(0);
        from->begin = NULL;
    }
    pushBack(to, item);
    return 1;
}

void merge(List * list, List * stack) {
    fprintf(stderr, "source code line number %d\n", __LINE__); fflush(0);
    while(move(stack, list));
}

List * split(char * str) {
    fprintf(stderr, "source code line number %d\n", __LINE__); fflush(0);
    int currentNum = -1;
    List * list = (List *) malloc(sizeof(List)), opersStack = {NULL, NULL, 0};
    assert(list != NULL);
    list->begin = list->end = NULL;
    list->isBroken = 0;
    for(int index = 0; str[index] != '\0' && !list->isBroken; index++) {
        fprintf(stderr, "source code line number %d\n", __LINE__); fflush(0);
        char symbol = str[index];
        if(symbol >= '0' && symbol <= '9') {
            fprintf(stderr, "source code line number %d\n", __LINE__); fflush(0);
            currentNum = (currentNum == -1 ? 0 : 10 * currentNum) + (int) (symbol - '0');
        }
        else {
            fprintf(stderr, "source code line number %d\n", __LINE__); fflush(0);
            if(currentNum != -1) {
                pushBackNum(list, currentNum);
                currentNum = -1;
            }
            switch(symbol) {
                case '+':
                case '-':
                    fprintf(stderr, "source code line number %d\n", __LINE__); fflush(0);
                    while(opersStack.end != NULL) {
                        if(opersStack.end->oper == '(') {
                            break;
                        }
                        move(&opersStack, list);
                    }
                    pushBackOper(&opersStack, symbol);
                    break;
                case '*':
                case '/':
                    fprintf(stderr, "source code line number %d\n", __LINE__); fflush(0);
                    if(opersStack.end != NULL) {
                        if(opersStack.end->oper == '*' || opersStack.end->oper == '/') {
                            move(&opersStack, list);
                        }
                    }
                    pushBackOper(&opersStack, symbol);
                    break;
                case '(':
                    fprintf(stderr, "source code line number %d\n", __LINE__); fflush(0);
                    list->isBroken |= index > 0 && isdigit(str[index - 1]);
                    pushBackOper(&opersStack, symbol);
                    break;
                case ')':
                    fprintf(stderr, "source code line number %d\n", __LINE__); fflush(0);
                    list->isBroken |= isdigit(str[index + 1]);
                    while(opersStack.end != NULL && opersStack.end->oper != '(') {
                            move(&opersStack, list);
                    }
                    list->isBroken |= !popBack(&opersStack);
                    break;
                default:
                    fprintf(stderr, "source code line number %d\n", __LINE__); fflush(0);
                    list->isBroken = 1;
            }
        }
    }
    if(currentNum != -1) {
        fprintf(stderr, "source code line number %d\n", __LINE__); fflush(0);
        pushBackNum(list, currentNum);
    }
    fprintf(stderr, "source code line number %d\n", __LINE__); fflush(0);
    merge(list, &opersStack);
    return list;
}

void count(List * numStack, char oper) {
    fprintf(stderr, "source code line number %d\n", __LINE__); fflush(0);
    if(numStack->end == NULL || numStack->end->prev == NULL) {
        fprintf(stderr, "source code line number %d\n", __LINE__); fflush(0);
        numStack->isBroken = 1;
        return;
    }
    if(numStack->isBroken == 1) {
        fprintf(stderr, "source code line number %d\n", __LINE__); fflush(0);
        return;
    }
    int res = -1, num2 = numStack->end->value, num1 = numStack->end->prev->value;
    popBack(numStack);
    popBack(numStack);
    switch(oper) {
        case '+':
            fprintf(stderr, "source code line number %d\n", __LINE__); fflush(0);
            res = num1 + num2;
            break;
        case '-':
            fprintf(stderr, "source code line number %d\n", __LINE__); fflush(0);
            res = num1 - num2;
            break;
        case '*':
            fprintf(stderr, "source code line number %d\n", __LINE__); fflush(0);
            res = num1 * num2;
            break;
        case '/':
            fprintf(stderr, "source code line number %d\n", __LINE__); fflush(0);
            if(num2 == 0) {
                fprintf(stderr, "source code line number %d\n", __LINE__); fflush(0);
                numStack->isBroken = 2;
                return;
            }
            res = num1 / num2;
            break;
        default:
            fprintf(stderr, "source code line number %d\n", __LINE__); fflush(0);
            numStack->isBroken = 1;
            return;
    }
    fprintf(stderr, "source code line number %d\n", __LINE__); fflush(0);
    pushBackNum(numStack, res);
}

List countAll(List * ariph) {
    fprintf(stderr, "source code line number %d\n", __LINE__); fflush(0);
    List numStack = {NULL, NULL, 0};
    if(ariph->isBroken) {
        fprintf(stderr, "source code line number %d\n", __LINE__); fflush(0);
        numStack.isBroken = 1;
        return numStack;
    }
    for(Item * iter = ariph->begin; iter != NULL && !numStack.isBroken; iter = iter->next) {
        fprintf(stderr, "source code line number %d\n", __LINE__); fflush(0);
        if(iter->isNum) {
            fprintf(stderr, "source code line number %d\n", __LINE__); fflush(0);
            pushBackNum(&numStack, iter->value);
        }
        else {
            fprintf(stderr, "source code line number %d\n", __LINE__); fflush(0);
            count(&numStack, iter->oper);
        }
    }
    if(numStack.begin != numStack.end || (numStack.end == NULL && numStack.isBroken != 2)) {
        numStack.isBroken = 1;
    }
    return numStack;
}

int main() {
    char str[1002];
    if(fgets(str, 1002, stdin) == NULL) {
        return 0;
    }
    str[strlen(str) - 1] = '\0';
    fprintf(stderr, "source code line number %d, strlen %d, str \"%s\"\n", __LINE__, (int) strlen(str), str); fflush(0);
    List * list = split(str), res = countAll(list);
    switch(res.isBroken) {
        case 0:
            fprintf(stderr, "source code line number %d\n", __LINE__); fflush(0);
            printf("%d\n", res.begin->value);
            break;
        case 1:
            fprintf(stderr, "source code line number %d\n", __LINE__); fflush(0);
            printf("syntax error\n");
            break;
        case 2:
            fprintf(stderr, "source code line number %d\n", __LINE__); fflush(0);
            printf("division by zero\n");
            break;
    }
    fprintf(stderr, "source code line number %d\n", __LINE__); fflush(0);
    clear(list);
    clear(&res);
    free(list);
    return 0;
}
