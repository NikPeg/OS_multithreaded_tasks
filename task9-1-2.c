#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[], char *envp[]) {
  int   semid;
  char pathname[]="task9-1-1.c";
  key_t key;
  struct sembuf mybuf;
  mybuf.sem_num = 0;
  mybuf.sem_op  = 1;
  mybuf.sem_flg = 0;
  // Generate IPC-key.
  if ((key = ftok(pathname,0)) < 0) {
    printf("The key has not been generated.\n");
    exit(-1);
  }
  // Get semaphores array.
  if ((semid = semget(key, 1, 0666 | IPC_CREAT)) < 0) {
    printf("The semaphores array has not been generated.\n");
    exit(-1);
  }
  // Increase counter.
  if (semop(semid, &mybuf, 1) < 0) {
    printf("Waiting is unsuccessful.\n");
    exit(-1);
  }
  // Printing.
  printf("The condition is met.\n");
  return 0;
}
