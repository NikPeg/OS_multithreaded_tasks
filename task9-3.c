#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

// Initially, the value of the semaphore is 1. 
//
// Parent process:
// -write a message
// -reduce the counter with D-operation
// -block the semaphore and allow the child process to read the message
//
// Child process:
// -read and write the message
// -incremet the counter using operation A
// -remove the lock from the semaphore
// -allow the parent process to read the message

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
    int N, i;
    printf("Enter the count of iterations (more than 2):\n> ");
    scanf("%d", &N);
    if (N <= 2) {
        printf("The count should be more than 2.\n");
        N = 2;
    }
    char pathname[] = "task9-3.c";
    // IPC-key.
    key_t key;
    // Semaphore ID.
    int semid;
    struct sembuf mybuf;
    // A handle for communication.
    int fd[2], result;
    // Vars for messages.
    int size;
    char resstring[18];
    // Create a pipe.
    if (pipe(fd) < 0) {
        printf("The pipe has not been opened.\n");
        exit(-1);
    }
    // Generate IPC-key.
    if ((key = ftok(pathname, 0)) < 0) {
        printf("The key has not been generated.\n");
        exit(-1);
    }
    // Get or create semaphore array.
    if ((semid = semget(key, 1, 0666)) < 0) {
        if ((semid = semget(key, 1, 0666 | IPC_CREAT)) < 0) {
            printf("The semaphore set has not been created.\n");
            exit(-1);
        }
        printf("Success\n");
    }
    // Child process.
    result = fork();
    for (i = 0; i < N; ++i) {
        // Check if it has been created.
        if (result < 0) {
            printf("Child[%d] process has not been created.\n", i);
            exit(-1);
        } else if (result > 0) {
            // Parent process.
            // Print the state.
            printf("%d parent process started.\n", i);
            // Write the message.
            size = write(fd[1], "Hello from parent!", 18);
            // Check the message.
            if (size != 18) {
                printf("%d parent process can not write all string to the pipe.\n", i);
                exit(-1);
            }
            printf("%d parent process wrote a message to child.\n", i);
            // Decrement the counter and block the parent.
            D(semid, &mybuf);
            // Read the message.
            size = read(fd[0], resstring, 18);
            // Check the message.
            if (size < 0) {
                printf("%d parent process can not read string from the pipe.\n", i);
                exit(-1);
            }
            printf("%d parent process read a message from child.\n> %s\n", i, resstring);
            // Print the state.
            printf("%d parent process finished.\n", i);
        } else {
            // Child process.
            // Print the current state.
            printf("%d child process started.\n", i);
            // Read a message.
            size = read(fd[0], resstring, 18);
            // Check the message.
            if (size < 0) {
                printf("%d child process can not read string from the pipe.\n", i);
                exit(-1);
            }
            printf("%d child process read message from parent.\n> %s\n", i, resstring);
            // Write the message.
            size = write(fd[1], "Hello from child!!", 18);
            // Check the message.
            if (size != 18) {
                printf("%d child process can not write all string to the pipe.\n", i);
                exit(-1);
            }
            printf("%d child process wrote message to parent.\n", i);
            // Increment the counter.
            // Unblock the parent process.
            A(semid, &mybuf);
            // Print the current state.
            printf("%d child process finished.\n", i);
        }
    }
    // Close the pipe.
    close(fd[0]);
    close(fd[1]);
    return 0;
}
