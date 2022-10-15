// Maria Moșneag 343C1

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define N 2
#define P N * N * N

float a[N][N], b[N][N], v[N][N][N];
pthread_mutex_t mutex[N][N][N];
pthread_barrier_t barrier;

void *thread_function(void *);

void print(float **a, int n);
void read_matrix_a();
void read_matrix_b();

int main() {
    read_matrix_a();
    read_matrix_b();

    pthread_t thread_id[P];
    int aux[P][3], index = 0;

    if (pthread_barrier_init(&barrier, NULL, P))
        return -1;

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            for (int k = 0; k < N; k++) {
                aux[index][0] = i;
                aux[index][1] = j;
                aux[index][2] = k;
                pthread_create(&thread_id[index], NULL, thread_function, aux[index]);
                index++;
            }
        }
    }

    index = 0;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            for (int k = 0; k < N; k++) {
                pthread_join(thread_id[index], NULL);
                index++;
            }
        }
    }

    // print results
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%f ", v[i][j][0]);
        }
        printf("\n");
    }

    pthread_barrier_destroy(&barrier);

    return 0;
}

void *thread_function(void *arg) {
    int *aux, i, j, k, p;
    
    aux = arg;
    i = aux[0];
    j = aux[1];
    k = aux[2];

    p = a[i][k] * b[k][j];
    v[i][j][k] = p;

    pthread_barrier_wait(&barrier);

    for (int m = 1; m < N; m *= 2) {
        if (k % (2 * m) == 0 && k + m < N) {
            v[i][j][k] += v[i][j][k + m];
        }
        pthread_barrier_wait(&barrier);
    }
}

void print_res() {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%f ", v[i][j][0]);
        }
        printf("\n");
    }
}

void read_matrix_a() {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            scanf("%f", &a[i][j]);
        }
    }
}

void read_matrix_b() {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            scanf("%f", &b[i][j]);
        }
    }
}
