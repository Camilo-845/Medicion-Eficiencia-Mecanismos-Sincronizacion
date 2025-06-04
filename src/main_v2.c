#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

enum Transaction_Type { DEP, RET, TRA };

typedef struct {
  int id;
  float balance;
} Count;

typedef struct {
  int origin;
  enum Transaction_Type type;
  float amount;
} Transaction;

int nCounts, nTransactions, nThreads;
Count *counts;
Transaction *transactions;
sem_t sem;

void *func_hilo(void *arg);
void read_file(const char *filename);
void print_balance();
int findAccount(int id);

int main(int argc, char *argv[]) {
  struct timeval start, end;

  const char *filename = argv[1];
  nThreads = atoi(argv[2]);

  // print_balance();
  read_file(filename);
  gettimeofday(&start, NULL);

  sem_init(&sem, 0, 1);

  pthread_t *threads = malloc(sizeof(pthread_t) * nThreads);
  for (int i = 0; i < nThreads; i++) {
    int *id = malloc(sizeof(int));
    *id = i;
    pthread_create(&threads[i], NULL, func_hilo, id);
  }

  for (int i = 0; i < nThreads; i++) {
    pthread_join(threads[i], NULL);
  }

  // print_balance();

  sem_destroy(&sem);
  free(threads);
  free(counts);
  free(transactions);

  gettimeofday(&end, NULL);
  double elapsed =
      (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1e6;
  printf("%.6f\n", elapsed);
  return EXIT_SUCCESS;
}

void *func_hilo(void *arg) {
  int idx = *(int *)arg;
  free(arg);

  int chunk = nTransactions / nThreads;
  int start = idx * chunk;
  int end = (idx == nThreads - 1) ? nTransactions : start + chunk;

  for (int i = start; i < end; i++) {
    Transaction tx = transactions[i];
    int src = findAccount(tx.origin);
    if (src == -1)
      continue;

    sem_wait(&sem);
    if (tx.type == DEP)
      counts[src].balance += tx.amount;
    else
      counts[src].balance -= tx.amount;
    sem_post(&sem);
  }
  return NULL;
}

void read_file(const char *filename) {
  FILE *f = fopen(filename, "r");
  if (!f) {
    perror("Error abriendo archivo");
    exit(EXIT_FAILURE);
  }

  fscanf(f, "%d", &nCounts);
  counts = malloc(sizeof(Count) * nCounts);
  for (int i = 0; i < nCounts; i++) {
    fscanf(f, "%d %f", &counts[i].id, &counts[i].balance);
  }

  fscanf(f, "%d", &nTransactions);
  transactions = malloc(sizeof(Transaction) * nTransactions);

  for (int i = 0; i < nTransactions; i++) {
    char tipo[4];
    fscanf(f, "%d %s %f", &transactions[i].origin, tipo,
           &transactions[i].amount);

    if (strcmp(tipo, "DEP") == 0) {
      transactions[i].type = DEP;
    } else if (strcmp(tipo, "RET") == 0) {
      transactions[i].type = RET;
    }
  }

  fclose(f);
}

int findAccount(int id) {
  for (int i = 0; i < nCounts; i++) {
    if (counts[i].id == id)
      return i;
  }
  return -1;
}

void print_balance() {
  printf("=== Estado de cuentas ===\n");
  for (int i = 0; i < nCounts; i++) {
    printf("Cuenta %d: %f\n", counts[i].id, counts[i].balance);
  }
}
