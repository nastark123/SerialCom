#include "cntl.h"

int display_output(char *out, int len, int out_mode) {
    switch(out_mode) {
        case MODE_ASCII:
            printf(ANSI_COLOR_GREEN "\nRead %d bytes from device\n", len);
            printf("Data: " ANSI_COLOR_RESET "%s\n", out);
            break;

        case MODE_HEX:
            printf(ANSI_COLOR_GREEN "\nRead %d bytes from device:\n" ANSI_COLOR_RESET, len);
            for(int i = 0; i < len; i++) {
                printf("%x ", out[i]);
            }
            printf("\n");
            break;

        case MODE_FILE:
            printf(ANSI_COLOR_GREEN "\nInput a filename to write to:\n" ANSI_COLOR_RESET);
            char *file = malloc(256);
            fgets(file, 256, stdin);
            file[strlen(file) - 1] = '\0';

            int f = open(file, O_RDWR);
            if(f < 0) printf(ANSI_COLOR_RED "Error opening file\n" ANSI_COLOR_RESET);
            write(f, out, len);
            close(f);
                    
            printf(ANSI_COLOR_GREEN "\nWrote %d bytes to file %s\n" ANSI_COLOR_RESET, len, file);
            free(file);
            break;

        default:
            printf(ANSI_COLOR_GREEN "\nRead %d bytes from device\n", len);
            printf("Data: " ANSI_COLOR_RESET "%s\n", out);
            break;
    }
}

unsigned int parse_cmd(char *cmd) {

    if(!strncmp(cmd, "!dc", 3)) { // disconnect from the device
        remove_cmd(cmd, 3);
        return DISCONNECT;
    } else if(!strncmp(cmd, "!chdev ", 7)) { // change the device we're connected to
        remove_cmd(cmd, 7);
        return CHDEVICE;
    } else if(!strncmp(cmd, "!chbd ", 6)) { // change the baudrate and reconnect
        remove_cmd(cmd, 6);
        return CHBAUD;
    } else if(!strncmp(cmd, "!lscmd", 6)) {
        remove_cmd(cmd, 6);
        return LSCMD;
    } else if(!strncmp(cmd, "!lsbd", 5)) {
        remove_cmd(cmd, 5);
        return LSBD;
    } else if(!strncmp(cmd, "!lsmode", 7)) {
        remove_cmd(cmd, 7);
        return LSMODE;
    } else if(!strncmp(cmd, "!imode ", 7)) {
        remove_cmd(cmd, 7);
        return IMODE;
    } else if(!strncmp(cmd, "!omode ", 7)) {
        remove_cmd(cmd, 7);
        return OMODE;
    }

    return NOTHING;
}

// removes the first n characters of cmd
int remove_cmd(char *cmd, int n) {
    char *tmp = malloc(strlen(cmd) - n);
    for(int i = n; i < strlen(cmd) - 1; i++) {
        tmp[i - n] = cmd[i];
    }

    strcpy(cmd, tmp);

    free(tmp);

    return 0;
}

int disconnect(serial_dev *dev) {
    int r = close(dev->fd);

    dev->fd = -1;

    return r;
}

int ch_dev(serial_dev *dev, char *cmd) {
    if(dev->fd > 0) {
        int r = disconnect(dev);
        if (r < 0) return -1;
    }

    dev->dev = realloc(dev->dev, strlen(cmd));
    strcpy(dev->dev, cmd);

    printf(ANSI_COLOR_GREEN "\nConnecting to %s\n" ANSI_COLOR_RESET, dev->dev);

    dev->fd = init_serial(dev);
    if(dev->fd < 0) return -1;

    return 0;
}

int ch_baud(serial_dev *dev, char *cmd) {
    int r = disconnect(dev);
    if (r < 0) return -1;

    dev->baud = strtol(cmd, NULL, 10);

    printf(ANSI_COLOR_GREEN "\nSetting baudrate to %d...\n" ANSI_COLOR_RESET, dev->baud);

    dev->baud = parse_baud(dev->baud);

    dev->fd = init_serial(dev);

    if(dev->fd < 0) return -1;

    return 0;
}

void ls_cmd(){
    printf(ANSI_COLOR_GREEN "\nWelcome to SerialCom!  All commands are preceded by a \"!\" and are as follows:\n");
    printf("dc - disconnect from the current serial device\n");
    printf("chdev - change the device you are currently connected to\n");
    printf("chbd - change the baud rate of the serial connection\n");
    printf("imode - change the mode to one of the supported input modes\n");
    printf("omode - change the mode to one of the supported input modes\n");
    printf("lscmd - list all commands\n");
    printf("lsbd - list all the supported baud rates compiled into this executable\n");
    printf("lsmode - lists all supported modes (ascii, hex, etc.)\n" ANSI_COLOR_RESET);
}

void ls_bd() {
    printf(ANSI_COLOR_GREEN "\nThe supported baud rates in this executable are:\n");
    printf("50, 75, 110, 134.5, 150, 200, 300, 600, 1200\n");
    printf("1800, 2400, 4800, 9600, 19200, 38400\n" ANSI_COLOR_RESET);
    #ifdef __USE_MISC
    printf(ANSI_COLOR_GREEN "\nExtended baud rates available are:\n");
    printf("57600, 115200, 230400, 460800, 500000\n");
    printf("576000, 921600, 1000000, 1152000\n");
    printf("1500000, 2000000, 2500000, 3000000\n");
    printf("3500000, 4000000\n" ANSI_COLOR_RESET);
    #endif
}

void ls_mode() {
    printf(ANSI_COLOR_GREEN "\nCurrently supported modes:\n");
    printf("ascii - interpret characters as ascii (default)\n");
    printf("hex - translate characters as space delimited hexadecimal\n");
    printf("file - reads data from a file and writes it to the serial device\n" ANSI_COLOR_RESET);
}

int ch_mode(char *cmd) {

    if(!strncmp(cmd, "ascii", 5)) {
        printf(ANSI_COLOR_GREEN "\nNow in ASCII mode\n" ANSI_COLOR_RESET);
        return MODE_ASCII;
    } else if(!strncmp(cmd, "hex", 3)) {
        printf(ANSI_COLOR_GREEN "\nNow in HEX mode\n" ANSI_COLOR_RESET);
        return MODE_HEX;
    } else if(!strncmp(cmd, "file", 4)) {
        printf(ANSI_COLOR_GREEN "\nNow in file mode\n" ANSI_COLOR_RESET);
        return MODE_FILE;
    }

}

// yes i realize im repeating myself, will probably refactor later
int init_mode(char *cmd) {

    if(!strncmp(cmd, "ascii", 5)) {
        printf(ANSI_COLOR_GREEN "\nSet to ASCII mode\n" ANSI_COLOR_RESET);
        return MODE_ASCII;
    } else if(!strncmp(cmd, "hex", 3)) {
        printf(ANSI_COLOR_GREEN "\nSet to HEX mode\n" ANSI_COLOR_RESET);
        return MODE_HEX;
    } else if(!strncmp(cmd, "file", 4)) {
        printf(ANSI_COLOR_GREEN "\nSet to file mode\n" ANSI_COLOR_RESET);
        return MODE_FILE;
    }

    printf(ANSI_COLOR_RED "\nUnrecognized mode, starting in ASCII\n" ANSI_COLOR_RESET);
    return MODE_ASCII;
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