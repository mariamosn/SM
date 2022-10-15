// Maria Moșneag 343C1

#include <stdio.h>
#include <stdlib.h>

#define N 2

void calculate(float **a, float **b, float **c, int n);
void print(float **a, int n);
int alloc(float ***a, float ***b, float ***c, int n);
void read_matrix(float **a, int n);
void free_all(float ***a, float ***b, float ***c, int n);

int main() {
    int n;
    float **a, **b, **c;

    // or read from stdin
    n = N;

    if (alloc(&a, &b, &c, n))
        return -1;

    read_matrix(a, n);
    read_matrix(b, n);

    calculate(a, b, c, n);

    print(c, n);
    free_all(&a, &b, &c, n);

    return 0;
}

void calculate(float **a, float **b, float **c, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            // c[i][j] = 0.0;
            for (int k = 0; k < n; k++) {
                c[i][j] += a[i][k] * b[k][j];
            }
        }
    }
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
