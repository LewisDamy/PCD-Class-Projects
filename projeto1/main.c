#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define DIMENSION 50
#define GENERATIONS 2000
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
            // printf("Vertice direto inferior\n");
            auxNeighborX[0] = 0;
            auxNeighborX[1] = 0;
            auxNeighborX[2] = 0;
            auxNeighborY[2] = 0;
            auxNeighborY[4] = 0;
            auxNeighborY[7] = 0;
        }
        else if (col == 0 && lin == (DIMENSION - 1))
        { // vertice esquerdo inferior
            // printf("Vertice esquerdo inferior\n");
            auxNeighborX[6] = 0;
            auxNeighborX[7] = 0;
            auxNeighborX[5] = 0;
            auxNeighborY[5] = DIMENSION - 1;
            auxNeighborY[3] = DIMENSION - 1;
            auxNeighborY[0] = DIMENSION - 1;
        }
        else if (col == (DIMENSION - 1) && lin == 0)
        { // vertice direito superior
            // printf("vertice direito superior\n");
            auxNeighborX[0] = DIMENSION - 1;
            auxNeighborX[1] = DIMENSION - 1;
            auxNeighborX[2] = DIMENSION - 1;
            auxNeighborY[2] = 0;
            auxNeighborY[4] = 0;
            auxNeighborY[7] = 0;
        }
        else if (col == 0 && lin == 0)
        { // vertice esquerdo superior
            // printf("vertice esquerdo superior\n");
            auxNeighborX[0] = DIMENSION - 1;
            auxNeighborX[1] = DIMENSION - 1;
            auxNeighborX[2] = DIMENSION - 1;
            auxNeighborY[0] = DIMENSION - 1;
            auxNeighborY[3] = DIMENSION - 1;
            auxNeighborY[5] = DIMENSION - 1;
        }
        else if (lin == (DIMENSION - 1))
        { // qualquer coluna interna da ultima linha
          // printf("qualquer coluna interna da ultima linha\n");
            auxNeighborX[5] = 0;
            auxNeighborX[6] = 0;
            auxNeighborX[7] = 0;
        }
        else if (lin == 0)
        { // qualquer coluna interna da primeira linha
            // printf("qualquer coluna interna da primeira linha\n");
            auxNeighborX[0] = DIMENSION - 1;
            auxNeighborX[1] = DIMENSION - 1;
            auxNeighborX[2] = DIMENSION - 1;
        }
    }
    else if (col == 0)
    {
        // printf("qualquer linha interna da primeira coluna\n");
        auxNeighborY[0] = DIMENSION - 1;
        auxNeighborY[3] = DIMENSION - 1;
        auxNeighborY[5] = DIMENSION - 1;
    }
    else if (col == DIMENSION - 1)
    {
        // printf("qualquer linha interna da ultima coluna\n");
        auxNeighborY[2] = 0;
        auxNeighborY[4] = 0;
        auxNeighborY[7] = 0;
    }
    else if (lin > DIMENSION - 1 || col > DIMENSION - 1)
    {
        printf("Falha - Ponto externo\n");
        return -1;
    }
    else
        for (int i = 0; i < 8; i++)
        {
            if (*originalGrid[auxNeighborX[i]][auxNeighborY[i]] > 0)
            {
                amountLivingNeighbors++;
            }
        }
    return amountLivingNeighbors;
}

void validateGameRules(int x, int y)
{
    // se a célula está viva
    if (*originalGrid[x][y] > 0.0)
    {
        // A. Células vivas com menos de 2 vizinhas vivas morrem por abandono
        if (getNeighbors(x, y) < 2 || getNeighbors(x, y) > 3)
        {
            *copyGrid[x][y] = 0.0; // morte por abandono
        }
        else
            *copyGrid[x][y] = 1.0;
    }
    else
    {
        if (getNeighbors(x, y) == 3)
            *copyGrid[x][y] = 1.0;
        else
            *copyGrid[x][y] = 0.0;
    }
}

void *threadFunction(void *param)
{
    struct ThreadParams *params = (struct ThreadParams *)param;
    // printf("%d: entrei\n", params->threadId);
    //  This code is executed concurrently
    //  printf("Thread %d: Starting at: %d, Finishing at: %d\n", params->threadId,
    //        params->range[0][0], params->range[0][1]);
    //  Chamada da função com os argumentos desejados
    for (int i = params->range[0][0]; i <= params->range[0][1]; i++)
    {
        // printf("thread %d, begin: %d, end: %d\n", params->threadId, params->range[0][0], params->range[0][1]);
        for (int j = 0; j < DIMENSION; j++)
        {
            validateGameRules(i, j);
        }
    }
    // validateGameRules(args[0], args[1], 0, 1);
    // printf("%d: retornei\n", params->threadId);
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
        if (partitionBoundaries[t][1] == DIMENSION)
            partitionBoundaries[t][1] = DIMENSION - 1;
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
    int line = 1, column = 1;

    *grid[line][column + 1] = 1.0;
    *grid[line + 1][column + 2] = 1.0;
    *grid[line + 2][column] = 1.0;
    *grid[line + 2][column + 1] = 1.0;
    *grid[line + 2][column + 2] = 1.0;

    // R-pentomino INIT
    line = 10, column = 30;

    *grid[line][column + 1] = 1.0;
    *grid[line][column + 2] = 1.0;
    *grid[line + 1][column] = 1.0;
    *grid[line + 1][column + 1] = 1.0;
    *grid[line + 2][column + 1] = 1.0;
}

void displayGrid()
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
            fprintf(file, "%.1f   ", *originalGrid[i - 1][j - 1]);
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

    partitionGrid(partitionBoundaries); // Call the partitionGrid function to calculate and store partition
    // printPartition(partitionBoundaries); // Print the partition boundaries for visualization
    setThreadsParameters(partitionBoundaries, params);
    int iteration;
    for (iteration = 0; iteration < GENERATIONS; iteration++)
    {
        threadCreateExec(params);
        // printf("1 done!\n");
        copyCopyToOriginal();
        // printf("2 done!\n");
        // if (iteration < 6 || iteration == GENERATIONS - 1)
        //    printf("---GENERATION %d---\nliving cells: %d\n", countLivingCells(originalGrid));
        if (iteration < 999)
            printf("---GENERATION %d--- living cells: %d\n", iteration, countLivingCells(originalGrid));
    }
    return 0;
}

int main()
{
    initGrid(originalGrid);
    initGrid(copyGrid);
    printf("Condicao inicial: %d\n", countLivingCells(originalGrid));
    displayGrid();
    lifeGameIterator();
    return 0;
}
