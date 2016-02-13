#include <stdio.h>
#include <stdlib.h>
#include <linux/i2c-dev.h>
#include <fcntl.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>


/*
 * ref: http://www.kernel.org/doc/Documentation/i2c/dev-interface
*/

int main(void)
{
    const char *i2cDevice = "/dev/i2c-1"; // i2c adapter addrress on raspberry pi - run i2cdetect -l to determine
    const int i2cAddress = 0x08; // slave address
    // buffer
    //unsigned char buf[3][4];
    int fd; // file used for i2c comms
    //int i,j,k,m;
    unsigned char *c;
    unsigned char a = 'I';
    c = &a;

    // initialize data in buffer
    //buf[0][1] = 'x';
    //buf[1][1] = 'y';
    //buf[2][1] = 'z';
    unsigned char xbuf[4]="x120";
    unsigned char ybuf[4]="y256";
    unsigned char zbuf[4]="z024";
    // open i2c on raspberry pi
    if ((fd = open(i2cDevice, O_RDWR)) < 0) {
        printf("Failed to open i2c port\n");
        exit(1);
    }

    // specify slave address
    if (ioctl(fd, I2C_SLAVE, i2cAddress) < 0) {
        printf("Unable to configure i2c address\n");
        exit(1);
    }

    //__u8 reg = 0x10; /* Device register to access - doesn't seem to matter much for arduino */
    //buf[0] = reg;

    // i2c write
    *c = '\n';
    while(1) {
        write(fd,xbuf,4);
        write(fd,c,1);
        write(fd,ybuf,4);
        write(fd,c,1);
        write(fd,zbuf,4);
        write(fd,c,1);
        write(fd,c,1);
        usleep(1000000);
        xbuf[3] = xbuf[3]+1;
        if(xbuf[3]%57==0)
            xbuf[3] = '0';
        ybuf[2] = ybuf[2]+1;
        if(ybuf[2]%57==0)
            ybuf[2] = '0';
        zbuf[1] = zbuf[1]+1;
        zbuf[3] = zbuf[3]+1;
        if(zbuf[1]%55==0)
            zbuf[1]='0';
        if(zbuf[3]%57==0)
            zbuf[3]='0';
    }
    //if (write(fd, buf, 10) != 10) {
     //   printf("Error writing to i2c\n");
    //}

/*
    // i2c read
    if (read(fd, buf, 4) != 4) {
        printf("Error reading from i2c\n");
    }
    else {
        for(i=0; i<4; i++) {
            printf("%c",buf[0][i]);
        }
        printf("\n");
    }
*/


/*
    for (i=0;i<=10;i++) {
        buf[0] = i;  // set register
        if (write(fd, buf, 1) != 1) {
            printf("error writing i2c register address\n");
            exit;
        }

        // I could of course read all 10 registers in one read here instead.
        if (read(fd, buf, 1) != 1) { // read back data into buf
            printf("Short read from i2c\n");
            exit(1);
        }

        printf("Register %u = %u\n", i, buf[0]);
    }
    printf("[done]\n");

#define REG_GPIO  0x0A
#define REG_IODIR 0x00

    buf[0] = REG_IODIR;
    buf[1] = 0x00;  // all pins set to output
    if (write(fd, buf, 2) != 2) {
        printf("error writing i2c iodir register\n");
        exit;
    }

    static unsigned char stateMap[] = {0x9, 0x1, 0x7, 0x6, 0xE, 0x8};
    unsigned char state = 0;
    while (1) {
        buf[0] = REG_GPIO;
        buf[1] = stateMap[state];
        //printf("state %d bits %u\n", state, buf[1]);
        if (write(fd, buf, 2) != 2) {
            printf("error writing i2c gpio register\n");
            exit;
        }
        usleep(400);
        state = (state + 1) % 6;
    } */
    return 0;
}
