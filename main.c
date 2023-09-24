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

void *threadFunction(void *param)
{
    struct ThreadParams *params = (struct ThreadParams *)param;

    // This code is executed concurrently
    printf("Thread %d: Starting at: %d, Finishing at: %d\n", params->threadId,
           params->range[0][0], params->range[0][1]);
    // Chamada da função com os argumentos desejados
    // validateGameRules(args[0], args[1], 0, 1);

    // Retornando NULL (ou algum outro valor se necessário)
    pthread_exit(NULL);
}

void threadCreateExec(threadParams params[THREADS])
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

int getNeighbors(float *whichGrid[DIMENSION][DIMENSION], int i, int j)
{
    float arrNeighbors[8];
    int amountLivingNeighbors = 0;

    // Relative positions of neighbors
    int neighborX[] = {-1, -1, -1, 0, 0, 1, 1, 1};
    int neighborY[] = {-1, 0, 1, -1, 1, -1, 0, 1};

    for (int k = 0; k < 8; k++)
    {
        int x = i + neighborX[k];
        int y = j + neighborY[k];

        // Condicao pra verificar se estamos em alguma das bordas do tabuleiro
        if (x < 0)
        {
            x = DIMENSION - 1; // Vizinho na borda superior
        }
        else if (x >= DIMENSION)
        {
            x = 0; // Vizinho na borda inferior
        }

        if (y < 0)
        {
            y = DIMENSION - 1; // Vizinho na borda esquerda
        }
        else if (y >= DIMENSION)
        {
            y = 0; // Vizinho na borda direita
        }

        arrNeighbors[k] = *whichGrid[x][y]; // Verifica o valor da célula (x, y) após o ajuste e o armazena no array
    }

    for (int k = 0; k < 8; k++)
    {
        amountLivingNeighbors += arrNeighbors[k];
    }

    return amountLivingNeighbors;
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

void validateGameRules(float *readGrid[DIMENSION][DIMENSION],
                       float *writeGrid[DIMENSION][DIMENSION], int x, int y)
{
    // se a célula está viva
    if (*readGrid[x][y] > 0.0)
    {
        // A. Células vivas com menos de 2 vizinhas vivas morrem por abandono
        if (getNeighbors(readGrid, x, y) < 2)
        {
            *writeGrid[x][y] = 0.0; // morte por abandono
        }
        // B. Células vivas com 2 ou 3 vizinhos devem permanecer vivas
        else if (getNeighbors(readGrid, x, y) == 2 ||
                 getNeighbors(readGrid, x, y) == 3)
        {
            *writeGrid[x][y] = 1.0; // permanece viva
        }
        // C. Células vivas com 4 ou mais vizinhos morrem por superpopulação
        else if (getNeighbors(readGrid, x, y) >= 4)
        {
            *writeGrid[x][y] = 0.0; // morte por superpopulação
        }
    }
    else
    {
        // D. Célula morta com exatamente 3 vizinhos se tornam vivas
        if (getNeighbors(readGrid, x, y) == 3)
        {
            *writeGrid[x][y] = 1.0; // vive
        }
    }
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
        // copyCopyToOriginal();
        countLivingCells(originalGrid);
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
