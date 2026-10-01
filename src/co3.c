#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <signal.h>
#include <errno.h>
#include <time.h>
#include "../include/ulms.h"

static volatile sig_atomic_t signal_received = 0;

static void ulms_signal_handler(int sig)
{
    if (sig == SIGUSR1)
        signal_received = 1;
}

void co3_ipc_demo(void)
{
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        perror("pipe");
        return;
    }

    printf("\n================ CO-3: ANONYMOUS PIPE ================\n");

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        close(pipefd[0]);
        close(pipefd[1]);
        return;
    }

    if (pid == 0) {
        close(pipefd[1]);

        char message[MAX_MESSAGE] = {0};
        ssize_t n = read(pipefd[0], message, sizeof(message) - 1);
        if (n > 0)
            printf("[CHILD/LAB MANAGER] Received via pipe: %s\n", message);

        close(pipefd[0]);
        _exit(0);
    }

    close(pipefd[0]);

    const char *request = "BOOK LAB-002 | Student: Yagnesh";
    if (write(pipefd[1], request, strlen(request) + 1) == -1)
        perror("write pipe");

    close(pipefd[1]);
    waitpid(pid, NULL, 0);

    printf("Anonymous pipe communication completed.\n");
}

void co3_fifo_demo(void)
{
    printf("\n================ CO-3: NAMED FIFO ====================\n");

    if (mkfifo(FIFO_PATH, 0666) == -1 && errno != EEXIST) {
        perror("mkfifo");
        return;
    }

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        unlink(FIFO_PATH);
        return;
    }

    if (pid == 0) {
        int fd = open(FIFO_PATH, O_RDONLY);
        if (fd == -1) {
            perror("FIFO child open");
            _exit(1);
        }

        char message[MAX_MESSAGE] = {0};
        ssize_t n = read(fd, message, sizeof(message) - 1);
        if (n > 0)
            printf("[ADMIN PROCESS] FIFO message: %s\n", message);

        close(fd);
        _exit(0);
    }

    sleep(1);

    int fd = open(FIFO_PATH, O_WRONLY);
    if (fd == -1) {
        perror("FIFO parent open");
        waitpid(pid, NULL, 0);
        unlink(FIFO_PATH);
        return;
    }

    const char *message = "Maintenance request: Operating Systems Lab projector";
    if (write(fd, message, strlen(message) + 1) == -1)
        perror("FIFO write");

    close(fd);
    waitpid(pid, NULL, 0);
    unlink(FIFO_PATH);

    printf("Named FIFO created, used, and removed successfully.\n");
}

void co3_signal_demo(void)
{
    printf("\n================ CO-3: SIGNALS ========================\n");

    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = ulms_signal_handler;
    sigemptyset(&sa.sa_mask);

    if (sigaction(SIGUSR1, &sa, NULL) == -1) {
        perror("sigaction");
        return;
    }

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        return;
    }

    if (pid == 0) {
        printf("[SIGNAL CHILD] PID=%ld waiting for SIGUSR1...\n", (long)getpid());
        struct timespec delay = {0, 100000000L};
        for (int i = 0; i < 50 && !signal_received; ++i)
            nanosleep(&delay, NULL);

        if (signal_received)
            printf("[SIGNAL CHILD] SIGUSR1 received: booking notification handled.\n");
        else
            printf("[SIGNAL CHILD] Timed out waiting for signal.\n");

        _exit(signal_received ? 0 : 1);
    }

    sleep(1);

    printf("[PARENT] Sending SIGUSR1 to PID=%ld\n", (long)pid);
    if (kill(pid, SIGUSR1) == -1)
        perror("kill");

    int status;
    waitpid(pid, &status, 0);
    printf("Signal handler demonstration completed.\n");
}
