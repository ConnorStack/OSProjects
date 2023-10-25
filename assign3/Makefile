# Define the source files and their corresponding object files
SRCS = main.c readyQueue.c IOQueue.c
OBJS = $(SRCS:.c=.o)

# Define the executable name
EXECUTABLE = proc

CC = gcc
CFLAGS = -g -Wall  # Add the -g flag for debugging information

# Build the executable
$(EXECUTABLE): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(EXECUTABLE)

# Compile source files into object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Dependencies for the object files
main.o: readyQueue.h IOQueue.h
readyQueue.o: readyQueue.h pcb.h
IOQueue.o: IOQueue.h pcb.h

# Clean up generated files
clean:
	rm -f $(OBJS) $(EXECUTABLE)

.PHONY: clean