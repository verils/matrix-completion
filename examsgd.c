#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define DATA_ROWS 5
#define DATA_COLS 4

#define DISTORTION_RATE .1

#define ROUNDS 2

#define STEP 2

#define RANDOM_MAX 10

void print_matrix(int (*matrix)[], int rows, int cols);

void copy_matrix(int (*source)[], int (*target)[], int rows, int cols);

void distort_matrix(int (*matrix)[], int rows, int cols, double rate);

void predicate_matrix(int pInt[5][4], int pInt1[5][4], int i, int i1);

void multiply_matrix(int (*result)[], int (*matrix_a)[], int (*matrix_b)[], int rows, int cols, int rounds);

void sgd(int (*result)[], int (*matrix_a)[], int (*matrix_b)[], int rows, int cols, int rounds, int step);

int main() {
    // Initialize random seed
    srand(time(NULL));

    int real_data[DATA_ROWS][DATA_COLS] = {
            {5, 0, 0, 1},
            {4, 1, 0, 1},
            {1, 1, 0, 5},
            {1, 0, 0, 4},
            {0, 1, 5, 4}
    };

    int training_data[DATA_ROWS][DATA_COLS];
    copy_matrix(real_data, training_data, DATA_ROWS, DATA_COLS);
    distort_matrix(training_data, DATA_ROWS, DATA_COLS, DISTORTION_RATE);

    printf("Training data:\n");
    print_matrix(training_data, DATA_ROWS, DATA_COLS);

    int predicated_data[DATA_ROWS][DATA_COLS] = {0};
    predicate_matrix(real_data, training_data, DATA_ROWS, DATA_COLS);

    printf("Predicated data:\n");
    print_matrix(predicated_data, DATA_ROWS, DATA_COLS);

//    printf("Random p matrix:\n");
//    print_matrix(random_matrix_p, DATA_ROWS, ROUNDS);
//
//    printf("Random q matrix:\n");
//    print_matrix(random_matrix_q, ROUNDS, DATA_COLS);
//
//    printf("Multiply result matrix:\n");
//    print_matrix(result, DATA_ROWS, DATA_COLS);

    return 0;
}

void predicate_matrix(int pInt[5][4], int pInt1[5][4], int i, int i1) {

}

void print_matrix(int (*matrix)[], int rows, int cols) {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            printf("%d ", (*matrix)[i * cols + j]);
        }
        printf("\n");
    }
    printf("\n");
}

void copy_matrix(int (*source)[], int (*target)[], int rows, int cols) {
    int total = rows * cols;
    for (int i = 0; i < total; ++i) {
        (*target)[i] = (*source)[i];
    }
}

void distort_matrix(int (*matrix)[], int rows, int cols, double rate) {
    int matrix_size = rows * cols;
    int distorted_size = (int) (matrix_size * rate);

    // Prepare a pool storing sequence index of matrix
    int pool[matrix_size];
    for (int i = 0; i < matrix_size; ++i) {
        pool[i] = i;
    }

    // Execute the distortion, generate unique random by selecting from pool
    int pool_limit = matrix_size;
    for (int i = 0; i < distorted_size; ++i) {
        int pool_index = rand() % pool_limit;
        int matrix_index = pool[pool_index];
        pool[pool_index] = pool[pool_limit - 1];
        pool_limit--;
        (*matrix)[matrix_index] = -1;
    }
}

void multiply_matrix(int (*result)[], int (*matrix_a)[], int (*matrix_b)[], int rows, int cols, int rounds) {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            int tmp;
            for (int k = 0; k < rounds; ++k) {
                tmp = (*matrix_a)[i * rounds + k] * (*matrix_b)[k * rounds + j];
            }
            (*result)[i * cols + j] = tmp;
        }
    }
}

void sgd(int (*result)[], int (*matrix_a)[], int (*matrix_b)[], int rows, int cols, int rounds, int step) {

}
