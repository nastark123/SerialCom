CC=gcc
CFLAGS=-I.

build: serial.c main.c
	$(CC) $(CFLAGS) serial.c main.c -o serial

clean:
	rm serial

rebuild:
	make clean
	make build