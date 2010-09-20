#include     <stdio.h>      /*标准输入输出定义*/
#include     <stdlib.h>     /*标准函数库定义*/
#include     <unistd.h>     /*Unix 标准函数定义*/
#include     <sys/types.h>
#include     <sys/stat.h>
#include     <fcntl.h>      /*文件控制定义*/
#include     <termios.h>    /*POSIX 终端控制定义*/
#include     <errno.h>      /*错误号定义*/
#include     "serial.h"

#define TRUE 1
#define FALSE 0

void set_speed(int fd, int speed)
{
    int i;
    int status;
    struct termios Opt; //定义了这样一个结构
    tcgetattr(fd, &Opt); //用来得到机器原端口的默认设置
    for ( i= 0; i < sizeof(speed_arr) / sizeof(int); i++)
    {
        if (speed == name_arr[i]) //判断传进来是否相等
        {
            tcflush(fd, TCIOFLUSH); //刷新输入输出缓冲
            cfsetispeed(&Opt, speed_arr[i]); //这里分别设置
            cfsetospeed(&Opt, speed_arr[i]);
            status = tcsetattr(fd, TCSANOW, &Opt); //这是立刻把bote rates设置真正写到串口中去
            if (status != 0)
                perror("tcsetattr fd1"); //设置错误
            return;
        }
        tcflush(fd,TCIOFLUSH); //同上
    }
}

int set_Parity(int fd,int databits,int stopbits,int parity)
{
    struct termios options; //定义一个结构
    if ( tcgetattr( fd,&options) != 0) //首先读取系统默认设置options中,必须
    {
        perror("SetupSerial 1");
        return(FALSE);
    }
    options.c_cflag |= CLOCAL | CREAD;
    options.c_cflag &= ~CSIZE; //这是设置c_cflag选项不按位数据位掩码
    switch (databits) /*设置数据位数*/
    {
        case 7:
            options.c_cflag |= CS7; //设置c_cflag选项数据位为7位
            break;
        case 8:
            options.c_cflag |= CS8; //设置c_cflag选项数据位为8位
            break;
        default:
            fprintf(stderr,"Unsupported data size\n"); //其他的都不支持
            return (FALSE);
    }
    switch (parity) //设置奇偶校验，c_cflag和c_iflag有效
    {
        case 'n':
        case 'N': //无校验 当然都不选
            options.c_cflag &= ~PARENB; /* Clear parity enable */
            options.c_iflag &= ~INPCK; /* Enable parity checking */
            break;
        case 'o'://奇校验其中PARENB校验位有效；PARODD奇校验
        case 'O': options.c_cflag |= (PARODD | PARENB); /* 设置为奇效验*/
            options.c_iflag |= INPCK; /* Disnable parity checking */
            break;
        case 'e':
        case 'E': //偶校验，奇校验不选就是偶校验了
            options.c_cflag |= PARENB; /* Enable parity */
            options.c_cflag &= ~PARODD; /* 转换为偶效验*/
            options.c_iflag |= INPCK; /* Disnable parity checking */
            break;
        default:
            fprintf(stderr,"Unsupported parity\n");
            return (FALSE);
    }
/* 设置停止位*/
    switch (stopbits) //这是设置停止位数，影响的标志是c_cflag
    {
        case 1:
            options.c_cflag &= ~CSTOPB; //不指明表示一位停止位
            break;
        case 2:
            options.c_cflag |= CSTOPB; //指明CSTOPB表示两位，只有两种可能
            break;
        default:
            fprintf(stderr,"Unsupported stop bits\n");
            return (FALSE);
    }
/* Set input parity option */
    if (parity != 'n') //这是设置输入是否进行校验
        options.c_iflag |= INPCK;

//这个地方是用来设置控制字符和超时参数的，一般默认即可。稍微要注意的是c_cc数组的VSTART 和 VSTOP 元素被设定成DC1 和 DC3，代表ASCII 标准的XON和XOFF字符。所以如果在传输这两个字符的时候就传不过去，这时需要把软件流控制屏蔽 options.c_iflag &= ~(IXON | IXOFF | IXANY);


    options.c_cc[VTIME] = 0; // 15 seconds
    options.c_cc[VMIN] = 110;

    tcflush(fd,TCIFLUSH); /* Update the options and do it NOW */ //刷新和立刻写进去
    if (tcsetattr(fd,TCSANOW,&options) != 0)
    {
        perror("SetupSerial 3");
        return (FALSE);
    }
    return (TRUE);
}

