#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

#include "../utils/IO.h"
#include "../utils/const.h"

#define NUM_THREADS 4

struct my_arg {
	int h;
	int w;
	rgb_t **crt;
	rgb_t **res;
	int id;
};

void *thread_function(void *arg) {
	int h, w, id;
	rgb_t **crt, **res;
	struct my_arg *data;
	int neigh, sum_r, sum_g, sum_b;
	int start, end;

	data = (struct my_arg *)arg;
	h = data->h;
	w = data->w;
	crt = data->crt;
	res = data->res;
	id = data->id;

	start = id * h / NUM_THREADS;
	end = ((id + 1) * h / NUM_THREADS) % (h + 1);

	for (int i = start; i < end; i++) {
		for (int j = 0; j < w; j++) {
			neigh = sum_r = sum_g = sum_b = 0;

			for (int a = -BLUR_DEGREE; a <= BLUR_DEGREE; a++) {
				for (int b = -BLUR_DEGREE; b <= BLUR_DEGREE; b++) {
					if (i + a >= 0 && i + a < h &&
						j + b >= 0 && j + b < w) {
						neigh++;
						sum_r += crt[i + a][j + b].r;
						sum_g += crt[i + a][j + b].g;
						sum_b += crt[i + a][j + b].b;
					}
				}
			}

			res[i][j].r = sum_r / neigh;
			res[i][j].g = sum_g / neigh;
			res[i][j].b = sum_b / neigh;
		}
	}

	pthread_exit(NULL);
}

void blur(int h, int w, rgb_t ***img) {
	pthread_t threads[NUM_THREADS];
    int ids[NUM_THREADS];
    void *status;
	struct my_arg thread_args[NUM_THREADS];

	rgb_t **res, **crt = *img;

	res = malloc(h * sizeof(rgb_t *));
	for (int i = 0; i < h; i++) {
		res[i] = calloc(w, sizeof(rgb_t));
	}

	for (int i = 0; i < NUM_THREADS; i++) {
		thread_args[i].crt = *img;
		thread_args[i].h = h;
		thread_args[i].w = w;
		thread_args[i].res = res;
		thread_args[i].id = i;
		pthread_create(&threads[i], NULL, thread_function, &thread_args[i]);
	}

	for (int i = 0; i < NUM_THREADS; i++) {
		pthread_join(threads[i], &status);
	}

	for (int i = 0; i < h; i++) {
		free((*img)[i]);
	}
	free(*img);

	*img = res;
}

int main() {
	int h, w;
	rgb_t **img, **res;
	clock_t start, end;

	img = read_img(IN_FILENAME, &h, &w);

	start = clock();

	blur(h, w, &img);
	
	end = clock();
	printf("Time for pthread version: %f seconds\n",
		(double)(end - start) / CLOCKS_PER_SEC);

	write_img(OUT_FILENAME, h, w, img);

	for (int i = 0; i < h; i++) {
		free(img[i]);
	}
	free(img);

	return 0;
}