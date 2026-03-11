CC = gcc
CFLAGS = -Wall -g

szsh: main.c
	$(CC) $(CFLAGS) main.c -o szsh

.PHONY: clean
clean:
	rm -f szsh
