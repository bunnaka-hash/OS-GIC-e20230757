
/* shm-producer.c - Writes to POSIX shared memory */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

int main(void) {
    const int SIZE = 4096;
    const char *name = "/OS-khorn-victor";

    const char *message_0 = "Hello, ";
    const char *message_1 = "this is shared memory IPC!";

    int shm_fd;
    void *ptr;

    /* Create or open the shared memory object */
    shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("shm_open");
        return EXIT_FAILURE;
    }

    /* Set the shared memory size */
    if (ftruncate(shm_fd, SIZE) == -1) {
        perror("ftruncate");
        close(shm_fd);
        return EXIT_FAILURE;
    }

    /* Map the shared memory into this process */
    ptr = mmap(NULL, SIZE, PROT_READ | PROT_WRITE,
               MAP_SHARED, shm_fd, 0);
    if (ptr == MAP_FAILED) {
        perror("mmap");
        close(shm_fd);
        return EXIT_FAILURE;
    }

    /* Write the message */
    snprintf((char *)ptr, SIZE, "%s%s", message_0, message_1);

    printf("Producer: wrote to shared memory '%s'\n", name);
    printf("Producer: message = \"%s%s\"\n",
           message_0, message_1);

    /* Release local resources; the shared memory remains available */
    munmap(ptr, SIZE);
    close(shm_fd);

    return EXIT_SUCCESS;
}

