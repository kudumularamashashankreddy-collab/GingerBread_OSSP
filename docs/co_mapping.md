# CO Mapping

## CO-1
System calls: open, read, write, close, getpid. The application runs in user space and requests kernel services through system-call interfaces.

## CO-2
fork creates child processes. execv replaces the child image with the booking-worker program. waitpid synchronizes parent and child. setpgid/getpgid demonstrate process groups.

## CO-3
Anonymous pipe provides parent-child communication. mkfifo/open/read/write demonstrate a named FIFO. SIGUSR1 and sigaction/kill demonstrate asynchronous signaling and signal handling.

## CO-4
calloc/realloc/free demonstrate dynamic allocation. mmap/munmap demonstrate virtual-memory mappings. /proc/self/maps shows the address-space layout. /proc/self/stat is used to observe page-fault counters.
