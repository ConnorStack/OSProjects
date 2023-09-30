CC = gcc
CFLAGS = -std=c99 -Wall -g -D_POSIX_C_SOURCE=199309L
LDFLAGS = -lpthread -lrt

all: cprog

clean:
	rm -f *.o cprog

cprog: main.c
	$(CC) $(CFLAGS) main.c -o cprog $(LDFLAGS)
