// Maria Moșneag 343C1

#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

#define MAIN 0
#define CHUNK 5
#define SECRET 7

int main(int argc, char *argv[]) {
    int numtasks, rank;
    int *v, secret, small_v[CHUNK];
 
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == MAIN) {
        v = malloc(numtasks * CHUNK * sizeof(int));
        for (int i = 0; i < numtasks * CHUNK; i++) {
            v[i] = i;
        }

        printf("original array: ");
        for (int i = 0; i < numtasks * CHUNK; i++)
            printf("%d ", v[i]);
        printf("\n");

        printf("compute: 2 * v[i] + secret\n");

        secret = SECRET;
        printf("secret: %d\n", secret);

    }

    MPI_Bcast(&secret, 1, MPI_INT, MAIN, MPI_COMM_WORLD);
    MPI_Scatter(v, CHUNK, MPI_INT, small_v, CHUNK, MPI_INT, MAIN, MPI_COMM_WORLD);

    for (int i = 0; i < CHUNK; i++) {
        small_v[i] = 2 * small_v[i] + secret;
    }

    MPI_Gather(small_v, CHUNK, MPI_INT, v, CHUNK, MPI_INT, MAIN, MPI_COMM_WORLD);

    if (rank == MAIN) {
        printf("result: ");
        for (int i = 0; i < numtasks * CHUNK; i++)
            printf("%d ", v[i]);
        printf("\n");
        free(v);
    }
 
    MPI_Finalize();

    return 0;
}
