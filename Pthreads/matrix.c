#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>

// Matrices
int **A, **B, **C;

// Argumentos
typedef struct {
    int start_row;
    int end_row;
    int n;
} ThreadData;

void *multiply(void *arg);
void fill_matrix_random(int **matrix, int size);
void print_matrix(int **matrix, int size);

int main(void) {

    srand(time(NULL));

    FILE *file4 = fopen("matrix_times_4_threads.csv", "w"); // Archivo CSV para 4 hilos
    FILE *file8 = fopen("matrix_times_8_threads.csv", "w"); // Archivo CSV para 8 hilos
    
    if (file4 == NULL || file8 == NULL) {
        printf("Error al abrir los archivos.\n");
        return 1;
    }

    fprintf(file4, "N,Threads,Time\n"); // Encabezado del CSV para 4 hilos
    fprintf(file8, "N,Threads,Time\n"); // Encabezado del CSV para 8 hilos

    const int sizes[] = {256, 512, 1024, 2048, 4096}; // Tamaños de matriz a probar
    const int num_sizes = sizeof(sizes) / sizeof(sizes[0]);
    const int threads[] = {4, 8}; // Número de hilos a probar
    const int num_threads = sizeof(threads) / sizeof(threads[0]);
    pthread_t *threads_handle;
    ThreadData *thread_data;

    struct timeval start, end;

    for (int t = 0; t < num_threads; t++) {
        for (int s = 0; s < num_sizes; s++) {

            const int N = sizes[s];
            const int THREAD_COUNT = threads[t];

            // Reservar memoria dinámica para las matrices
            A = (int **)malloc(N * sizeof(int *));
            B = (int **)malloc(N * sizeof(int *));
            C = (int **)malloc(N * sizeof(int *));
            for (int i = 0; i < N; i++) {
                A[i] = (int *)malloc(N * sizeof(int));
                B[i] = (int *)malloc(N * sizeof(int));
                C[i] = (int *)malloc(N * sizeof(int));
            }

            // Llenar las matrices A y B con valores aleatorios
            fill_matrix_random(A, N);
            fill_matrix_random(B, N);

            // Medir el tiempo de ejecución de los 3 bucles
            gettimeofday(&start, NULL);

            // Crear y asignar hilos
            threads_handle = (pthread_t *)malloc(THREAD_COUNT * sizeof(pthread_t));
            thread_data = (ThreadData *)malloc(THREAD_COUNT * sizeof(ThreadData));

            int rows_per_thread = N / THREAD_COUNT;

            // Crear hilos y asignarle tareas
            for (int i = 0; i < THREAD_COUNT; i++) {
                thread_data[i].n = N;
                thread_data[i].start_row = i * rows_per_thread;
                thread_data[i].end_row = (i == THREAD_COUNT - 1) ? N : (i + 1) * rows_per_thread;
                pthread_create(&threads_handle[i], NULL, multiply, &thread_data[i]);
            }

            // Esperar a que los hilos terminen
            for (int i = 0; i < THREAD_COUNT; i++) {
                pthread_join(threads_handle[i], NULL);
            }

            // Finalizacion de la medicion del tiempo
            gettimeofday(&end, NULL);

            // Calcular el tiempo transcurrido
            double elapsed_time = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1e6;
            printf("Nro de threads: %d, tamano de matriz: %d, Tiempo de ejecucion: %f segundos\n\n", THREAD_COUNT, N, elapsed_time);

            if (THREAD_COUNT == 4) {
                fprintf(file4, "%d,%d,%f\n", N, THREAD_COUNT, elapsed_time);
            } else if (THREAD_COUNT == 8) {
                fprintf(file8, "%d,%d,%f\n", N, THREAD_COUNT, elapsed_time);
            }

            // Liberar memoria de la matrices e hilos
            for (int i = 0; i < N; i++) {
                free(A[i]);
                free(B[i]);
                free(C[i]);
            }
            free(A);
            free(B);
            free(C);    
            free(threads_handle);
            free(thread_data); 
        }
    }

    fclose(file4);
    fclose(file8);
    return 0;
}

void *multiply(void *arg) {
    ThreadData *data = (ThreadData *)arg;
    for (int i = data->start_row; i < data->end_row; i++) {
        for (int j = 0; j < data->n; j++) {
            C[i][j] = 0;
            for (int k = 0; k < data->n; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    pthread_exit(NULL);
}

void fill_matrix_random(int **matrix, int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            matrix[i][j] = (rand() % 2001) - 1000;  // Valores entre -1000 y 1000
        }
    }
}

void print_matrix(int **matrix, int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}
