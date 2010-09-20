#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <unistd.h>
#include "Fusion_module.h"

#define BUFSIZE 25
#define uint unsigned int
#define ufloat unsigned float
#define MULTI_DEBUG 1
#define SINGLE_DEBUG 1
#define DEBUG 1

static int fusion_flag = NOT_NEED_SINGLE;

float signal_fusion(psingle_data data)
{
    float upbuf[BUFSIZE], downbuf[BUFSIZE];
    struct buf buff[BUFSIZE];
    int i, j, sizeup, sizedown, ng, ng2, sensorIDt, state, newup, newdown;
    float temp, sumtemp;
    float middle;
    float fusiondata;
    float upfour, downfour, outdis;
    float lamda1, lamda2, average1, average2;
    single_data *sdt;
    int size;

    //memcpy(buf, datalist, size);
    ng = size = 6;
    buff[size].data = '\0';
    printf("in signal_fusion:\nsensor:%d\n", data->sensorID);

    for(sdt = data, i = 0; i < size; i++)
        printf("%f\n", sdt->data[i]);

    for(sdt = data, i = 0; i < size; i++)
    {
        buff[i].data = sdt->data[i];
        buff[i].state = 1;
    }

    for(i = 1; i < size; i++)
    {
        temp = buff[i].data;
        for(j = i; j > 0 && buff[j-1].data > temp; j--)
        {
            buff[j].data = buff[j - 1].data;
        }
        buff[j].data = temp;
    }

    printf("after sort:\n");

    for(i = 0; i < size; i++)
        printf("%f  \n", buff[i].data);

    if(size%2)
    {
        middle = buff[size/2].data;
        sizeup = size/2 + 1;
        printf("sizeup: %d\n", sizeup);

        if(sizeup%2)
        {
            downfour = buff[sizeup/2].data;
            sizedown = sizeup/2 + size/2;
            upfour = buff[sizedown].data;
            printf("odd %f %f %f\n", middle, upfour, downfour);
        }
        else
        {
            downfour = (buff[sizeup/2].data + buff[sizeup/2 - 1].data)/2;
            sizedown = sizeup/2 + size/2;
            upfour = (buff[sizedown].data + buff[sizedown - 1].data)/2;
            printf("odd %f %f %f\n", middle, upfour, downfour);
        }
    }

    else
    {
        middle = (buff[size/2].data + buff[size/2 - 1].data)/2;
        sizeup = size/2 + 1;
        printf("sizeup: %d\n", sizeup);
        if(sizeup%2)
        {
            downfour = buff[sizeup/2].data;
            sizedown = sizeup/2 + size/2;
            upfour = buff[sizedown - 1].data;
            printf("dual %f %f %f\n", middle, upfour, downfour);
        }
        else
        {
            downfour = (buff[sizeup/2].data + buff[sizeup/2 - 1].data)/2;
            sizedown = sizeup/2 + size/2 - 1;
            upfour = (buff[sizedown].data + buff[sizedown - 1].data)/2;
            printf("dual %f %f %f\n", middle, upfour, downfour);
        }
    }

    outdis = upfour - downfour;
    for(i = 0; i < size; i++)
    {
        if(buff[i].data < (downfour - outdis)||buff[i].data > (upfour + outdis))
        {
            buff[i].state = 0;
        }
    }
    for(i = 0; i < size; i++)
        printf("buff[%d] = %f , state = %s\n", i, buff[i].data, buff[i].state==0?"DISABLE":"ENABLE");

    if(ng%2)
    {
        newdown = ng/2 + 1;
        newup = ng - newdown;
        for(i = 0, j = 0; i < newdown && j < size; j++)
        {
            if(buff[j].state == ENABLE)
            {
                downbuf[i] = buff[j].data;
                printf("downbuf[%d] = %f = buff[%d]\n", i, downbuf[i], j);
                i++;
            }
        }
        for(i = 0, j; i < newup && j < size; j++)
        {
            if(buff[j].state == ENABLE)
            {
                upbuf[i] = buff[j].data;
                printf("upbuf[%d] = %f = buff[%d]\n", i, upbuf[i], j);
                i++;
            }
        }
    }
    else
    {
        newdown = ng/2;
        newup = ng - newdown;
        for(i = 0, j = 0; i < newdown && j < size; j++)
        {
            if(buff[j].state == ENABLE)
            {
                downbuf[i] = buff[j].data;
                printf("downbuf[%d] = %f = buff[%d]\n", i, downbuf[i], j);
                i++;
            }
        }
        for(i = 0, j; i < newup && j < size; j++)
        {
            if(buff[j].state == ENABLE)
            {
                upbuf[i] = buff[j].data;
                printf("upbuf[%d] = %f = buff[%d]\n", i, upbuf[i], j);
                i++;
            }
        }
    }

    sumtemp = 0;
    for(i = 0; i < newdown; i++)
        sumtemp += downbuf[i];
    average1 = sumtemp/newdown;
    printf("downaver: %f\n", average1);
    sumtemp = 0;
    for(i = 0; i < newup; i++)
        sumtemp += upbuf[i];
    average2 = sumtemp/newup;
    printf("upaver: %f\n", average2);

    sumtemp = 0;
    for(i = 0; i < newdown; i++)
        sumtemp += pow((downbuf[i] - average1), 2.0);
    lamda1 = sumtemp/(newdown - 1);
    sumtemp = 0;
    for(i = 0; i < newup; i++)
        sumtemp += pow((upbuf[i] - average2), 2.0);
    lamda2 = sumtemp/(newup - 1);
    printf("%f %f\n", lamda1, lamda2);

    fusiondata = lamda2/(lamda1 + lamda2)*average1 + lamda1/(lamda1 + lamda2)*average2;

    printf("fusiondata: %f\n", fusiondata);

    return fusiondata;
}

