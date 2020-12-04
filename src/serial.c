#include "serial.h"

// sends the data pointed to by buff and reads the response back into buff
// returns the number of bytes read
int send_and_rec_data(serial_dev *dev, void *buff) {

    write(dev->fd, buff, strlen(buff));

    memset(buff, 0, 256);

    return read(dev->fd, buff, 256);
}

// returns a file descriptor to the serial device if successful, or error code if not
// dev is a string that is the path to the device, usually /dev/ttyX or /dev/ttyACMX
// timeout is in .1's of a second and can range from 0 to 255, defaults to 10 if out of bounds
int initialize_serial(serial_dev *dev) {
    dev->fd = open(dev->dev, O_RDWR | O_NOCTTY | O_SYNC);
    // printf("fd: %d\n", dev->fd);
    // printf("Past fd\n");

    if(dev->fd < 0) return -1;

    struct termios *t;
    t = malloc(sizeof(struct termios));

    if(tcgetattr(dev->fd, t) != 0) return -1;
    // printf("Past tcgetattr\n");

    cfmakeraw(t);
    // printf("Past cfmakeraw\n");

    t->c_cc[VMIN] = 0;
    t->c_cc[VTIME] = (dev->timeout < 255) ? dev->timeout : 10;

    cfsetispeed(t, dev->baud);
    cfsetospeed(t, dev->baud);
    // printf("Past setting speeds\n");

    if(tcsetattr(dev->fd, TCSANOW, t) != 0) return -1;
    // printf("Past tcsetattr\n");

    free(t);

    return dev->fd;
}

// takes the passed baudrate and converts it to the macros in termios.h
// defaults to 9600 baud, because that seems pretty standard
int parse_baud(long baud) {
    switch(baud) {
        case 50:
            return B50;
        case 75:
            return B75;
        case 110:
            return B110;
        case 134:
            return B134;
        case 150:
            return B150;
        case 200:
            return B200;
        case 300:
            return B300;
        case 600:
            return B600;
        case 1200:
            return B1200;
        case 1800:
            return B1800;
        case 2400:
            return B2400;
        case 4800:
            return B4800;
        case 9600:
            return B9600;
        case 19200:
            return B19200;
        case 38400:
            return B38400;

        // if the extended baudrates are defined, allow those too
        #ifdef __USE_MISC
            case 57600:
                return B57600;
            case 115200:
                return B115200;
            case 230400:
                return B230400;
            case 460800:
                return B460800;
            case 500000:
                return B500000;
            case 576000:
                return B576000;
            case 921600:
                return B921600;
            case 1000000:
                return B1000000;
            case 1152000:
                return B1152000;
            case 1500000:
                return B1500000;
            case 2000000:
                return B2000000;
            case 2500000:
                return B2500000;
            case 3000000:
                return B3000000;
            case 3500000:
                return B3500000;
            case 4000000:
                return B4000000;    
        #endif

        default:
            return B9600;
    }

}