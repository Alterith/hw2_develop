SRC=mm_alloc.c main.c
EXEC=hw2

CC=gcc
CFLAGS=-g -Wall -std=gnu99
LIBS = -lm
LDFLAGS=

OBJS=$(SRC:.c=.o)

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) -o $@ $(LIBS)

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@ $(LIBS)

clean:
	rm -rf $(EXEC) $(OBJS)
