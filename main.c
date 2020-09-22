#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#define ulli unsigned long long int
#define ldoub long double

int digitToInt(char textDigit, int numeralSystem) {
    int digit = -1;
    if(isdigit(textDigit)) digit = (int) (textDigit - '0');
        else {
            switch(textDigit) {
                case 'a':
                case 'A':
                    digit = 10;
                    break;
                case 'b':
                case 'B':
                    digit = 11;
                    break;
                case 'c':
                case 'C':
                    digit = 12;
                    break;
                case 'd':
                case 'D':
                    digit = 13;
                    break;
                case 'e':
                case 'E':
                    digit = 14;
                    break;
                case 'f':
                case 'F':
                    digit = 15;
                    break;
                default:
                    return -1;
            }
        }
    if(digit >= numeralSystem) return -1;
    return digit;
}

char intToDigit(int digit) {
    return (digit > 9 ? 'a' + (char) (digit - 10) : '0' + (char) digit);
}

void swap(char *a, char *b) {
    char t = *a;
    *a = *b;
    *b = t;
}

void strrev(char *str, int length) {
    for(int i = 0; i < length / 2; i++)
        swap(&str[i], &str[length - 1 - i]);
}

void convertNumber(char* inNumber, int numSysIn, char* outNumber, int numSysOut) {
    ulli number = 0, trunc = 0;
    ldoub fract = .0;
    int period = -1, power = 0, pos = 0;

    for(; power < 16 && inNumber[power] != '\0'; power++) {
        if(inNumber[power] == '.') {
            period = power;
            continue;
        }
        number *= numSysIn;
        number += (int) digitToInt(inNumber[power], numSysIn);
    }
    if(period == -1) trunc = number;
    else {
        fract = (ldoub) number / powl(numSysIn, power - period - 1.0);
        trunc = (ulli) fract;
        fract -= trunc;

        for(int i = 0; i < 12; i++) {
            fract *= numSysOut;
            int digit = (int) fract;
            outNumber[i] = intToDigit(digit);
            fract -= digit;
        }
        strrev(outNumber, 12);
        outNumber[12] = '.';
        pos = 13;
    }
    do {
        int digit = trunc % numSysOut;
        outNumber[pos] = intToDigit(digit);
        pos++;
        trunc /= numSysOut;
    } while(trunc > 0);
    strrev(outNumber, pos);
}

int main() {
    const char ERR[] = "bad input\n";
    const int MAX_IN_LENGTH = 15, MAX_OUT_LENGTH = 60;
    int numeralSystemFrom, numeralSystemTo;
    char numberFrom[MAX_IN_LENGTH], numberTo[MAX_OUT_LENGTH];
    for(int i = 0; i < MAX_OUT_LENGTH; i++) numberTo[i] = '\0';

    scanf("%d %d\n", &numeralSystemFrom, &numeralSystemTo);
    gets(numberFrom);

    if((numeralSystemFrom - 2) * (numeralSystemFrom - 16) > 0 || (numeralSystemTo - 2) * (numeralSystemTo - 16) > 0) {
        printf(ERR);
        return 0;
    }
    for(int i = 0; i < strlen(numberFrom); i++) {
        char textDigit = numberFrom[i];
        if(textDigit == '.') continue;
        int digit = digitToInt(textDigit, numeralSystemFrom);
        if(digit == -1) {
            printf(ERR);
            return 0;
        }
    }
    if(numeralSystemFrom == numeralSystemTo) {
        printf("%s\n", numberFrom);
        return 0;
    }
    convertNumber(numberFrom, numeralSystemFrom, numberTo, numeralSystemTo);

    printf("%s\n", numberTo);
    return 0;
}
