#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define BLOCK_SIZE 2
#define MATRIX_SIZE 1000

void matrix_multiply(int **mat1, int **mat2, int **res, int n) {
    int i, j, k, ii, jj, kk;
    int sum;

    for (i = 0; i < n; i += BLOCK_SIZE) {
        for (j = 0; j < n; j += BLOCK_SIZE) {
            for (k = 0; k < n; k += BLOCK_SIZE) {
                for (ii = i; ii < i + BLOCK_SIZE; ii++) {
                    for (jj = j; jj < j + BLOCK_SIZE; jj++) {
                        sum = 0;
                        for (kk = k; kk < k + BLOCK_SIZE; kk++) {
                            sum += mat1[ii][kk] * mat2[kk][jj];
                        }
                        res[ii][jj] += sum;
                    }
                }
            }
        }
    }
}

void print_matrix(int** matrix, int size) {
    int i, j;
    for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }

    printf("\n");
}

int main() {
    struct timespec start_time, end_time;
    double time_used;
    int **mat1, **mat2, **res;
    int i, j;

    mat1 = (int **) malloc(sizeof(int *) * MATRIX_SIZE);
    for (i = 0; i < MATRIX_SIZE; i++) {
        mat1[i] = (int *) malloc(sizeof(int) * MATRIX_SIZE);
    }

    mat2 = (int **) malloc(sizeof(int *) * MATRIX_SIZE);
    for (i = 0; i < MATRIX_SIZE; i++) {
        mat2[i] = (int *) malloc(sizeof(int) * MATRIX_SIZE);
    }

    res = (int **) malloc(sizeof(int *) * MATRIX_SIZE);
    for (i = 0; i < MATRIX_SIZE; i++) {
        res[i] = (int *) malloc(sizeof(int) * MATRIX_SIZE);
    }

    // initialize matrices with random values
    for (i = 0; i < MATRIX_SIZE; i++) {
        for (j = 0; j < MATRIX_SIZE; j++) {
            mat1[i][j] = rand() % 10;
            mat2[i][j] = rand() % 10;
            res[i][j] = 0;
        }
    }

    // print first matrix
    printf("1st matrix:\n");
    print_matrix(mat1, MATRIX_SIZE);

    // print second matrix
    printf("2st matrix:\n");
    print_matrix(mat2, MATRIX_SIZE);

    clock_gettime(CLOCK_MONOTONIC, &start_time);
    matrix_multiply(mat1, mat2, res, MATRIX_SIZE);
    clock_gettime(CLOCK_MONOTONIC, &end_time);

    // print the result matrix
    for (i = 0; i < MATRIX_SIZE; i++) {
        for (j = 0; j < MATRIX_SIZE; j++) {
            printf("%d ", res[i][j]);
        }
        printf("\n");
    }

    time_used = (end_time.tv_sec - start_time.tv_sec) +
                (end_time.tv_nsec - start_time.tv_nsec) / 1000000000.0;

    printf("Time: %f seconds\n", time_used);

    // free memory
    for (i = 0; i < MATRIX_SIZE; i++) {
        free(mat1[i]);
        free(mat2[i]);
        free(res[i]);
    }
    free(mat1);
    free(mat2);
    free(res);

    return 0;
}
