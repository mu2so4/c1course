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
        list->begin = list->end = item;
    }
    else {
        list->end->next = item;
        list->end = item;
    }
}

void pushBackNum(List * list, int num) {
    Item * item = (Item *) malloc(sizeof(Item));
    assert(item != NULL);
    item->isNum = 1;
    item->value = num;
    pushBack(list, item);
}

void pushBackOper(List * list, char oper) {
    Item * item = (Item *) malloc(sizeof(Item));
    assert(item != NULL);
    item->isNum = 0;
    item->oper = oper;
    pushBack(list, item);
}

int popBack(List * list) {
    if(list->end == NULL) {
        return 0;
    }
    Item * prev = list->end->prev;
    free(list->end);
    if(prev != NULL) {
        prev->next = NULL;
    }
    else {
        list->begin = NULL;
    }
    list->end = prev;
    return 1;
}

void clear(List * list) {
    while(popBack(list));
}

int move(List * from, List * to) {
    if(from->end == NULL) {
        return 0;
    }
    Item * item = from->end;
    from->end = from->end->prev;
    if(from->end != NULL) {
        from->end->next = NULL;
    }
    else {
        from->begin = NULL;
    }
    pushBack(to, item);
    return 1;
}

void merge(List * list, List * stack) {
    while(move(stack, list));
}

List * split(char * str) {
    int currentNum = -1;
    List * list = (List *) malloc(sizeof(List)), opersStack = {NULL, NULL, 0};
    assert(list != NULL);
    list->begin = list->end = NULL;
    list->isBroken = 0;
    for(int index = 0; str[index] != '\0' && !list->isBroken; index++) {
        char symbol = str[index];
        if(isdigit(symbol)) {
            currentNum = (currentNum == -1 ? 0 : 10 * currentNum) + (int) (symbol - '0');
        }
        else {
            if(currentNum != -1) {
                pushBackNum(list, currentNum);
                currentNum = -1;
            }
            switch(symbol) {
                case '+':
                case '-':
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
                    if(opersStack.end != NULL) {
                        if(opersStack.end->oper == '*' || opersStack.end->oper == '/') {
                            move(&opersStack, list);
                        }
                    }
                    pushBackOper(&opersStack, symbol);
                    break;
                case '(':
                    pushBackOper(&opersStack, symbol);
                    break;
                case ')':
                    while(opersStack.end != NULL && opersStack.end->oper != '(') {
                            move(&opersStack, list);
                    }
                    list->isBroken |= popBack(&opersStack) ? 0 : 1;
                    break;
                default:
                    list->isBroken = 1;
            }
        }
    }
    if(currentNum != -1) {
        pushBackNum(list, currentNum);
    }
    merge(list, &opersStack);
    return list;
}

void count(List * numStack, char oper) {
    if(numStack->end == NULL || numStack->end->prev == NULL) {
        numStack->isBroken = 1;
        return;
    }
    if(numStack->isBroken == 1) {
        return;
    }
    int res = -1, num2 = numStack->end->value, num1 = numStack->end->prev->value;
    popBack(numStack);
    popBack(numStack);
    switch(oper) {
        case '+':
            res = num1 + num2;
            break;
        case '-':
            res = num1 - num2;
            break;
        case '*':
            res = num1 * num2;
            break;
        case '/':
            if(num2 == 0) {
                numStack->isBroken = 2;
                return;
            }
            res = num1 / num2;
            break;
        default:
            numStack->isBroken = 1;
            return;
    }
    pushBackNum(numStack, res);
}

List countAll(List * ariph) {
    List numStack = {NULL, NULL, 0};
    if(ariph->isBroken) {
        numStack.isBroken = 1;
        return numStack;
    }
    for(Item * iter = ariph->begin; iter != NULL && !numStack.isBroken; iter = iter->next) {
        if(iter->isNum) {
            pushBackNum(&numStack, iter->value);
        }
        else {
            count(&numStack, iter->oper);
        }
    }
    if(numStack.begin != numStack.end) {
        numStack.isBroken = 1;
    }
    return numStack;
}

int main() {
    char str[1002];
    assert(fgets(str, 1000, stdin));
    str[strlen(str) - 1] = '\0';
    List * list = split(str), res = countAll(list);
    switch(res.isBroken) {
        case 0:
            printf("%d\n", res.begin->value);
            break;
        case 1:
            printf("syntax error\n");
            break;
        case 2:
            printf("division by zero\n");
            break;
    }
    clear(list);
    clear(&res);
    free(list);
    return 0;
}
