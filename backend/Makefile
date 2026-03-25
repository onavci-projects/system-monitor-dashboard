CC = gcc
CFLAGS = -Wall -Wextra -g
LDFLAGS = -lncurses

TARGET = sistem_monitoru
OBJS = main.o memory.o cpu.o disk.o network.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS)

%.o: %.c monitor.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean