#include <stdio.h>

#define ROWS 3
#define COLS 3

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

int main() {
    int grid[ROWS][COLS] = {{1, 0, 0}, {0, 0, 0}, {0, 0, 0}};

    int i = 1;
    int j = 2;  // Change i and j to access different elements in the grid

    printNeighbors(grid, i, j);

    return 0;
}
