
/* shm-consumer.c - Reads from POSIX shared memory */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

int main(void) {
    const int SIZE = 4096;
    const char *name = "/OS-khorn-victor";

    int shm_fd;
    void *ptr;

    /* Open the existing shared memory object */
    shm_fd = shm_open(name, O_RDONLY, 0666);
    if (shm_fd == -1) {
        perror("shm_open");
        fprintf(stderr, "Hint: Did you run shm-producer first?\n");
        return EXIT_FAILURE;
    }

    /* Map shared memory as read-only */
    ptr = mmap(NULL, SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);
    if (ptr == MAP_FAILED) {
        perror("mmap");
        close(shm_fd);
        return EXIT_FAILURE;
    }

    /* Display the message */
    printf("Consumer: reading from shared memory '%s'\n", name);
    printf("Consumer: message = \"%s\"\n", (char *)ptr);

    /* Unmap and close local resources */
    munmap(ptr, SIZE);
    close(shm_fd);

    /* Remove the shared memory name */
    if (shm_unlink(name) == -1) {
        perror("shm_unlink");
        return EXIT_FAILURE;
    }

    printf("Consumer: shared memory unlinked.\n");

    return EXIT_SUCCESS;
}

