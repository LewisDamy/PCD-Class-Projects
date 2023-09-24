#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define DIMENSION 50
#define GENERATIONS 5
#define THREADS 4

// Define the original and copy grids as global 2D arrays of float pointers
float *originalGrid[DIMENSION][DIMENSION];
float *copyGrid[DIMENSION][DIMENSION];
struct ThreadParams
{
    int threadId;
    int range[1][2];
};
typedef struct ThreadParams threadParams;
void startThreads()
{
    // TODO
}

int getNeighbors(int lin, int col) // x, y de uma celula
{
    int amountLivingNeighbors = 0;
    int auxNeighborX[] = {lin - 1, lin - 1, lin - 1, lin, lin, lin + 1, lin + 1, lin + 1};
    int auxNeighborY[] = {col - 1, col, col + 1, col - 1, col + 1, col - 1, col, col + 1};

    if (lin == (DIMENSION - 1) || lin == 0)
    {
        if (col == (DIMENSION - 1) && lin == (DIMENSION - 1))
        { // vertice direto inferior
            auxNeighborX[5] = 0;
            auxNeighborX[6] = 0;
            auxNeighborX[7] = 0;
            auxNeighborY[3] = 0;
            auxNeighborY[5] = 0;
            auxNeighborY[8] = 0;
        }
        else if (col == 0 && lin == (DIMENSION - 1))
        { // vertice esquerdo inferior
            auxNeighborX[7] = 0;
            auxNeighborX[8] = 0;
            auxNeighborY[0] = DIMENSION - 1;
            auxNeighborY[3] = DIMENSION - 1;
        }
        else if (col == (DIMENSION - 1) && lin == 0)
        { // vertice direito superior
            auxNeighborX[0] = DIMENSION - 1;
            auxNeighborX[1] = DIMENSION - 1;
            auxNeighborX[2] = DIMENSION - 1;
            auxNeighborY[2] = 0;
            auxNeighborY[6] = 0;
            auxNeighborY[7] = 0;
        }
        else if (col == 0 && lin == 0)
        { // vertice esquerdo superior
            auxNeighborX[0] = DIMENSION - 1;
            auxNeighborX[1] = DIMENSION - 1;
            auxNeighborX[2] = DIMENSION - 1;
            auxNeighborY[0] = DIMENSION - 1;
            auxNeighborY[3] = DIMENSION - 1;
            auxNeighborY[4] = col - 1;
            auxNeighborY[5] = DIMENSION - 1;
        }
        else if (lin == (DIMENSION - 1))
        { // qualquer coluna interna da ultima linha
            auxNeighborX[5] = 0;
            auxNeighborX[6] = 0;
            auxNeighborX[7] = 0;
        }
        else if (lin == 0)
        { // qualquer coluna interna da primeira linha
            auxNeighborX[0] = DIMENSION - 1;
            auxNeighborX[1] = DIMENSION - 1;
            auxNeighborX[2] = DIMENSION - 1;
        }
    }
    else if (col == 0)
    {
        auxNeighborY[0] = DIMENSION - 1;
        auxNeighborY[3] = DIMENSION - 1;
        auxNeighborY[5] = DIMENSION - 1;
    }
    else if (col == DIMENSION - 1)
    {
        auxNeighborY[2] = 0;
        auxNeighborY[4] = 0;
        auxNeighborY[7] = 0;
    }

    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (*originalGrid[auxNeighborX[i]][auxNeighborY[j]] > 0)
            {
                amountLivingNeighbors++;
            }
        }
    }
    return amountLivingNeighbors;
}

void validateGameRules(int x, int y)
{
    // se a célula está viva
    if (*originalGrid[x][y] > 0.0)
    {
        printf("vizinhos %d\n", getNeighbors(x, y));
        // A. Células vivas com menos de 2 vizinhas vivas morrem por abandono
        if (getNeighbors(x, y) < 2)
        {
            *copyGrid[x][y] = 0.0; // morte por abandono
        }
        // B. Células vivas com 2 ou 3 vizinhos devem permanecer vivas
        else if (getNeighbors(x, y) == 2 ||
                 getNeighbors(x, y) == 3)
        {
            *copyGrid[x][y] = 1.0; // permanece viva
        }
        // C. Células vivas com 4 ou mais vizinhos morrem por superpopulação
        else if (getNeighbors(x, y) >= 4)
        {
            *copyGrid[x][y] = 0.0; // morte por superpopulação
        }
    }
    else
    {
        printf("vizinhos %d\n", getNeighbors(x, y));
        // D. Célula morta com exatamente 3 vizinhos se tornam vivas
        if (getNeighbors(x, y) == 3)
            *copyGrid[x][y] = 1.0; // vive
    }
}

void *threadFunction(void *param)
{
    struct ThreadParams *params = (struct ThreadParams *)param;
    // This code is executed concurrently
    // printf("Thread %d: Starting at: %d, Finishing at: %d\n", params->threadId,
    //       params->range[0][0], params->range[0][1]);
    // Chamada da função com os argumentos desejados
    for (int i = params->range[params->threadId][0]; i <= params->range[params->threadId][1]; i++)
    {
        for (int j = 0; j < DIMENSION; j++)
        {
            validateGameRules(i, j);
        }
    }
    // validateGameRules(args[0], args[1], 0, 1);

    // Retornando NULL (ou algum outro valor se necessário)
    pthread_exit(NULL);
}

