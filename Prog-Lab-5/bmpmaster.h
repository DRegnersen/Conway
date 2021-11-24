#ifndef PROG_LAB_5_BMPMASTER_H
#define PROG_LAB_5_BMPMASTER_H

#define MONOCHROME 1

typedef int Pixel;

typedef struct {
    int Red;
    int Green;
    int Blue;
    int Alpha;
} Color;

typedef struct {
    char Type[2];
    unsigned long Size;
    unsigned int Reserved1;
    unsigned int Reserved2;
    unsigned long OffBits;
} FileHeader;

typedef struct {
    unsigned long Size;
    long Width;
    long Height;
    unsigned int Planes;
    unsigned int BitCount;
    unsigned long Compression;
    unsigned long SizeImage;
    long XPelsPerMeter;
    long YPelsPerMeter;
    unsigned long ClrUsed;
    unsigned long ClrImportant;
    int ClrTabSize;
    Color *ColorTable;
} InfoHeader;

Pixel **declarePixelArray(int height, int width);

FileHeader readFileHeader(FILE *in);

InfoHeader readInfoHeader(FILE *in);

Pixel **parsePixelArray(FILE *in, FileHeader file_h, InfoHeader info_h);

void createFile(char *filename, FileHeader file_h, InfoHeader info_h, Pixel **pixel_arr);

void putByteIntoArray(FILE *in, InfoHeader info_h, Pixel ***array, int start_idx);

#endif //PROG_LAB_5_BMPMASTER_H
