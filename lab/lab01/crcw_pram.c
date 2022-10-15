// Maria Moșneag 343C1

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define N 2
#define P N * N * N

float **a, **b, **c;
pthread_mutex_t mutex[N][N];

void *thread_function(void *);

void print(float **a, int n);
int alloc(float ***a, float ***b, float ***c, int n);
void read_matrix(float **a, int n);
void free_all(float ***a, float ***b, float ***c, int n);
int init_mutex();
void destroy_mutex();

int main() {
    alloc(&a, &b, &c, N);
    read_matrix(a, N);
    read_matrix(b, N);

    if (init_mutex()) {
        free_all(&a, &b, &c, N);
        return -1;
    }

    pthread_t thread_id[P];
    int v[P][3], index = 0;

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            for (int k = 0; k < N; k++) {
                v[index][0] = i;
                v[index][1] = j;
                v[index][2] = k;
                pthread_create(&thread_id[index], NULL, thread_function, v[index]);
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

    print(c, N);
    
    free_all(&a, &b, &c, N);
    destroy_mutex();

    return 0;
}

void *thread_function(void *arg) {
    int *v, i, j, k, p;
    
    v = arg;
    i = v[0];
    j = v[1];
    k = v[2];

    p = a[i][k] * b[k][j];

    pthread_mutex_lock(&mutex[i][j]);
    c[i][j] += p;
    pthread_mutex_unlock(&mutex[i][j]);
}

void print(float **a, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%f ", a[i][j]);
        }
        printf("\n");
    }
}

int alloc(float ***a, float ***b, float ***c, int n) {
    // allocate memory for a
    *a = malloc(n * sizeof(float *));
    if (*a == NULL)
        return -1;
    for (int i = 0; i < n; i++) {
        (*a)[i] = malloc(n * sizeof(float));
        if ((*a)[i] == NULL) {
            // free a
            for (int j = 0; j < i; j++)
                free((*a)[j]);
            free(*a);
            return -1;
        }
    }

    // allocate memory for b
    *b = malloc(n * sizeof(float *));
    if (*b == NULL) {
        // free a
        for (int j = 0; j < n; j++)
            free((*a)[j]);
        free(*a);

        return -1;
    }
    for (int i = 0; i < n; i++) {
        (*b)[i] = malloc(n * sizeof(float));
        if ((*b)[i] == NULL) {
            // free a
            for (int j = 0; j < n; j++)
                free((*a)[j]);
            free(*a);

            // free b
            for (int j = 0; j < i; j++)
                free((*b)[j]);
            free(*b);

            return -1;
        }
    }

    // allocate memory for c
    *c = calloc(n, sizeof(float *));
    if (*c == NULL) {
        // free a and b
        for (int j = 0; j < n; j++) {
            free((*a)[j]);
            free((*b)[j]);
        }
        free(*a);
        free(*b);

        return -1;
    }
    for (int i = 0; i < n; i++) {
        (*c)[i] = calloc(n, sizeof(float));
        if ((*c)[i] == NULL) {
            // free a and b
            for (int j = 0; j < n; j++) {
                free((*a)[j]);
                free((*b)[j]);
            }
            free(*a);
            free(*b);

            // free c
            for (int j = 0; j < i; j++)
                free((*c)[j]);
            free(*c);

            return -1;
        }
    }

    return 0;
}

void read_matrix(float **a, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            scanf("%f", &a[i][j]);
        }
    }
}

void free_all(float ***a, float ***b, float ***c, int n) {
    for (int i = 0; i < n; i++) {
        free((*a)[i]);
        free((*b)[i]);
        free((*c)[i]);
    }
    free(*a);
    free(*b);
    free(*c);
}

int init_mutex() {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (pthread_mutex_init(&mutex[i][j], NULL))
                return -1;
        }
    }

    return 0;
}

void destroy_mutex() {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            pthread_mutex_destroy(&mutex[i][j]);
        }
    }
}