void threadCreateExec(threadParams params[THREADS]) // params possui ID, linha de começo e fim
{
    pthread_t threads[THREADS];
    for (int t = 0; t < THREADS; t++)
    {
        int rc = pthread_create(&threads[t], NULL, threadFunction, (void *)&params[t]);
        if (rc)
        {
            printf("Error: Unable to create thread %d\n", t);
            exit(-1);
        }
    }
    for (int t = 0; t < THREADS; t++)
        pthread_join(threads[t], NULL);
}

void printPartition(int partitionBoundaries[THREADS][2])
{
    for (int t = 0; t < THREADS; t++)
    {
        printf("Partition %d: Start Row = %d, End Row = %d\n", t,
               partitionBoundaries[t][0], partitionBoundaries[t][1]);
    }
}

// Function to partition the grid and return an array of partition boundaries
void partitionGrid(int partitionBoundaries[][2]) // testar com -1 no for dps
{
    // Calculate the size of each partition in rows
    int partitionSize = DIMENSION / THREADS;

    for (int t = 0; t < THREADS; t++)
    {
        // Calculate the starting and ending row indices for this partition
        if (t > 0)
        {
            partitionBoundaries[t][0] = (t * partitionSize) + 1;
        }
        else
            partitionBoundaries[t][0] = t * partitionSize; // Start row
        partitionBoundaries[t][1] = (t == THREADS - 1)
                                        ? DIMENSION
                                        : (t + 1) * partitionSize; // End row
    }
}

void initGrid(float *grid[DIMENSION][DIMENSION])
{
    // Allocate memory for the grid
    for (int i = 0; i < DIMENSION; i++)
    {
        for (int j = 0; j < DIMENSION; j++)
        {
            grid[i][j] = (float *)malloc(sizeof(float));
            if (grid[i][j] == NULL)
            {
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

void displayGrid(float *grid[DIMENSION][DIMENSION])
{
    FILE *file = fopen("display.txt", "w");

    if (file == NULL)
    {
        printf("Error opening the file.\n");
        return;
    }

    fprintf(file, "      ");
    for (int j = 1; j <= DIMENSION; j++)
    {
        fprintf(file, "[%02d]  ", j);
    }
    fprintf(file, "\n");

    for (int i = 1; i <= DIMENSION; i++)
    {
        fprintf(file, "[%02d]  ", i);
        for (int j = 1; j <= DIMENSION; j++)
        {
            fprintf(file, "%.1f   ", *grid[i - 1][j - 1]);
        }
        fprintf(file, "\n");
    }

    fclose(file);
}

// Function to copy data from the original grid to the copy grid
void copyOriginalToCopy()
{
    for (int i = 0; i < DIMENSION; i++)
    {
        for (int j = 0; j < DIMENSION; j++)
        {
            *copyGrid[i][j] = *originalGrid[i][j];
        }
    }
}

// Function to copy data from the copy grid to the original grid
void copyCopyToOriginal()
{
    for (int i = 0; i < DIMENSION; i++)
    {
        for (int j = 0; j < DIMENSION; j++)
        {
            *originalGrid[i][j] = *copyGrid[i][j];
        }
    }
}

void printCell(float *whichGrid[DIMENSION][DIMENSION], int i, int j)
{
    printf("cell value: %0.1f\n", *whichGrid[0][1]);
}

int countLivingCells(float *whichGrid[DIMENSION][DIMENSION])
{
    /* Rule based on topic 5.
        Crie um procedimento (ou trecho de código) que, ao finalizar todas as
        iterações/gerações, compute a quantidade de células vivas.
        ATENCAO: Considere que uma célula viva tem seu valor maior que zero.
    */
    int livingCells = 0;
    for (int x = 0; x < DIMENSION; x++)
    {
        for (int y = 0; y < DIMENSION; y++)
        {
            if (*whichGrid[x][y] > 0.0)
            {
                livingCells++;
            }
        }
    }
    return livingCells;
}

void setThreadsParameters(int partitionBoundaries[THREADS][2], threadParams params[THREADS])
{

    for (int t = 0; t < THREADS; t++)
    {
        params[t].threadId = t;
        params[t].range[0][0] = partitionBoundaries[t][0];
        params[t].range[0][1] = partitionBoundaries[t][1];
    }
}

int lifeGameIterator()
{
    int rc;
    threadParams params[THREADS];
    int partitionBoundaries[THREADS][2]; // Array partition for each [thread][0] -> (start) / [thread][1] -> (end)
    malloc(THREADS * sizeof(struct ThreadParams));

    partitionGrid(partitionBoundaries);  // Call the partitionGrid function to calculate and store partition
    printPartition(partitionBoundaries); // Print the partition boundaries for visualization
    setThreadsParameters(partitionBoundaries, params);

    for (int iteration = 0; iteration < GENERATIONS; iteration++)
    {
        printf("---GENERATION %d---\n", iteration + 1);
        threadCreateExec(params);
        copyCopyToOriginal();
        printf("living cells: %d\n", countLivingCells(originalGrid));
    }
    return 0;
}

int main()
{
    // Initialize the original and copy grids
    initGrid(originalGrid);
    initGrid(copyGrid);
    lifeGameIterator();

    printf("living cells after rules applied: %i\n",
           countLivingCells(copyGrid));

    // int result = getNeighbors(originalGrid, 3, 1);
    // printf("result: %i\n", result);
    // printf("living cells in grid: %i\n", countLivingCells(copyGrid));
    return 0;
}
