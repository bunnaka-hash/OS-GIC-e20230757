/*
 * winprocess.c — Process creation on Windows using CreateProcess()
 *
 * Demonstrates how to create, inspect, and wait for a child process
 * using the Windows Win32 API.
 */

#include <stdio.h>
#include <windows.h>

int main(void) {
    /*
     * STARTUPINFO (si):
     * Specifies the window station, desktop, standard handles, and
     * appearance of the main window for a process at creation time.
     */
    STARTUPINFO si;

    /*
     * PROCESS_INFORMATION (pi):
     * Receives identification information about the new process and
     * its primary thread (e.g., process handle, thread handle, PID, TID).
     */
    PROCESS_INFORMATION pi;

    /*
     * ZeroMemory ensures structures contain no garbage memory.
     * Setting si.cb to sizeof(si) is required by the Windows API
     * so it knows the size/version of the structure being passed.
     */
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    /* Print the current running process's unique process identifier (PID) */
    printf("Parent process (PID: %lu) — creating child...\n", GetCurrentProcessId());

    /*
     * Note: lpCommandLine must be in a writable buffer because
     * CreateProcess() can modify this string internally.
     */
    char cmd[] = "mspaint.exe";

    if (!CreateProcess(
            NULL,           /* Let Windows search the PATH */
            cmd,            /* Executable / command line */
            NULL,           /* Process security attributes */
            NULL,           /* Thread security attributes */
            FALSE,          /* Inherit handles */
            0,              /* Creation flags */
            NULL,           /* Environment */
            NULL,           /* Current directory */
            &si,            /* Startup info */
            &pi             /* Process information */
        ))
    {
        fprintf(stderr, "CreateProcess failed (error %lu).\n", GetLastError());
        return 1;
    }

    /* Process spawned successfully — print child process IDs */
    printf("Child process created!\n");
    printf("  Child PID:       %lu\n", pi.dwProcessId);
    printf("  Child Thread ID: %lu\n", pi.dwThreadId);

    printf("Parent: waiting for child (mspaint) to exit...\n");
    printf(">>> Open Task Manager now and take a screenshot! <<<\n");
    printf(">>> Then close mspaint to let this program finish. <<<\n");

    /*
     * WaitForSingleObject:
     * Blocks execution of the parent process until the child process terminates.
     * A process handle becomes "signaled" when the process exits.
     * INFINITE specifies that the parent will wait indefinitely without timing out.
     */
    WaitForSingleObject(pi.hProcess, INFINITE);

    printf("Parent: child has exited. Cleaning up.\n");

    /*
     * CloseHandle:
     * Releases the parent's open references to the child's process and thread kernel objects.
     * Failure to close these handles causes a kernel handle leak.
     */
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return 0;
}