#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "../include/ulms.h"

static void run_menu(void)
{
    int choice;

    while (1) {
        print_main_menu();
        choice = read_int("Enter your choice: ");

        switch (choice) {
            case 1:
                co1_system_call_demo();
                break;
            case 2:
                co1_view_labs();
                break;
            case 3:
                co1_write_booking_log("ULMS: Booking log created from main menu.\n");
                break;
            case 4:
                co2_process_demo();
                break;
            case 5:
                co2_booking_workflow();
                break;
            case 6:
                co2_show_process_info();
                break;
            case 7:
                co3_ipc_demo();
                break;
            case 8:
                co3_fifo_demo();
                break;
            case 9:
                co3_signal_demo();
                break;
            case 10:
                co4_memory_demo();
                break;
            case 11:
                co4_virtual_memory_demo();
                break;
            case 12:
                co4_page_fault_demo();
                break;
            case 13:
                co4_address_space_demo();
                break;
            case 0:
                printf("\nExiting ULMS. Goodbye!\n");
                return;
            default:
                printf("Invalid choice.\n");
        }
    }
}

int main(int argc, char *argv[])
{
    if (argc >= 2 && strcmp(argv[1], "booking-worker") == 0) {
        const char *lab = argc >= 3 ? argv[2] : "Unknown Lab";
        const char *student = argc >= 4 ? argv[3] : "Unknown Student";
        booking_worker(lab, student);
        return 0;
    }

    if (argc >= 2 && strcmp(argv[1], "ipc-worker") == 0) {
        /* Reserved worker entry point; IPC demos manage their own children. */
        return 0;
    }

    print_banner();
    run_menu();
    return 0;
}

void print_banner(void)
{
    printf("\n============================================================\n");
    printf("       UNIVERSITY LABORATORY MANAGEMENT SYSTEM\n");
    printf("============================================================\n");
    printf(" OS Project: CO-1 + CO-2 + CO-3 + CO-4\n");
    printf(" Linux / WSL2 / Ubuntu / C\n");
    printf("============================================================\n");
}

void print_main_menu(void)
{
    printf("\n---------------------- ULMS MENU ---------------------------\n");
    printf("CO-1  1. System-call demonstration\n");
    printf("CO-1  2. View laboratory data\n");
    printf("CO-1  3. Write booking log\n");
    printf("CO-2  4. Process creation & lifecycle\n");
    printf("CO-2  5. Laboratory booking process (fork + exec + wait)\n");
    printf("CO-2  6. Current process information\n");
    printf("CO-3  7. Anonymous pipe IPC\n");
    printf("CO-3  8. Named FIFO IPC\n");
    printf("CO-3  9. Signals & signal handler\n");
    printf("CO-4 10. Dynamic memory management\n");
    printf("CO-4 11. Virtual memory with mmap()\n");
    printf("CO-4 12. Page-fault analysis\n");
    printf("CO-4 13. Address-space analysis (/proc/self/maps)\n");
    printf("  0. Exit\n");
    printf("------------------------------------------------------------\n");
}

int read_int(const char *prompt)
{
    char buffer[64];
    char *end;
    long value;

    printf("%s", prompt);
    if (!fgets(buffer, sizeof(buffer), stdin))
        return -1;

    value = strtol(buffer, &end, 10);
    if (end == buffer)
        return -1;

    return (int)value;
}
