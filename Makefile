CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g
TARGET = ulms

SRC = src/main.c src/co1.c src/co2.c src/co3.c src/co4.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

run: $(TARGET)
	./$(TARGET)

debug: $(TARGET)
	gdb ./$(TARGET)

valgrind: $(TARGET)
	valgrind --leak-check=full --show-leak-kinds=all ./$(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: all run debug valgrind clean
