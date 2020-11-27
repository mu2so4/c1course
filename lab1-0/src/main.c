#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <locale.h>
typedef struct StopSymbols StopSymbols;
typedef struct Haystack Haystack;
const int PORTION_LENGTH = 100;

struct StopSymbols {
    int shifts[256];
    int needleLength;
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
    for(int index = 0; index < 256; index++) {
        bank.shifts[index] = 0;
    }
    for(int index = 0; index < bank.needleLength - 1; index++) {
        addShift(&bank, needle[index], bank.needleLength - index - 1);
    }
    return bank;
}

struct Haystack {
    char * portion;
    int startIndex;
    FILE * file;
};

Haystack createHaystack(FILE * file) {
    Haystack haystack = {.startIndex = 0, .file = file};
    haystack.portion = (char *) malloc(PORTION_LENGTH * 2 + 1);
    if(haystack.portion == NULL) {
        printf("Out of memory\n");
        abort();
    }
    for(int index = 0; index < PORTION_LENGTH * 2; index++) {
        char symbol;
        if(fscanf(file, "%c", &symbol) < 1) {
            haystack.portion[index] = '\0';
        }
        else {
            haystack.portion[index] = symbol;
        }
    }
    haystack.portion[PORTION_LENGTH * 2] = '\0';
    return haystack;
}

void destroyHaystack(Haystack * haystack) {
    free(haystack->portion);
}

char getSymbol(Haystack * haystack, int index) {
    assert(index >= haystack->startIndex);
    if(index - haystack->startIndex >= PORTION_LENGTH * 2 && !feof(haystack->file)) {
        int pos = 0;
        for(; pos < PORTION_LENGTH; pos++) {
            haystack->portion[pos] = haystack->portion[pos + PORTION_LENGTH];
        }
        for(pos = PORTION_LENGTH; pos < PORTION_LENGTH * 2; pos++) {
            if(fscanf(haystack->file, "%c", &haystack->portion[pos]) < 1) {
                haystack->portion[pos] = '\0';
            }
        }
        haystack->startIndex += PORTION_LENGTH;
    }
    return index >= haystack->startIndex + PORTION_LENGTH * 2 && feof(haystack->file) ? '\0' :
            haystack->portion[index - haystack->startIndex];
}

void BoyerMooreSearch(char * needle, FILE * haystackInput) {
    StopSymbols bank = createShiftsBank(needle);
    Haystack haystack = createHaystack(haystackInput);
    for(unsigned index = bank.needleLength - 1;;) {
        int current = bank.needleLength - 1, pos = index;
        for(; current >= 0; current--, pos--) {
            char symbol = getSymbol(&haystack, pos);
            if(symbol == '\0') {
                destroyHaystack(&haystack);
                return;
            }
            printf("%d ", pos + 1);
            if(symbol != needle[current]) {
                index += shiftNeedle(&bank, getSymbol(&haystack, index));
                break;
            }
        }
        if(current == -1) {
            index += shiftNeedle(&bank, getSymbol(&haystack, index));
        }
    }
}

int main() {
    setlocale(LC_ALL, "Rus");

    char needle[20];
    if(fgets(needle, 20, stdin) == NULL) {
        return 0;
    }
    needle[strlen(needle) - 1] = '\0';
    BoyerMooreSearch(needle, stdin);
    return 0;
}
