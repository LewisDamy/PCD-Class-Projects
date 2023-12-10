#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define N 2048       // Size of the grid
#define ROUNDS 2000  // Number of iterations

int getNeighbors(int **grid, int x, int y, int numRows) {
    int count = 0;
    int dx[] = {-1, -1, -1, 0,
                1,  1,  1,  0};  // Relative X positions of neighbors
    int dy[] = {-1, 0, 1,  1,
                1,  0, -1, -1};  // Relative Y positions of neighbors

    for (int i = 0; i < 8; i++) {
        int newX = x + dx[i];
        int newY = y + dy[i];

        // Handle wrap-around for rows
        if (newX < 0)
            newX = numRows + 1;
        else if (newX >= numRows + 2)
            newX = 0;

        // Handle wrap-around for columns
        if (newY < 0)
            newY = N - 1;
        else if (newY >= N)
            newY = 0;

        count += grid[newX][newY];
    }

    return count;
}

void initNeighbors(int *upperNeighbor, int *lowerNeighbor, int processId,
                   int numProcesses) {
    if (processId == 0) {
        // First process
        *upperNeighbor = processId + 1;
        *lowerNeighbor = numProcesses - 1;  // Wrap-around
    } else if (processId == numProcesses - 1) {
        // Last process
        *upperNeighbor = 0;  // Wrap-around
        *lowerNeighbor = processId - 1;
    } else {
        // Middle processes
        *upperNeighbor = processId + 1;
        *lowerNeighbor = processId - 1;
    }
}

void exchangeNeighbors(int **grid, int numRows, int upperNeighbor,
                       int lowerNeighbor, int currentRound, int processId) {
    MPI_Status status;

    // Process with even ID first sends to the upper neighbor and then to the
    // lower neighbor
    if (processId % 2 == 0) {
        // Receive the row from the upper neighbor into the zeroth row (extra
        // row)
        MPI_Recv(grid[numRows + 1], N, MPI_INT, upperNeighbor, currentRound,
                 MPI_COMM_WORLD, &status);
        // Receive the row from the lower neighbor into the last row (extra row)
        MPI_Recv(grid[numRows], N, MPI_INT, lowerNeighbor, currentRound,
                 MPI_COMM_WORLD, &status);

        // Send the second-to-last row (bottom row) to the lower neighbor
        MPI_Send(grid[numRows - 1], N, MPI_INT, lowerNeighbor, currentRound,
                 MPI_COMM_WORLD);
        // Send the first row (top row) to the upper neighbor
        MPI_Send(grid[0], N, MPI_INT, upperNeighbor, currentRound,
                 MPI_COMM_WORLD);
    } else {
        // Process with odd ID first receives from the upper neighbor and then
        // from the lower neighbor

        // Send the second-to-last row (bottom row) to the lower neighbor
        MPI_Send(grid[numRows - 1], N, MPI_INT, lowerNeighbor, currentRound,
                 MPI_COMM_WORLD);
        // Send the first row (top row) to the upper neighbor
        MPI_Send(grid[0], N, MPI_INT, upperNeighbor, currentRound,
                 MPI_COMM_WORLD);

        // Receive the row from the upper neighbor into the zeroth row (extra
        // row)
        MPI_Recv(grid[numRows + 1], N, MPI_INT, upperNeighbor, currentRound,
                 MPI_COMM_WORLD, &status);
        // Receive the row from the lower neighbor into the last row (extra row)
        MPI_Recv(grid[numRows], N, MPI_INT, lowerNeighbor, currentRound,
                 MPI_COMM_WORLD, &status);
    }
}

void initGrid(int **grid, int numRows, int processId) {
    // Initialize the grid with initial living cells
    // Specific cells are set to live based on processId
    for (int i = 0; i < numRows + 2; i++) {
        for (int j = 0; j < N; j++) {
            grid[i][j] = 0;
        }
    }
    // Only the first process initializes the specific patterns
    if (processId == 0) {
        // GLIDER
        int lin = 1, col = 1;
        grid[lin][col + 1] = 1;
        grid[lin + 1][col + 2] = 1;
        grid[lin + 2][col] = 1;
        grid[lin + 2][col + 1] = 1;
        grid[lin + 2][col + 2] = 1;

        // R-pentomino
        lin = 10;
        col = 30;
        grid[lin][col + 1] = 1;
        grid[lin][col + 2] = 1;
        grid[lin + 1][col] = 1;
        grid[lin + 1][col + 1] = 1;
        grid[lin + 2][col + 1] = 1;
    }
}

void calculateLivingCells(int **grid, int **newGrid, int numRows) {
    for (int i = 0; i < numRows; i++) {
        for (int j = 0; j < N; j++) {
            int amountNeighbours = getNeighbors(grid, i, j, numRows);

            if ((amountNeighbours == 2 && grid[i][j] == 1) ||
                amountNeighbours == 3) {
                newGrid[i][j] = 1;
            } else {
                newGrid[i][j] = 0;
            }
        }
    }
}

int amountLivingCells(int **grid, int numRows) {
    int count = 0;
    for (int i = 1; i <= numRows; i++) {
        for (int j = 0; j < N; j++) {
            if (grid[i][j] == 1) {
                count++;
            }
        }
    }
    return count;
}

int main(int argc, char *argv[]) {
    int processId, numProcesses, sumLivingCells = 0;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numProcesses);
    MPI_Comm_rank(MPI_COMM_WORLD, &processId);

    int numRows = N / numProcesses;
    int **grid, **newGrid;
    clock_t start, end;
    double cpu_time_used;

    // Allocate memory for grid and newGrid
    grid = (int **)malloc((numRows + 2) * sizeof(int *));
    newGrid = (int **)malloc((numRows + 2) * sizeof(int *));

    if (!grid || !newGrid) {
        fprintf(stderr, "Failed to allocate memory for grids\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    // Grid columns allocation
    for (int i = 0; i < (numRows + 2); i++) {
        grid[i] = (int *)malloc(N * sizeof(int));
        newGrid[i] = (int *)malloc(N * sizeof(int));
    }

    // Grid initialization
    initGrid(grid, numRows, processId);
    initGrid(newGrid, numRows, processId);

    int upperNeighbor, lowerNeighbor;
    initNeighbors(&upperNeighbor, &lowerNeighbor, processId, numProcesses);

    if (processId == 0) {
        start = clock();
    }

    for (int round = 0; round < ROUNDS; round++) {
        // Exchange border rows with neighboring processes
        if (numProcesses > 1) {
            exchangeNeighbors(grid, numRows, upperNeighbor, lowerNeighbor,
                              round, processId);
        }

        calculateLivingCells(grid, newGrid, numRows);

        // Swap grids
        int **temp = grid;
        grid = newGrid;
        newGrid = temp;

        MPI_Barrier(MPI_COMM_WORLD);
    }

    // Compute and reduce the final sum of living cells
    int localSum = amountLivingCells(grid, numRows);

    // Reduce: Sum all local counts to get the total number of living cells
    MPI_Reduce(&localSum, &sumLivingCells, 1, MPI_INT, MPI_SUM, 0,
               MPI_COMM_WORLD);

    // printf("Local Sum processo %i: %i\n", processId, localSum);

    if (processId == 0) {
        // double end = omp_get_wtime();
        end = clock();
        double cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
        printf("Celulas vivas no final: %d\n", sumLivingCells);
        printf("Tempo de CPU utilizado: %.3fs\n", cpu_time_used);
    }

    MPI_Finalize();
    return 0;
}
