CC=gcc
CFLAGS=-I.

build: serial.c main.c cntl.c
	$(CC) $(CFLAGS) serial.c cntl.c main.c -o serial

clean:
	rm serial

rebuild:
	make clean
	make build