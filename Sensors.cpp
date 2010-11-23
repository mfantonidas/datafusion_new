#include <malloc.h>
#include "Sensors.h"
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <sys/time.h>
#include <time.h>
#include <signal.h>
#include <stdio.h>

static PSensorNode psensornode;
static sensorData sensordata;

void init_sensors()
{
    int i;

    for(i = 0; i < 6; i++)
    {
        sensordata.data.sd.tempdata[i] = 0;
        sensordata.data.md[i].tempdata = 0;
        sensordata.data.sd.humidata[i] = 0;
        sensordata.data.md[i].humidata = 0;
        sensordata.sensorinfo[i].sensorID = 0;
        sensordata.sensorinfo[i].isValiable = 0;
        sensordata.sensorinfo[i].isAdded = 0;
        sensordata.sensorCount = 0;
        sensordata.datatype = UNSTARTED;
    }
}

sensorData *get_sensordata()
{
    return &sensordata;
}

void create_sensor_list()
{
    /*
     *  Test sensors and creating a sensor list
     * */
    //just for a test
    int i;
    int fd;
    int count;
    int res, ready = 0;
    char buf[255];
    NSensor *aSensor = NULL;
    PSensorNode psn = NULL;
    PSensorNode pstemp = NULL;
    NSensorNode *nsn = NULL;

    /*fd = open_port(3);
    if(fd <= 0)
        return;
    if(set_opt(fd, 19200, 8, 'N', 1) <= 0)
        return;
        printf("open sucessful!\n");

    while(!ready)
    {
        res = read(fd, buf, 255);
        if(res)
        {
            buf[res] = '\0';
            for(i = 0; i < res; i++)
            {
                if(buf[i] == 'N')
                {
                    count = buf[i + 1] - 48;
                    printf("%d\n", count);
                    ready = 1;
                    break;
                }
            }
        }
        usleep(100000);
    }

    close(fd);*/

    for(i = 0; i < 3/*count*/; ++i)
    {
        aSensor = (NSensor *)malloc(sizeof(NSensor));
//        aSensor->name = (char *)malloc(sizeof(char)*20);
        aSensor->isEnabled = 0;
        aSensor->avalible = 1;
        aSensor->isUsing = 0;
        aSensor->sensorID = i;
        aSensor->name = "sensor";

        nsn = (NSensorNode *)malloc(sizeof(NSensorNode));
        nsn->node = aSensor;
        nsn->next = NULL;
        if (psn == NULL)
        {
            psn = nsn;
            pstemp = psn;
        }
        pstemp->next = nsn;
        pstemp = pstemp->next;
        nsn = nsn->next;
    }

    psensornode = psn;
//    return psn;
}

PSensorNode get_sensor_list()
{
    return psensornode;
}

char *get_sensor_name(int sensorID, PSensorNode list)
{
    char *name;
    int i;
    NSensorNode *nsn;

    nsn = list;
    while(nsn)
    {
        if (nsn->node->sensorID == sensorID)
        {
            name = nsn->node->name;
            return name;
        }
        nsn = nsn->next;
    }
    return NULL;
}

void set_sensor_stat(int sensorID, PSensorNode *list, int stat)
{
    int i;
    NSensorNode *nsn;

    nsn = *list;
    while(nsn)
    {
        if ((nsn)->node->sensorID == sensorID)
        {
            nsn->node->isEnabled = stat;
            return;
        }
        nsn = nsn->next;
    }
    return;
}

int get_sensor_stat(int sensorID, PSensorNode list)
{
    int i;
    NSensorNode *nsn;
    int stat;

    nsn = list;
    while(nsn)
    {
        if(nsn->node->sensorID == sensorID)
        {
            stat = nsn->node->isEnabled;
            return stat;
        }
        nsn = nsn->next;
    }
    return NULL;
}

void init_sensor_stat(PSensorNode *list)
{
    NSensorNode *nsn;

    nsn = *list;
    while(nsn)
    {
        nsn->node->isEnabled = 0;
        nsn = nsn->next;
    }
}

void set_sensor_mode(int sensorID, PSensorNode *list, int mode)
{
    int i;
    NSensorNode *nsn;

    nsn = *list;
    while(nsn)
    {
        if(nsn->node->sensorID == sensorID)
        {
            nsn->node->isUsing = mode;
            return;
        }
        nsn = nsn->next;
    }
    return;
}

int get_sensor_mode(int sensorID, PSensorNode list)
{
    int i;
    NSensorNode *nsn;
    int mode;

    nsn = list;
    while(nsn)
    {
        if (nsn->node->sensorID == sensorID)
        {
            mode = nsn->node->isUsing;
            return mode;
        }
        nsn = nsn->next;
    }
    return NULL;
}

/*void del_sensor_list(PSensorNode *list)
{
    NSensorNode *nsn;
    PSensorNode psn;
    NSensor *ns;

    psn = *list;
    while(psn)
    {
        nsn = psn;
        ns = nsn->node;
//        if(ns)
//        {
            free(ns);
//        }
        psn = psn->next;
        free(nsn);
    }
    }*/

void del_sensor_list()
{
    NSensorNode *nsn = NULL;
    for(psensornode; psensornode != 0; )
    {
        nsn = psensornode;
        if(nsn->node)
            free(nsn->node);
        psensornode = psensornode->next;
        free(nsn);
        nsn = NULL;
    }
    psensornode = NULL;
}

void itoa(int i, char *string)
{
    int power, j;

    j = i;
    for(power = 1; j >= 10; j /= 10)
        power *= 10;
    for(; power > 0; power /= 10)
    {
        *string ++ = '0' + i/power;
        i %= power;
    }
    *string = '\0';
}
