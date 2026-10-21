#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main(void) {
    const char *text = "Hello from Operating Systems class!\n";
    const char *confirm = "File created successfully!\n";

    int fd = open("output.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);

    if (fd == -1) {
        const char *err = "Error opening file\n";
        write(STDERR_FILENO, err, strlen(err));
        return 1;
    }

    if (write(fd, text, strlen(text)) == -1) {
        const char *err = "Error writing file\n";
        write(STDERR_FILENO, err, strlen(err));
        close(fd);
        return 1;
    }

    close(fd);

    write(STDOUT_FILENO, confirm, strlen(confirm));
    return 0;
}
