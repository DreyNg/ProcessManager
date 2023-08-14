CC = gcc
CFLAGS = -Wall -g
LDFLAGS = -lm

TARGET = allocate
SOURCES = main.c linkedlist.c fileio.c memory.c scheduler.c
OBJECTS = $(SOURCES:.c=.o)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS) $(LDFLAGS)
	
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)

format:
	clang-format -style=file -i $(SOURCES)
