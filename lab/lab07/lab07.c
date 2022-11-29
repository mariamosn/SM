// Maria Moșneag 343C1

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define MAIN 0
#define CHUNK 5
#define SECRET 7
#define NUM_THREADS 4

int *v, secret;
pthread_barrier_t barrier;

void *f(void *arg) {
	int id = *(int *)arg;

	if (id == MAIN) {
		v = malloc(NUM_THREADS * CHUNK * sizeof(int));
		for (int i = 0; i < NUM_THREADS * CHUNK; i++) {
			v[i] = i;
		}

		printf("original array: ");
		for (int i = 0; i < NUM_THREADS * CHUNK; i++)
			printf("%d ", v[i]);
		printf("\n");

		printf("compute: 2 * v[i] + secret\n");

		secret = SECRET;
        printf("secret: %d\n", secret);
	}

	pthread_barrier_wait(&barrier);

	for (int i = id * CHUNK; i < (id + 1) * CHUNK; i++) {
		v[i] = 2 * v[i] + secret;
	}

	pthread_barrier_wait(&barrier);

	if (id == MAIN) {
        printf("result: ");
        for (int i = 0; i < NUM_THREADS * CHUNK; i++)
            printf("%d ", v[i]);
        printf("\n");
        free(v);
    }

	pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    pthread_t threads[NUM_THREADS];
    int ids[NUM_THREADS];
    void *status;

	pthread_barrier_init(&barrier, NULL, NUM_THREADS);

	for (int i = 0; i < NUM_THREADS; i++) {
		ids[i] = i;
		pthread_create(&threads[i], NULL, f, &ids[i]);
	}

	for (int i = 0; i < NUM_THREADS; i++) {
		pthread_join(threads[i], &status);
	}

	pthread_barrier_destroy(&barrier);

	pthread_exit(NULL);

    return 0;
}
