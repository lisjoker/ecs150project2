# Target library
lib := libuthread.a
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -pedantic
SRC = queue.c

all: $(lib)

## TODO: Phase 1
# Compile source files and create the static library archive
$(lib): queue.o uthread.o context.o private.o preempt.o
	ar rcs $(lib) queue.o uthread.o context.o private.o preempt.o

# Compile each source file into an object file
%.o: %.c
    $(CC) $(CFLAGS) -c -o $@  $<

private.o: private.h
	$(CC) $(CFLAGS) -c -o $@ $<

# Clean generated files
clean:
    rm -f $(lib)
