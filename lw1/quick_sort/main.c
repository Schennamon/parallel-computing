#include <stdio.h>
#include <stdbool.h>
#include "../lib/lib.h"

void quick_sort(int arr[], int left, int right);

int main() {
  int numbers[] = { 61, 109, 149, 111, 34, 2, 24, 119, 122, 125, 27, 145 };
  int size = sizeof(numbers) / sizeof(numbers[0]);

  printf("Before sorting:\n");
  print_array(numbers, size);

  printf("After sorting:\n");
  quick_sort(numbers, 0, size - 1);
  print_array(numbers, size);

  return 0;
}

void quick_sort(int arr[], int left, int right) {
  int i = left;
  int j = right;
  int temp;
  int pivot = arr[(left + right) / 2];

  // Partition the array
  while (i <= j) {
    while (arr[i] < pivot) {
      i++;
    }
    while (arr[j] > pivot) {
      j--;
    }
    if (i <= j) {
      temp = arr[i];
      arr[i] = arr[j];
      arr[j] = temp;
      i++;
      j--;
    }
  }

  // Recursively sort the sub-arrays
  if (left < j) {
    quick_sort(arr, left, j);
  }
  if (i < right) {
    quick_sort(arr, i, right);
  }
}
