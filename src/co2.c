#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>
#include <errno.h>
#include "../include/ulms.h"

void co2_process_demo(void)
{
    printf("\n================ CO-2: PROCESS LIFECYCLE ================\n");
    printf("Parent PID before fork: %ld\n", (long)getpid());

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        return;
    }

    if (pid == 0) {
        printf("[CHILD] PID=%ld, PPID=%ld\n", (long)getpid(), (long)getppid());
        printf("[CHILD] State: Running -> Terminating\n");
        _exit(0);
    }

    printf("[PARENT] Created child PID=%ld\n", (long)pid);
    printf("[PARENT] Waiting for child using waitpid()...\n");

    int status;
    if (waitpid(pid, &status, 0) == -1) {
        perror("waitpid");
        return;
    }

    if (WIFEXITED(status))
        printf("[PARENT] Child terminated with exit code %d.\n", WEXITSTATUS(status));
    else
        printf("[PARENT] Child ended abnormally.\n");

    printf("Synchronization: parent waited for child completion.\n");
}

void co2_booking_workflow(void)
{
    char lab[128];
    char student[128];

    printf("\n================ CO-2: BOOKING PROCESS ================\n");
    printf("Enter laboratory name: ");
    if (!fgets(lab, sizeof(lab), stdin)) return;
    lab[strcspn(lab, "\n")] = '\0';

    printf("Enter student name: ");
    if (!fgets(student, sizeof(student), stdin)) return;
    student[strcspn(student, "\n")] = '\0';

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        return;
    }

    if (pid == 0) {
        /*
         * Child becomes a separate process group leader, demonstrating
         * process-group creation before exec().
         */
        if (setpgid(0, 0) == -1)
            perror("setpgid");

        char *const args[] = {
            "./ulms", "booking-worker", lab, student, NULL
        };

        printf("[BOOKING CHILD] PID=%ld PGID=%ld\n",
               (long)getpid(), (long)getpgrp());

        execv(args[0], args);
        perror("execv");
        _exit(127);
    }

    printf("[PARENT] Booking process PID=%ld created.\n", (long)pid);
    sleep(1);

    pid_t pgid = getpgid(pid);
    if (pgid != -1)
        printf("[PARENT] Child process group ID: %ld\n", (long)pgid);

    int status;
    if (waitpid(pid, &status, 0) == -1) {
        perror("waitpid");
        return;
    }

    if (WIFEXITED(status))
        printf("[PARENT] Booking worker completed with status %d.\n",
               WEXITSTATUS(status));
}

void co2_show_process_info(void)
{
    printf("\n================ CO-2: PROCESS INFO =================\n");
    printf("PID  : %ld\n", (long)getpid());
    printf("PPID : %ld\n", (long)getppid());
    printf("PGID : %ld\n", (long)getpgrp());
    printf("This process is currently executing in the ULMS shell session.\n");
}

void booking_worker(const char *lab_name, const char *student_name)
{
    printf("\n[BOOKING WORKER]\n");
    printf("PID: %ld\n", (long)getpid());
    printf("PPID: %ld\n", (long)getppid());
    printf("PGID: %ld\n", (long)getpgrp());
    printf("Student: %s\n", student_name);
    printf("Laboratory: %s\n", lab_name);
    printf("Booking state: PROCESSING -> APPROVED\n");

    FILE *fp = fopen("logs/booking.log", "a");
    if (fp) {
        fprintf(fp, "Student=%s | Lab=%s | Status=APPROVED | PID=%ld\n",
                student_name, lab_name, (long)getpid());
        fclose(fp);
    } else {
        perror("fopen booking.log");
    }

    sleep(1);
    printf("[BOOKING WORKER] Completed.\n");
}
