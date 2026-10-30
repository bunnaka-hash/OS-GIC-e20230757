
/* sender.c - Sends a message through a POSIX message queue */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mqueue.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include "common.h"

int main(void) {
    mqd_t mq;
    struct mq_attr attr = {0};

    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = MAX_SIZE;
    attr.mq_curmsgs = 0;

    /* Create or open the message queue */
    mq = mq_open(QUEUE_NAME, O_CREAT | O_WRONLY, 0644, &attr);
    if (mq == (mqd_t)-1) {
        perror("mq_open");
        return EXIT_FAILURE;
    }

    const char *message =
        "Hello from sender! This is message queue IPC.";

    /* Send the message, including its null terminator */
    if (mq_send(mq, message, strlen(message) + 1, 0) == -1) {
        perror("mq_send");
        mq_close(mq);
        return EXIT_FAILURE;
    }

    printf("Sender: message sent to queue '%s'\n", QUEUE_NAME);
    printf("Sender: message = \"%s\"\n", message);

    if (mq_close(mq) == -1) {
        perror("mq_close");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

