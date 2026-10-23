#include <windows.h>
#include <stdio.h>

int main(void) {
    HANDLE file = CreateFileA(
        "output_win.txt",
        GENERIC_WRITE,
        0,
        NULL,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );

    if (file == INVALID_HANDLE_VALUE) {
        printf("Error creating file: %lu\n", GetLastError());
        return 1;
    }

    const char *message = "Hello from Operating Systems class!\n";
    DWORD bytesWritten;

    if (!WriteFile(file, message, (DWORD)strlen(message),
                   &bytesWritten, NULL)) {
        printf("Error writing file: %lu\n", GetLastError());
        CloseHandle(file);
        return 1;
    }

    CloseHandle(file);
    printf("File created successfully!\n");
    return 0;
}
