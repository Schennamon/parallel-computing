#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

void counting_sort(int* array, int size, int rank, int num_procs) {
    int max_value = 0;
    for (int i = 0; i < size; i++) {
        if (array[i] > max_value) {
            max_value = array[i];
        }
    }
    MPI_Allreduce(&max_value, &max_value, 1, MPI_INT, MPI_MAX, MPI_COMM_WORLD);

    int* counts = (int*) calloc(max_value + 1, sizeof(int));
    int* output = (int*) calloc(size, sizeof(int));

    for (int i = 0; i < size; i++) {
        counts[array[i]]++;
    }

    for (int i = 1; i <= max_value; i++) {
        counts[i] += counts[i - 1];
    }

    for (int i = size - 1; i >= 0; i--) {
        output[--counts[array[i]]] = array[i];
    }

    for (int i = 0; i < size; i++) {
        array[i] = output[i];
    }

    free(counts);
    free(output);
}

int main(int argc, char** argv) {
    int rank, num_procs;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

    int size = 100;
    int* array = (int*) malloc(sizeof(int) * size);

    // Инициализация массива случайными значениями
    srand(1 + rank);
    for (int i = 0; i < size; i++) {
        array[i] = rand() % 1000;
    }

    // Вывод первоначального массива
    printf("Rank %d, initial array: ", rank);
    for (int i = 0; i < size; i++) {
        printf("%d ", array[i]);
    }
    printf("\n\n");

    // Сортировка подсчетом
    counting_sort(array, size, rank, num_procs);

    // Вывод отсортированного массива
    printf("Rank %d, sorted array: ", rank);
    for (int i = 0; i < size; i++) {
        printf("%d ", array[i]);
    }
    printf("\n\n");

    // Сбор всех отсортированных массивов в один массив
    int* sorted_array = NULL;
    if (rank == 0) {
        sorted_array = (int*) malloc(sizeof(int) * size * num_procs);
    }
    MPI_Gather(array, size, MPI_INT, sorted_array, size, MPI_INT, 0, MPI_COMM_WORLD);

    // Вывод единого отсортированного массива на 0 процессе
    if (rank == 0) {
        printf("Final sorted array: ");
        for (int i = 0; i < size * num_procs; i++) {
            printf("%d ", sorted_array[i]);
        }
        printf("\n\n");
        free(sorted_array);
    }

    MPI_Finalize();

    return 0;
}
