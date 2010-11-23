#ifndef SERIALCOMM_H
#define SERIALCOMM_H

extern char *somename;
extern int number;

typedef struct sensorData{
    int num;
    int temperture;
    int humidity;

}NsensorData, *PsensorData;

int open_port(int port);
int set_opt(int fd, int nSpeed, int nBits, char nEvent, int nStop);
unsigned int check_sensor(int fd);
int read_sensor();
int control_sensor();


extern void sensor_init();
extern void check_sensor();

#endif //SERIALCOMM_H
