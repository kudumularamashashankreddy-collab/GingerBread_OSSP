#ifndef ULMS_H
#define ULMS_H

#include <sys/types.h>

#define FIFO_PATH "ipc/lab_admin_fifo"
#define MAX_MESSAGE 256

void co1_system_call_demo(void);
void co1_view_labs(void);
void co1_write_booking_log(const char *message);

void co2_process_demo(void);
void co2_booking_workflow(void);
void co2_show_process_info(void);

void co3_ipc_demo(void);
void co3_fifo_demo(void);
void co3_signal_demo(void);

void co4_memory_demo(void);
void co4_virtual_memory_demo(void);
void co4_page_fault_demo(void);
void co4_address_space_demo(void);

void booking_worker(const char *lab_name, const char *student_name);

void print_banner(void);
void print_main_menu(void);
int read_int(const char *prompt);

#endif
