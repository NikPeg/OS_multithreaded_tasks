#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
    int msqid; // IPC descriptor
    pid_t getpid(void);
    key_t key; // IPC-key
    char pathname[] = "task11-3-1.c"; // filename
    int message_lenth, max_length;
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
    // Messages information
    mybuf.mtype = 1;
    mybuf.info.pid = getpid();
    // Read the number
    printf("Enter float number: ");
    scanf("%f", &(mybuf.info.finfo));
    message_lenth = sizeof(mybuf.info);
    // Send the message to the queue
    if (msgsnd(msqid, (struct msgbuf *) &mybuf, message_lenth, 0) < 0) {
        printf("Can\'t send message to queue\n");
        msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
        exit(-1);
    }
    printf("Message from %d was sent.\n", getpid());
    max_length = sizeof(myservbuf.finfo);
    // Try to read from the queue
    if ((message_lenth = msgrcv(msqid, &myservbuf, max_length, getpid(), 0)) < 0) {
        printf("Can't receive message from queue\n");
        exit(-1);
    }
    printf("Response:\t%f\n", myservbuf.finfo);
    return 0;
}
