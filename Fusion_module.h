#ifndef FUSION_MODULE_H
#define FUSION_MODULE_H

typedef struct SensorData{
    float data;
    int state;
    int sensorID;
    int sortID;
    int outTimes;
    SensorData *next;
    int avalible;
}sensor_data, *psensor_data;

typedef struct SingleData
{
    float data[6];
    int state;
    int sensorID;
}single_data, *psingle_data;

struct buf{
        float data;
        int sensorID;
        int state;
};

#define DISABLE 0
#define ENABLE 1
#define NEED_SINGLE 1
#define NOT_NEED_SINGLE 0

psensor_data creat_test_data(float *testdata, int size);                             //create multi date for test
psingle_data creat_single_test_data(float *testdata, int sensorID);                  //create single date for test
void change_test_data(psensor_data *testdata, int num, int size, float *src);        //change multi data for test
void change_test_datas(psingle_data *testdata, int sensorID, float *src);            //change single data for test
void del_data(psensor_data *testdatam, psingle_data *testdatas);                     //delete a data list point
int check_counts(psensor_data *checkdata);                                           //when a catch round over, clear sensor state of outTime
int check_datas(psensor_data *checkdata);                                            //check if there is a sensor or more unuseful
struct buf* to_sort(psensor_data data);                                              //input a sensor data list, and output a sorted data list
void profile_data(psensor_data *data, int size);                                     //the way to check out data, and change the state of sensor data list
float create_fusion_data_multi(struct buf *databuf, int size);                       //input a sorted data list, and output fusioned data
float signal_fusion(psingle_data data);                                              //if it is single fusion model, use it
void change_fusion_al();                                                             //change fusion alogimite from multi to single
int check_sensors(psensor_data sensordata);                                          //check sensors whether there is not enough sensors for multi fusion

#endif //FUSION_MODULE_H
