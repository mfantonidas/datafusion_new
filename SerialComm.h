#ifndef SERIALCOMM_H
#define SERIALCOMM_H

#include <stdio.h>
#include <unistd.h>

extern char *somename;
extern int number;

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

extern void sensor_init();
extern void read_sensor();
extern void control_sensor();
extern void check_sensor();
  
#endif //SERIALCOMM_H
