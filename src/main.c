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
    
    if(argc < 3) {
        printf("Usage: device baudrate\n");
        return -1;
    }

    long baud = strtol(argv[2], NULL, 10);

    baud = parse_baud(baud);

    serial_dev *dev;
    dev = malloc(sizeof(serial_dev));
    dev->baud = baud; // baudrate for the serial communication
    dev->dev = malloc(strlen(argv[1])); // path to the device we're connecting to
    strcpy(dev->dev, argv[1]);
    dev->timeout = 10; // timeout in .1's of a second for reads from this device


    initialize_serial(dev);

    if(dev->fd < 0) {
        printf("Error %d while initializing: ", errno);
        printf("%s\n", strerror(errno));

        return -1;
    }

    printf("Device opened and initialized\n");

    char *buff = malloc(256);
    
    for(;;) {
        memset(buff, 0, 256);

        printf("\nInput a string to send to the device (max 255 characters) or input a command:\n");
        fgets(buff, 256, stdin);

        int n = 0;

        switch(parse_cmd(buff)) {
            case NOTHING:
                n = send_and_rec_data(dev, buff);
                printf("\nRead %d bytes from device\n\n", n);
                printf("Data: %s\n", buff);
                break;

            case DISCONNECT:
                n = disconnect(dev);
                if (n < 0) {
                    printf("\nError %d: ", errno);
                    printf("%s\n", strerror(errno));
                }
                break;

            case CHDEVICE:
                n = ch_dev(dev, buff);
                if (n < 0) {
                    printf("\nError %d: ", errno);
                    printf("%s\n", strerror(errno));
                }
                break;

            case CHBAUD:
                n = ch_baud(dev, buff);
                if (n < 0) {
                    printf("\nError %d: ", errno);
                    printf("%s\n", strerror(errno));
                }
                break;

            default:
                break;
        }

    }

    return 0;
}