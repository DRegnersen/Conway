#include <stdio.h>
#include <string.h>
#include "bmpmaster.h"
#include "conway.h"

#define LINESIZE 200

int parseInt(char *number) {
    int result = 0;
    int length = strlen(number);
    int power = 1;

    for (int i = length - 1; i >= 0; i--) {
        result += (number[i] - '0') * power;
        power *= 10;
    }

    return result;
}

void setFileName(char *dest, char *directory, char *initial, int number) {
    char new_filename[LINESIZE];
    char global_name[LINESIZE];

    sscanf(strrchr(initial, '\\'), "\\%[^.]", global_name);
    sprintf(new_filename, "%s\\new_%s_%d.bmp", directory, global_name, number);

    strcpy(dest, new_filename);
}

int main(int argc, char **argv) {
    char *filename, *directory;
    int iter_count = 0, max_iter = 0;
    int frequency = 1;
    short isSrc = 0, isDir = 0, isMax = 0;

    for (int i = 1; i < argc; i++) {
        char *command = argv[i];
        if (!strcmp(command, "--input")) {
            filename = argv[++i];
            isSrc = 1;
        } else if (!strcmp(command, "--output")) {
            directory = argv[++i];
            isDir = 1;
        } else if (!strcmp(command, "--max_iter")) {
            max_iter = parseInt(argv[++i]);
            isMax = 1;
        } else if (!strcmp(command, "--dump_freq")) {
            frequency = parseInt(argv[++i]);
        } else {
            printf("Command '%s' is unacceptable\n", command);
        }
    }

    if (isSrc && isDir) {
        FILE *in = fopen(filename, "rb");

        if (in == NULL) {
            printf("File does not exist.\n");
        }

        FileHeader file_h = readFileHeader(in);

        InfoHeader info_h = readInfoHeader(in);

        Pixel **pixel_arr = parsePixelArray(in, file_h, info_h);

        fclose(in);

        char isIterated[LINESIZE];

        printf("Print 'stop' to quit.\n");
        printf("ITERATE:");
        gets(isIterated);

        while (strcmp(isIterated, "stop") != 0 && (!isMax || iter_count < max_iter)) {
            for (int i = 0; i < frequency; i++) {
                iterateLife(&pixel_arr, info_h.Height, info_h.Width);

                char new_filename[LINESIZE];
                setFileName(new_filename, directory, filename, ++iter_count);

                createFile(new_filename, file_h, info_h, pixel_arr);
            }
            printf("ITERATE:");
            gets(isIterated);
        }

        pixel_arr = NULL;
    } else {
        if (!isSrc) {
            printf("Source file is absent.\n");
        }
        if (!isDir) {
            printf("Directory is absent.\n");
        }
    }
    return 0;
}