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

# Dependencies
queue.o: queue.c queue.h
uthread.o: uthread.c uthread.h context.h queue.h private.h preempt.h sem.h
context.o: context.c context.h
private.o: private.c private.h
sem.o: sem.c sem.h queue.h private.h uthread.h
preempt.o: preempt.c preempt.h uthread.h context.h queue.h private.h sem.h

clean:
	rm -f $(lib) $(objs)
