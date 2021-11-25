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

    new_ih.ClrTabSize = 1;
    for (int i = 0; i < new_ih.BitCount; i++) {
        new_ih.ClrTabSize *= 2;
    }

    new_ih.ColorTable = (Color *) malloc(new_ih.ClrTabSize * sizeof(Color));

    if (new_ih.ColorTable == NULL) {
        printf("ERROR! Segmentation fault.\n");
        return new_ih;
    }

    for (int i = 0; i < new_ih.ClrTabSize; i++) {
        new_ih.ColorTable[i].Red = fgetc(in);
        new_ih.ColorTable[i].Green = fgetc(in);
        new_ih.ColorTable[i].Blue = fgetc(in);
        new_ih.ColorTable[i].Alpha = fgetc(in);
    }

    return new_ih;
}

Pixel **parsePixelArray(FILE *in, FileHeader file_h, InfoHeader info_h) {
    fseek(in, file_h.OffBits, SEEK_SET);

    Pixel **pixel_arr = declarePixelArray(info_h.Height, info_h.Width);

    for (int i = info_h.Height - 1; i >= 0; i--) {
        int cur_byte = fgetc(in);
        int read_byte_count = 1;

        for (int j = 0; j < info_h.Width; j++) {
            pixel_arr[i][j] = (cur_byte & (1 << (7 - j % 8))) >> (7 - j % 8);

            if (7 - j % 8 == 0) {
                cur_byte = fgetc(in);
                read_byte_count++;
            }
        }

        while (read_byte_count % 4 != 0) {
            fgetc(in);
            read_byte_count++;
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

    for (int i = 0; i < info_h.ClrTabSize; i++) {
        fprintf(out, "%c%c%c%c", info_h.ColorTable[i].Red, info_h.ColorTable[i].Green,
                info_h.ColorTable[i].Blue, info_h.ColorTable[i].Alpha);
    }

    for (int i = info_h.Height - 1; i >= 0; i--) {
        int write_byte_count = 0;
        int new_byte = 0;
        int bit_pos = 7;

        for (int j = 0; j < info_h.Width; j++) {
            new_byte += pixel_arr[i][j] << bit_pos;
            bit_pos--;

            if (bit_pos < 0) {
                fprintf(out, "%c", new_byte);
                write_byte_count++;
                new_byte = 0;
                bit_pos = 7;
            }
        }

        if (new_byte != 0) {
            fprintf(out, "%c", new_byte);
            write_byte_count++;
        }

        while (write_byte_count % 4 != 0) {
            fprintf(out, "%c", 0);
            write_byte_count++;
        }
    }

}