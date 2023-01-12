#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

int D(int sem, struct sembuf *buf) {
    buf->sem_op = -1;
    buf->sem_flg = 0;
    buf->sem_num = 0;
    return semop(sem, buf, 1);
}

int A(int sem, struct sembuf *buf) {
    buf->sem_op = 1;
    buf->sem_flg = 0;
    buf->sem_num = 0;
    return semop(sem, buf, 1);
}

int main() {
    // Pointer to the shared memory.
    int *array;
    int necessary_to_initialize = 1;
    long i;
    int shmid;
    int semid;
    char pathname[] = "task9-2-1.c";
    key_t key;
    struct sembuf mybuf;
    // Generate IPC-key.
    if ((key = ftok(pathname, 0)) < 0) {
        printf("The key has not been generated.\n");
        exit(-1);
    }
    // Create or get a shared memory segment.
    if ((shmid = shmget(key, 3 * sizeof(int), 0666 | IPC_CREAT | IPC_EXCL)) < 0) {
        // Find the problem.
        if (errno != EEXIST) {
            printf("The shared memory has not created.\n");
            exit(-1);
        } else {
            if ((shmid = shmget(key, 3 * sizeof(int), 0)) < 0) {
                printf("The shared memory has not been found.\n");
                exit(-1);
            }
            necessary_to_initialize = 0;
        }
    }
    // Map shared memory to the address space of the current process.
    if ((array = (int *)shmat(shmid, NULL, 0)) == (int *)(-1)) {
        printf("The shared memory has not been attached.\n");
        exit(-1);
    }
    // Get or create semaphore by key.
    if ((semid = semget(key, 1, 0666)) < 0) {
        if ((semid = semget(key, 1, 0666 | IPC_CREAT)) < 0) {
            printf("Semaphore set has not been created.\n");
            exit(-1);
        }
        printf("Success!\n");
        // Increase counter. 
        A(semid, &mybuf);
    }
    // Inititalize the array if it is necessary.
    if (necessary_to_initialize) {
        array[0] = 0;
        array[1] = 1;
        array[2] = 1;
    } else {
        // Decrease counter.
        D(semid, &mybuf);
        array[0] += 1;
        for (i = 0; i < 2000000000L; i++) ;
        array[2] += 1;
        // Increase counter.
        A(semid, &mybuf);
    }
    printf("Program 1 started %d times, program 2 started %d times, total - %d times\n", array[0], array[1], array[2]);
    // Delete shared memory from address space of the current process.
    if (shmdt(array) < 0) {
        printf("Shared memory has not been deleted.\n");
        exit(-1);
    }
    return 0;
}
