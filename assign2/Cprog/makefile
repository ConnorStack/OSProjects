CC = gcc
CFLAGS = -Wall -g 

all: cprog

clean:
	rm -f *.o cprog

cprog: main.c
	$(CC) $(CFLAGS) main.c -o cprog -lpthread
