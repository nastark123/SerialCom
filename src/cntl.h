#ifndef CNTL_H

#define CNTL_H

#include "serial.h"

#define NOTHING 0
#define DISCONNECT 1
#define CHDEVICE 2
#define CHBAUD 3
#define LSCMD 4
#define LSBD 5
#define LSMODE 6
#define CHMODE 7

#define MODE_ASCII 0
#define MODE_HEX 1
#define MODE_FILE 2

unsigned int parse_cmd(char *cmd);
int disconnect(serial_dev *dev);
int ch_dev(serial_dev *dev, char *cmd);
int ch_baud(serial_dev *dev, char *cmd);
void ls_cmd();
void ls_bd();
void ls_mode();
int ch_mode(char *cmd);
int parse_hex(char *str);

#endif