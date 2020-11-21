#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <locale.h>
typedef struct StopSymbols StopSymbols;

struct StopSymbols {
    int shifts[256];
    unsigned needleLength;
};

unsigned shiftNeedle(const StopSymbols * bank, unsigned char symbol) {
    if(bank->shifts[symbol]) {
        return bank->shifts[symbol];
    }
    return bank->needleLength;
}

void addShift(StopSymbols * bank, unsigned char symbol, int pos) {
    if(!bank->shifts[symbol] || pos < bank->shifts[symbol]) {
        bank->shifts[symbol] = pos;
    }
}

StopSymbols createShiftsBank(char * needle) {
    StopSymbols bank = {.needleLength = strlen(needle)};
    for(unsigned index = 0; index < 256; index++) {
        bank.shifts[index] = 0;
    }
    for(unsigned index = 0; index < bank.needleLength - 1; index++) {
        addShift(&bank, needle[index], bank.needleLength - index - 1);
    }
    return bank;
}

char * getString() {
    char * str = (char *) malloc(sizeof(char) * 100);
    char symbol;
    unsigned size = 1;
    while(scanf("%c", &symbol) && !feof(stdin)) {
        if(symbol == '\n') {
            break;
        }
        size++;
        if(size % 100 == 1) {
            str = (char *) realloc(str, size + 99);
            if(str == NULL) {
                size = 0;
                break;
            }
        }
        str[size - 2] = symbol;
    }
    if(size) {
        str[size - 1] = '\0';
    }
    str = realloc(str, size);
    assert(str != NULL);
    return str;
}

void BoyerMooreSearch(char * needle, char * haystack) {
    StopSymbols bank = createShiftsBank(needle);
    unsigned haystackLength = strlen(haystack);
    if(bank.needleLength > haystackLength && haystackLength) {
        printf("%d ", bank.needleLength);
        return;
    }
    for(unsigned index = bank.needleLength - 1; index < haystackLength;) {
        int current = bank.needleLength - 1, pos = index;
        for(; current >= 0; current--, pos--) {
            printf("%d ", pos + 1);
            if(haystack[pos] != needle[current]) {
                index += shiftNeedle(&bank, haystack[index]);
                break;
            }
        }
        if(current == -1) {
            index += shiftNeedle(&bank, haystack[index]);
        }
    }
}

int main() {
    setlocale(LC_ALL, "Rus");
    char needle[20];
    if(fgets(needle, 19, stdin) == NULL) {
        return 0;
    }
    needle[strlen(needle) - 1] = '\0';
    char * haystack = getString();
    BoyerMooreSearch(needle, haystack);
    free(haystack);
    return 0;
}
