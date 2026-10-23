#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdio.h>

int main(void) {
    char buffer[512];

    DIR *dir = opendir(".");

    if (dir == NULL) {
        const char *err = "Error opening directory\n";
        write(STDERR_FILENO, err, sizeof("Error opening directory\n") - 1);
        return 1;
    }

    int len = snprintf(buffer, sizeof(buffer),
                       "%-30s %10s\n", "Filename", "Size (bytes)");
    if (len > 0 && (size_t)len < sizeof(buffer)) {
        write(STDOUT_FILENO, buffer, (size_t)len);
    }

    len = snprintf(buffer, sizeof(buffer),
                   "%-30s %10s\n",
                   "------------------------------", "----------");
    if (len > 0 && (size_t)len < sizeof(buffer)) {
        write(STDOUT_FILENO, buffer, (size_t)len);
    }

    struct dirent *entry;
    struct stat fileStat;

    while ((entry = readdir(dir)) != NULL) {
        if (stat(entry->d_name, &fileStat) == 0) {
            len = snprintf(buffer, sizeof(buffer), "%-30s %10ld\n",
                           entry->d_name, (long)fileStat.st_size);
            if (len > 0 && (size_t)len < sizeof(buffer)) {
                write(STDOUT_FILENO, buffer, (size_t)len);
            }
        }
    }

    closedir(dir);
    return 0;
}
