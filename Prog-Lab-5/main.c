#include <stdio.h>
#include <stdlib.h>
#include "bmpmaster.h"

int main() {
    FILE *in = fopen("C:\\Users\\deofr\\C\\Prog-Lab-5\\giraffe.bmp", "rb");

    if (in == NULL) {
        printf("ERROR! File not found.\n");
    }

    FileHeader file_h = readFileHeader(in);

    InfoHeader info_h = readInfoHeader(in);

    Pixel **array = parsePixelArray(in, file_h, info_h);

    for (int i = 0; i < info_h.Width; i++) {
        for (int j = 0; j < info_h.Height; j++) {
            //printf("R: %d, G: %d, B: %d\n", array[i][j].Red, array[i][j].Green, array[i][j].Blue);
            if (array[j][i].Red == 0) {
                printf("X");
            } else {
                printf(" ");
            }
        }
        printf("\n");
    }

    createFile("C:\\Users\\deofr\\C\\Prog-Lab-5\\new-file.bmp", file_h, info_h, array);

    for (int i = 0; i < info_h.Width; i++) {
        free(array[i]);
        array[i] = NULL;
    }
    free(array);
    array = NULL;

    return 0;
}
