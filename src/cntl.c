#include "cntl.h"

unsigned int parse_cmd(char *cmd) {

    if(!strncmp(cmd, "!dc", 3)) { // disconnect from the device
        return DISCONNECT;
    } else if(!strncmp(cmd, "!chdev ", 7)) { // change the device we're connected to
        return CHDEVICE;
    } else if(!strncmp(cmd, "!chbd ", 6)) { // change the baudrate and reconnect
        return CHBAUD;
    } else if(!strncmp(cmd, "!lscmd ", 7)) { // change the device we're connected to
        return LSCMD;
    } else if(!strncmp(cmd, "!lsbd ", 6)) { // change the baudrate and reconnect
        return LSBD;
    } else if(!strncmp(cmd, "!lsmode ", 8)) { // change the device we're connected to
        return LSMODE;
    } else if(!strncmp(cmd, "!chmode ", 8)) { // change the baudrate and reconnect
        return CHMODE;
    }

    return NOTHING;
}

int disconnect(serial_dev *dev) {
    int r = close(dev->fd);

    dev->fd = -1;

    return r;
}

int ch_dev(serial_dev *dev, char *cmd) {
    int r = disconnect(dev);
    if (r < 0) return -1;
    dev->dev = realloc(dev->dev, strlen(cmd) - 8);

    // we go up to strlen - 1 to avoid getting the newline character, which can cause issues
    for(int i = 7; i < strlen(cmd) - 1; i++) {
        dev->dev[i - 7] = cmd[i];
    }

    printf("\nConnecting to %s\n", dev->dev);

    dev->fd = initialize_serial(dev);
    if(dev->fd < 0) return -1;

    return 0;
}

int ch_baud(serial_dev *dev, char *cmd) {
    int r = disconnect(dev);
    if (r < 0) return -1;

    char *temp = malloc(strlen(cmd) - 7);
    // we go up to strlen - 1 to avoid getting the newline character, which can cause issues
    for(int i = 6; i < strlen(cmd) - 1; i++) {
        temp[i - 6] = cmd[i];
    }
    dev->baud = strtol(temp, NULL, 10);

    printf("\nSetting baudrate to %d...\n", dev->baud);

    dev->baud = parse_baud(dev->baud);

    free(temp);

    dev->fd = initialize_serial(dev);

    if(dev->fd < 0) return -1;

    return 0;
}

void ls_cmd(){
    printf("\nWelcome to SerialCom!  All commands are preceded by a \"!\" and are as follows:\n");
    printf("dc - disconnect from the current serial device\n");
    printf("chdev - change the device you are currently connected to\n");
    printf("chbd - change the baud rate of the serial connection\n");
    printf("chmode - change the mode to one of the supported modes\n");
    printf("lscmd - list all commands\n");
    printf("lsbd - list all the supported baud rates compiled into this executable\n");
    printf("lsmode - lists all supported modes (ascii, hex, etc.)\n");
}

void ls_bd() {
    printf("\nThe supported baud rates in this executable are:\n");
    printf("50, 75, 110, 134.5, 150, 200, 300, 600, 1200\n");
    printf("1800, 2400, 4800, 9600, 19200, 38400\n");
    #ifdef __USE_MISC
    printf("\nExtended baud rates available are:\n");
    printf("57600, 115200, 230400, 460800, 500000\n");
    printf("576000, 921600, 1000000, 1152000\n");
    printf("1500000, 2000000, 2500000, 3000000\n");
    printf("3500000, 4000000\n");
    #endif
}

void ls_mode() {
    printf("\nCurrently supported modes:\n");
    printf("ascii - interpret characters as ascii (default)\n");
    printf("hex - translate characters as space delimited hexadecimal\n");
}

int ch_mode(char *cmd) {
    char *mode = malloc(strlen(cmd) - 7);

    for(int i = 8; i < strlen(cmd); i++) {
        mode[i - 8] = cmd[i];
    }

    if(!strncmp(mode, "ascii", 5)) {
        free(mode);
        printf("\nNow in ASCII mode\n");
        return MODE_ASCII;
    } else if(!strncmp(mode, "hex", 3)) {
        free(mode);
        printf("\nNow in HEX mode\n");
        return MODE_HEX;
    }
}


// converts the characters in str to hex byte values and stores them in str
// returns the number of bytes that were written to str
int parse_hex(char *str) {
    char *tok;

    char *tmp = malloc(strlen(str));
    int i = 0;

    tok = strtok(str, " ");

    while (tok != NULL) {
        tmp[i++] = (strtol(tok, NULL, 16) & 0xFF);
        tok = strtok(NULL, " ");
    }

    memcpy(str, tmp, i);

    free(tmp);

    return i;
}