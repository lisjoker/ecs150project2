# Target library
objs := queue.o uthread.o context.o private.o sem.o preempt.o
lib := libuthread.a
CC := gcc
CFLAGS := -Wall -Werror

all: $(lib)

$(lib): $(objs)
	ar rcs $(lib) $(objs)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

private.o: private.h
	$(CC) $(CFLAGS) -c -o $@ $<


clean:
	rm -f *~ $(lib) $(objs)
