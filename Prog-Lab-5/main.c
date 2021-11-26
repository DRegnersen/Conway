#include <stdio.h>
#include "bmpmaster.h"
#include "conway.h"

int main() {
    FILE *in = fopen("C:\\Users\\deofr\\C\\Prog-Lab-5\\mickey.bmp", "rb");

    if (in == NULL) {
        printf("ERROR! File not found.\n");
    }

    FileHeader file_h = readFileHeader(in);

    InfoHeader info_h = readInfoHeader(in);

    Pixel **array = parsePixelArray(in, file_h, info_h);

    ft_iterateLife(&array, info_h.Height, info_h.Width, 5);

    for (int i = 0; i < info_h.Height; i++) {
        for (int j = 0; j < info_h.Width; j++) {
            if (array[i][j] == 0) {
                printf("X");
            } else {
                printf(" ");
            }
        }
        printf("\n");
    }

    createFile("C:\\Users\\deofr\\C\\Prog-Lab-5\\new-file.bmp", file_h, info_h, array);

    array = NULL;
    return 0;
}
