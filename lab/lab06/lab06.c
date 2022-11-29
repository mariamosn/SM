// Maria Moșneag 343C1

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define MAIN 0
#define CHUNK 5
#define SECRET 7
#define TASKS 5

int main(int argc, char *argv[]) {
    int numtasks, rank;
    int *v, secret, small_v[CHUNK];

    #pragma omp parallel
    {
        if (omp_get_thread_num() == MAIN) {
            numtasks = TASKS;
            v = malloc(numtasks * CHUNK * sizeof(int));
        }

        #pragma omp barrier

        #pragma omp for
        for (int i = 0; i < numtasks * CHUNK; i++) {
            v[i] = i;
        }

        if (omp_get_thread_num() == MAIN) {
            printf("original array: ");
            for (int i = 0; i < numtasks * CHUNK; i++)
                printf("%d ", v[i]);
            printf("\n");

            printf("compute: 2 * v[i] + secret\n");

            secret = SECRET;
            printf("secret: %d\n", secret);
        }

        #pragma omp barrier

        #pragma omp for
        for (int i = 0; i < numtasks * CHUNK; i++) {
            v[i] = 2 * v[i] + secret;
        }

        if (omp_get_thread_num() == MAIN) {
            printf("result: ");
            for (int i = 0; i < numtasks * CHUNK; i++)
                printf("%d ", v[i]);
            printf("\n");
            free(v);
        }
    }
    return 0;
}
