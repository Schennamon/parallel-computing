#include <stdio.h>
#include <stdbool.h>
#include "../lib/lib.h"

void shell_sort(int numbers[], size);

int main() {
  int numbers[] = { 6, 10, 14, 1 };
  int size = sizeof(numbers) / sizeof(numbers[0]);

  printf("Before sorting:\n");
  print_array(numbers, size);

  printf("After sorting:\n");
  shell_sort(numbers, size);
  print_array(numbers, size);

  return 0;
}

void shell_sort(int numbers[], int n) {
  for (int gap = n / 2; gap > 0; gap /= 2) {
    for (int i = gap; i < n; i++) {
      int temp = numbers[i];
      int j;

      for (j = i; j >= gap && numbers[j - gap] > temp; j -= gap) {
        numbers[j] = numbers[j - gap];
      }
      numbers[j] = temp;
    }
  }
}