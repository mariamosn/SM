#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "mpi.h"

#include "../utils/IO.h"
#include "../utils/const.h"

#define COORDINATOR 0

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

int main() {
	int numtasks, rank;
	MPI_Datatype stype;
	clock_t start_time, end_time;

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

		start_time = clock();

		data[W] = w;
		data[H] = h;
		data[B_D] = BLUR_DEGREE;
		for (int i = 1; i < numtasks; i++) {
			int start = i * h / numtasks - BLUR_DEGREE;
			int end = (start + h / numtasks + BLUR_DEGREE) % (h + 1);

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
		end_line = (data[B_D] + data[H] / numtasks) % (data[CNT] + 1);
	}

	res = malloc((end_line - start_line) * sizeof(rgb_t *));
	for (int i = start_line; i < end_line; i++) {
		res[i - start_line] = malloc(data[W] * sizeof(rgb_t));
	}

	// apply filter
	for (int i = start_line; i < end_line; i++) {
		for (int j = 0; j < data[W]; j++) {
			neigh = sum_r = sum_g = sum_b = 0;
			for (int a = -BLUR_DEGREE; a <= BLUR_DEGREE; a++) {
				for (int b = -BLUR_DEGREE; b <= BLUR_DEGREE; b++) {
					if (i + a >= 0 && i + a < data[CNT] &&
						j + b >= 0 && j + b < data[W]) {
						neigh++;
						sum_r += img[i + a][j + b].r;
						sum_g += img[i + a][j + b].g;
						sum_b += img[i + a][j + b].b;
					}
				}
			}
			res[i - start_line][j].r = sum_r / neigh;
			res[i - start_line][j].g = sum_g / neigh;
			res[i - start_line][j].b = sum_b / neigh;
		}
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
			int end = (start + h / numtasks) %  (h + 1);
			for (int j = start; j < end; j++) {
				int x;
				MPI_Recv(img[j], w, stype, i, 0,
					MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			}
		}

		end_time = clock();
		printf("Time for MPI version: %f seconds\n",
			(double)(end_time - start_time) / CLOCKS_PER_SEC);

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
