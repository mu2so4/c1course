#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
typedef struct CharItem CharItem;
typedef struct CharHashList CharHashList;

int hash(const char * str, int size) {
    int res = 0;
    for(int index = size - 1; index >= 0; index--) {
        fprintf(stderr, "code of %c: %d\n", str[index], (unsigned char) str[index]); fflush(0);
        unsigned char symbol = str[index];
        res = res * 3 + (unsigned char) (symbol - (symbol > 127 ? 1 : 0)) % 3;
    }
    return res;
}

struct CharItem {
    unsigned char symbol;
    CharItem * next;
};

struct CharHashList {
    int needleLength, currentLength, multiplier, hash;
    CharItem * begin, * end;
};

CharHashList createHashList(int needleSize) {
    CharHashList list = {needleSize, 0, 1, 0, NULL, NULL};
    return list;
}

void pushBack(CharHashList * list, unsigned char symbol) {
    if(!list->needleLength) {
        return;
    }
    if(list->currentLength != list->needleLength) {
        CharItem * item = (CharItem *) malloc(sizeof(CharItem));
        if(item == NULL) {
            printf("Out of memory\n");
            abort();
        }
        item->next = NULL;
        item->symbol = symbol;
        if(!list->currentLength) {
            list->begin = item;
        }
        else {
            list->end->next = item;
        }
        list->end = item;
        list->currentLength++;
        list->hash += (symbol - (symbol > 127 ? 1 : 0)) % 3 * list->multiplier;
        list->multiplier *= 3;
    }
    else {
        list->hash = list->hash / 3 + symbol % 3 * (list->multiplier / 3);
        list->begin->symbol = symbol;
        CharItem * item = list->begin;
        list->begin = item->next;
        list->end->next = item;
        list->end = item;
        item->next = NULL;
    }
}

void clear(CharHashList * str) {
    while(str->begin != NULL) {
        CharItem * item = str->begin->next;
        free(str->begin);
        str->begin = item;
    }
    str->currentLength = 0;
    str->multiplier = 1;
}


int main() {
    setlocale(LC_ALL, "Russian");

    char needle[20];
    if(fgets(needle, 20, stdin) == NULL) {
        return 0;
    }
    int size = strlen(needle);
    needle[size - 1] = '\0';
    size--;
    int needleHash = hash(needle, size);
    printf("%d", needleHash);
    CharHashList list = createHashList(size);
    if(size) {
        char symbol;
        for(int index = 1; scanf("%c", &symbol) > 0; index++) {
            pushBack(&list, symbol);
            if(needleHash == list.hash && list.currentLength == list.needleLength) {
                CharItem * iter = list.begin;
                for(int checkIndex = index - list.needleLength + 1;
                        checkIndex <= index; checkIndex++, iter = iter->next) {
                    printf(" %d", checkIndex);
                    if(iter->symbol != needle[checkIndex - (index - list.needleLength + 1)]) {
                        break;
                    }
                }
            }

        }
    }
    clear(&list);
    return 0;
}

