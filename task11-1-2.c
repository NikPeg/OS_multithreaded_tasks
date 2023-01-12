	#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define LAST_MESSAGE 255

int main(void) {
    int msqid; // IPC descriptor for the message queue
    key_t key; // IPC-key
    char pathname[] = "task11-1-1.c"; // filename
    // Message structure
    struct mymsgbuf {
        long mtype;
        struct {
            short sinfo;
            float finfo;
        } info;
    } mybuf;
    int message_length, max_length;
    // IPC-key generating
    if ((key = ftok(pathname, 0)) < 0) {
        printf("Can\'t generate key\n");
        exit(-1);
    }
    // Get or create messages queue
    if ((msqid = msgget(key, 0666 | IPC_CREAT)) < 0) {
        printf("Can\'t get msqid\n");
        exit(-1);
    }
    // Read messages in FIFO
    while (1) {
        max_length = sizeof(mybuf.info);
        // Try to read an information from queue
        if ((message_length = msgrcv(msqid, (struct msgbuf *) &mybuf, max_length, 0, 0)) < 0) {
            printf("The message has not been receivied from the queue.\n");
            exit(-1);
        }
        // Stop reading
        if (mybuf.mtype == LAST_MESSAGE) {
            msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
            exit(0);
        }
        printf("%ld message, information = [%d ; %f]\n", mybuf.mtype, mybuf.info.sinfo, mybuf.info.finfo);
    }
    return 0;
}
