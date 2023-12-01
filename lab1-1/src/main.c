#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
typedef struct CharItem CharItem;
typedef struct CharHashList CharHashList;

int hash(const char *str, size_t size) {
    int res = 0;
    for(int index = (int) size - 1; index >= 0; index--) {
        res = res * 3 + (unsigned char) str[index] % 3;
    }
    return res;
}

struct CharItem {
    unsigned char symbol;
    CharItem *next;
};

struct CharHashList {
    size_t needleLength;
    size_t currentLength;
    int multiplier;
    int hash;
    CharItem *begin, *end;
};

CharHashList createHashList(size_t needleSize) {
    CharHashList list = {needleSize, 0, 1, 0, NULL, NULL};
    return list;
}

CharItem *createCharItem(unsigned char symbol) {
    CharItem *item = (CharItem*) calloc(1, sizeof(CharItem));
    if(item == NULL) {
        return NULL;
    }
    item->symbol = symbol;
    return item;
}

int pushBack(CharHashList *list, unsigned char symbol) {
    if(list->currentLength != list->needleLength) {
        CharItem *item = createCharItem(symbol);
        if(item == NULL) {
            return 0;
        }
        if(!list->currentLength) {
            list->begin = item;
        }
        else {
            list->end->next = item;
        }
        list->end = item;
        list->currentLength++;
        list->hash += symbol % 3 * list->multiplier;
        list->multiplier *= 3;
	if(list->currentLength == list->needleLength) {
            list->end->next = list->begin;
	    list->multiplier /= 3;
	}
    }
    else {
        list->hash = list->hash / 3 + symbol % 3 * list->multiplier;
        list->begin->symbol = symbol;
        list->begin = list->begin->next;
        list->end = list->end->next;
    }
    return 1;
}

void clear(CharHashList * str) {
    if(str->end != NULL) {
        str->end->next = NULL;
    }
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
    size_t size = strlen(needle);
    needle[size - 1] = '\0';
    size--;
    int needleHash = hash(needle, size);
    printf("%d", needleHash);
    if(size == 0) {
        printf("\n");
        return 0;
    }
    CharHashList list = createHashList(size);
    char subHaystack[BUFSIZ + 1];
    size_t position = 1;
    while(fgets(subHaystack, BUFSIZ, stdin) != NULL) {
        size_t subsize = strlen(subHaystack);
	for(size_t index = 0; index < subsize; index++) {
            if(pushBack(&list, subHaystack[index]) == 0) {
                perror("out of memory");
                clear(&list);
                return 1;
            }
            if(needleHash == list.hash && list.currentLength == list.needleLength) {
                CharItem *iter = list.begin;
                for(size_t checkIndex = position - list.needleLength + 1; checkIndex <= position; checkIndex++, iter = iter->next) {
                    printf(" %ld", checkIndex);
                    if(iter->symbol != (unsigned char) needle[checkIndex - (position - list.needleLength + 1)]) {
                        break;
                    }
                }
            }
	    position++;
	}
    }
    printf("\n");
    clear(&list);
    return 0;
}

