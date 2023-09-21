#include <stdio.h>

#define ROWS 3
#define COLS 3
#define DIMENSION 3

float* originalGrid[DIMENSION][DIMENSION];

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

void printNeighbors(int grid[ROWS][COLS], int i, int j) {
    // Define relative positions of neighbors using modulo arithmetic
    int neighborsX[] = {-1, -1, -1, 0, 0, 1, 1, 1};
    int neighborsY[] = {-1, 0, 1, -1, 1, -1, 0, 1};

    printf("Neighbors of [%d][%d]:\n", i, j);

    for (int k = 0; k < 8; k++) {  // Loop through all possible neighbors
        int x = (i + neighborsX[k] + ROWS) % ROWS;  // Apply modulo for wrapping
        int y = (j + neighborsY[k] + COLS) % COLS;

        printf("[%d][%d] = %d\n", x, y, grid[x][y]);
    }
}

int getNeighbors(float* whichGrid[DIMENSION][DIMENSION], int i, int j) {
    float arrNeighbors[8];
    int amountLivingNeighbors = 0;

    // Relative positions of neighbors
    int neighborX[] = {-1, -1, -1, 0, 0, 1, 1, 1};
    int neighborY[] = {-1, 0, 1, -1, 1, -1, 0, 1};

    if (i == 0 || j == 0) {
        printf("entrou no if\n");
        for (int k = 0; k < 8; k++) {
            int x = (i + neighborX[k] + DIMENSION) % DIMENSION;
            int y = (j + neighborY[k] + DIMENSION) % DIMENSION;
            arrNeighbors[k] = *whichGrid[x][y];

            printf("[%i][%i] = %f\n", x, y, *whichGrid[x][y]);
        }
    } else {
        printf("entrou no else\n");
        for (int k = 0; k < 8; k++) {
            int x = i + neighborX[k];
            int y = j + neighborY[k];
            arrNeighbors[k] = *whichGrid[x][y];

            printf("[%i][%i] = %f\n", x, y, *whichGrid[x][y]);
        }
    }
    for (int k = 0; k < 8; k++) {
        printf("arrNeighbors[%i] = %f\n", k, arrNeighbors[k]);
        amountLivingNeighbors += arrNeighbors[k];
    }
    return amountLivingNeighbors;
}

int main() {
    int grid[ROWS][COLS] = {{1, 0, 0}, {0, 0, 0}, {0, 0, 0}};

    int i = 1;
    int j = 2;  // Change i and j to access different elements in the grid

    printNeighbors(grid, i, j);

    // REAL GRID SAMPLE
    initGrid(originalGrid);

    int result = getNeighbors(originalGrid, 1, 2);
    printf("result: %i\n", result);

    return 0;
}
