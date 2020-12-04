#include "cntl.h"

unsigned int parse_cmd(char *cmd) {

    if(!strncmp(cmd, "!dc", 3)) { // disconnect from the device
        return DISCONNECT;
    } else if(!strncmp(cmd, "!chdev ", 7)) { // change the device we're connected to
        return CHDEVICE;
    } else if(!strncmp(cmd, "!chbd ", 6)) { // change the baudrate and reconnect
        return CHBAUD;
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