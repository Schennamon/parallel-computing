#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define BLOCK_SIZE 2
#define MATRIX_SIZE 6

void matrix_multiply(int **mat1, int **mat2, int **res, int n, int rank, int num_procs) {
    int i, j, k, ii, jj, kk;
    int sum;

    // divide matrices into blocks
    int block_size = n / num_procs;
    int start_row = rank * block_size;
    int end_row = (rank + 1) * block_size;
    if (rank == num_procs - 1) {
        end_row = n;
    }

    // allocate buffers for blocks
    int **mat1_block = (int **) malloc(sizeof(int *) * block_size);
    for (i = 0; i < block_size; i++) {
        mat1_block[i] = (int *) malloc(sizeof(int) * n);
    }

    int **mat2_block = (int **) malloc(sizeof(int *) * n);
    for (i = 0; i < n; i++) {
        mat2_block[i] = (int *) malloc(sizeof(int) * block_size);
    }

    int **res_block = (int **) malloc(sizeof(int *) * block_size);
    for (i = 0; i < block_size; i++) {
        res_block[i] = (int *) malloc(sizeof(int) * n);
        for (j = 0; j < n; j++) {
            res_block[i][j] = 0;
        }
    }

    // scatter mat1 and broadcast mat2
    MPI_Scatter(&mat1[0][0], block_size * n, MPI_INT, &mat1_block[0][0], block_size * n, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&mat2[0][0], n * n, MPI_INT, 0, MPI_COMM_WORLD);

    // perform matrix multiplication
    for (i = 0; i < block_size; i += BLOCK_SIZE) {
        for (j = 0; j < n; j += BLOCK_SIZE) {
            for (k = 0; k < n; k += BLOCK_SIZE) {
                for (ii = i; ii < i + BLOCK_SIZE; ii++) {
                    for (jj = j; jj < j + BLOCK_SIZE; jj++) {
                        sum = 0;
                        for (kk = k; kk < k + BLOCK_SIZE; kk++) {
                            sum += mat1_block[ii][kk] * mat2[kk][jj];
                        }
                        res_block[ii][jj] += sum;
                    }
                }
            }
        }
    }

    // gather the result
    MPI_Gather(&res_block[0][0], block_size * n, MPI_INT, &res[0][0], block_size * n, MPI_INT, 0, MPI_COMM_WORLD);

    // free memory
    for (i = 0; i < block_size; i++) {
        free(mat1_block[i]);
        free(res_block[i]);
    }
    for (i = 0; i < n; i++) {
        free(mat2_block[i]);
    }
    free(mat1_block);
    free(mat2_block);
    free(res_block);
}

int main(int argc, char **argv) {
    int **mat1, **mat2, **res;
    int i, j, my_rank, num_procs;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

    // initialize matrices with random values
    for (i = 0; i < MATRIX_SIZE; i++) {
        for (j = 0; j < MATRIX_SIZE; j++) {
            mat1[i][j] = rand() % 10;
            mat2[i][j] = rand() % 10;
            res[i][j] = 0;
        }
    }

    matrix_multiply(mat1, mat2, res, MATRIX_SIZE, my_rank, num_procs);

    // print the result matrix from each process
    for (i = 0; i < MATRIX_SIZE; i++) {
        for (j = 0; j < MATRIX_SIZE; j++) {
            printf("%d ", res[i][j]);
        }
        printf("\n");
    }

    MPI_Finalize();
    return 0;
}