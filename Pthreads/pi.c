#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>

// Variables globales
long points_in_circle = 0; // Contador de puntos dentro del círculo
pthread_mutex_t mutex;    // Mutex para proteger la variable compartida

typedef struct {
    long num_points;
} ThreadData;

// Función que ejecutarán los hilos
void* monte_carlo_pi(void* arg);

int main() {

    FILE *file4 = fopen("pi_4_threads.csv", "w");
    FILE *file8 = fopen("pi_8_threads.csv", "w");
    FILE *file16 = fopen("pi_16_threads.csv", "w");

    if (file4 == NULL || file8 == NULL || file16 == NULL) {
        printf("Error al abrir los archivos.\n");
        return 1;
    }

    fprintf(file4, "num_points,threads,pi_estimate,execution_time,precision\n");
    fprintf(file8, "num_points,threads,pi_estimate,execution_time,precision\n");
    fprintf(file16, "num_points,threads,pi_estimate,execution_time,precision\n");

    const int num_points_list[] = {1000, 10000, 100000, 1000000, 10000000};
    const int num_experiments = sizeof(num_points_list) / sizeof(num_points_list[0]);
    const int threads[] = {4, 8, 16};
    const int num_threads = sizeof(threads) / sizeof(threads[0]);
    const double pi_real = 3.141592653589793;

    pthread_t *threads_handle;
    ThreadData *thread_data;
    pthread_mutex_init(&mutex, NULL);

    struct timeval start, end;

    for (int t = 0; t < num_threads; t++) {
        for (int i = 0; i < num_experiments; i++) {
            const int THREAD_COUNT = threads[t];
            const long NUM_POINTS = num_points_list[i];
            const long POINTS_PER_THREAD = NUM_POINTS / THREAD_COUNT;

            gettimeofday(&start, NULL); // Inicio de medición de tiempo

            threads_handle = (pthread_t *)malloc(THREAD_COUNT * sizeof(pthread_t));
            thread_data = (ThreadData *)malloc(THREAD_COUNT * sizeof(ThreadData));

            // Crear los hilos
            for (int i = 0; i < THREAD_COUNT; i++) {
                thread_data[i].num_points = POINTS_PER_THREAD;
                pthread_create(&threads_handle[i], NULL, monte_carlo_pi, &thread_data[i]);
            }

            // Esperar a que los hilos terminen
            for (int i = 0; i < THREAD_COUNT; i++) {
                pthread_join(threads_handle[i], NULL);
            }

            // Calcular la estimación de Pi
            const double pi_estimate = 4.0 * points_in_circle / (double)NUM_POINTS;
            gettimeofday(&end, NULL); // Fin de medición de tiempo

            points_in_circle = 0;
            const double elapsed_time = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0;
            const double precision = fabs(pi_estimate - pi_real);
            printf("Nro de threads: %d, Nro de puntos: %ld, Estimacion de Pi: %f\n", THREAD_COUNT, NUM_POINTS, pi_estimate);
            printf("Tiempo de ejecucion: %f segundos\n\n", elapsed_time);

            if (THREAD_COUNT == 4) {
                fprintf(file4, "%ld,%d,%.6f,%.6f,%.6f\n", NUM_POINTS, THREAD_COUNT, pi_estimate, elapsed_time, precision);
            } else if (THREAD_COUNT == 8) {
                fprintf(file8, "%ld,%d,%.6f,%.6f,%.6f\n", NUM_POINTS, THREAD_COUNT, pi_estimate, elapsed_time, precision);
            } else if (THREAD_COUNT == 16) {
                fprintf(file16, "%ld,%d,%.6f,%.6f,%.6f\n", NUM_POINTS, THREAD_COUNT, pi_estimate, elapsed_time, precision);
            }

            free(threads_handle);
            free(thread_data); 
        }
    }

    pthread_mutex_destroy(&mutex);
    fclose(file4);
    fclose(file8);
    fclose(file16);
    return 0;
}

void* monte_carlo_pi(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    long local_points_in_circle = 0;
    unsigned int seed = time(NULL) ^ pthread_self(); // Semilla única por hilo

    for (long i = 0; i < data->num_points; i++) {
        double x = (double)rand_r(&seed) / RAND_MAX;
        double y = (double)rand_r(&seed) / RAND_MAX;
        if ((x * x + y * y) <= 1.0) {
            local_points_in_circle++;
        }
    }

    // Sección crítica: actualizar el contador global
    pthread_mutex_lock(&mutex);
    points_in_circle += local_points_in_circle;
    pthread_mutex_unlock(&mutex);

    pthread_exit(NULL);
}