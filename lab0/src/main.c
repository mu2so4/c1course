#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#define FRACTION_PRECISION 12
#define MAX_IN_LENGTH 15
#define MAX_OUT_LENGTH 60
typedef unsigned long long int bigInteger;
typedef long double bigDouble;

int digitToInt(char textDigit, int numeralSystem) {
    int digit = -1;
    if(isdigit(textDigit)) {
        digit = (int) (textDigit - '0');
    }
    else {
        if(isupper(textDigit)) {
            digit = textDigit - 'A' + 10;
        }
        if(islower(textDigit)) {
            digit = textDigit - 'a' + 10;
        }
    }
    if(digit >= numeralSystem) {
        return -1;
    }
    return digit;
}

char intToDigit(int digit) {
    return (digit > 9 ? 'a' + (char) (digit - 10) : '0' + (char) digit);
}

int isBetweenNotStrict(int leftBound, int rightBound, int number) {
    return number >= leftBound && number <= rightBound;
}

void swap(char *a, char *b) {
    char t = *a;
    *a = *b;
    *b = t;
}

void strrev(char *str, int length) {
    for(int i = 0; i < length / 2; i++) {
        swap(&str[i], &str[length - 1 - i]);
    }
}

void convertNumber(char* inNumber, int numSysIn, char* outNumber, int numSysOut) {
    bigInteger number = 0, trunc = 0;
    int period = -1, power = 0, pos = 0, fractExists = 0;

    for(; power < MAX_IN_LENGTH + 1 && inNumber[power] != '\0'; power++) {
        if(inNumber[power] == '.') {
            period = power;
            continue;
        }
        int digit = (int) digitToInt(inNumber[power], numSysIn);
        number *= numSysIn;
        number += digit;
        fractExists |= period != -1 && digit != 0;
    }
    power--;
    if(period == -1) {
        trunc = number;
    }
    else {
        bigDouble fraction = (bigDouble) number / powl(numSysIn, power - period);
        trunc = (bigInteger) fraction;
        fraction -= trunc;
        if(fractExists) {
            for(int i = 0; i < FRACTION_PRECISION; i++) {
                fraction *= numSysOut;
                int digit = (int) fraction;
                outNumber[i] = intToDigit(digit);
                fraction -= digit;
            }
            strrev(outNumber, strlen(outNumber));
            outNumber[FRACTION_PRECISION] = '.';
            pos = FRACTION_PRECISION + 1;
        }
    }
    do {
        int digit = trunc % numSysOut;
        outNumber[pos] = intToDigit(digit);
        pos++;
        trunc /= numSysOut;
    } while(trunc > 0);
    strrev(outNumber, strlen(outNumber));
}

int main() {
    const char ERR[] = "bad input\n";
    int numeralSystemFrom, numeralSystemTo, periodsCount = 0;
    char numberFrom[MAX_IN_LENGTH], numberTo[MAX_OUT_LENGTH];
    for(int i = 0; i < MAX_OUT_LENGTH; i++) {
        numberTo[i] = '\0';
    }

    if(scanf("%d %d\n", &numeralSystemFrom, &numeralSystemTo) < 2) {
        return 0;
    }
    if(fgets(numberFrom, sizeof(numberFrom), stdin) == NULL) {
        return 0;
    }

    numberFrom[strlen(numberFrom) - 1] = '\0';

    if(!isBetweenNotStrict(2, 16, numeralSystemFrom) ||
       !isBetweenNotStrict(2, 16, numeralSystemTo)) {
        printf(ERR);
        return 0;
    }
    for(unsigned i = 0; i < strlen(numberFrom); i++) {
        char textDigit = numberFrom[i];
        if(textDigit == '.') {
            periodsCount++;
            if(periodsCount > 1 || i == 0) {
                printf(ERR);
                return 0;
            }
            continue;
        }
        int digit = digitToInt(textDigit, numeralSystemFrom);
        if(digit == -1) {
            printf(ERR);
            return 0;
        }
    }
    if(numberFrom[strlen(numberFrom) - 1] == '.') {
        printf(ERR);
        return 0;
    }
    convertNumber(numberFrom, numeralSystemFrom, numberTo, numeralSystemTo);

    printf("%s\n", numberTo);
    return 0;
}
