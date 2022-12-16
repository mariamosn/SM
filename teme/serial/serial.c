#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../utils/IO.h"
#include "../utils/const.h"

void blur(int h, int w, rgb_t ***img) {
	int neigh, sum_r, sum_g, sum_b;
	rgb_t **res, **crt = *img;

	res = malloc(h * sizeof(rgb_t *));
	for (int i = 0; i < h; i++) {
		res[i] = calloc(w, sizeof(rgb_t));
	}

    for (int i = 0; i < h; i++) {
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
    printf("Time for serial version: %f seconds\n",
		(double)(end - start) / CLOCKS_PER_SEC);

	write_img(OUT_FILENAME, h, w, img);

	for (int i = 0; i < h; i++) {
        free(img[i]);
    }
    free(img);

	return 0;
}
