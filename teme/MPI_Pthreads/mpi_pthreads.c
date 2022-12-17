#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "mpi.h"
#include <pthread.h>

#include "../utils/IO.h"
#include "../utils/const.h"

#define COORDINATOR 0
#define LOCAL_NUM_THREADS 2

#define W 0
#define H 1
#define CNT 2
#define B_D 3

void define_struct(MPI_Datatype *stype) {
    int blocklens[3] = {1, 1, 1};
    MPI_Datatype types[3] = {
                            MPI_UNSIGNED_CHAR,
                            MPI_UNSIGNED_CHAR,
                            MPI_UNSIGNED_CHAR
                            };
    MPI_Aint disps[3] = {
                        offsetof(rgb_t, r),
                        offsetof(rgb_t, g),
                        offsetof(rgb_t, b)
                        };
    MPI_Type_create_struct(3, blocklens, disps, types, stype);
    MPI_Type_commit(stype);
}

struct my_arg {
	int start_line;
	int end_line;
	int w;
	int h;
	rgb_t **crt;
	rgb_t **res;
	int id;
	int b_d;
	int num_th;
};

void *thread_function(void *arg) {
	int w, h, id, b_d, num_th;
	rgb_t **crt, **res;
	struct my_arg *data;
	int neigh, sum_r, sum_g, sum_b;
	int start, end, start_line, end_line;

	data = (struct my_arg *)arg;
	crt = data->crt;
	res = data->res;
	start_line = data->start_line;
	end_line = data->end_line;
	w = data->w;
	h = data->h;
	b_d = data->b_d;
	id = data->id;
	num_th = data->num_th;

	int lines = end_line - start_line;
	start = start_line + id * lines / num_th;
	end = start_line + (id + 1) * lines / num_th;
	if (end > end_line)
		end = end_line;

	for (int i = start; i < end; i++) {
		for (int j = 0; j < w; j++) {
			neigh = sum_r = sum_g = sum_b = 0;

			for (int a = -b_d; a <= b_d; a++) {
				for (int b = -b_d; b <= b_d; b++) {
					if (i + a >= 0 && i + a < h &&
						j + b >= 0 && j + b < w) {
						neigh++;
						sum_r += crt[i + a][j + b].r;
						sum_g += crt[i + a][j + b].g;
						sum_b += crt[i + a][j + b].b;
					}
				}
			}

			res[i - start_line][j].r = sum_r / neigh;
			res[i - start_line][j].g = sum_g / neigh;
			res[i - start_line][j].b = sum_b / neigh;
		}
	}

	pthread_exit(NULL);
}

int main() {
	int numtasks, rank;
	MPI_Datatype stype;
	double start_time, end_time;

	int h, w;
	rgb_t **img, **res;
	int neigh, sum_r, sum_g, sum_b, start_line, end_line;
	int data[4];

	MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	define_struct(&stype);

	if (rank == COORDINATOR) {
		img = read_img(IN_FILENAME, &h, &w);

		start_time = MPI_Wtime();

		data[W] = w;
		data[H] = h;
		data[B_D] = BLUR_DEGREE;
		for (int i = 1; i < numtasks; i++) {
			int start = i * h / numtasks - BLUR_DEGREE;
			if (start < 0)
				start = 0;
			int end = start + h / numtasks + BLUR_DEGREE;
			if (end > h)
				end = h;

			// send metadata: w, h, lines to be sent and blur degree
			data[CNT] = end - start;
			MPI_Send(data, 4, MPI_INT, i, 0, MPI_COMM_WORLD);

			// send rows from the image
			for (int j = start; j < end; j++) {
				MPI_Send(img[j], w, stype, i, 0, MPI_COMM_WORLD);
			}
		}

		data[CNT] = h / numtasks + BLUR_DEGREE;
		start_line = 0;
		end_line = h / numtasks;

	} else {
		// get metadata from coordinator
		MPI_Recv(data, 4, MPI_INT, COORDINATOR, 0,
			MPI_COMM_WORLD, MPI_STATUS_IGNORE);

		// allocate memory for the rows we're about to recieve
		img = malloc(data[CNT] * sizeof(rgb_t *));
		for (int i = 0; i < data[CNT]; i++) {
			img[i] = malloc(data[W] * sizeof(rgb_t));
		}

		for (int i = 0; i < data[CNT]; i++) {
			MPI_Recv(img[i], data[W], stype, COORDINATOR, 0,
				MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		}

		start_line = data[B_D];
		end_line = data[B_D] + data[H] / numtasks;
		if (end_line > data[CNT])
			end_line = data[CNT];
	}

	res = malloc((end_line - start_line) * sizeof(rgb_t *));
	for (int i = start_line; i < end_line; i++) {
		res[i - start_line] = malloc(data[W] * sizeof(rgb_t));
	}

	// apply filter
	pthread_t threads[LOCAL_NUM_THREADS];
    int ids[LOCAL_NUM_THREADS];
    void *status;
	struct my_arg thread_args[LOCAL_NUM_THREADS];

	for (int i = 0; i < LOCAL_NUM_THREADS; i++) {
		thread_args[i].crt = img;
		thread_args[i].res = res;
		thread_args[i].start_line = start_line;
		thread_args[i].end_line = end_line;
		thread_args[i].w = data[W];
		thread_args[i].h = data[CNT];
		thread_args[i].b_d = data[B_D];
		thread_args[i].id = i;
		thread_args[i].num_th = LOCAL_NUM_THREADS;
		pthread_create(&threads[i], NULL, thread_function, &thread_args[i]);
	}

	for (int i = 0; i < LOCAL_NUM_THREADS; i++) {
		pthread_join(threads[i], &status);
	}

	if (rank == COORDINATOR) {
		for (int i = start_line; i < end_line; i++) {
			for (int j = 0; j < w; j++) {
				img[i][j].r = res[i][j].r;
				img[i][j].g = res[i][j].g;
				img[i][j].b = res[i][j].b;
			}
		}

		// receive the filtered rows
		for (int i = 1; i < numtasks; i++) {
			int start = i * h / numtasks;
			int end = start + h / numtasks;
			if (end > h)
				end = h;

			for (int j = start; j < end; j++) {
				int x;
				MPI_Recv(img[j], w, stype, i, 0,
					MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			}
		}

		end_time = MPI_Wtime();
		printf("Time for MPI version: %f seconds\n", end_time - start_time);

		write_img(OUT_FILENAME, h, w, img);

		for (int i = 0; i < h; i++) {
			free(img[i]);
		}
		free(img);

	} else {
		for (int i = 0; i < end_line - start_line; i++) {
			MPI_Send(res[i], data[W], stype, COORDINATOR, 0, MPI_COMM_WORLD);
		}

		for (int i = 0; i < data[CNT]; i++) {
			free(img[i]);
		}
		free(img);
	}

	for (int i = start_line; i < end_line; i++) {
		free(res[i - start_line]);
	}
	free(res);

	MPI_Finalize();

	return 0;
}
