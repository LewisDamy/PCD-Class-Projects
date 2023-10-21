#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <windows.h>

#define THREADS 5 // A ultima é o servidor
int req[THREADS - 1];
int res[THREADS - 1];
int SOMA, protect = 0;

void arrInit()
{
  for (int i = 0; i < THREADS; i++)
  {
    req[i] = 0;
    res[i] = 0;
  }
}

void criticalSection(int t)
{
  int local = SOMA;
  Sleep(rand() % 2);
  SOMA = local + 1;
  printf("thread %d: %d\n", t, SOMA);
}

void *client(void *arg)
{
  int client_id = *((int *)arg);
  while (1)
  {
    if (protect == 1)
    {
      while (res[client_id - 1] != 1)
      {
        req[client_id - 1] = 1;
      }
      criticalSection(client_id);
      res[client_id - 1] = 0;
    }
    else
      criticalSection(client_id);
  }
  pthread_exit(NULL);
}

void *serverExecute(void *arg)
{
  printf("Server listening...\n");
  while (1)
  {
    for (int i = 0; i < THREADS; i++)
    {
      if (req[i] != 0)
      {
        res[i] = 1;
        while (res[i] == 1)
        {
        }
        req[i] = 0;
      }
    }
  }
  pthread_exit(NULL);
}

void threadsCreator()
{
  pthread_t process[THREADS];
  int client_ids[THREADS - 1];
  int rc;
  for (int t = 0; t < THREADS; t++)
  {
    if (t < THREADS - 1)
    {
      client_ids[t] = t + 1;
      rc = pthread_create(&process[t], NULL, client, &client_ids[t]);
    }
    else
      rc = pthread_create(&process[t], NULL, serverExecute, NULL);
    if (rc)
    {
      printf("Error: Unable to create thread %d\n", t + 1);
      exit(-1);
    }
  }
  for (int t = 0; t < THREADS; t++)
    pthread_join(process[t], NULL);
  printf("All threads joined\n");
}

int main(int argc, char *argv[])
{
  if (argc > 1)
  {
    protect = atoi(argv[1]);
  }
  protect == 1 ? printf("Acesso restrito a sessao critica\n") : printf("Acesso concorrente a sessao critica\n");
  arrInit();
  threadsCreator();
  return 0;
}
