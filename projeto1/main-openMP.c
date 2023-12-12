#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

#define DIMENSION 50
#define GENERATIONS 2000
#define NUM_THREADS 4

// Define the original and copy grids as global 2D arrays of float pointers
int *originalGrid[DIMENSION][DIMENSION];
int *copyGrid[DIMENSION][DIMENSION];

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
  if (*originalGrid[x][y] > 0)
  {
    // A. Células vivas com menos de 2 vizinhas vivas morrem por abandono
    if (getNeighbors(x, y) < 2 || getNeighbors(x, y) > 3)
    {
      *copyGrid[x][y] = 0; // morte por abandono
    }
    else
      *copyGrid[x][y] = 1;
  }
  else
  {
    if (getNeighbors(x, y) == 3)
      *copyGrid[x][y] = 1;
    else
      *copyGrid[x][y] = 0;
  }
}

void calculateLivingCells()
{
  int i, j;
#pragma omp parallel for private(j) num_threads(NUM_THREADS)
  for (i = 0; i < DIMENSION; i++)
  {
    for (j = 0; j < DIMENSION; j++)
    {
      validateGameRules(i, j);
    }
  }
}

void initGrid(int *grid[DIMENSION][DIMENSION])
{
  // Allocate memory for the grid
  for (int i = 0; i < DIMENSION; i++)
  {
    for (int j = 0; j < DIMENSION; j++)
    {
      grid[i][j] = (int *)malloc(sizeof(int));
      if (grid[i][j] == NULL)
      {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(1);
      }
      *grid[i][j] = 0;
    }
  }

  // GLIDER INIT
  int line = 1, column = 1;

  *grid[line][column + 1] = 1;
  *grid[line + 1][column + 2] = 1;
  *grid[line + 2][column] = 1;
  *grid[line + 2][column + 1] = 1;
  *grid[line + 2][column + 2] = 1;

  // R-pentomino INIT
  line = 10, column = 30;

  *grid[line][column + 1] = 1;
  *grid[line][column + 2] = 1;
  *grid[line + 1][column] = 1;
  *grid[line + 1][column + 1] = 1;
  *grid[line + 2][column + 1] = 1;
}

void displayGrid()
{
  FILE *file = fopen("openMPdisplay.txt", "w");

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
      fprintf(file, "%d   ", *originalGrid[i - 1][j - 1]);
    }
    fprintf(file, "\n");
  }

  fclose(file);
}

// Function to copy data from the copy grid to the original grid
void copyCopyToOriginal()
{
  int i, j;
#pragma omp parallel for private(j) num_threads(NUM_THREADS)
  for (int i = 0; i < DIMENSION; i++)
  {
    for (int j = 0; j < DIMENSION; j++)
    {
      *originalGrid[i][j] = *copyGrid[i][j];
    }
  }
}

void printCell(int i, int j)
{
  printf("cell value: %0.1f\n", *originalGrid[0][1]);
}

int amountLivingCells()
{
  int x, y, livingCells = 0;
#pragma omp parallel for private(y) reduction(+ : livingCells) num_threads(NUM_THREADS)
  for (x = 0; x < DIMENSION; x++)
  {
    for (y = 0; y < DIMENSION; y++)
    {
      if (*originalGrid[x][y] > 0)
      {
        livingCells++;
      }
    }
  }
  return livingCells;
}

int lifeGameIterator()
{
  struct timespec start, end;
  int iteration;
  clock_gettime(CLOCK_MONOTONIC, &start);
  for (iteration = 0; iteration < GENERATIONS; iteration++)
  {
    calculateLivingCells();
    copyCopyToOriginal();
    if (iteration < 5)
      printf("---GENERATION %d--- living cells: %d\n", iteration, amountLivingCells(originalGrid));
  }
  printf("---GENERATION %d--- living cells: %d\n", iteration, amountLivingCells(originalGrid));
  clock_gettime(CLOCK_MONOTONIC, &end);
  double elapsed_time = (end.tv_sec - start.tv_sec) +
                        (end.tv_nsec - start.tv_nsec) / 1e9;
  printf("\nTempo de laco geracoes: %f seconds\n", elapsed_time);
  return 0;
}

int main()
{
  struct timespec start, end;
  clock_gettime(CLOCK_MONOTONIC, &start);
  initGrid(originalGrid);
  initGrid(copyGrid);
  printf("Condicao inicial: %d\n", amountLivingCells(originalGrid));
  // displayGrid();
  lifeGameIterator();
  // Measure end time
  clock_gettime(CLOCK_MONOTONIC, &end);
  displayGrid();
  // Calculate elapsed time
  double elapsed_time = (end.tv_sec - start.tv_sec) +
                        (end.tv_nsec - start.tv_nsec) / 1e9;

  printf("Tempo de execucao do codigo: %f seconds\n", elapsed_time);
  return 0;
}