psensor_data creat_test_data(float *testdata, int size)
{
    int i;
    sensor_data *sdt;
    psensor_data sds = NULL, sde = NULL;

    for(i = 0; i < size; i++)
    {
        sdt = (sensor_data *)malloc(sizeof(sensor_data));
        sdt->sensorID = i;

        sdt->next = NULL;
        if(sds == NULL)
        {
            sds = sdt;
            sde = sdt;
        }
        sde->next = sdt;
        sde = sdt;
    }

    for(sdt = sds, i = 0; sdt != NULL; sdt = sdt->next, i++)
    {
        sdt->data = testdata[i];
        sdt->state = ENABLE;
        sdt->outTimes = 0;
        sdt->avalible = ENABLE;
    }

    return sds;
}

psingle_data creat_single_test_data(float *testdata, int sensorID)
{
    single_data *sdt;
    int i;

    sdt = (psingle_data)malloc(sizeof(single_data));
    sdt->sensorID = sensorID;

    for(i = 0; i < 6; i++)
    {
        sdt->data[i] = testdata[i];
    }

    return sdt;
}



void change_test_data(psensor_data *testdata, int num, int size, float *src)
{
    int i;
    sensor_data *sdt;

    for(i = 0, sdt = *testdata; i < size && sdt != NULL; sdt = sdt->next, i++)
    {
        sdt->data = src[i];
        sdt->state = ENABLE;
    }
}

void change_test_datas(psingle_data *testdata, int sensorID, float *src)
{
    int i;
    single_data *sdt;

    sdt = *testdata;
    sdt->sensorID = sensorID;
    for(i = 0; i < 6; i++)
        sdt->data[i] = src[i];
}

int main()
{
    int i, count = 0;
    int size;
    sensor_data *sdt;
    psensor_data sds = NULL;
    psingle_data sds1 = NULL;
    float testdata[4][6] = {
        {26.70, 26.10, 25.90, 25.80, 25.10, 24.80},
        {26.30, 25.40, 27.00, 25.60, 26.10, 25.30},
        {25.40, 26.20, 25.90, 24.90, 25.30, 26.70},
        {24.90, 25.70, 24.80, 26.10, 25.50, 26.10}
    };
    float fusiondata;
    struct buf *buff1 = NULL;

//#ifdef MULTI_DEBUG
    sds = creat_test_data(testdata[0], 6);
    printf("sizeof sd: %d\n", size = 6);
//#endif
    while(1)
    {
        count++;
            if(count >= 4)
                count = 0;
//#ifdef DEBUG
        if(fusion_flag == NOT_NEED_SINGLE)
        {
//#endif
            printf("in main:\n");
//#ifdef MULTI_DEBUG
            printf("multi!!!\n");
            for(sdt = sds; sdt != NULL; sdt = sdt->next)
                printf("sensorID:%d data:%f state:%d outtimes:%d\n", sdt->sensorID, sdt->data, sdt->state, sdt->outTimes);

            profile_data(&sds, size);
            buff1 = to_sort(sds);
            fusiondata = create_fusion_data_multi(buff1, size);

            printf("result: %f\n", fusiondata);
            check_counts(&sds);
            check_datas(&sds);
            fusion_flag = check_sensors(sds);
            printf("fusion_flag = %s\n", fusion_flag == NEED_SINGLE?"NEED_SINGLE":"NOT_NEED_SINGLE");

            change_test_data(&sds, count, size, testdata[count]);
            sleep(1);
        }
//#endif
//#ifdef SINGLE_DEBUG
        else
        {
            if(sds1 == NULL)
                sds1 = creat_single_test_data(testdata[0], 0);
            printf("single!!!\n");
            for(i = 0; i < 6; i++)
                printf("sensorID:%d data[%d]: %f\n", sds1->sensorID, i, sds1->data[i]);
            fusiondata = signal_fusion(sds1);
            change_test_datas(&sds1, 1, testdata[count]);
            sleep(1);
        }
//#endif
    }
//    sort_func(&sds, size);
//    for(sdt = sds; sdt != NULL; sdt = sdt->next)
//        printf("%d %f %d\n", sdt->sensorID, sdt->data, sdt->state);
    del_data(&sds, &sds1);
    printf("delete ok\n");

    return 0;
}

