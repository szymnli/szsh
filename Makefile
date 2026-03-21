CC = gcc
CFLAGS = -Wall -g

SRCS = main.c parse.c execute.c builtin.c
OBJS = $(SRCS:.c=.o)

szsh: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o szsh

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -f szsh $(OBJS)
