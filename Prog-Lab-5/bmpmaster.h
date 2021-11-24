#ifndef PROG_LAB_5_BMPMASTER_H
#define PROG_LAB_5_BMPMASTER_H

#define MONOCHROME 1

#pragma pack(1)
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
} InfoHeader;

typedef struct {
    int Red;
    int Green;
    int Blue;
    int Alpha;
} Pixel;

Pixel **declarePixelArray(int height, int width);

FileHeader readFileHeader(FILE *in);

InfoHeader readInfoHeader(FILE *in);

Pixel **parsePixelArray(FILE *in, FileHeader file_h, InfoHeader info_h);

#endif //PROG_LAB_5_BMPMASTER_H
