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
#define IMODE 7
#define OMODE 8

#define MODE_UNKNOWN -1
#define MODE_ASCII 1
#define MODE_HEX 2
#define MODE_FILE 3

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

int display_output(char *out, int len, int out_mode);
unsigned int parse_cmd(char *cmd);
int remove_cmd(char *cmd, int n);
int disconnect(serial_dev *dev);
int ch_dev(serial_dev *dev, char *cmd);
int ch_baud(serial_dev *dev, char *cmd);
void ls_cmd();
void ls_bd();
void ls_mode();
int ch_mode(char *cmd);
int init_mode(char *cmd);
int parse_hex(char *str);
void print_help();
int parse_opts(serial_dev *dev, char **args, int len);

#endif