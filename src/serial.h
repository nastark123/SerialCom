#ifndef SERIAL_H

#define SERIAL_H

#include <errno.h>
#include <termios.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>

typedef struct serial_dev {
    int fd;
    char dev[PATH_MAX];
    int baud;
    int timeout;
    int in_mode;
    int out_mode;
} serial_dev;

int init_serial(serial_dev *dev);

int parse_baud(long baud);

int send_and_rec_data(serial_dev *dev, void *buff, int n);

#endif