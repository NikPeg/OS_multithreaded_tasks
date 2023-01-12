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
    int *size;
    int fsize;
    FILE *file;
    // Open itself.
    file = fopen(pathname, "r");
    // Get file size.
    fseek(file, 0L, SEEK_END);
    fsize = ftell(file);
    // Start pointer.
    rewind(file);
    fclose(file);
    // Generate IPC-key.
    if ((key = ftok(pathname, 0)) < 0) {
        printf("The key has not been generated.\n");
        exit(-1);
    }
    // Create or get shared memory.
    if ((shmid = shmget(key, sizeof(int), 0666)) < 0) {
        printf("Can\'t find shared memory\n");
        exit(-1);
    }
    // Map shared memory to the current process address space.
    if ((size = (int *) shmat(shmid, NULL, 0)) == (int *) (-1)) {
        printf("Can't attach shared memory\n");
        exit(-1);
    }
    text = (char *) (size + 1);
    // Print the code.
    for (int i = 0; i < *size; i++) {
        putchar(text[i]);
    }
    // Delete the shared memory.
    if (shmdt(size) < 0) {
        printf("The shared memory has not been deleted.\n");
        exit(-1);
    }
    // Delete the segment.
    if (shmctl(shmid, IPC_RMID, NULL) < 0) {
        printf("The segment memory has not been deleted.\n");
        exit(-1);
    }
    return 0;
}
