#ifndef SERIAL_H

#define SERIAL_H

#include <errno.h>
#include <termios.h>
#include <fcntl.h>

int initialize_serial(char *dev, int baud, int timeout);

int parse_baud(long baud);

#endif