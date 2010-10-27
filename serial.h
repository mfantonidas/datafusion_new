#ifndef SERIAL_H
#define SERIAL_H

#include     <stdio.h>      /*标准输入输出定义*/
#include     <stdlib.h>     /*标准函数库定义*/
#include     <unistd.h>     /*Unix 标准函数定义*/
#include     <sys/types.h>
#include     <sys/stat.h>
#include     <fcntl.h>      /*文件控制定义*/
#include     <termios.h>    /*POSIX 终端控制定义*/
#include     <errno.h>      /*错误号定义*/

#define TRUE 1
#define FALSE 0

typedef struct{
    int temperture;
    int humidity;
    int sensorID;
}SensorData;

typedef enum{ TTYS0, TTYS1, TTYSAC0, TTYSAC1, TTYUSB0, TTYUSB1 }SerialPort;

static int speed_arr[] = { B38400, B19200, B9600, B4800, B2400, B1200, B300,
                    B38400, B19200, B9600, B4800, B2400, B1200, B300 };
static int name_arr[] = { 38400, 19200, 9600, 4800, 2400, 1200, 300,
                   38400, 19200, 9600, 4800, 2400, 1200, 300 };

extern int get_speed(int speed);
extern int set_Parity(int fd,int databits,int stopbits,int parity);
extern int OpenDev(char *Dev);
extern void data_filter(char *databuf, SensorData *sd, int bufsize, int pksize);
//int init_serial(SerialPort port);

#endif //SERIAL_H
