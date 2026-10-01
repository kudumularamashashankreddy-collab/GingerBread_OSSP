#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <stdint.h>
#include "../include/ulms.h"

static int read_fault_counts(unsigned long *minor_faults,
                             unsigned long *major_faults)
{
    FILE *fp = fopen("/proc/self/stat", "r");
    if (!fp) return -1;

    char comm[256];
    char state;
    unsigned long dummy;
    unsigned long minflt = 0, cminflt = 0, majflt = 0, cmajflt = 0;

    /*
     * /proc/self/stat has "(comm)" as field 2, so read until ')' first.
     */
    if (fscanf(fp, "%*d (%255[^)]) %c", comm, &state) != 2) {
        fclose(fp);
        return -1;
    }

    /* Fields 4..9 */
    for (int i = 0; i < 6; ++i) {
        if (fscanf(fp, "%lu", &dummy) != 1) {
            fclose(fp);
            return -1;
        }
    }

    /* Fields 10..13: minflt, cminflt, majflt, cmajflt */
    if (fscanf(fp, "%lu %lu %lu %lu",
               &minflt, &cminflt, &majflt, &cmajflt) != 4) {
        fclose(fp);
        return -1;
    }

    fclose(fp);
    *minor_faults = minflt;
    *major_faults = majflt;
    return 0;
}

void co4_memory_demo(void)
{
    printf("\n================ CO-4: DYNAMIC MEMORY ================\n");

    size_t count = 10;
    int *equipment = calloc(count, sizeof(int));
    if (!equipment) {
        perror("calloc");
        return;
    }

    for (size_t i = 0; i < count; ++i)
        equipment[i] = (int)(1000 + i);

    printf("calloc(): allocated %zu equipment records.\n", count);

    int *resized = realloc(equipment, 20 * sizeof(int));
    if (!resized) {
        perror("realloc");
        free(equipment);
        return;
    }

    equipment = resized;
    for (size_t i = count; i < 20; ++i)
        equipment[i] = (int)(1000 + i);

    printf("realloc(): expanded allocation from 10 to 20 records.\n");
    printf("Sample equipment IDs: %d %d %d\n",
           equipment[0], equipment[10], equipment[19]);

    free(equipment);
    printf("free(): dynamic memory released successfully.\n");
}

void co4_virtual_memory_demo(void)
{
    printf("\n================ CO-4: VIRTUAL MEMORY ================\n");

    const size_t size = 16 * 1024 * 1024;
    char *region = mmap(NULL, size,
                        PROT_READ | PROT_WRITE,
                        MAP_PRIVATE | MAP_ANONYMOUS,
                        -1, 0);

    if (region == MAP_FAILED) {
        perror("mmap");
        return;
    }

    printf("mmap(): mapped %zu MB of virtual memory.\n", size / (1024 * 1024));

    size_t page = (size_t)sysconf(_SC_PAGESIZE);
    printf("System page size: %zu bytes\n", page);

    for (size_t i = 0; i < size; i += page)
        region[i] = (char)(i / page);

    printf("Touched one byte per page to make the mapping active.\n");

    if (munmap(region, size) == -1)
        perror("munmap");
    else
        printf("munmap(): virtual-memory mapping released.\n");
}

void co4_page_fault_demo(void)
{
    printf("\n================ CO-4: PAGE FAULT ANALYSIS ============\n");

    unsigned long before_minor, before_major;
    unsigned long after_minor, after_major;

    if (read_fault_counts(&before_minor, &before_major) == -1) {
        perror("read /proc/self/stat");
        return;
    }

    const size_t size = 32 * 1024 * 1024;
    char *region = mmap(NULL, size,
                        PROT_READ | PROT_WRITE,
                        MAP_PRIVATE | MAP_ANONYMOUS,
                        -1, 0);

    if (region == MAP_FAILED) {
        perror("mmap");
        return;
    }

    size_t page = (size_t)sysconf(_SC_PAGESIZE);
    volatile unsigned long checksum = 0;

    for (size_t i = 0; i < size; i += page) {
        region[i] = 1;
        checksum += (unsigned char)region[i];
    }

    if (read_fault_counts(&after_minor, &after_major) == -1) {
        perror("read /proc/self/stat");
        munmap(region, size);
        return;
    }

    printf("Minor faults before: %lu\n", before_minor);
    printf("Minor faults after : %lu\n", after_minor);
    printf("Major faults before: %lu\n", before_major);
    printf("Major faults after : %lu\n", after_major);
    printf("Checksum: %lu\n", checksum);
    printf("The increase in page-fault counters shows virtual-memory pages being populated.\n");

    munmap(region, size);
}

void co4_address_space_demo(void)
{
    printf("\n================ CO-4: ADDRESS SPACE ================\n");

    printf("Reading /proc/self/maps for the running ULMS process...\n\n");

    FILE *fp = fopen("/proc/self/maps", "r");
    if (!fp) {
        perror("fopen /proc/self/maps");
        return;
    }

    char line[512];
    while (fgets(line, sizeof(line), fp))
        fputs(line, stdout);

    fclose(fp);

    printf("\nTypical regions include executable code, data, heap, shared libraries,\n");
    printf("mmap areas and the stack. These are virtual-address mappings.\n");
}
