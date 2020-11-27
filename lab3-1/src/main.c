#include <stdio.h>
#include <stdlib.h>

void swap(int * num1, int * num2) {
    if(num1 == NULL || num2 == NULL || num1 == num2) {
        return;
    }
    int t = *num1;
    *num1 = *num2;
    *num2 = t;
}

int max(int * array, int unsortedSize, int index1, int index2) {
    switch((index1 < unsortedSize) * 2 + (index2 < unsortedSize)) {
        case 0:
            return -1;
        case 1:
            return index2;
        case 2:
            return index1;
    }
    return array[index1] > array[index2] ? index1 : index2;
}

void createBinaryHeap(int * array, int unsortedSize, int index) {
    if(index >= unsortedSize) {
        return;
    }
    int nodes = max(array, unsortedSize, index * 2 + 1, index * 2 + 2);
    int greatest = (nodes == -1 ? index : max(array, unsortedSize, index, nodes));
    swap(&array[index], &array[greatest]);
    if(greatest != index) {
        createBinaryHeap(array, unsortedSize, greatest);
    }
}

int main() {
    int cou;
    if(scanf("%d", &cou) < 1) {
        return 0;
    }
    int * array = (int *) malloc(sizeof(int) * cou);
    if(array == NULL) {
        printf("Out of memory\n");
        abort();
    }
    for(int index = 0; index < cou; index++) {
        if(scanf("%d", &array[index]) < 1) {
            free(array);
            return 0;
        }
    }
    for(int index = cou / 2 - 1; index >= 0; index--) {
        createBinaryHeap(array, cou, index);
    }
    for(int index = cou - 1; index > 0; index--) {
        swap(&array[0], &array[index]);
        createBinaryHeap(array, index, 0);
    }
    for(int index = 0; index < cou; index++) {
        printf("%d ", array[index]);
    }
    free(array);
    return 0;
}
