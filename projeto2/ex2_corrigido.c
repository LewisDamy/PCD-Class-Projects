#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define N 2048
#define GENERATIONS 2000
#define NUM_THREADS 1

void displayGrid(int ***grid) {
    FILE *file = fopen("display.txt", "w");

    if (file == NULL) {
        printf("Error opening the file.\n");
        return;
    }

    fprintf(file, "      ");
    for (int j = 1; j <= N; j++) {
        fprintf(file, "[%02d]  ", j);
    }
    fprintf(file, "\n");

    for (int i = 1; i <= N; i++) {
        fprintf(file, "[%02d]  ", i);
        for (int j = 1; j <= N; j++) {
            fprintf(file, "%.1f   ", *grid[i - 1][j - 1]);
        }
        fprintf(file, "\n");
    }

    fclose(file);
}

void initZeroValueGrid(int ***grid) {
    // Allocate memory for each row
    *grid = (int **)malloc(N * sizeof(int *));
    if (*grid == NULL) {
        fprintf(stderr, "Memory allocation failed for rows.\n");
        exit(1);
    }

    // Allocate memory for each column in each row
    for (int i = 0; i < N; i++) {
        (*grid)[i] = (int *)malloc(N * sizeof(int));
        if ((*grid)[i] == NULL) {
            fprintf(stderr, "Memory allocation failed for columns.\n");
            exit(1);
        }
        for (int j = 0; j < N; j++) {
            (*grid)[i][j] = 0;  // Initialize with zero
        }
    }
}

void initGridDefaultValues(int **grid) {
    // GLIDER
    int line = 1, column = 1;
    grid[line][column + 1] = 1;
    grid[line + 1][column + 2] = 1;
    grid[line + 2][column] = 1;
    grid[line + 2][column + 1] = 1;
    grid[line + 2][column + 2] = 1;

    if (N > 40) {
        // R-entomino
        line = 10;
        column = 30;
        grid[line][column + 1] = 1;
        grid[line][column + 2] = 1;
        grid[line + 1][column] = 1;
        grid[line + 1][column + 1] = 1;
        grid[line + 2][column + 1] = 1;
    }
}

void invertMatrix(int ***matriz_A, int ***matriz_B) {
    int **aux = *matriz_A;

    *matriz_A = *matriz_B;
    *matriz_B = aux;
}

int getNeighbors(int **grid, int x, int y) {
    int count = 0;
    int dx[] = {-1, -1, -1, 0,
                1,  1,  1,  0};  // Relative X positions of neighbors
    int dy[] = {-1, 0, 1,  1,
                1,  0, -1, -1};  // Relative Y positions of neighbors

    for (int i = 0; i < 8; i++) {
        int newX = (x + dx[i] + N) % N;  // Wrap-around for X
        int newY = (y + dy[i] + N) % N;  // Wrap-around for Y

        count += grid[newX][newY];
    }

    return count;
}

void calculateLivingCells(int ***grid, int ***newGrid) {
#pragma omp parallel for num_threads(NUM_THREADS)
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            int amoutNeighbors = getNeighbors(*grid, i, j);
            if ((amoutNeighbors == 6 && 0 && (*grid)[i][j] == 0) ||
                amoutNeighbors == 3 ||
                (amoutNeighbors == 2 && (*grid)[i][j] == 1)) {
                (*newGrid)[i][j] = 1;
            } else {
                (*newGrid)[i][j] = 0;
            }
        }
    }

    invertMatrix(grid, newGrid);
}

int amountLivingCells(int **grid) {
    int livingCells = 0;
#pragma omp parallel for reduction(+ : livingCells)
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            livingCells += grid[i][j];
        }
    }
    return livingCells;
}

int lifeGameIterator() {  // MOVED TO MAIN FUNCTION
    // int iteration;
    // for (iteration = 0; iteration < GENERATIONS; iteration++) {
    //     calculateLivingCells();
    //     if (iteration < 999)
    //         printf("---GENERATION %d--- living cells: %d\n", iteration,
    //                amountLivingCells(originalGrid));
    // }
    // printf("---GENERATION %d--- living cells: %d\n", iteration,
    //        amountLivingCells(originalGrid));
    // return 0;
}

int main() {
    // Define the original and copy grids as global 2D arrays of float pointers
    int **grid, **newGrid;
    int livingCellsCount = 0;
    clock_t start, end;
    double cpu_time_used;

    initZeroValueGrid(&grid);
    initZeroValueGrid(&newGrid);
    initGridDefaultValues(grid);

    start = clock();  // start time
    for (int iteration = 0; iteration < GENERATIONS; iteration++) {
        calculateLivingCells(&grid, &newGrid);
    }
    end = clock();

    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    livingCellsCount = amountLivingCells(grid);

    printf("Grid dimension: %i\nNum_threads: %i\n", N, NUM_THREADS);
    printf("Generations: %i\n", GENERATIONS);
    printf("Tempo de CPU utilizado: %.3fs\n", cpu_time_used);
    printf("Celulas vivas no final: %d\n", livingCellsCount);

    // Free memory allocated for grid/newGrid
    for (int i = 0; i < N; i++) {
        free(grid[i]);
        free(newGrid[i]);
    }
    free(grid);
    free(newGrid);

    return 0;
}