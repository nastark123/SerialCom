#ifndef CNTL_H

#define CNTL_H

#include "serial.h"

// array to hold the recognized commands, parse_cmd will return the index of the command passed to it
extern char *cmds[];

// arrays to hold recognized flags that can be passed to the program when it is first started
extern char *flags_no_arg[];
extern char *flags_w_arg[];

// default size for buffer
#define DEFAULT_BUFF_SIZE 256

// buffer for writing to and reading from the serial device
extern int buff_size;

extern char *buff;

// array length definitions for convenience
#define CMD_LENGTH 10
#define FLAGS_NO_ARG_LENGTH 6
#define FLAGS_W_ARG_LENGTH 6

#define NOTHING 0
#define DISCONNECT 1
#define CHDEVICE 2
#define CHBAUD 3
#define LSCMD 4
#define LSBD 5
#define LSMODE 6
#define IMODE 7
#define OMODE 8
#define CHREAD 9
#define CHWRITE 10

#define HELP 1
#define READ 2
#define WRITE 3
#define NO_READ 4
#define NO_WRITE 5
#define MONITOR 6

#define DEVICE 1
#define BAUD 2
#define OUT_MODE 3
#define IN_MODE 4
#define TIMEOUT 5
#define BUFFER_SIZE 6

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

int parse_hex(char *str);

void print_help();

int parse_flags(serial_dev *dev, char **args, int len);

#endif