void del_data(psensor_data *testdatam, psingle_data *testdatas)
{
    sensor_data *sdt;
    psensor_data sd;
    single_data *sdt1;

    for(sdt = *testdatam; sdt != NULL; sdt = sdt->next)
    {
        sd = sdt;
        free(sd);
    }
    sdt = sd = NULL;
    sdt1 = *testdatas;
    free(sdt1);
    sdt1 = NULL;
}

int check_counts(psensor_data *checkdata)
{
    int ret;
    static int count = 0;
    sensor_data *sdt;

    printf("count: %d\n", count);

    if(++count < 20)
        return ret = 0;
    for(sdt = *checkdata; sdt != NULL; sdt = sdt->next)
        sdt->outTimes = 0;
    count = 0;
    return 1;
}

int check_datas(psensor_data *data)
{
    sensor_data *sdt;

    for(sdt = *data; sdt != NULL; sdt = sdt->next)
        if(sdt->outTimes > 3)
        {
            printf("sensor %d is out!\n", sdt->sensorID);//
            sdt->avalible = DISABLE;
            sdt->outTimes = 0;
        }
    return 0;
}

struct buf* to_sort(psensor_data data)
{
    int i;
    struct buf retbuf[BUFSIZE];

    sensor_data *sd = NULL;

    for(sd = data; sd != NULL; sd = sd->next)
    {
        retbuf[sd->sortID].data = sd->data;
        retbuf[sd->sortID].state = sd->state;
        retbuf[sd->sortID].sensorID = sd->sensorID;
    }

    return retbuf;
}

void profile_data(psensor_data *data, int size)
{
    float upbuf[BUFSIZE], downbuf[BUFSIZE];
    int i, j, sizeup, sizedown, ng, ng2, sensorIDt, state, newup, newdown;
    float temp, sumtemp;
    float middle;
    float upfour, downfour, outdis;
    sensor_data *sd = *data;
    struct buf buff1[BUFSIZE];

    //memcpy(buf, datalist, size);
    ng = size;
    printf("%d\n", size);
    printf("in profile_data:\n");

    for(sd; sd != NULL; sd = sd->next)
        printf("%d %f %s\n", sd->sensorID, sd->data, sd->state == 1?"ENABLE":"UNABLE");

    for(sd = *data, i = 0; sd != NULL; sd = sd->next, i++)
    {
        buff1[i].data = sd->data;
        buff1[i].sensorID = sd->sensorID;
        buff1[i].state = ENABLE;
    }

    for(i = 1; i < size; i++)
    {
        temp = buff1[i].data;
        sensorIDt = buff1[i].sensorID;
        for(j = i; j > 0 && buff1[j-1].data > temp; j--)
        {
            buff1[j].data = buff1[j - 1].data;
            buff1[j].sensorID = buff1[j - 1].sensorID;
        }
        buff1[j].data = temp;
        buff1[j].sensorID = sensorIDt;
    }

    for(sd = *data; sd != NULL; sd = sd->next)
    {
        for(i = 0; i < size; i++)
        {
            if(buff1[i].sensorID == sd->sensorID)
            {
                sd->sortID = i;
                break;
            }
        }
    }

    printf("after sort:\n");

    for(i = 0; i < size; i++)
        printf("%d %f %s \n", buff1[i].sensorID, buff1[i].data, buff1[i].state == 1?"ENABLE":"UNABLE");

    if(size%2)
    {
        middle = buff1[size/2].data;
        sizeup = size/2 + 1;
        printf("sizeup: %d\n", sizeup);

        if(sizeup%2)
        {
            downfour = buff1[sizeup/2].data;
            sizedown = sizeup/2 + size/2;
            upfour = buff1[sizedown].data;
            printf("odd %f %f %f\n", middle, upfour, downfour);
        }
        else
        {
            downfour = (buff1[sizeup/2].data + buff1[sizeup/2 - 1].data)/2;
            sizedown = sizeup/2 + size/2;
            upfour = (buff1[sizedown].data + buff1[sizedown - 1].data)/2;
            printf("odd %f %f %f\n", middle, upfour, downfour);
        }
    }

    else
    {
        middle = (buff1[size/2].data + buff1[size/2 - 1].data)/2;
        sizeup = size/2 + 1;
        printf("sizeup: %d\n", sizeup);
        if(sizeup%2)
        {
            downfour = buff1[sizeup/2].data;
            sizedown = sizeup/2 + size/2;
            upfour = buff1[sizedown - 1].data;
            printf("dual %f %f %f\n", middle, upfour, downfour);
        }
        else
        {
            downfour = (buff1[sizeup/2].data + buff1[sizeup/2 - 1].data)/2;
            sizedown = sizeup/2 + size/2 - 1;
            upfour = (buff1[sizedown].data + buff1[sizedown - 1].data)/2;
            printf("dual %f %f %f\n", middle, upfour, downfour);
        }
    }

    outdis = upfour - downfour;
    for(sd = *data; sd != NULL; sd = sd->next)
    {
        if(sd->data < (downfour - outdis)||sd->data > (upfour + outdis))
        {
            sd->state = 0;
            sd->outTimes++;

            for(i = 0; i < size; i++)
            {
                if(sd->sensorID == buff1[i].sensorID)
                {
                    buff1[i].state = sd->state;
                    printf("buf[%d] = %f is %s\n", buff1[i].sensorID, buff1[i].data, buff1[i].state==0?"out of data":"useful");
                }
            }
        }
    }
}

