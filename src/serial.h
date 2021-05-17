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

#define READ_SERIAL 0x01
#define WRITE_SERIAL 0x02
#define MONITOR_SERIAL 0x04

#define MODE_UNKNOWN -1
#define MODE_ASCII 1
#define MODE_HEX 2
#define MODE_FILE 3

typedef struct serial_dev {
    int fd;
    char dev[PATH_MAX];
    int baud;
    int timeout;
    int in_mode;
    int out_mode;
    unsigned char rw_flag;
} serial_dev;

int init_serial(serial_dev *dev);

int parse_baud(long baud);

int send_and_rec_data(serial_dev *dev, void *buff, int n);

void serial_dev_make_default(serial_dev *dev);

int write_data(serial_dev *dev, void *buff, int n);

int read_data(serial_dev *dev, void *buff, int n);

#endif