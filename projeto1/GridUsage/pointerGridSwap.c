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

void displayGrid(float* grid[DIMENSION][DIMENSION], char* fileName) {
    FILE* file = fopen(fileName, "w");

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

int main() {
    // Initialize the original and copy grids
    initGrid(originalGrid);
    initGrid(copyGrid);

    // Modify data in the original grid as needed
    float modItemCopyGrid = 0.7;
    *copyGrid[25][25] = modItemCopyGrid;

    // Copy data from the original grid to the copy grid
    copyCopyToOriginal();

    // Display original grid after copy been modified and copied back to it
    displayGrid(originalGrid, "displayOriginalGrid.txt");

    float modItemOrigGrid = 0.5;
    *originalGrid[24][24] = modItemOrigGrid;

    // Copy data from the copy grid back to the original grid
    copyOriginalToCopy();

    // Display the original grid (or the copy grid, if desired)
    displayGrid(originalGrid, "displayCopyGrid.txt");

    // Free memory for both grids
    for (int i = 0; i < DIMENSION; i++) {
        for (int j = 0; j < DIMENSION; j++) {
            free(originalGrid[i][j]);
            free(copyGrid[i][j]);
        }
    }

    return 0;
}