int OpenDev(char *Dev)
{
    int fd = open( Dev, O_RDWR ); //| O_NOCTTY | O_NDELAY这种方式看open函数
    if (-1 == fd)
    { /*设置数据位数*/
        perror("Can't Open Serial Port");
        return -1;
    }
    else
        return fd;
}

/*int init_serial(SerialPort port)
{
    char *dev;

    switch(port)
    {
        case TTYS0:
            dev = "/dev/ttyS0";
            break;
        case TTYS1:
            dev = "/dev/ttyS1";
            break;
        case TTYSAC0:
            dev = "/dev/ttySAC0";
            break;
        case TTYSAC1:
            dev = "/dev/ttySAC1";
            break;
        case TTYUSB0:
            dev = "/dev/ttyUSB0";
            break;
        case TTYUSB1:
            dev = "/dev/ttyUSB1";
            break;
    }
    printf("%s\n", dev);

    fd = OpenDev(dev);
    if (-1 == fd)
    {   //设置数据位数
        perror("Can't Open Serial Port");
        return -1;
    }
    if (fd>0)
        set_speed(fd,19200); //打开后设置波特率19200
    else
    {
        printf("Can't Open Serial Port!\n");
        exit(0);
    }
    if (set_Parity(fd, 8, 1,'n')== FALSE) //设置8，1，n 注意，这里和上面要和下位机相符才可能通信
    {
        printf("Set Parity Error\n");
        exit(1);
    }
    return 0;
}*/

void data_filter(char *databuf, SensorData *sd, int bufsize, int pksize)
{
    int i, count, j;
    int packlength, humilength, templength, headlength;
    int datanum;
    int packsize;
    SensorData *s_d = sd;

    printf("in filter!\n");
    printf ("%s\n", databuf);
    for(i = 0; i < bufsize; i++)
    {
        //printf ("in i circle!\n");
        if(databuf[i] == 'S')
        {
            printf("S");
            count = databuf[i + 1] - 48;
            if(count > 5)
                count = 5;
            for(j = 0; j < pksize; j++)
                s_d[j].sensorID = count;
        }
        if(databuf[i] == 'H')
        {
            datanum  = databuf[i + 2] - 48;
            packlength = databuf[i + 4] - 48;
            if(packlength == 2)
                s_d[datanum].humidity = (databuf[i + 6] - 48) * 10 + databuf[i + 7] - 48;
            else if(packlength == 3)
                s_d[datanum].humidity = (databuf[i + 6] - 48) * 100 + (databuf[i + 7] - 48) * 10 + databuf[i + 8] - 48;
            else if(packlength == 1)
                s_d[datanum].humidity = (databuf[i + 6] - 48);
        }
        if(databuf[i] == 'T')
        {
            datanum  = databuf[i + 2] - 48;
            templength =  databuf[i + 4]- 48;
            if(templength == 2)
                s_d[datanum].temperture = (databuf[i + 6] - 48) * 10 + databuf[i + 7] - 48;
            else if(templength == 3)
                s_d[datanum].temperture = (databuf[i + 6] - 48) * 100 + (databuf[i + 7] - 48) * 10 + databuf[i + 8] -48;
            else if(templength == 1)
                s_d[datanum].temperture = databuf[i + 6] - 48;
        }

        /*if(datanum == 5)
            isReady = 1;
        else
        isReady = 0;*/
    }
    return;
}

