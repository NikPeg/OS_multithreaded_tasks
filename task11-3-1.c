#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

int main() {
    int msqid; // IPC descriptor
    key_t key; // IPC-key
    char pathname[] = "task11-3-1.c"; // filename
    int message_length, max_length;
    // Message structure
    struct msgbuf {
        long mtype;
        struct {
            pid_t pid;
            float finfo;
        } info;
    } mybuf;
    // Server message structure
    struct servmsgbuf {
        long mtype;
        float finfo;
    } myservbuf;
    // Generate IPC-ключ
    if ((key = ftok(pathname, 0)) < 0) {
        printf("Can\'t generate key\n");
        exit(-1);
    }
    // Get or create the queue
    if ((msqid = msgget(key, 0666 | IPC_CREAT)) < 0) {
        printf("Can\'t get msqid\n");
        exit(-1);
    }
    // Read messages in FIFO
    while (1) {
        max_length = sizeof(mybuf.info);
        // Try to read a message from queue
        if ((message_length = msgrcv(msqid, (struct msgbuf *)&mybuf, max_length, 1, 0)) < 0) {
            printf("Can't receive message from queue\n");
            exit(-1);
        }
        printf("Request from client %d\n", mybuf.info.pid);
        // Messages information
        myservbuf.mtype = mybuf.info.pid;
        // Calculate the result
        myservbuf.finfo = mybuf.info.finfo * mybuf.info.finfo;
        message_length = sizeof(myservbuf.finfo);
        // Send message to the queue
        if (msgsnd(msqid, (struct servmsggbuf *)&myservbuf, message_length, 0) < 0) {
            printf("The message has not been sent\n");
            msgctl(msqid, IPC_RMID, (struct msqid_ds *)NULL);
            exit(-1);
        }
        printf("\nThe response has been sent\n");
    }
    return 0;
}
