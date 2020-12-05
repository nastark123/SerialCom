#ifndef SERIAL_H

#define SERIAL_H

#include <errno.h>
#include <termios.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

typedef struct serial_dev {
    int fd;
    char *dev;
    int baud;
    int timeout;
} serial_dev;

int initialize_serial(serial_dev *dev);

int parse_baud(long baud);

int send_and_rec_data(serial_dev *dev, void *buff, int n);

#endif