#include <malloc.h>
#include "Sensors.h"
#include <string.h>

PSensorNode create_sensor_list()
{
    /*
     *  Test sensors and creating a sensor list
     * */
    //just for a test
    int i;
    NSensor *aSensor = NULL;
    PSensorNode psn = NULL;
    PSensorNode pstemp = NULL;
    NSensorNode *nsn = NULL;

    for(i = 0; i < 3; ++i)
    {
        aSensor = (NSensor *)malloc(sizeof(NSensor));
        aSensor->name = (char *)malloc(sizeof(char)*20);
        aSensor->isEnabled = 1;
        aSensor->isUsing = 0;
        aSensor->sensorID = i;
        aSensor->name = "a sensor";

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

    return psn;
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

void del_sensor_list(PSensorNode *list)
{
    NSensorNode *nsn;
    PSensorNode psn;
    NSensor *ns;

    psn = *list;
    while(psn)
    {
        nsn = psn;
        ns = nsn->node;
        if(ns)
            free(ns);
        psn = psn->next;
        free(nsn);
    }
    psn = NULL;
    nsn = NULL;
    ns = NULL;
}
