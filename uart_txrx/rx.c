/* rs232_rx.c */
#include <sys/types.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>
#include <string.h>
#define BAUDRATE        B38400
#define _POSIX_SOURCE   1

int main(int argc, char *argv[])
{

    int    fd, c=0, res;
    struct termios  oldtio, newtio;

    char  buf[256];

    if (argc < 2) {
        printf("Usage: %s path/to/uart\n", argv[0]);
    }

    printf("Start...\n");
    fd = open(argv[1], O_RDWR|O_NOCTTY);

    if (fd < 0) {
        perror(argv[1]);
        exit(1);
    }

    printf("Open...\n");
    tcgetattr(fd, &oldtio);
    bzero(&newtio, sizeof(newtio));

    newtio.c_cflag = CS8|CLOCAL|CREAD|ECHO;

    newtio.c_iflag = IGNPAR;
    newtio.c_oflag = 0;
    cfsetispeed(&newtio, BAUDRATE);

    /* 設定為正規模式 */
    newtio.c_lflag = ICANON;
    tcflush(fd, TCIFLUSH);
    tcsetattr(fd, TCSANOW, &newtio);

    printf("Reading...\n");

    fd_set master;
    fd_set read_fds;
    int fdmax;
    
    FD_ZERO(&master);
    FD_ZERO(&read_fds);
    FD_SET(fd, &master);
    fdmax = fd;
    
    while(1) {
        read_fds = master;
        if (select(fdmax + 1, & read_fds, NULL, NULL, NULL) == -1) {
            perror("select");
            exit(4);
        }
        res = read(fd, buf, 255);

        if(res==0)
            continue;
        buf[res]=0;

        printf("res=%d  buf=%s\n", res, buf);
        if (buf[0] == '@') break;
    }

    printf("Close...\n");
    close(fd);
    tcsetattr(fd, TCSANOW, &oldtio);

    return 0;
}
