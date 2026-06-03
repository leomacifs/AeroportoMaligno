CC      = gcc
CFLAGS  = -Wall -Wextra -std=c11 -g
TARGET  = anac
SRCS    = main.c grafo_aeroportos.c matriz_esparsa.c
OBJS    = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

run: all
	./$(TARGET)

.PHONY: all clean run
