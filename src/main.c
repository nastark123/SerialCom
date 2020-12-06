#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include "serial.h"
#include "cntl.h"

int main(int argc, char *argv[]) {

    char *setup = malloc(256);
    serial_dev *dev;
    dev = malloc(sizeof(serial_dev));

    printf(ANSI_COLOR_GREEN "Welcome to SerialCom!  What device would you like to connect to?\n" ANSI_COLOR_RESET);
    fgets(setup, 256, stdin);
    setup[strlen(setup) - 1] = '\0';
    dev->dev = malloc(strlen(setup));
    strcpy(dev->dev, setup);

    printf(ANSI_COLOR_GREEN "\nWhat baud rate would you like to use?\n" ANSI_COLOR_RESET);
    memset(setup, 0, 256);
    fgets(setup, 256, stdin);
    long baud = strtol(argv[2], NULL, 10);
    baud = parse_baud(baud);
    dev->baud = baud;

    printf(ANSI_COLOR_GREEN "\nWhat would you like as your starting input mode (ascii, hex, or file)?\n" ANSI_COLOR_RESET);
    memset(setup, 0, 256);
    fgets(setup, 256, stdin);
    dev->in_mode = init_mode(setup);

    printf(ANSI_COLOR_GREEN "\nWhat would you like as your starting output mode (ascii, hex, or file)?\n" ANSI_COLOR_RESET);
    memset(setup, 0, 256);
    fgets(setup, 256, stdin);
    dev->out_mode = init_mode(setup);

    printf(ANSI_COLOR_GREEN "\nWhat would you like the timeout to be (in 0.1's of a second):\n" ANSI_COLOR_RESET);
    memset(setup, 0, 256);
    fgets(setup, 256, stdin);
    dev->timeout = strtol(setup, NULL, 10);

    free(setup);

    initialize_serial(dev);

    if(dev->fd < 0) {
        printf(ANSI_COLOR_RED "Error %d while initializing: " ANSI_COLOR_RESET, errno);
        printf(ANSI_COLOR_RED "%s\n", strerror(errno));
    } else {
        printf(ANSI_COLOR_GREEN "Device opened and initialized\n" ANSI_COLOR_RESET);
    }
    char *buff = malloc(256);
    
    for(;;) {
        memset(buff, 0, 256);

        printf(ANSI_COLOR_GREEN "\nInput a string to send to the device (max 255 characters) or input a command:\n" ANSI_COLOR_RESET);
        fgets(buff, 256, stdin);

        int n = 0;

        switch(parse_cmd(buff)) {
            case NOTHING:
                // remove the newline character from the buffer
                buff[strlen(buff) - 1] = '\0';

                if(dev->in_mode == MODE_HEX) {
                    n = parse_hex(buff);
                } else if(dev->in_mode == MODE_FILE) {
                    int f = open(buff, O_RDONLY);
                    if(f < 0) printf(ANSI_COLOR_RED "\nError opening file\n" ANSI_COLOR_RESET);
                    n = read(f, buff, 256);
                    close(f);
                } else {
                    n = strlen(buff);
                }

                n = send_and_rec_data(dev, buff, n);
                
                display_output(buff, n, dev->out_mode);

                // not sure if this is necessary, but if memcpy resizes I think it is
                buff = realloc(buff, 256);

                break;

            case DISCONNECT:
                n = disconnect(dev);
                if (n < 0) {
                    printf(ANSI_COLOR_RED "\nError %d: " ANSI_COLOR_RESET, errno);
                    printf(ANSI_COLOR_RED "%s\n" ANSI_COLOR_RESET, strerror(errno));
                }
                break;

            case CHDEVICE:
                n = ch_dev(dev, buff);
                if (n < 0) {
                    printf(ANSI_COLOR_RED "\nError %d: " ANSI_COLOR_RESET, errno);
                    printf(ANSI_COLOR_RED "%s\n" ANSI_COLOR_RESET, strerror(errno));
                }
                break;

            case CHBAUD:
                n = ch_baud(dev, buff);
                if (n < 0) {
                    printf(ANSI_COLOR_RED "\nError %d: " ANSI_COLOR_RESET, errno);
                    printf(ANSI_COLOR_RED "%s\n" ANSI_COLOR_RESET, strerror(errno));
                }
                break;

            case LSCMD:
                ls_cmd();
                break;

            case LSBD:
                ls_bd();
                break;

            case LSMODE:
                ls_mode();
                break;

            case IMODE:
                dev->in_mode = ch_mode(buff);
                break;

            case OMODE:
                dev->out_mode = ch_mode(buff);
                break;

            default:
                break;
        }

    }

    return 0;
}