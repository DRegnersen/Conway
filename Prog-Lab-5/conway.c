#include <stdio.h>
#include <stdlib.h>
#include "conway.h"

int **declare(int height, int width) {
    int **new_arr = NULL;
    new_arr = (int **) malloc(height * sizeof(int *));

    if (new_arr == NULL) {
        printf("ERROR! Segmentation fault.\n");
        return NULL;
    }

    for (int i = 0; i < height; i++) {
        new_arr[i] = (int *) malloc(width * sizeof(int));
        if (new_arr[i] == NULL) {
            printf("ERROR! Segmentation fault.\n");
            return NULL;
        }
    }
    return new_arr;
}

int getNeighbours(int **array, int height, int width, int y, int x) {
    int nb_count = 0;

    nb_count += (x - 1 >= 0) ? !array[y][x - 1] == REVERSED : 0;
    nb_count += (x + 1 < width) ? !array[y][x + 1] == REVERSED : 0;
    nb_count += (y - 1 >= 0) ? !array[y - 1][x] == REVERSED : 0;
    nb_count += (y + 1 < height) ? !array[y + 1][x] == REVERSED : 0;
    nb_count += (x - 1 >= 0 && y - 1 >= 0) ? !array[y - 1][x - 1] == REVERSED : 0;
    nb_count += (x + 1 < width && y - 1 >= 0) ? !array[y - 1][x + 1] == REVERSED : 0;
    nb_count += (x - 1 >= 0 && y + 1 < height) ? !array[y + 1][x - 1] == REVERSED : 0;
    nb_count += (x + 1 < width && y + 1 < height) ? !array[y + 1][x + 1] == REVERSED : 0;

    return nb_count;
}

void iterateLife(int ***array, int height, int width) {
    int **new_arr = declare(height, width);

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int nb_count = getNeighbours((*array), height, width, i, j);
            if (!(*array)[i][j] == REVERSED) {
                new_arr[i][j] = !(nb_count == 2 || nb_count == 3) == REVERSED;
            } else {
                new_arr[i][j] = !(nb_count == 3) == REVERSED;
            }
        }
    }

    // free memory
    for (int i = 0; i < height; i++) {
        free((*array)[i]);
        (*array)[i] = NULL;
    }
    free(*array);

    (*array) = new_arr;
}

void ft_iterateLife(int ***array, int height, int width, int times) {
    for (int i = 0; i < times; i++) {
        iterateLife(array, height, width);
    }
}

short isEmpty(int **array, int height, int width) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (array[i][j] == !REVERSED) {
                return 0;
            }
        }
    }
    return 1;
}