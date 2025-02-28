CC = gcc
CFLAGS = -Wall -Wextra -g  # Enable warnings and debugging
SRCS = engine.c init.c      # Source files
OBJS = $(SRCS:.c=.o)        # Convert .c to .o
TARGET = engine             # Output file

all: $(TARGET)
	./$(TARGET).exe            

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJS)
