#include "IO.h"
#include <string.h>

#define LINELEN 10

rgb_t **read_img(char *filename, int *h, int *w) {
    rgb_t **img;
    FILE *in;
    char line[LINELEN];
    int ctrl;

    in = fopen(filename, "r");
    
    fscanf(in, "%s", line);
    if (strcmp(line, "P6")) {
        printf("Error: Wrong input file.\n");
        return NULL;
    }

    fscanf(in, "%d%d%d", w, h, &ctrl);
    if (ctrl != 255) {
        printf("Error: Wrong input file.\n");
        return NULL;
    }

    printf("h: %d\nw: %d\n", *h, *w);

    fgetc(in);

    img = malloc(*h * sizeof(rgb_t *));
    for (int i = 0; i < *h; i++) {
        img[i] = malloc(*w * sizeof(rgb_t));
    }

    for (int i = 0; i < *h; i++) {
        for (int j = 0; j < *w; j++) {
            img[i][j].r = fgetc(in);
            img[i][j].g = fgetc(in);
            img[i][j].b = fgetc(in);
        }
    }

    fclose(in);

    rgb_t **ret = img;

    return ret;
}

void write_img(char *filename, int h, int w, rgb_t **img) {
    FILE *out = fopen(filename, "w");

    fprintf(out, "P6\n%d %d\n255\n", w, h);

    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            fprintf(out, "%c%c%c", img[i][j].r, img[i][j].g, img[i][j].b);
        }        
    }

    fclose(out);
}
