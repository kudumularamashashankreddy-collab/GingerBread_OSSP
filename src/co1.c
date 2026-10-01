#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include "../include/ulms.h"

void co1_system_call_demo(void)
{
    printf("\n================ CO-1: SYSTEM CALLS ================\n");
    printf("Current PID: %ld\n", (long)getpid());
    printf("User-space program -> system-call interface -> Linux kernel\n");
    printf("System calls used by ULMS include open(), read(), write(), close(), getpid().\n");

    int fd = open("logs/co1_syscall.log", O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (fd == -1) {
        perror("open");
        return;
    }

    const char *msg = "CO-1 system-call demonstration executed.\n";
    if (write(fd, msg, strlen(msg)) == -1)
        perror("write");
    close(fd);

    printf("Successfully used open() -> write() -> close().\n");
}

void co1_view_labs(void)
{
    int fd = open("data/labs.dat", O_RDONLY);
    if (fd == -1) {
        perror("open data/labs.dat");
        return;
    }

    char buffer[1024];
    ssize_t n = read(fd, buffer, sizeof(buffer) - 1);
    if (n == -1) {
        perror("read");
        close(fd);
        return;
    }
    buffer[n] = '\0';

    printf("\n================ LABORATORY DATA ===================\n");
    if (write(STDOUT_FILENO, buffer, (size_t)n) == -1)
        perror("write");
    close(fd);

    printf("=====================================================\n");
    printf("Data path accessed using open/read/write/close.\n");
}

void co1_write_booking_log(const char *message)
{
    int fd = open("logs/booking.log", O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (fd == -1) {
        perror("open booking.log");
        return;
    }

    if (write(fd, message, strlen(message)) == -1)
        perror("write booking.log");
    else
        printf("Booking event appended to logs/booking.log\n");

    close(fd);
}
