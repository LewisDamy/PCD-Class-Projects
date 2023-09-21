#include <stdio.h>
#include <stdlib.h>

#define DIMENSION 50

// Define the original and copy grids as global 2D arrays of float pointers
float* originalGrid[DIMENSION][DIMENSION];
float* copyGrid[DIMENSION][DIMENSION];

void initGrid(float* grid[DIMENSION][DIMENSION]) {
    // Allocate memory for the grid
    for (int i = 0; i < DIMENSION; i++) {
        for (int j = 0; j < DIMENSION; j++) {
            grid[i][j] = (float*)malloc(sizeof(float));
            if (grid[i][j] == NULL) {
                fprintf(stderr, "Memory allocation failed.\n");
                exit(1);
            }
            *grid[i][j] = 0.0;
        }
    }

    // GLIDER INIT
    int line = 0, column = 0;

    *grid[line][column + 1] = 1.0;
    *grid[line + 1][column + 2] = 1.0;
    *grid[line + 2][column] = 1.0;
    *grid[line + 2][column + 1] = 1.0;
    *grid[line + 2][column + 2] = 1.0;

    // R-pentomino INIT
    line = 9, column = 29;

    *grid[line][column + 1] = 1.0;
    *grid[line][column + 2] = 1.0;
    *grid[line + 1][column] = 1.0;
    *grid[line + 1][column + 1] = 1.0;
    *grid[line + 2][column + 1] = 1.0;
}

void displayGrid(float* grid[DIMENSION][DIMENSION]) {
    FILE* file = fopen("display.txt", "w");

    if (file == NULL) {
        printf("Error opening the file.\n");
        return;
    }

    fprintf(file, "      ");
    for (int j = 1; j <= DIMENSION; j++) {
        fprintf(file, "[%02d]  ", j);
    }
    fprintf(file, "\n");

    for (int i = 1; i <= DIMENSION; i++) {
        fprintf(file, "[%02d]  ", i);
        for (int j = 1; j <= DIMENSION; j++) {
            fprintf(file, "%.1f   ", *grid[i - 1][j - 1]);
        }
        fprintf(file, "\n");
    }

    fclose(file);
}

// Function to copy data from the original grid to the copy grid
void copyOriginalToCopy() {
    for (int i = 0; i < DIMENSION; i++) {
        for (int j = 0; j < DIMENSION; j++) {
            *copyGrid[i][j] = *originalGrid[i][j];
        }
    }
}

// Function to copy data from the copy grid to the original grid
void copyCopyToOriginal() {
    for (int i = 0; i < DIMENSION; i++) {
        for (int j = 0; j < DIMENSION; j++) {
            *originalGrid[i][j] = *copyGrid[i][j];
        }
    }
}

void printCell(float* whichGrid[DIMENSION][DIMENSION], int i, int j) {
    printf("cell value: %0.1f\n", *whichGrid[0][1]);
}

int getNeighbors(float* whichGrid[DIMENSION][DIMENSION], int i, int j) {
    float arrNeighbors[8];
    int amountLivingNeighbors = 0;

    if (i == 0 || j == 0) {
        printf("TODO\n");
    } else {
        arrNeighbors[0] = *whichGrid[i - 1][j - 1];  // TOP LEFT ITEM
        arrNeighbors[1] = *whichGrid[i - 1][j];      // TOP MIDDLE ITEM
        arrNeighbors[2] = *whichGrid[i - 1][j + 1];  // TOP RIGHT ITEM
        arrNeighbors[3] = *whichGrid[i][j - 1];      // LEFT ITEM
        arrNeighbors[4] = *whichGrid[i][j + 1];      // RIGHT ITEM
        arrNeighbors[5] = *whichGrid[i + 1][j - 1];  // BOTTOM LEFT ITEM
        arrNeighbors[6] = *whichGrid[i + 1][j];      // BOTTOM MIDDLE ITEM
        arrNeighbors[7] = *whichGrid[i + 1][j + 1];  // BOTTOM RIGHT ITEM
    }
    // loop to make the math
    for (int i = 0; i < 8; i++) {
        amountLivingNeighbors += arrNeighbors[i];
    }
    return amountLivingNeighbors;
}

int main() {
    // Initialize the original and copy grids
    initGrid(originalGrid);
    initGrid(copyGrid);

    // printf("cell value: %f\n", *originalGrid[0][1]);

    // TODO
    int result = getNeighbors(originalGrid, 0, 1);
    printf("result: %i\n", result);

    return 0;
}
