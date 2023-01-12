#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define LAST_MESSAGE 255 // Message type for termination of program task11-1-2.c

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
    int message_length;
    // IPC-key generating
    if ((key = ftok(pathname, 0)) < 0) {
        printf("The key has not been generated.\n");
        exit(-1);
    }
    // Get or create messages queue
    if ((msqid = msgget(key, 0666 | IPC_CREAT)) < 0) {
        printf("Messages queue's id has not been get.\n");
        exit(-1);
    }
    // Send message
    for (int i = 0; i < 5; i++) {
        // Message information
        mybuf.mtype = 1;
        mybuf.info.sinfo = i;
        mybuf.info.finfo = i * 1000;
        // Find message length
        message_length = sizeof(mybuf.info);
        // Send message to the queue
        if (msgsnd(msqid, (struct msgbuf *) &mybuf, message_length, 0) < 0) {
            printf("The message has not been sent to the queue.\n");
            msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
            exit(-1);
        }
        printf("%d message was sent with information [%d ; %f]\n", i, mybuf.info.sinfo, mybuf.info.finfo);
    }
    // Send last message
    mybuf.mtype = LAST_MESSAGE;
    message_length = 0;
    if (msgsnd(msqid, (struct msgbuf *) &mybuf, message_length, 0) < 0) {
        printf("The message has not been sent to the queue.\n");
        msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
        exit(-1);
    }
    return 0;
}
