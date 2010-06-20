#ifndef SENSORS_H
#define SENSORS_H

#define ENABLED 1
#define DISABLED 0
#define USING 1
#define UNUSING 0

typedef struct Sensor{
    int isEnabled;
    int isUsing;
    int avalible;
    int sensorID;
    char *name;
    float catchData[];
}NSensor, *PSensor;

typedef struct SensorNode{
    NSensor *node;
    struct SensorNode *next;
}NSensorNode, *PSensorNode;

void create_sensor_list();
PSensorNode get_sensor_list();
char *get_sensor_name(int sensorID, PSensorNode list);
void set_sensor_stat(int sensorID, PSensorNode *list, int stat);
int get_sensor_stat(int sensorID, PSensorNode list);
void init_sensor_stat(PSensorNode *list);
void set_sensor_mode(int sensorID, PSensorNode *list, int mode);
int get_sensor_mode(int sensorID, PSensorNode list);
//void del_sensor_list(PSensorNode *list);
void del_sensor_list();
void itoa(int i, char *string);

#endif //SENSORS_H
