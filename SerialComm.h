#ifndef SERIALCOMM_H
#define SERIALCOMM_H

extern char *somename;
extern int number;

int open_port(int port);
int set_opt(int fd, int nSpeed, int nBits, char nEvent, int nStop);
unsigned int check_sensor(int fd);
unsigned int read_sensor(int fd);
int control_sensor();

extern void sensor_init();
extern void check_sensor();

#endif //SERIALCOMM_H
