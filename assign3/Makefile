SRCS = main.c readyQueue.c IOQueue.c
OBJS = $(SRCS:.c=.o)

EXECUTABLE = proc

CC = gcc
CFLAGS = -g -Wall  

$(EXECUTABLE): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(EXECUTABLE)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

main.o: readyQueue.h IOQueue.h
readyQueue.o: readyQueue.h pcb.h
IOQueue.o: IOQueue.h pcb.h

clean:
	rm -f $(OBJS) $(EXECUTABLE)

.PHONY: clean


SRCS = main.c readyQueue.c IOQueue.c
OBJS = $(SRCS:.c=.o)

EXECUTABLE = proc

CC = gcc
CFLAGS = -std=c99 -Wall -g -D_POSIX_C_SOURCE=199309L -lpthread -lrt

$(EXECUTABLE): $(OBJS)
	$(CC) $(OBJS) -o $(EXECUTABLE)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

main.o: readyQueue.h IOQueue.h PCB.h
readyQueue.o: readyQueue.h PCB.h
IOQueue.o: IOQueue.h PCB.h

.PHONY: clean PCB.h

clean:
	rm -f $(OBJS) $(EXECUTABLE)