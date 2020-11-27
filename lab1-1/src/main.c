#include <stdio.h>
#include <stdlib.h>

int main() {
    FILE * input = fopen("in.txt", "r"), * output = fopen("out.txt", "w");
    char symbol;
    while(fscanf(input, "%c", &symbol) && !feof(input));
    fprintf(output, "%d\n", symbol);
    fclose(input);
    fclose(output);
    return 0;
}
