#include <stdio.h>

// 2048 x 2048
#define DIMENSION 50
#define ITERATIONS 20

float grid[DIMENSION][DIMENSION];

void initGrid() {
    // GLIDER INIT
    int line = 1, column = 1;

    grid[line][column + 1] = 1.0;
    grid[line + 1][column + 2] = 1.0;
    grid[line + 2][column] = 1.0;
    grid[line + 2][column + 1] = 1.0;
    grid[line + 2][column + 2] = 1.0;

    // R-pentomino INIT
    line = 10, column = 30;

    grid[line][column + 1] = 1.0;
    grid[line][column + 2] = 1.0;
    grid[line + 1][column] = 1.0;
    grid[line + 1][column + 1] = 1.0;
    grid[line + 2][column + 1] = 1.0;
}

void displayGrid() {
    FILE *file = fopen("display.txt", "w");  // Open the file for writing

    if (file == NULL) {
        printf("Error opening the file.\n");
        return;
    }

    // Print the header row
    fprintf(file, "      ");
    for (int j = 1; j <= DIMENSION; j++) {
        fprintf(file, "[%02d]  ", j);
    }
    fprintf(file, "\n");

    // Print the grid values
    for (int i = 1; i <= DIMENSION; i++) {
        // change to %04 when DIMENSION 2048
        fprintf(file, "[%02d]  ", i);
        for (int j = 1; j <= DIMENSION; j++) {
            fprintf(file, "%.1f   ", grid[i][j]);
        }
        fprintf(file, "\n");
    }

    fclose(file);  // Close the file
}

int main(int args, char *argv[]) {
    initGrid();
    displayGrid();
    return 0;
}
