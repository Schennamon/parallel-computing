#include <stdio.h>
#include <stdbool.h>
#include "../lib/lib.h"

void bubble_sort(int numbers[], size);

int main() {
  int numbers[] = { 5, 4, 3, 2, 1 };
  int size = sizeof(numbers) / sizeof(numbers[0]);

  printf("Before sorting:\n");
  print_array(numbers, size);

  printf("After sorting:\n");
  bubble_sort(numbers, size);
  print_array(numbers, size);

  return 0;
}

void bubble_sort(int numbers[], size) {
  int i, j, temp;
  bool sorted;

  for(i = 1; sorted == false; i++){
    sorted = true;

    for(j = 0; j < size; j++){
      if(numbers[j] < numbers[j - 1]) {
        temp = numbers[j];
        numbers[j] = numbers[j - 1];
        numbers[j - 1] = temp;

        sorted = false;
      }
    }
  }
}