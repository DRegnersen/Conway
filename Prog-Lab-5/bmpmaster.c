#include <stdio.h>
#include <stdlib.h>
#include "bmpmaster.h"

Pixel **declarePixelArray(int height, int width) {
    Pixel **new_arr = NULL;
    new_arr = (Pixel **) malloc(height * sizeof(Pixel *));

    if (new_arr == NULL) {
        printf("ERROR! Segmentation fault.\n");
        return NULL;
    }

    for (int i = 0; i < height; i++) {
        new_arr[i] = (Pixel *) malloc(width * sizeof(Pixel));
        if (new_arr[i] == NULL) {
            printf("ERROR! Segmentation fault.\n");
            return NULL;
        }
    }
    return new_arr;
}

long convertFromBytes(FILE *in, int bytes_num) {
    long result = 0;
    long power = 1;

    for (int i = 0; i < bytes_num; i++) {
        result += fgetc(in) * power;
        power *= 256;
    }

    return result;
}

void putBytes(FILE *out, long value, int bytes_num) {
    for (int i = 0; i < bytes_num; i++) {
        fprintf(out, "%c", value % 256);
        value /= 256;
    }
}

FileHeader readFileHeader(FILE *in) {
    FileHeader new_fh;

    new_fh.Type[0] = fgetc(in);
    new_fh.Type[1] = fgetc(in);
    new_fh.Type[2] = 0;

    new_fh.Size = convertFromBytes(in, 4);

    new_fh.Reserved1 = convertFromBytes(in, 2);
    new_fh.Reserved2 = convertFromBytes(in, 2);

    new_fh.OffBits = convertFromBytes(in, 4);

    return new_fh;
}

InfoHeader readInfoHeader(FILE *in) {
    InfoHeader new_ih;

    new_ih.Size = convertFromBytes(in, 4);

    new_ih.Width = convertFromBytes(in, 4);

    new_ih.Height = convertFromBytes(in, 4);

    new_ih.Planes = convertFromBytes(in, 2);

    new_ih.BitCount = convertFromBytes(in, 2);

    if (new_ih.BitCount == 0) {
        printf("WARNING! Not the BMP file format.\n");
    } else if (MONOCHROME) {
        if (new_ih.BitCount != 1) {
            printf("WARNING! Not a monochrome picture.\n");
        }
    }

    new_ih.Compression = convertFromBytes(in, 4);

    if (new_ih.Compression != 0) {
        printf("WARNING! File is compressed.\n");
    }

    new_ih.SizeImage = convertFromBytes(in, 4);

    new_ih.XPelsPerMeter = convertFromBytes(in, 4);

    new_ih.YPelsPerMeter = convertFromBytes(in, 4);

    new_ih.ClrUsed = convertFromBytes(in, 4);

    new_ih.ClrImportant = convertFromBytes(in, 4);

    return new_ih;
}

Pixel **parsePixelArray(FILE *in, FileHeader file_h, InfoHeader info_h) {
    fseek(in, file_h.OffBits, SEEK_SET);

    Pixel **pixel_arr = declarePixelArray(info_h.Height, info_h.Width);

    for (int i = info_h.Height - 1; i >= 0; i--) {
        for (int j = 0; j < info_h.Width; j++) {
            if (MONOCHROME) {
                pixel_arr[i][j].Color = (fgetc(in) == 0) ? 0 : 255;
                // pixel_arr[i][j].Color = fgetc(in);
            } else {
                pixel_arr[i][j].Red = fgetc(in);
                pixel_arr[i][j].Green = fgetc(in);
                pixel_arr[i][j].Blue = fgetc(in);
                pixel_arr[i][j].Alpha = fgetc(in);
            }
        }
    }

    return pixel_arr;
}

void createFile(char *filename, FileHeader file_h, InfoHeader info_h, Pixel **pixel_arr) {
    FILE *out = fopen(filename, "wb");

    if (out == NULL) {
        printf("ERROR! File writing fault.\n");
    }

    fprintf(out, "%s", file_h.Type);

    putBytes(out, file_h.Size, 4);

    putBytes(out, file_h.Reserved1, 2);
    putBytes(out, file_h.Reserved2, 2);

    putBytes(out, file_h.OffBits, 4);

    putBytes(out, info_h.Size, 4);

    putBytes(out, info_h.Width, 4);

    putBytes(out, info_h.Height, 4);

    putBytes(out, info_h.Planes, 2);

    putBytes(out, info_h.BitCount, 2);

    putBytes(out, info_h.Compression, 4);

    putBytes(out, info_h.SizeImage, 4);

    putBytes(out, info_h.XPelsPerMeter, 4);

    putBytes(out, info_h.YPelsPerMeter, 4);

    putBytes(out, info_h.ClrUsed, 4);

    putBytes(out, info_h.ClrImportant, 4);

    for (int i = 54; i < file_h.OffBits; i++) {
        fprintf(out, "%c", 0);
    }

    for (int i = info_h.Height - 1; i >= 0; i--) {
        for (int j = 0; j < info_h.Width; j++) {
            if (MONOCHROME) {
                fprintf(out, "%c", pixel_arr[i][j].Color);
            } else {
                fprintf(out, "%c%c%c%c", pixel_arr[i][j].Red, pixel_arr[i][j].Green,
                        pixel_arr[i][j].Blue, pixel_arr[i][j].Alpha);
            }
        }
    }
}