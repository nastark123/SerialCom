#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <limits.h>
#include "serial.h"
#include "cntl.h"

int main(int argc, char *argv[]) {

    serial_dev *dev = malloc(sizeof(serial_dev));
    memset(dev, 0, sizeof(serial_dev));

    if(argc > 1) {
        if(parse_opts(dev, argv, argc) < 0) return 0;
    }

    char *buff = malloc(PATH_MAX);
    memset(buff, 0, PATH_MAX);

    if(strlen(dev->dev) == 0) {
        printf(ANSI_COLOR_GREEN "You have not specified a device to connect to.  Please input one now:\n" ANSI_COLOR_RESET);
        fgets(buff, PATH_MAX, stdin);
        buff[strlen(buff) - 1] = '\0';
        strncpy(dev->dev, buff, PATH_MAX);
    }

    if(dev->baud == 0) dev->baud = B9600;

    if(dev->timeout == 0) dev->timeout = 10;

    if(dev->in_mode == 0) dev->in_mode = MODE_ASCII;

    if(dev->out_mode == 0)  dev->out_mode = MODE_ASCII;

    init_serial(dev);

    if(dev->fd < 0) {
        printf(ANSI_COLOR_RED "Error %d while initializing: " ANSI_COLOR_RESET, errno);
        printf(ANSI_COLOR_RED "%s\n", strerror(errno));
        return 0;
    } else {
        printf(ANSI_COLOR_GREEN "Device opened and initialized\n" ANSI_COLOR_RESET);
    }
    
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