#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum Transaction_Type { DEP, RET, TRA };

typedef struct {
  int count;
  enum Transaction_Type type;
  float amount;
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

int nCounts;
int nDays;

Count *counts;
Daily_Transactions *daily_transactions;

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("Arguments missing");
    return EXIT_FAILURE;
  }
  char *fileName = argv[1];
  int nThreads = atoi(argv[2]);

  read_file(fileName);
  print_balance(0);
  return EXIT_SUCCESS;
}

void *func_hilo(void *arg) {
  int idx = *(int *)arg;
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
        int dst_count;
        fscanf(file, "%d", &dst_count);
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
