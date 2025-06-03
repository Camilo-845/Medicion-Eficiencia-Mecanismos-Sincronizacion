#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum Transaction_Type { DEP, RET, TRA };

typedef struct {
  int count;
  enum Transaction_Type type;
  float amount;
  int dest_count;
} Transaction;

typedef struct {
  int id;
  float balance;
} Count;

typedef struct {
  int nTransactions;
  Transaction *transactions;
} Daily_Transactions;

void *func_hilo(void *arg);
void read_file(const char *filename);
void print_balance(int day);
int findAccount(int count);

int nCounts;
int nDays;
int nThreads;

Count *counts;
Daily_Transactions *daily_transactions;
pthread_mutex_t *account_mutexes;
pthread_barrier_t dailyBarrier;
pthread_barrier_t balanceBarrier;

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("Arguments missing");
    return EXIT_FAILURE;
  }
  char *fileName = argv[1];
  nThreads = atoi(argv[2]);

  read_file(fileName);
  print_balance(0);

  account_mutexes = malloc(sizeof(pthread_mutex_t) * nCounts);
  pthread_barrier_init(&dailyBarrier, NULL, nThreads + 1);
  pthread_barrier_init(&balanceBarrier, NULL, nThreads + 1);

  for (int i = 0; i < nCounts; i++) {
    if (pthread_mutex_init(&account_mutexes[i], NULL) != 0) {
      fprintf(stderr, "Error initializing mutex for account %d\n", i);
      exit(EXIT_FAILURE);
    }
  }

  pthread_t *threads = malloc(sizeof(pthread_t) * nThreads);
  for (int i = 0; i < nThreads; i++) {
    int *arg = malloc(sizeof(int));
    *arg = i;
    pthread_create(&threads[i], NULL, func_hilo, (void *)arg);
  }

  for (int day = 0; day < nDays; day++) {
    pthread_barrier_wait(&balanceBarrier);
    print_balance(day + 1);
    pthread_barrier_wait(&dailyBarrier);
  }

  for (int i = 0; i < nThreads; i++) {
    pthread_join(threads[i], NULL);
  }

  for (int i = 0; i < nCounts; i++) {
    pthread_mutex_destroy(&account_mutexes[i]);
  }
  free(account_mutexes);
  return EXIT_SUCCESS;
}

void *func_hilo(void *arg) {
  int idx = *(int *)arg;
  for (int day = 0; day < nDays; day++) {
    pthread_barrier_wait(&balanceBarrier);
    int nTransactions = daily_transactions[day].nTransactions;
    int delta = nTransactions / nThreads;
    int start = idx * delta;
    int end = (idx == nThreads - 1) ? nTransactions : start + delta;
    // printf("hilo %d en dia %d: rango [%d,%d]\n", idx, day, start, end);

    for (int i = start; i < end; i++) {
      int account = daily_transactions[day].transactions[i].count;
      int type = daily_transactions[day].transactions[i].type;
      float amount = daily_transactions[day].transactions[i].amount;

      int countPos = findAccount(account);
      if (countPos == -1) {
        printf("Error buscando posicion cuenta %d\n", account);
        continue;
      }

      if (type != TRA) {
        pthread_mutex_lock(&account_mutexes[countPos]);
        if (type == DEP) {
          counts[countPos].balance += amount;
          // printf("Haciendo deposito a cuenta %d de %f\n", account, amount);
        } else if (type == RET) {
          counts[countPos].balance -= amount;
          // printf("Haciendo retiro a cuenta %d de %f\n", account, amount);
        }
        pthread_mutex_unlock(&account_mutexes[countPos]);
      } else {
        int destAccount = daily_transactions[day].transactions[i].dest_count;
        int destPos = findAccount(destAccount);

        if (destPos == -1) {
          printf("Error buscando cuenta destino %d\n", destAccount);
          continue;
        }

        // Ordenar los locks para evitar deadlocks
        if (countPos < destPos) {
          pthread_mutex_lock(&account_mutexes[countPos]);
          pthread_mutex_lock(&account_mutexes[destPos]);
        } else if (countPos > destPos) {
          pthread_mutex_lock(&account_mutexes[destPos]);
          pthread_mutex_lock(&account_mutexes[countPos]);
        } else {
          // Misma cuenta como origen y destino, no es una transferencia válida
          printf("Transferencia inválida: misma cuenta %d\n", account);
          continue;
        }

        // Transferencia: restar del origen y sumar al destino
        counts[countPos].balance -= amount;
        counts[destPos].balance += amount;

        pthread_mutex_unlock(&account_mutexes[countPos]);
        pthread_mutex_unlock(&account_mutexes[destPos]);
      }
    }
    pthread_barrier_wait(&dailyBarrier);
  }
  return NULL;
}

void read_file(const char *filename) {
  FILE *file = fopen(filename, "r");

  fscanf(file, "%d", &nCounts);
  counts = malloc(sizeof(Count) * nCounts);
  for (int i = 0; i < nCounts; i++) {
    fscanf(file, "%d %f", &counts[i].id, &counts[i].balance);
  }
  fscanf(file, "%d", &nDays);

  daily_transactions = malloc(sizeof(Daily_Transactions) * nDays);
  for (int i = 0; i < nDays; i++) {
    fscanf(file, "%d", &daily_transactions[i].nTransactions);
    daily_transactions[i].transactions =
        malloc(sizeof(Transaction) * daily_transactions[i].nTransactions);

    for (int j = 0; j < daily_transactions[i].nTransactions; j++) {
      fscanf(file, "%d", &daily_transactions[i].transactions[j].count);

      char type[4];
      fscanf(file, "%s", &type);
      if (strcmp(type, "RET") == 0) {
        daily_transactions[i].transactions[j].type = RET;
      } else if (strcmp(type, "DEP") == 0) {
        daily_transactions[i].transactions[j].type = DEP;
      } else {
        daily_transactions[i].transactions[j].type = TRA;
      }
      fscanf(file, "%f", &daily_transactions[i].transactions[j].amount);
      if (daily_transactions[i].transactions[j].type == TRA) {
        fscanf(file, "%d", &daily_transactions[i].transactions[j].dest_count);
      } else {
        daily_transactions[i].transactions[j].dest_count = -1;
      }
    }
  }

  fclose(file);
}

void print_balance(int day) {
  printf("=== Balance del día %d ===\n", day);
  for (int i = 0; i < nCounts; i++) {
    printf("Cuenta %d: %.2f\n", counts[i].id, counts[i].balance);
  }
  printf("=========================\n");
}

int findAccount(int count) {
  for (int i = 0; i < nCounts; i++) {
    if (counts[i].id == count) {
      return i;
    }
  }
  return -1;
}
