#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include "serial.h"

int main(int argc, char *argv[]) {
    
    if(argc < 3) {
        printf("Usage: device baudrate\n");
        return -1;
    }

    long baud = strtol(argv[2], NULL, 10);

    // printf("%ld\n", baud);

    baud = parse_baud(baud);

    // printf("%ld\n", baud);
    // printf("%s\n", argv[1]);

    char *cur_dev = malloc(strlen(argv[1]));
    strcpy(cur_dev, argv[1]);

    int fd = initialize_serial(cur_dev, baud, 10);

    // printf("%d\n", fd);

    if(fd < 0) {
        printf("Error %d: ", errno);
        printf("%s\n", strerror(errno));

        return -1;
    }

    printf("Device opened and initialized\n");

    char *buff = calloc(256, 1);
    
    for(;;) {
        memset(buff, 0, 256);

        printf("Input a string to send to the device (max 255 characters) or input a command:\n");
        fgets(buff, 256, stdin);

        int r = 0;

        if(buff[0] == '!') {
            if(!strncmp(buff, "!dc", 3)) { // disconnect from the device
                r = close(fd);
                printf("%d\n", r);
                fd = -1;
            } else if(!strncmp(buff, "!chdev ", 7)) {
                r = close(fd);
                printf("%d\n", r);

                cur_dev = realloc(cur_dev, strlen(buff) - 8);
                for(int i = 8; i < strlen(buff); i++) {
                    cur_dev[i - 8] = buff[i];
                }

                fd = initialize_serial(cur_dev, baud, 10);
            } else if(!strncmp(buff, "!chbd ", 6)) {
                r = close(fd);
                printf("%d\n", r);

                char *temp = malloc(strlen(buff) - 7);
                printf("Past malloc\n");
                for(int i = 7; i < strlen(buff); i++) {
                    temp[i - 7] = buff[i];
                }
                printf("Past for loop\n");

                baud = strtol(temp, NULL, 10);
                printf("Past strtol\n");
                baud = parse_baud(baud);
                printf("Past parse_baud\n");

                free(temp);
                printf("Past free\n");

                fd = initialize_serial(cur_dev, baud, 10);
                printf("Successfully changed baud\n");
            }
        
        } else {

            // printf("length of buff: %ld\n", strlen(buff));

            // for(int i = 0; i < 256; i++) {
            //     printf(" %x", buff[i]);
            // }
            // printf("\n");

            int n = write(fd, buff, strlen(buff));

            printf("Wrote %d bytes\n", n);

            memset(buff, 0, 256);

            n = read(fd, buff, 256);
        
            printf("Read %d bytes\n", n);
            printf("%s\n", buff);
        }
    }

    return 0;
}