# University Laboratory Management System

OS subject project implemented in C on Ubuntu/WSL2.

## CO mapping

- CO-1: Linux system calls, user/kernel service model, shell execution
- CO-2: process creation, exec, waitpid, process groups and lifecycle
- CO-3: anonymous pipes, named FIFO, signals and signal handlers
- CO-4: malloc/calloc/realloc/free, mmap/munmap, /proc maps and page-fault counters

## Build

```bash
make
```

## Run

```bash
./ulms
```

## Debug

```bash
make debug
```

## Memory analysis

```bash
make valgrind
```

## Useful Linux commands

```bash
ps -ef
pstree -p
cat /proc/<PID>/status
cat /proc/<PID>/maps
cat /proc/<PID>/stat
strace ./ulms
```

## Note

The program is an OS-focused terminal application. It intentionally demonstrates operating-system mechanisms as part of its final workflow.
