#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

int main() {
    int shmid;
    char pathname[] = "task7-1-1.c";
    key_t key;
    char *text;
    int fsize = 0;
    int *size;
    FILE *file;
    // Open itself.
    file = fopen(pathname, "r");
    // Get file size.
    fseek(file, 0L, SEEK_END);
    fsize = ftell(file);
    // Start pointer.
    rewind(file);
    // Generate IPC-key.
    if ((key = ftok(pathname, 0)) < 0) {
        printf("The key has not been generated.\n");
        exit(-1);
    }
    // Create or get shared memory.
    if ((shmid = shmget(key, sizeof(int) + fsize * sizeof(char), 0666 | IPC_CREAT)) < 0) {
        printf("The shared memory has not been created.\n");
        exit(-1);
    }
    // Map shared memory to the current process address space.
    if ((size = (int *) shmat(shmid, NULL, 0)) == (int *) (-1)) {
        printf("The shared memory has not been attached.\n");
        exit(-1);
    }
    *size = fsize;
    text = (char *) (size + 1);
    // Write to the shared memory.
    for (int i = 0; i < fsize; i++) {
        text[i] = fgetc(file);
    }
    fclose(file);
    // Delete the shared memory.
    if (shmdt(size) < 0) {
        printf("The shared memory has not been deleted.\n");
        exit(-1);
    }
    return 0;
}
