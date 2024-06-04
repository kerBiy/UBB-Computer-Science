// 12a. Write a C program that reads a matrix of integers from a file. It then creates as 
// many threads as there are rows in the matrix, each thread calculates the sum of the numbers 
// on a row. The main process waits for the threads to finish, then prints the sums.

#include <stdio.h>
#include <pthread.h>

#define MAX_ROWS 10001
#define MAX_COLS 1000

pthread_mutex_t mutex;

int a[MAX_ROWS][MAX_COLS];
int row_count, col_count;
int res;

void *handler(void *value) {
    int row = (int)value;
    
    int sum = 0;
    for (int col = 0; col < col_count; ++col) {
        sum += a[row][col];
    }

    pthread_mutex_lock(&mutex);
    res += sum;
    pthread_mutex_unlock(&mutex);

    return NULL;
}

int main() {
    FILE *file = fopen("input.txt", "r");

    if (file == NULL) {
        perror("Error file");
        return 1;
    }

    fscanf(file, "%d", &row_count);
    fscanf(file, "%d", &col_count);

    for (int i = 0 ; i < row_count; ++i) {
        for (int j = 0; j < col_count; ++j) {
            fscanf(file, "%d", &a[i][j]);
        }
    }
    
    fclose(file);

    pthread_mutex_init(&mutex, NULL);

    pthread_t thread[row_count];
    for (int i = 0; i < row_count; ++i) {
        pthread_create(&thread[i], NULL, handler, (void*)i);
    }

    for (int i = 0; i < row_count; ++i) {
        pthread_join(thread[i], NULL);
    }

    printf("The sum is: %d\n", res);
    return 0;
}
