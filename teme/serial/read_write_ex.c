#include <stdio.h>
#include <stdlib.h>
#include "../utils/IO.h"

int main() {
	int h, w;
	rgb_t **img;
	img = read_img("../img/1.ppm", &h, &w);
	write_img("../img/out/1_out.ppm", h, w, img);

	for (int i = 0; i < h; i++) {
        free(img[i]);
    }
    free(img);

	return 0;
}