float create_fusion_data_multi(struct buf *databuf, int size)
{
    int i, j, ng = 0;
    int newdown, newup;
    float upbuf[BUFSIZE], downbuf[BUFSIZE];
    float lamda1, lamda2, average1, average2;
    float fusiondata, sumtemp;
    struct buf buffer[10];

    for(i = 0; i < size; i++)
    {
        buffer[i].data = databuf[i].data;
        buffer[i].state = databuf[i].state;
        buffer[i].sensorID = databuf[i].sensorID;
    }

    for(i = 0; i < size; i++)
    {
        if(buffer[i].state == ENABLE)
            ng++;
    }
    printf("value members: %d\n", ng);

    if(ng%2)
    {
        newdown = ng/2 + 1;
        newup = ng - newdown;
        printf("newdown: %d, newup: %d\n", newdown, newup);
        for(i = 0, j = 0; i < newdown && j < size; j++)
        {
            if(buffer[j].state == ENABLE)
            {
                downbuf[i] = buffer[j].data;
                printf("downbuf[%d] = %f = buffer[%d]\n", i, downbuf[i], j);
                i++;
            }
        }
        for(i = 0, j; i < newup && j < size; j++)
        {
            if(buffer[j].state == ENABLE)
            {
                upbuf[i] = buffer[j].data;
                printf("upbuf[%d] = %f = buffer[%d]\n", i, upbuf[i], j);
                i++;
            }
        }
    }
    else
    {
        newdown = ng/2;
        newup = ng - newdown;
        printf("newdown: %d, newup: %d\n", newdown, newup);
        for(i = 0, j = 0; i < newdown && j < size; j++)
        {
            if(buffer[j].state == ENABLE)
            {
                downbuf[i] = buffer[j].data;
                printf("downbuf[%d] = %f = buffer[%d]\n", i, downbuf[i], j);
                i++;
            }
        }
        for(i = 0, j; i < newup && j < size; j++)
        {
            if(buffer[j].state == ENABLE)
            {
                upbuf[i] = buffer[j].data;
                printf("upbuf[%d] = %f = buffer[%d]\n", i, upbuf[i], j);
                i++;
            }
        }
    }

    sumtemp = 0;
    for(i = 0; i < newdown; i++)
        sumtemp += downbuf[i];
    average1 = sumtemp/newdown;
    printf("downaver: %f\n", average1);
    sumtemp = 0;
    for(i = 0; i < newup; i++)
        sumtemp += upbuf[i];
    average2 = sumtemp/newup;
    printf("upaver: %f\n", average2);

    sumtemp = 0;
    for(i = 0; i < newdown; i++)
        sumtemp += pow((downbuf[i] - average1), 2.0);
    lamda1 = sumtemp/(newdown - 1);
    sumtemp = 0;
    for(i = 0; i < newup; i++)
        sumtemp += pow((upbuf[i] - average2), 2.0);
    lamda2 = sumtemp/(newup - 1);
    printf("%f %f\n", lamda1, lamda2);

    fusiondata = lamda2/(lamda1 + lamda2)*average1 + lamda1/(lamda1 + lamda2)*average2;

    printf("fusiondata: %f\n", fusiondata);

    return fusiondata;
}

void change_fusion_al(psingle_data *singledata)
{

}

int check_sensors(psensor_data sensordata)
{
    int ret = NOT_NEED_SINGLE;
    int count = 0;
    sensor_data *sdt;

    for(sdt = sensordata; sdt != NULL; sdt = sdt->next)
    {
        if(sdt->avalible == DISABLE)
            count++;
    }
    if(count >= 2)
    {
        ret = NEED_SINGLE;
        printf("check single\n");

    }

    return ret;
}
