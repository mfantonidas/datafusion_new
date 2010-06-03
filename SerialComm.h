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
};

struct 
