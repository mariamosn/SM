#include <stdio.h>
#include <stdlib.h>

struct rgb {
    unsigned char r;
    unsigned char g;
    unsigned char b;
} typedef rgb_t;

rgb_t **read_img(char *filename, int *h, int *w);
void write_img(char *filename, int h, int w, rgb_t **img);
