#include <stdio.h>
#include <stdlib.h>

#define DIMENSION 50

// Define the grid as a 2D array of float pointers
float* grid[DIMENSION][DIMENSION];

void initGrid() {
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
    FILE* file = fopen("displayCopy.txt", "w");

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

// Function to make a copy of the original grid and save it to a new file
void copyAndSaveGrid(float* originalGrid[DIMENSION][DIMENSION]) {
    float* copyGrid[DIMENSION][DIMENSION];

    // Allocate memory for the copyGrid and copy values
    for (int i = 0; i < DIMENSION; i++) {
        for (int j = 0; j < DIMENSION; j++) {
            copyGrid[i][j] = (float*)malloc(sizeof(float));
            if (copyGrid[i][j] == NULL) {
                fprintf(stderr, "Memory allocation failed.\n");
                exit(1);
            }
            *copyGrid[i][j] = *originalGrid[i][j];
        }
    }

    // Display and save the copyGrid to a new file
    displayGrid(copyGrid);

    // Free memory for the copyGrid
    for (int i = 0; i < DIMENSION; i++) {
        for (int j = 0; j < DIMENSION; j++) {
            free(copyGrid[i][j]);
        }
    }
}

int main() {
    initGrid();
    displayGrid(grid);

    // Make a copy of the original grid and save it to a new file
    copyAndSaveGrid(grid);

    // Free memory for the original grid
    for (int i = 0; i < DIMENSION; i++) {
        for (int j = 0; j < DIMENSION; j++) {
            free(grid[i][j]);
        }
    }

    return 0;
}
