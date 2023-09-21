#include <stdio.h>
#include <stdlib.h>

#define DIMENSION 50
#define ITERATIONS 10

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

    // Relative positions of neighbors
    int neighborX[] = {-1, -1, -1, 0, 0, 1, 1, 1};
    int neighborY[] = {-1, 0, 1, -1, 1, -1, 0, 1};

    if (i == 0 || j == 0) {
        for (int k = 0; k < 8; k++) {
            int x = (i + neighborX[k] + DIMENSION) % DIMENSION;
            int y = (j + neighborY[k] + DIMENSION) % DIMENSION;
            arrNeighbors[k] = *whichGrid[x][y];
        }
    } else {
        for (int k = 0; k < 8; k++) {
            int x = i + neighborX[k];
            int y = j + neighborY[k];
            arrNeighbors[k] = *whichGrid[x][y];
        }
    }
    for (int k = 0; k < 8; k++) {
        amountLivingNeighbors += arrNeighbors[k];
    }
    return amountLivingNeighbors;
}

int countLivingCells(float* whichGrid[DIMENSION][DIMENSION], int i, int j) {
    //  TODO
    // For loop that iterate thought all the grid and returns the sum
    // of += grid[i][j] value

    /* Rule based on topic 5.
        Crie um procedimento (ou trecho de código) que, ao finalizar todas as
        iterações/gerações, compute a quantidade de células vivas.
        ATENCAO: Considere que uma célula viva tem seu valor maior que zero.
    */
}

void validateGameRules(float* grid[DIMENSION][DIMENSION], int x, int y) {
   // se a célula está viva
    if (*grid[x][y] > 0.0) {
        // A. Células vivas com menos de 2 vizinhas vivas morrem por abandono
        if (liveNeighbors < 2) {
            *newGrid[x][y] = 0.0; // morte por abandono
        }
        // B. Células vivas com 2 ou 3 vizinhos devem permanecer vivas
        else if (liveNeighbors == 2 || liveNeighbors == 3) {
            *newGrid[x][y] = 1.0; // permanece viva
        }
        // C. Células vivas com 4 ou mais vizinhos morrem por superpopulação
        else if (liveNeighbors >= 4) {
            *newGrid[x][y] = 0.0; // morte por superpopulação
        }
    } else {
        // D. Célula morta com exatamente 3 vizinhos se tornam vivas
        if (liveNeighbors == 3) {
            *newGrid[x][y] = 1.0; // vive
        }
    }
}

void lifeGameIterator() {
    // TODO
    //
    /* Rule based on topic 4.
       Crie um laço de repetição para executar um determinado número máximo de
       iterações do jogo, ou seja, determine a quantidade de gerações sucessivas
       do tabuleiro que devem ser geradas.
     */
    // USE VARIABLE: ITERATIONS (defined at line 5)
}

int main() {
    // Initialize the original and copy grids
    initGrid(originalGrid);
    initGrid(copyGrid);

    int result = getNeighbors(originalGrid, 1, 2);
    printf("result: %i\n", result);

    return 0;
}
