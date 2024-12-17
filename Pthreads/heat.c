#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>

// Argumentos
typedef struct {
    double *u;
    double *u_next;
    int start;
    int end;
    int N;
    double alpha;
} ThreadData;

void *compute_diffusion(void *arg);
void heat_diffusion(double *u, double *u_next, int N, int iterations, double alpha, int num_threads);

int main() {
    FILE *file = fopen("heat_results_dense.csv", "w");
    if (file == NULL) {
        printf("Error al abrir el archivo.\n");
        return 1;
    }

    fprintf(file, "N,iterations,execution_time\n");

    double alpha = 0.01;
    int num_threads = 4; // Numero de hilos

    int N_values[] = {1000, 2000, 5000, 10000, 20000, 50000, 100000, 200000, 500000};
    int iterations_values[] = {100, 200, 300, 500, 1000, 2000, 5000};

    struct timeval start, end;

    for (int i = 0; i < sizeof(N_values) / sizeof(N_values[0]); i++) {
        int N = N_values[i];
        for (int j = 0; j < sizeof(iterations_values) / sizeof(iterations_values[0]); j++) {
            int iterations = iterations_values[j];

            double *u = malloc(N * sizeof(double));
            double *u_next = malloc(N * sizeof(double));

            for (int k = 0; k < N; k++) {
                u[k] = (k == 0 || k == N - 1) ? 100.0 : 0.0; // Condiciones iniciales
            }

            // Inicio de la medicion del tiempo
            gettimeofday(&start, NULL);

            heat_diffusion(u, u_next, N, iterations, alpha, num_threads);

            // Final de la medicion del tiempo
            gettimeofday(&end, NULL);
            double elapsed_time = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1e6;

            fprintf(file, "%d,%d,%.6f\n", N, iterations, elapsed_time);

            free(u);
            free(u_next);
        }
    }

    fclose(file);
    return 0;
}

void *compute_diffusion(void *arg) {
    ThreadData *data = (ThreadData *)arg;
    for (int i = data->start; i < data->end; i++) {
        data->u_next[i] = data->u[i] + data->alpha * (data->u[i - 1] - 2 * data->u[i] + data->u[i + 1]);
    }
    pthread_exit(NULL);
}

void heat_diffusion(double *u, double *u_next, int N, int iterations, double alpha, int num_threads) {
    pthread_t threads_handle[num_threads];
    ThreadData thread_data[num_threads];

    for (int iter = 0; iter < iterations; iter++) {
        int chunk_size = (N - 2) / num_threads;
        for (int t = 0; t < num_threads; t++) {
            thread_data[t].u = u;
            thread_data[t].u_next = u_next;
            thread_data[t].start = t * chunk_size + 1;
            thread_data[t].end = (t == num_threads - 1) ? (N - 1) : (thread_data[t].start + chunk_size);
            thread_data[t].N = N;
            thread_data[t].alpha = alpha;
            pthread_create(&threads_handle[t], NULL, compute_diffusion, &thread_data[t]);
        }

        for (int t = 0; t < num_threads; t++) {
            pthread_join(threads_handle[t], NULL);
        }

        // Intercambie u y u_next para la siguiente iteración
        double *temp = u;
        u = u_next;
        u_next = temp;
    }
}