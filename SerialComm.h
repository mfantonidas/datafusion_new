#ifndef SERIALCOMM_H
#define SERIALCOMM_H

extern char *somename;
extern int number;

#include <QThread>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <termios.h>

struct SensorData{
	int sensorID;
	char *sensorName;
	int isErr;
	int tempData;
	int humiData;
	int state;
  	int sensorCount;
};

struct SensorNode{
	int SensorID;
	struct SensorData *sd;
	struct SensorNode *next;
};

class SensorReader:public QThread
{
	Q_OBJECT
public:
	SensorReader();
	int fd_port;
	int open_port(int port);
	int set_opt(int fd, int nSpeed, int nBits, char nEvent, int nStop);
	int init_sensor_reader(int fd);
	unsigned int check_sensor(int fd);
	unsigned int read_sensor(int fd);
	int control_sensor();
private:
	struct SensorData *sd;
	struct SensorNode *sn;	
};

extern void sensor_init();
extern void read_sensor();
extern void control_sensor();
extern void check_sensor();
  
#endif //SERIALCOMM_H
