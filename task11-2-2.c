#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define LAST_MESSAGE 255 // Message type for termination of program task11-2-2.c

int main(void) {
    // IPC descriptor for the queue
    int msqid;
    key_t key; // IPC-key
    char pathname[] = "task11-2-1.c"; // filename
    // Message structure
    struct mymsgbuf {
        long mtype;
        struct {
            char text[23];
            int num;
        } info;
    } mybuf;
    int message_length, max_length;
    // Generate IPC-ключ
    if ((key = ftok(pathname, 0)) < 0) {
        printf("Can\'t generate key\n");
        exit(-1);
    }
    // Get or create a queue
    if ((msqid = msgget(key, 0666 | IPC_CREAT)) < 0) {
        printf("Can\'t get msqid\n");
        exit(-1);
    }
    // Read messages in FIFO
    printf("\'task02-2.c\' started receiving messages\n");
    for (int i = 0; i < 5; i++) {
        max_length = sizeof(mybuf.info);
        // Try to get a message from queue
        if ((message_length = msgrcv(msqid, (struct msgbuf *) &mybuf, max_length, 0, 0)) < 0) {
            printf("The message has not been received\n");
            exit(-1);
        }
        printf("message type = %ld, info = [%d ; \'%s\']\n", mybuf.mtype, mybuf.info.num, mybuf.info.text);
    }
    printf("\'task02-2.c\' finished sending messages\n");
    printf("---\n");
    printf("\'task02-2.c\' start sending messages\n");
    for (int i = 0; i < 5; i++) {
        // Message information
        mybuf.mtype = 2;
        mybuf.info.num = i;
        strcpy(mybuf.info.text, "Message from script 2.");
        message_length = sizeof(mybuf.info);
        // Send message to the queue
        if (msgsnd(msqid, (struct msgbuf *) &mybuf, message_length, 0) < 0) {
            printf("The message has not been sent to the queue.\n");
            msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
            exit(-1);
        }
        printf("Message %d was sent with info = [%d ; \'%s\']\n", i, mybuf.info.num, mybuf.info.text);
    }
    printf("\'task02-1.c\' finished sending messages\n");
    return 0;
}
