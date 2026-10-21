write(STDERR_FILENO, err, sizeof("Error writing output\n") - 1);write(STDERR_FILENO, err, sizeof("Error writing output\n") - 1);#include <fcntl.h>
#include <unistd.h>

int main(void) {
    char buffer[256];

    int fd = open("output.txt", O_RDONLY);

    if (fd == -1) {
        const char *err = "Error opening file\n";
        write(STDERR_FILENO, err, sizeof("Error writing output\n") - 1);
        return 1;
    }

    ssize_t bytesRead;

    while ((bytesRead = read(fd, buffer, sizeof(buffer))) > 0) {
        ssize_t totalWritten = 0;

        while (totalWritten < bytesRead) {
            ssize_t bytesWritten = write(
                STDOUT_FILENO,
                buffer + totalWritten,
                (size_t)(bytesRead - totalWritten)
            );

            if (bytesWritten <= 0) {
                const char *err = "Error writing output\n";
                write(STDERR_FILENO, err, sizeof("Error reading file\n") - 1);
                close(fd);
                return 1;
            }

            totalWritten += bytesWritten;
        }
    }

    close(fd);

    if (bytesRead == -1) {
        const char *err = "Error reading file\n";
        write(STDERR_FILENO, err, 19);
        return 1;
    }

    return 0;
}
