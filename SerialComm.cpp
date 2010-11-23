#include "SerialComm.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <sys/time.h>
#include <time.h>
#include <signal.h>
#include <string.h>

#define BAUDRATE B19200
#define MODEMDEVICE "/dev/ttyUSB0"
#define _POSIX_SOURCE 1 /* POSIX 系统相容 */
#define FALSE 0
#define TRUE 1

static int fd;
static char databuf[255];
static NsensorData sensordata[3];

int open_port(int port)
{
    /*
    *  Open COM port,
    *  if port=1, open COM1;if port=2, open COM2
    */
    long vdisable;
    int fd;
    if(port == 1)
    {
        fd = open("/dev/ttySAC1", O_RDWR | O_NOCTTY | O_NDELAY);
        if(!fd)
        {
            /*open com1 err*/
            return 0;
        }
    }
    else if(port == 2)
    {
        fd = open("/dev/ttySAC2", O_RDWR | O_NOCTTY | O_NDELAY);
        if(!fd)
        {
            /*open com2 err*/
            return 0;
        }
    }
    else
    {
        fd = open("/dev/ttyUSB0", O_RDWR | O_NOCTTY | O_NDELAY);
        if(!fd)
            return 0;
    }

    if(fcntl(fd, F_SETFL, 0) < 0)
    {
        /*
        *  open com err
        */
        return 0;
    }
    if(isatty(STDIN_FILENO) == 0)
    {
        /*
        *  open com err
        */
        return 0;
    }

    return fd;
}

int set_opt(int fd, int nSpeed, int nBits, char nEvent, int nStop)
{
    /*
    *   set patmeter of COM port
    *   fd: file descript of COM port
    *   nSpeed: baut rate
    *   nBits:  numbers of data bits
    *   nEven: PARE mode O:odd E:even N:none
    *   nStop: stop bits
    */
    struct termios newtio, oldtio;
    if(tcgetattr(fd, &oldtio) != 0)
        return 0;

    bzero(&newtio, sizeof(newtio));
    newtio.c_cflag |= CLOCAL | CREAD;
    newtio.c_cflag &= ~CSIZE;

    switch(nBits)
    {
    case 7:
        newtio.c_cflag |= CS7;
        break;
    case 8:
        newtio.c_cflag |= CS8;
        break;
    }

    switch(nEvent)
    {
    case 'O':
        newtio.c_cflag |= PARENB;
        newtio.c_cflag |= PARODD;
        newtio.c_iflag |= (INPCK | ISTRIP);
        break;
    case 'E':
        newtio.c_cflag |= PARENB;
        newtio.c_cflag &= ~PARODD;
        newtio.c_iflag |= (INPCK | ISTRIP);
        break;
    case 'N':
        newtio.c_cflag &= ~PARENB;
    }

    switch(nSpeed)
    {
    case 19200:
        cfsetispeed(&newtio, B19200);
        cfsetospeed(&newtio, B19200);
        break;
    case 115200:
        cfsetispeed(&newtio, B115200);
        cfsetospeed(&newtio, B115200);
        break;
    default:
        cfsetispeed(&newtio, B9600);
        cfsetospeed(&newtio, B9600);
        break;
    }

    if(nStop == 1)
        newtio.c_cflag &= ~ CSTOPB;
    else if(nStop == 2)
        newtio.c_cflag |= CSTOPB;
    newtio.c_cc[VTIME] = 0;
    newtio.c_cc[VMIN] = 0;
    tcflush(fd, TCIFLUSH);
    if((tcsetattr(fd, TCSANOW, &newtio)) != 0)
        return 0;

    return 1;
}

int read_sensor()
{
    int res;
    int i, count;

    res = read(fd, databuf, 255);
    if(res)
    {
        databuf[res] = '\n';
        for(i = 0; i < res;)
        {
            if(databuf[i] == 'S')
            {
                count = databuf[i + 1] - 48;
                sensordata[count - 1].num = count;
                for(i; i < res;)
                {
                    if(databuf[i] == 'T')
                    {
                        sensordata[count - 1].temperture = (databuf[i + 1] - 48) * 10 + databuf[i + 2] - 48;
                        for(i; i < res;)
                        {
                            if(databuf[i] == 'H')
                            {
                                sensordata[count - 1].humidity = (databuf[i + 1] - 48) * 10 + databuf[i + 2] - 48;
                                break;
                            }
                            i++;
                        }
                        break;
                    }
                    i++;
                }
            }
            i++;
        }
    }
}

/*int SensorReader::init_sensor_reader(int port)
{
    unsigned char buffer[256];
    int readnum;
    unsigned int timer;
    int i = 0;

    memset(buffer, 0, sizeof(buffer));

    while(i < 256)
    {
        readnum = read(fd, buffer, 1);
        if(readnum)
            ++i;
        usleep(100);
        timer++;
        if(timer > 1000)
            return 0;
    }
}
static struct SensorNode *sensors;

static void init_sensor()
{
  struct SensorNode *sn;
  int counter = 0;
  int i = 0;

}*/

