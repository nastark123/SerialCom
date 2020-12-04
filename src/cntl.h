#ifndef CNTL_H

#define CNTL_H

#include "serial.h"

#define NOTHING 0
#define DISCONNECT 1
#define CHDEVICE 2
#define CHBAUD 3

unsigned int parse_cmd(char *cmd);
int disconnect(serial_dev *dev);
int ch_dev(serial_dev *dev, char *cmd);
int ch_baud(serial_dev *dev, char *cmd);

#endif