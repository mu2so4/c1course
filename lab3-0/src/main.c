#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int randomInt(int lower, int upper) {
    return rand() % (upper - lower) + lower;
}

void swap(int *number1, int *number2) {
    int exchange = *number1;
    *number1 = *number2;
    *number2 = exchange;
}

void quickSort(int *arr, int startIndex, int endIndex) {
    int left = startIndex + 1, right = endIndex - 1, same = 0;
    if(startIndex >= right) {
        return;
    }
    swap(&arr[startIndex], &arr[randomInt(startIndex, endIndex)]);
    while(left < right) {
        if(arr[startIndex] > arr[left]) {
            left++;
        }
        else if(arr[startIndex] == arr[left]) {
            if(same) {
                left++;
            }
            else {
                swap(&arr[left], &arr[right]);
                right--;
            }
            same = !same;
        }
        else {
            swap(&arr[left], &arr[right]);
            right--;
        }
    }
    if(arr[startIndex] < arr[left]) {
        left--;
    }
    swap(&arr[startIndex], &arr[left]);

    quickSort(arr, startIndex, left);
    quickSort(arr, left + 1, endIndex);
}

int main() {
    srand(time(NULL));
    int length;
    if(scanf("%d\n", &length) < 1) {
        return 0;
    }
    int array[length];
    for(int i = 0; i < length; i++) {
        if(scanf("%d", &array[i]) < 1) {
            return 0;
        }
    }

    quickSort(array, 0, length);

    for(int i = 0; i < length; i++) {
        if(i != 0) {
            printf(" ");
        }
        printf("%d", array[i]);
    }
    printf("\n");

    return 0;
}
