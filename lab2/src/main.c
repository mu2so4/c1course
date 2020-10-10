#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#define SEQUENCE_MAX_LENGTH 100
#define DIGITS_COUNT 10
#define BAD_INPUT "bad input\n"

void printAllVarsInternal(
        const char number[SEQUENCE_MAX_LENGTH],
        int* count,
        char* candidate,
        char* digits,
        short freeDigits,
        short depth,
        short more) {
    //printf("%d %d\n", *count, freeDigits);
    if(*count == 0) return;
    if(freeDigits == 0) {
        printf("%s\n", candidate);
        *count = *count - 1;
        return;
    }

    for(short index = 0; index < DIGITS_COUNT; index++) {
        if(digits[index] == 0) continue;
        digits[index] = 0;
        char current = '0' + index;
        candidate[strlen(candidate)] = current;
        //printf("%s\n", candidate);
        if(more) printAllVarsInternal(number, count, candidate, digits, freeDigits - 1, depth + 1, 1);
        else if(!(current < number[depth] || (freeDigits == 1 && current == number[depth])))
            printAllVarsInternal(number, count, candidate, digits, freeDigits - 1, depth + 1, current > number[depth]);

        candidate[strlen(candidate) - 1] = '\0';
        digits[index] = 1;
    }
}

int printAllVars(const char number[SEQUENCE_MAX_LENGTH], int count) {
    short freeDigits = 0;
    char candidate[DIGITS_COUNT + 1], digits[DIGITS_COUNT];
    for(int index = 0; index < DIGITS_COUNT; index++)
        digits[index] = candidate[index] = 0;
    candidate[DIGITS_COUNT] = '\0';

    for(unsigned short index = 0; index < strlen(number); index++) {
        char symbol = number[index];
        if(!isdigit(symbol)) return 0;
        short digit = symbol - '0';
        digits[digit]++;
        freeDigits++;
        if(digits[digit] > 1) return 0;
    }
    if(freeDigits == 0) return 0;

    printAllVarsInternal(number, &count, candidate, digits, freeDigits, 0, 0);
    return 1;
}

int main() {
    char sequence[SEQUENCE_MAX_LENGTH];
    int count;
    if(!fgets(sequence, SEQUENCE_MAX_LENGTH, stdin)) exit(1);
    sequence[strlen(sequence) - 1] = '\0';
    if(!scanf("%d", &count)) exit(1);

    if(!printAllVars(sequence, count)) printf(BAD_INPUT);
    return 0;
}
