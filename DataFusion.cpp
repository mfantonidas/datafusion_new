#include <qpushbutton.h>
#include <qlabel.h>
#include <qtopia/qpeapplication.h>
//#include <qpaintevent.h>
#include <qpen.h>
#include <qpoint.h>
#include "DataFusion.h"
#include "TestData.h"
#include <stdlib.h>
#include <stdio.h>

class PixPaint;
static int count = 0;
//static int TtyFd;
static struct termios BackupTtyAttr;
static char buffer[20];
static char Char = 0;
/*static int sensorID_s = 0;
static int sensorID = 0;
static int tempv_s = 0;
static int tempv = 0, templ = 0;
static int humiv_s = 0;
static int humiv = 0, humil = 0;
static int datacount_s = 0;
static int datacountv = 0;
staint templ_s = 0, humil_s = 0;*/

//static SensorData sensordata[6];

void *arg = NULL;

static inline void WaitFdWriteable(int Fd)
{
    fd_set WriteSetFD;
    FD_ZERO(&WriteSetFD);
    FD_SET(Fd, &WriteSetFD);
    if (select(Fd + 1, NULL, &WriteSetFD, NULL, NULL) < 0) {
        perror(strerror(errno));
    }
}

static int init_serial(SerialPort port, int speed)
{
    char *dev;
    int DeviceSpeed;
    struct termios TtyAttr;
    int ByteBits = CS8;

    switch(port)
    {
        case TTYS0:
            dev = "/dev/ttyS0";
            break;
        case TTYS1:
            dev = "/dev/ttyS1";
            break;
        case TTYSAC0:
            dev = "/dev/ttySAC0";
            break;
        case TTYSAC1:
            dev = "/dev/ttySAC1";
            break;
        case TTYUSB0:
            dev = "/dev/ttyUSB0";
            break;
        case TTYUSB1:
            dev = "/dev/ttyUSB1";
            break;
    }

    fd = open(dev, O_RDWR, 0);
    if (fd < 0)
        perror("Unable to open device");
    if (fcntl(fd, F_SETFL, O_NONBLOCK) < 0)
        perror("Unable set to NONBLOCK mode");

    DeviceSpeed = get_speed(speed);

    memset(&TtyAttr, 0, sizeof(struct termios));
    TtyAttr.c_iflag = IGNPAR;
    TtyAttr.c_cflag = DeviceSpeed | HUPCL | ByteBits | CREAD | CLOCAL;
    TtyAttr.c_cc[VMIN] = 1;
    if (tcsetattr(fd, TCSANOW, &TtyAttr) < 0)
        perror("Unable to set comm port");
    /*TtyFd = open("/dev/tty", O_RDWR | O_NDELAY, 0);//
    if (TtyFd < 0)
        perror("Unable to open tty");
    TtyAttr.c_cflag = DeviceSpeed | HUPCL | ByteBits | CREAD | CLOCAL;
    if (tcgetattr(TtyFd, &BackupTtyAttr) < 0)
        perror("Unable to get tty");
    if (tcsetattr(TtyFd, TCSANOW, &TtyAttr) < 0)
    perror("Unable to set tty");*/
    /*char *dev;

    switch(port)
    {
        case TTYS0:
            dev = "/dev/ttyS0";
            break;
        case TTYS1:
            dev = "/dev/ttyS1";
            break;
        case TTYSAC0:
            dev = "/dev/ttySAC0";
            break;
        case TTYSAC1:
            dev = "/dev/ttySAC1";
            break;
        case TTYUSB0:
            dev = "/dev/ttyUSB0";
            break;
        case TTYUSB1:
            dev = "/dev/ttyUSB1";
            break;
    }
    printf("%s\n", dev);

    fd = OpenDev(dev);
    if (-1 == fd)
    { //设置数据位数
        perror("Can't Open Serial Port");
        return -1;
    }
    if (fcntl(CommFd, F_SETFL, O_NONBLOCK) < 0)
        perror("Unable set to NONBLOCK mode");

    if (fd>0)
        set_speed(fd,19200); //打开后设置波特率19200
    else
    {
        printf("Can't Open Serial Port!\n");
        exit(0);
    }
    if (set_Parity(fd, 8, 1,'n')== FALSE) //设置8，1，n 注意，这里和上面要和下位机相符才可能通信
    {
        printf("Set Parity Error\n");
        exit(1);
    }
    return 0;*/
    //printf("open tty success!\n");
}

void *thread0(void *arg)
{
    char buffeer[20];
    char stringbuffer[5];
    int sensorID_s = 0;
    int sensorID = 0;
    int tempv_s = 0;
    int tempv = 0, templ = 0;
    int humiv_s = 0;
    int humiv = 0, humil = 0;
    int datacount_s = 0;
    int datacountv = 0;
    int templ_s = 0, humil_s = 0;
    int i;
    sensorData *sensordata;
    DataFusionForm *dff = (DataFusionForm *)arg;

    sensordata = get_sensordata();
    //sds_t = creat_single_test_data(init_data, );

    for (;;) {

        fd_set ReadSetFD;

        FD_ZERO(&ReadSetFD);
        FD_SET( fd, &ReadSetFD);
        //FD_SET( TtyFd, &ReadSetFD);
        if (select(fd + 1, &ReadSetFD, NULL, NULL, NULL) < 0) {
            perror(strerror(errno));
        }
/*# define max(x,y) ( ((x) >= (y)) ? (x) : (y) )
        if (select(max(fd, TtyFd) + 1, &ReadSetFD, NULL, NULL, NULL) < 0) {
            perror(strerror(errno));
        }
        # undef max*/
        if (FD_ISSET(fd, &ReadSetFD)) {
            static int count1 = 0;
            static int read_ok = 0;

            while (read(fd, &Char, 1) == 1) {
                static int count_t = 0, count_t_l = 0, count_h = 0, count_h_l = 0;
                static int t_ok = 0, h_ok = 0;

                //WaitFdWriteable(TtyFd);

                if(Char == 32 || Char == 33 || (Char >= 48 && Char <= 57) || (Char >= 65 && Char <= 90) || (Char >= 97 && Char <= 122))
                {
                    if(Char != 'x')
                        strcat(buffeer,&Char);
                    else
                    {
                        strcat(buffeer,&Char);
                        read_ok = 1;
                    }

                }
                /*if (write(TtyFd, &Char, 1) < 0) {
                    Error(strerror(errno));
                    }*/

            }
            if(read_ok == 1)
            {
                if(sensordata->datatype == UNSTARTED)
                {
                    for (i = 0; i < 7 && buffeer[i] != 'x'; ++i)
                    {
                        if(buffeer[i] == 'a')
                            if(buffeer[i + 1] == 'v')
                            {
//                                printf("read av %c\n", buffeer[i + 2]);

                                (sensordata->sensorinfo[(buffeer[i + 2] - 49)]).sensorID = buffeer[i + 2] - 48;
                                (sensordata->sensorinfo[(buffeer[i + 2] - 49)]).isValiable = TRUE;
//                                printf("read av %d over\n", (sensordata->sensorinfo[0]).sensorID);

                            }
                    }
                    for (i = 0; i < 4 && buffeer[i] != 'x'; ++i)
                    {
                        if(buffeer[i] == 'i')
                            if(buffeer[i + 1] == 'k')
                                sensordata->info_read_ok = TRUE;
                    }
                    if(sensordata->info_read_ok == TRUE)
                    {
                        //printf("read state: %d\n", sensordata->info_read_ok);

                        for(i = 0; i < 6; i++)
                        {
//                            printf("read state: %d\n", sensordata->info_read_ok);
//                            printf("(sensordata->sensorinfo[%d]).isValiable == %s\n", i, (sensordata->sensorinfo[i]).isValiable == TRUE?"TRUE":"FALSE");


                            if((sensordata->sensorinfo[i]).isValiable == TRUE)
                            {
//                                printf("sensor:%d\n", sensordata->sensorinfo[i].sensorID);
                                if(sensordata->sensorinfo[i].isAdded != TRUE)
                                {
                                    itoa((sensordata->sensorinfo[i]).sensorID, stringbuffer);
                                    dff->ListBoxSensors->insertItem(QString("sensor")+QString(stringbuffer));
                                    (sensordata->sensorinfo[i]).isAdded = TRUE;
                                }
                                dff->PushButtonStart->setEnabled(true);
                            }
                        }
                    }

                }
                //printf ("\n\r%s\n", buffeer);
                for(i = 0; i < 20 && buffeer[i] != 'x'; i++)
                {
                    if(buffeer[i] == 'S')
                    {
                        sensorID = buffeer[i + 1] - 48;
                        if(buffeer[i + 2] == 'E' && sensordata->sensorinfo[0].isValiable == 1)
                        {
                            sensordata->sensorinfo[0].isValiable = 0;
                            isStart = 0;
                            sensordata->datatype = UNSTARTED;
                            dff->PushButtonStart->setDisabled(true);
                            dff->ListBoxSensors->removeItem(sensordata->sensorID);
                            for(i = 0; i < 6; i++)
                            {
                                sensordata->data.sd.tempdata[i] = 0;
                                sensordata->data.sd.humidata[i] = 0;
                                sensordata->sensorinfo[0].isAdded = FALSE;
                            }
                            for(i = 0; i < 140; i++)
                            {
                                temptest[i] = 0;
                                humitest[i] = 0;
                                tempbufferF[i] = 0;
                                humibufferF[i] = 0;
                            }
                            printf("sensor out!\n");
                        }
                    }
                    if(buffeer[i] == 'T')
                    {
                        datacountv = buffeer[i + 1] - 48;
                        templ = buffeer[i + 3] - 48;
                        if(templ == 0 || templ == 1)
                            tempv = buffeer[i + 4] - 48;
                        else if(templ == 2)
                            tempv = (buffeer[i + 4] - 48)*10 + buffeer[i + 5] - 48;
                        else if(templ == 3)
                            tempv = (buffeer[i + 4] - 48)*100 + (buffeer[i + 5] - 48)*10 + buffeer[i + 6] - 48;
                    }
                    if(buffeer[i] == 'H')
                    {
                        humil = buffeer[i + 3] - 48;
                        if(humil == 0 || humil == 1)
                            humiv = buffeer[i + 4] - 48;
                        else if(humil == 2)
                            humiv = (buffeer[i + 4] - 48)*10 + buffeer[i + 5] - 48;
                        else if(humil == 3)
                            humiv = (buffeer[i + 4] - 48)*100 + (buffeer[i + 5] - 48)*10 + buffeer[i + 6] - 48;
                    }
                }
                if(sensordata->datatype == SINGLE_MODE)
                {
                    static int counter = 0;
                    sensordata->data.sd.sensorID = sensorID;
                    if(tempv != 0)
                    {
                        sensordata->data.sd.tempdata[counter] = tempv;
                        temptest[139] = tempv;
                    }
                    if(humiv != 0)
                    {
                        sensordata->data.sd.humidata[counter] = humiv;
                        humitest[139] = humiv;
                    }
                    if(tempv != 0 && humiv != 0)
                    {
                        counter++;
                        printf("\n\rsensorID:%d,templ:%d,tempv:%d,humil:%d,humiv:%d,count:%d", sensorID, templ,tempv,humil, humiv, datacountv);
                    }
                    if(counter > 5)
                        counter = 0;
                }

                memset(buffeer, 0, sizeof(buffeer));
                buffeer[0] = '\0';
                read_ok = 0;
            }

        }
        /*if (FD_ISSET(TtyFd, &ReadSetFD)) {
            while (read(TtyFd, &Char, 1) == 1) {
                static int EscKeyCount = 0;
                WaitFdWriteable(fd);
                printf("%c\n", Char);

                if (write(fd, &Char, 1) < 0) {
                    perror(strerror(errno));
                }
                if (Char == '\x1b') {
                    EscKeyCount ++;
                    if (EscKeyCount >= 3)
                        goto ExitLabel;
                } else
                EscKeyCount = 0;
            }
            }*/
    }
/*ExitLabel:
    if (tcsetattr(TtyFd, TCSANOW, &BackupTtyAttr) < 0)
    perror("Unable to set tty");*/
    return 0;
    /*int ret;
    char *buf = (char *)arg;
    struct timeval aTime;
    aTime.tv_sec = 5;
    aTime.tv_usec = 0;

    while(1)
    {
        FD_ZERO(&rd);
        FD_SET(fd,&rd);
        //printf("in father!\n");

        //close(pipe_fd[0]);
        ret = select(fd+1,&rd,NULL,NULL,&aTime);

        if(ret > 0)
        {
            if(FD_ISSET(fd,&rd))
            {
                printf("in father!\n");
                bzero(buff, sizeof(buff));

                nread = read(fd,buff,112);
                if(nread > 1)
                {
                     notEmpty = 1;
                     printf("nread = %d,%s\n",nread,buff);
                     //pthread_mutex_lock(&buff_lock);
                     if(write(pipe_fd[1], buff, 115) != -1)
                         printf("write pipe success!\n");
                }
                printf("nread = %d,%s\n",nread,buff);
            }
        }
        //       close(pipe_fd[0]);
        //sleep(1);
        }*/
}

/*void *thread1(void *arg)
{
    int i;
    char *buf = (char *)arg;
    while(1)
    {
        sleep(1);
        //down_read(&wrsem);
//        close(pipe_fd[1]);
        //pthread_mutex_lock(&buff_lock);
        printf("notEmpty: %d\n", notEmpty);
        if(notEmpty)
        {
            pipe_size = read(pipe_fd[0], buffr, 115);
            printf("pipisize: %d\n", pipe_size);

            if(pipe_size > 0)
            {
                printf("pipe_size: %d\n", pipe_size);
                data_filter(buffr, sensordata, 112, sdsize);
                printf ("child:%s\n", buffr);
                for(i = 0; i < sdsize; i++)
                {
                    printf ("sensor:%d,%d: temperture: %d, humidity: %d\n", sensordata[i].sensorID, i, sensordata[i].temperture, sensordata[i].humidity);
                }
                notEmpty = 0;
            }
        }
        else
            printf("empty\n");

        //pthread_mutex_unlock(&buff_lock);

    }
    }*/

void init_pipe()
{
    if(pipe(pipe_fd) < 0)
    {
        printf ("create pipe err!\n");
        return;
    }
}

DataFusionForm::DataFusionForm(QWidget *parent, const char *name, WFlags fl):
DataFusionBaseForm(parent, name, fl)
{
    isStart = 0;
    isAbnormal = 0;
    singleSensor[0] = 0;
    singleSensor[1] = 0;
    singleSensor[2] = 0;

    char buffer[5];
    QString qs;

    tabpic = new tabGraph(this);
    TabWidget2->addTab(tabpic, tr("Graph"));
    connect(PushButtonStart, SIGNAL(clicked()), this, SLOT(start_catch()));
    connect(PushButtonStop, SIGNAL(clicked()), this, SLOT(stop_catch()));
    lcdtimer = new QTimer(this, "lcdtimer");
    serialtimer = new QTimer(this, "serialtimer");
    connect(lcdtimer, SIGNAL(timeout()), this, SLOT(lcd_show()));
    connect(serialtimer, SIGNAL(timeout()), this, SLOT(sensor_fresh()));
    //connect(serialtimer, SIGNAL(timeout()), this, SLOT(serial_catch()));
    connect(RadioButtonAuto, SIGNAL(clicked()), this, SLOT(mode_change()));
    connect(RadioButtonManual, SIGNAL(clicked()), this, SLOT(mode_change()));
    connect(RadioButtonOne, SIGNAL(clicked()), this, SLOT(mode_change()));
    connect(RadioButtonMany, SIGNAL(clicked()), this, SLOT(mode_change()));
    connect(PushButtonChooseS, SIGNAL(clicked()), this, SLOT(sensor_choose_ok()));
    connect(PushButtonFresh, SIGNAL(clicked()), this, SLOT(sensor_fresh()));

    PushButtonChooseS->setDisabled(true);
    PushButtonStart->setDisabled(true);
    PushButtonStop->setDisabled(true);
    LCDNumberFT->display("0.0");
    LCDNumberFH->display("0.0");
    //SCREEN_WITTH = ::ClientRect().Width();
    //SCREEN_HIGHT = ::ClientRect().height();

    create_sensor_list();
    /*s = get_sensor_list();
    if(s)
    printf("%s\n", s->node->name);*/

    init_sensors();

    //sensordata = get_sensordata();

    /*for(s; s!=0; )
    {
        if (s->node->avalible == 1)
        {
            itoa(s->node->sensorID, buffer);
            ListBoxSensors->insertItem(QString(s->node->name)+QString(buffer));
            qs = ListBoxSensors->text(s->node->sensorID);
        }
        s = s->next;
    }
    s = NULL;*/

    //pthread_mutex_init(&buff_lock, NULL);

    //sdsize = sizeof(sensordata)/sizeof(SensorData);

    notEmpty = 0;
    for (int i = 0; i < 200; ++i)
    {
        temptest[i] = 0;
        humitest[i] = 0;
    }
    init_serial(TTYUSB0, 19200);
    init_pipe();

    ::pthread_create(&thread[0], NULL, thread0, (void *)this);
//    ::pthread_create(&thread[1], NULL, thread1, (void *)arg);
    /*
    if(thread[0] !=0) {                   //comment4
        pthread_join(thread[0],NULL);
        printf("线程1已经结束\n");
    }
    if(thread[1] !=0) {                //comment5
        pthread_join(thread[1],NULL);
        printf("线程2已经结束\n");
        }*/

    //lcdtimer->start(200);*/
    //serialtimer->start(200);
}

DataFusionForm::~DataFusionForm()
{
    isStart = 0;
    lcdtimer->stop();
    serialtimer->stop();
    del_sensor_list();
    /*if (tcsetattr(TtyFd, TCSANOW, &BackupTtyAttr) < 0)
      perror("Unable to set tty");*/
}

void DataFusionForm::paint()
{
}

void DataFusionForm::start_catch()
{
    PSensorNode psn = NULL;
    sensorData *sd = NULL;
    char buf[10];
    QString qs;
    int count = 0;

    check_radio();
    PushButtonStop->setEnabled(true);
    if(isAuto)
    {
        sd = get_sensordata();

        for (int i = 0; i < 6; ++i)
        {
            if (sd->sensorinfo[i].isValiable == TRUE)
            {
                count++;
            }
        }
        if(count < 3)
            sd->datatype = SINGLE_MODE;

        if (sd->datatype == SINGLE_MODE)
        {
            ::write(fd, "1", 1);
            //printf("write 1\n");
            itoa(sd->sensorID + 1, buf);
            qs += QString(" ");
            qs += QString(buf);
        }
        /*psn = get_sensor_list();
        for(psn; psn != 0; )
        {
            if(psn->node->avalible)
            {
                psn->node->isEnabled = 1;
                itoa(psn->node->sensorID, buf);
                qs += QString(" ");
                qs += QString(buf);
            }
            psn = psn->next;
            }*/
        if(isSingle == 1)
            LineEditMode->setText(QString("Manual, using sensors: " + qs));
        else
            LineEditMode->setText(QString("Auto, using sensors: ") + qs);
    }

    isStart = 1;
    lcdtimer->start(200);
    serialtimer->start(90);
}

void DataFusionForm::stop_catch()
{
    isStart = 0;
    lcdtimer->stop();
    serialtimer->stop();
}

void DataFusionForm::check_radio()
{
    if (RadioButtonAuto->isChecked())
    {
        isAuto = 1;
    }
    //else
    //    isAuto = 0;

    if (RadioButtonOne->isChecked())
    {
        isSingle = 1;
    }
    else
        isSingle = 0;
}

void DataFusionForm::mode_change()
{
    if (RadioButtonAuto->isChecked())
    {
        RadioButtonOne->setDisabled(true);
        RadioButtonMany->setDisabled(true);
        PushButtonChooseS->setDisabled(true);
    }
    else if(RadioButtonManual->isChecked())
    {
        RadioButtonOne->setEnabled(true);
        RadioButtonMany->setEnabled(true);

        if (RadioButtonOne->isChecked())
            PushButtonChooseS->setEnabled(true);
        else if(RadioButtonMany->isChecked())
            PushButtonChooseS->setDisabled(true);
    }

}

void DataFusionForm::lcd_show()
{
    PSensorNode psn = NULL;
    sensorData *sd = NULL;
    int enableS[6] = {0, 0, 0, 0, 0, 0};

    if (isStart)
    {
        sd = get_sensordata();
        psn = get_sensor_list();

        for (int i = 0; i < 6; ++i)
        {
            enableS[sd->sensorinfo[i].sensorID - 1] = sd->sensorinfo[i].isValiable;
        }
        /*for(psn; psn != 0;)
        {
            enableS[psn->node->sensorID] = psn->node->isEnabled;
            psn = psn->next;
            }*/

        if (enableS[0])
        {
            LCDNumberST1->display(temptest[139]);
            LCDNumberSH1->display(humitest[139]);
        }

        if (enableS[1])
        {
            //LCDNumberST2->display(tempbuffer2[199]);
            //LCDNumberSH2->display(/*humibuffer2[199]*/78.2);
        }

        if (enableS[2])
        {
            //LCDNumberST3->display(/*tempbuffer3[199]*/25.3);
            //LCDNumberSH3->display(/*humibuffer3[199]*/73.5);
        }

        if(sd->datatype == SINGLE_MODE)
        {
            if (enableS[0] | enableS[1] | enableS[2])
            {

                LCDNumberFT->display((float)tempbufferF[139] + 0.1);
                LCDNumberFH->display(humibufferF[139]+0.1);
            }
        }
        //LCDNumberST4->display(/*tempbuffer3[199]*/23.6);
        //LCDNumberSH4->display(/*humibuffer3[199]*/74.4);
        //LCDNumberST5->display(/*tempbuffer3[199]*/22.9);
        //LCDNumberSH5->display(/*humibuffer3[199]*/74.3);
        //LCDNumberST6->display(/*tempbuffer3[199]*/24.7);
        //LCDNumberSH6->display(/*humibuffer3[199]*/76.6);
    }
    else
    {
        LCDNumberST1->display(0);
        LCDNumberSH1->display(0);
        LCDNumberFT->display("0.0");
        LCDNumberFH->display("0.0");
        /*LCDNumber2->display(0);
        LCDNumber3->display(0);
        LCDNumber4->display(0);
        LCDNumber5->display(0);

        LCDNumber2_2->display(0);
        LCDNumber3_2->display(0);
        LCDNumber4_2->display(0);
        LCDNumber5_6->display(0);*/
    }
}

void DataFusionForm::serial_catch()
{
    if(fock_fd == 0)
    {
        printf("child!\n");
    }
//    else if(fock_fd > 0)
    //      printf("father!\n");
}

void DataFusionForm::sensor_choose_ok()
{
    QString qs;
    char buf[5];
    int i, counts;
    PSensorNode psn;
    sensorData *sd;

    psn = get_sensor_list();
    sd = get_sensordata();
    qs = ListBoxSensors->currentText();
    counts = ListBoxSensors->count();
    init_sensor_stat(&psn);

    for (i = 0; i < counts; ++i)
    {
        itoa(i, buf);
        if(qs == (QString("sensor") + QString(buf)))
        {
            //singleSensor[i] = 1;
            //set_sensor_stat(i, &psn, 1);
            TextLabelSingleS->setText(QString("sensor") + QString(buf));
            LineEditMode->setText(QString("Manual, using sensor: ") + QString(buf));
            break;
        }
    }
}

void DataFusionForm::sensor_fresh()
{
    static int keepdata, count_t = 0, count_h = 0;
    sensorData *sensordata = NULL;
    float fusion_data_t, fusion_data_h;

    sensordata = get_sensordata();
    //printf("freshed\n");
    if(sensordata->datatype == SINGLE_MODE)
    {
        /*if (sensordata->data.sd.tempdata[0] != 0)
        {
            temptest[139] = sensordata->data.sd.tempdata[0];
            }*/
        //temptest[199] = sensordata->data.sd.tempdata[0];
        if(sds_t == NULL)
        {
            sds_t = creat_single_test_data(init_data, sensordata->sensorID);
        }
        if(sds_h == NULL)
        {
            sds_h = creat_single_test_data(init_data, sensordata->sensorID);
        }

        temptest[139] = sensordata->data.sd.tempdata[0];
        if(temptest[139] != 0)
        {
            sds_t->data[count_t] = (float)temptest[139];
            count_t++;
            if(count_t > 5)
                count_t = 0;
        }
        humitest[139] = sensordata->data.sd.humidata[0];
        if(humitest[139] != 0)
        {
            sds_h->data[count_h] = (float)humitest[139];
            count_h++;
            if(count_h > 5)
                count_h = 0;
        }
        if(sds_t->data[5] != 0 && sds_h->data[5] != 0)
        {
            fusion_data_t = signal_fusion(sds_t);
            fusion_data_h = signal_fusion(sds_h);
            tempbufferF[139] = fusion_data_t;
            humibufferF[139] = fusion_data_h;
            printf("fusion_data_t: %f, fusion_data_h: %f\n", fusion_data_t, fusion_data_h);
        }

        /*if (sensordata->data.sd.humidata[0] != 0)
        {
            humibuffer1[199] = sensordata->data.sd.humidata[0];
            printf("tt: %d tb: %d hb: %d\n", temptest[199], tempbuffer1[199], humibuffer1[199]);
            }*/
    }
}

tabGraph::tabGraph(QWidget *parent):
QWidget(parent)
{
    /*for(int i = 0; i < 200; i++)
    {
        tempbuffer1[i] = (int)(sin((i*3.14)/100)*60);
        }*/

    /*for (int i = 0; i < 200; ++i)
    {
        tempbuffer2[i] = 0;
    }

    for (int i = 0; i < 200; ++i)
    {
        tempbuffer3[i] = 0;
    }

    for(int i = 0; i < 200; i++)
    {
        humibuffer1[i] = 0;
    }

    for(int i = 0; i < 200; ++i)
    {
        humibuffer2[i] = 0;
    }

    for(int i = 0; i < 200; ++i)
    {
        humibuffer3[i] = 0;
        }*/

    temptext =  new QLabel( this, "temptext" );
    temptext->setGeometry( QRect(10, 10, 70, 10) );
    temptext->setText( tr( "Tempture:  " ) );

    tempshow = new PixTemp(this);
    tempshow->setGeometry( QRect(10, 30, 270, 120) );
    tempshow->setPalette( QPalette( QColor(192, 192, 192) ) );
    tempshow->setFrameStyle( QFrame::Panel | QFrame::Sunken );

    humitext =  new QLabel( this, "humitext" );
    humitext->setGeometry( QRect(10, 160, 70, 10) );
    humitext->setText( tr( "Humidity:  " ) );

    humishow = new PixHumi(this);
    humishow->setGeometry(QRect(10, 180, 270, 120));
    humishow->setPalette( QPalette( QColor(192, 192, 192) ) );
    humishow->setFrameStyle( QFrame::Panel | QFrame::Sunken );

    drawtimer = new QTimer(this, "drawtimer");
    connect(drawtimer, SIGNAL(timeout()), this, SLOT(flushBuff()));
    drawtimer->start(70);

    /*
     *  Random data for test
     * */
    //flushtimer = new QTimer(this, "flushtimer");
    //connect(flushtimer, SIGNAL(timerout()), this, SLOT(flush_test_buff()));
    //flushtimer->start(10);
}

tabGraph::tabGraph(DataFusionForm *parent)
{
    QFont tmpf("Arial",8);
    /*for(int i = 0; i < 200; i++)
    {
        temptest[i] = (int)(sin((i*3.14)/100)*60);
    }
    for(int i = 0; i < 200; i++)
    {
        humitest[i] = (int)(sin((i*3.14)/200)*60);
        }*/
    //printf("in tabGraph\n");
    /*for (int i = 0; i < 200; ++i)
    {
        tempbuffer2[i] =  (int)(sin((i*3.14)/100)*60);
        }*/

    /*for (int i = 0; i < 200; ++i)
    {
        tempbuffer3[i] = 0;
    }

    for (int i = 0; i < 200; ++i)
    {
        tempbufferF[i] = (tempbuffer1[i] + tempbuffer2[i] + tempbuffer3[i])/3;
    }

    for(int i = 0; i < 200; i++)
    {
        humibuffer1[i] = 0;
    }

    for(int i = 0; i < 200; ++i)
    {
        humibuffer2[i] = 0;
    }

    for(int i = 0; i < 200; ++i)
    {
        humibuffer3[i] = 0;
    }

    for (int i = 0; i < 200; ++i)
    {
        humibufferF[i] = (humibuffer1[i] + humibuffer2[i] + humibuffer3[i])/3;
        }*/

    colorblue = new QFrame(this, "colorblue");
    //colorblue->setGeometry(QRect(10, 310, 10, 10));
    //colorblue->setPalette( QPalette( blue/*QColor(192, 192, 192)*/ ) );
    //colorblue->setFrameStyle( QFrame::Panel | QFrame::Sunken );
    cs1 = new QLabel(this, "cs1");
    //cs1->setGeometry(QRect(25, 310, 50, 10));
    //cs1->setText(tr(": sensor1"));
    colorred = new QFrame(this, "colorred");
    //colorred->setGeometry(QRect(100, 310, 10, 10));
    //colorred->setPalette( QPalette( red/*QColor(192, 192, 192)*/ ) );
    //colorred->setFrameStyle( QFrame::Panel | QFrame::Sunken );
    cs2 = new QLabel(this, "cs2");
    //cs2->setGeometry(QRect(115, 310, 50, 10));
    //cs2->setText(tr(": sensor2"));
    coloryellow = new QFrame(this, "coloryellow");
    //coloryellow->setGeometry(QRect(10, 330, 10, 10));
    //coloryellow->setPalette( QPalette( yellow/*QColor(192, 192, 192)*/ ) );
    //coloryellow->setFrameStyle( QFrame::Panel | QFrame::Sunken );
    cs3 = new QLabel(this, "cs3");
    //cs3->setGeometry(QRect(25, 330, 50, 10));
    //cs3->setText(tr(": sensor3"));
    colorblack = new QFrame(this, "colorblack");
    //colorblack->setGeometry(QRect(100, 330, 10, 10));
    //colorblack->setPalette( QPalette( black/*QColor(192, 192, 192)*/ ) );
    //colorblack->setFrameStyle( QFrame::Panel | QFrame::Sunken );
    cf = new QLabel(this, "cf");
    //cf->setGeometry(QRect(115, 330, 50, 10));
    //cf->setText(tr(": fused"));

    tempp1 = new QLabel(this, "tempp1");
    tempp1->setGeometry(QRect(0, 30, 10, 10));
    tempp1->setFont(tmpf);
    tempp1->setText(tr("40"));
    tempp2 = new QLabel(this, "tempp2");
    tempp2->setGeometry(QRect(0, 50, 10, 10));
    tempp2->setFont(tmpf);
    tempp2->setText(tr("30"));
    tempp3 = new QLabel(this, "tempp3");
    tempp3->setGeometry(QRect(0, 70, 10, 10));
    tempp3->setFont(tmpf);
    tempp3->setText(tr("20"));
    tempp4 = new QLabel(this, "tempp4");
    tempp4->setGeometry(QRect(0, 90, 10, 10));
    tempp4->setFont(tmpf);
    tempp4->setText(tr("10"));
    tempp5 = new QLabel(this, "tempp5");
    tempp5->setGeometry(QRect(0, 110, 10, 10));
    tempp5->setFont(tmpf);
    tempp5->setText(tr("0"));
    sect = new QLabel(this, "sect");
    sect->setGeometry(QRect(190, 130, 40, 10));
    sect->setText(tr("time: s"));

    humip1 = new QLabel(this, "humip1");
    humip1->setFont(QFont("Arial",6));
    humip1->setGeometry(QRect(0, 151, 10, 10));
    humip1->setText(tr("100"));
    humip2 = new QLabel(this, "humip2");
    humip2->setFont(tmpf);
    humip2->setGeometry(QRect(0, 171, 10, 10));
    humip2->setText(tr(""));
    humip3 = new QLabel(this, "humip3");
    humip3->setFont(tmpf);
    humip3->setGeometry(QRect(0, 191, 10, 10));
    humip3->setText(tr(""));
    humip4 = new QLabel(this, "humip4");
    humip4->setFont(tmpf);
    humip4->setGeometry(QRect(0, 211, 10, 10));
    humip4->setText(tr(""));
    humip5 = new QLabel(this, "humip5");
    humip5->setFont(tmpf);
    humip5->setGeometry(QRect(0, 231, 10, 10));
    humip5->setText(tr(""));
    sech = new QLabel(this, "sech");
    sech->setGeometry(QRect(190, 251, 40, 10));
    sech->setText(tr("time: s"));

    //tempp->setFont
    xs1 = new QFrame(this, "xs1");
    xs2 = new QFrame(this, "xs2");
    xs3 = new QFrame(this, "xs3");
    xs4 = new QFrame(this, "xs4");
    xs5 = new QFrame(this, "xs5");
    ys1 = new QFrame(this, "ys1");
    ys2 = new QFrame(this, "ys2");
    ys3 = new QFrame(this, "ys3");
    ys4 = new QFrame(this, "ys4");
    ys5 = new QFrame(this, "ys5");
    xsh1 = new QFrame(this, "xsh1");
    xsh2 = new QFrame(this, "xsh2");
    xsh3 = new QFrame(this, "xsh3");
    xsh4 = new QFrame(this, "xsh4");
    xsh5 = new QFrame(this, "xsh5");
    ysh1 = new QFrame(this, "ysh1");
    ysh2 = new QFrame(this, "ysh2");
    ysh3 = new QFrame(this, "ysh3");
    ysh4 = new QFrame(this, "ysh4");
    ysh5 = new QFrame(this, "ysh5");

    xs1->setGeometry(QRect(51, 126, 1, 3));
    xs1->setFrameStyle(QFrame::VLine | QFrame::Plain);
    xs2->setGeometry(QRect(97, 126, 1, 3));
    xs2->setFrameStyle(QFrame::VLine | QFrame::Plain);
    xs3->setGeometry(QRect(143, 126, 1, 3));
    xs3->setFrameStyle(QFrame::VLine | QFrame::Plain);
    xs4->setGeometry(QRect(189, 126, 1, 3));
    xs4->setFrameStyle(QFrame::VLine | QFrame::Plain);
    xs5->setGeometry(QRect(235, 126, 1, 3));
    xs5->setFrameStyle(QFrame::VLine | QFrame::Plain);

    ys1->setGeometry(QRect(11, 36, 3, 1));
    ys1->setFrameStyle(QFrame::HLine | QFrame::Plain);
    ys2->setGeometry(QRect(11, 56, 3, 1));
    ys2->setFrameStyle(QFrame::HLine | QFrame::Plain);
    ys3->setGeometry(QRect(11, 76, 3, 1));
    ys3->setFrameStyle(QFrame::HLine | QFrame::Plain);
    ys4->setGeometry(QRect(11, 96, 3, 1));
    ys4->setFrameStyle(QFrame::HLine | QFrame::Plain);
    ys5->setGeometry(QRect(11, 116, 3, 1));
    ys5->setFrameStyle(QFrame::HLine | QFrame::Plain);

    xsh1->setGeometry(QRect(51, 250, 1, 3));
    xsh1->setFrameStyle(QFrame::VLine | QFrame::Plain);
    xsh2->setGeometry(QRect(97, 250, 1, 3));
    xsh2->setFrameStyle(QFrame::VLine | QFrame::Plain);
    xsh3->setGeometry(QRect(143, 250, 1, 3));
    xsh3->setFrameStyle(QFrame::VLine | QFrame::Plain);
    xsh4->setGeometry(QRect(189, 250, 1, 3));
    xsh4->setFrameStyle(QFrame::VLine | QFrame::Plain);
    xsh5->setGeometry(QRect(235, 250, 1, 3));
    xsh5->setFrameStyle(QFrame::VLine | QFrame::Plain);

    ysh1->setGeometry(QRect(11, 153, 3, 1));
    ysh1->setFrameStyle(QFrame::HLine | QFrame::Plain);
    ysh2->setGeometry(QRect(11, 173, 3, 1));
    ysh2->setFrameStyle(QFrame::HLine | QFrame::Plain);
    ysh3->setGeometry(QRect(11, 193, 3, 1));
    ysh3->setFrameStyle(QFrame::HLine | QFrame::Plain);
    ysh4->setGeometry(QRect(11, 213, 3, 1));
    ysh4->setFrameStyle(QFrame::HLine | QFrame::Plain);
    ysh5->setGeometry(QRect(11, 233, 3, 1));
    ysh5->setFrameStyle(QFrame::HLine | QFrame::Plain);

    scale_t_x = new QFrame(this, "scale_t_x");
    scale_t_y = new QFrame(this, "scale_t_y");
    scale_t_x->setGeometry(QRect(11, 128, 220, 1));
    scale_t_x->setFrameStyle(QFrame::HLine | QFrame::Plain);
    scale_t_y->setGeometry(QRect(11, 16, 1, 112));
    scale_t_y->setFrameStyle(QFrame::VLine | QFrame::Plain);
    scale_h_x = new QFrame(this, "scale_h_x");
    scale_h_y = new QFrame(this, "scale_h_y");
    scale_h_x->setGeometry(QRect(11, 252, 220, 1));
    scale_h_x->setFrameStyle(QFrame::HLine | QFrame::Plain);
    scale_h_y->setGeometry(QRect(11, 144, 1, 108));
    scale_h_y->setFrameStyle(QFrame::VLine | QFrame::Plain);

    x1 = new QFrame(this, "x1");
    x2 = new QFrame(this, "x2");
    y1 = new QFrame(this, "y1");
    y2 = new QFrame(this, "y2");
    x1->setGeometry(QRect(12, 18, 1, 2));
    x1->setFrameStyle(QFrame::VLine | QFrame::Plain);
    x2->setGeometry(QRect(13, 19, 1, 1));
    x2->setFrameStyle(QFrame::VLine | QFrame::Plain);
    y1->setGeometry(QRect(228, 127, 2, 1));
    y1->setFrameStyle(QFrame::HLine | QFrame::Plain);
    y2->setGeometry(QRect(228, 126, 1, 1));
    y2->setFrameStyle(QFrame::VLine | QFrame::Plain);
    xh1 = new QFrame(this, "xh1");
    xh2 = new QFrame(this, "xh2");
    yh1 = new QFrame(this, "yh1");
    yh2 = new QFrame(this, "yh2");
    xh1->setGeometry(QRect(12, 145, 1, 2));
    xh1->setFrameStyle(QFrame::VLine | QFrame::Plain);
    xh2->setGeometry(QRect(13, 146, 1, 1));
    xh2->setFrameStyle(QFrame::VLine | QFrame::Plain);
    yh1->setGeometry(QRect(228, 251, 2, 1));
    yh1->setFrameStyle(QFrame::VLine | QFrame::Plain);
    yh2->setGeometry(QRect(228, 250, 1, 1));
    yh2->setFrameStyle(QFrame::VLine | QFrame::Plain);

    //xs1->setGeometry(QRect(12, 17, 1, 1));
    //xs1->setFrameStyle(QFrame::VLine | QFrame::Plain);

    temptext = new QLabel( this, "temptext" );
    temptext->setGeometry( QRect(20, 7, 70, 10) );
    temptext->setText( tr( "Temperature:'C " ) );

    tempshow = new PixTemp(this);
    tempshow->setGeometry( QRect(15, 25, 210, 100) );
    tempshow->setPalette( QPalette( white/*QColor(192, 192, 192)*/ ) );
    tempshow->setFrameStyle( QFrame::Panel | QFrame::Sunken );

    humitext = new QLabel( this, "humitext" );
    humitext->setGeometry( QRect(20, 140, 70, 10) );
    humitext->setText( tr( "Humidity:  %" ) );

    humishow = new PixHumi(this);
    humishow->setGeometry(QRect(15, 150, 210, 100));
    humishow->setPalette( QPalette( white/*QColor(192, 192, 192)*/ ) );
    humishow->setFrameStyle( QFrame::Panel | QFrame::Sunken );

    drawtimer = new QTimer(this, "drawtimer");
    connect(drawtimer, SIGNAL(timeout()), this, SLOT(flushBuff()));
    drawtimer->start(100);

    /*
     *  Random data for test
     * */
    //flushtimer = new QTimer(this, "flushtimer");
    //connect(flushtimer, SIGNAL(timerout()), this, SLOT(flush_test_buff()));
    //flushtimer->start(10);
}

tabGraph::~tabGraph()
{
    drawtimer->stop();
}

void tabGraph::flush_test_buff()
{
    //////////
    unsigned int seed;
    struct timeval now;
    float tmp;
    int rand;
    int i=0,j;
    gettimeofday(&now, NULL);
    seed = now.tv_sec ^ now.tv_usec;
    for(int i = 0; i < 139; i++)
    {
        humibuffer2[i] = humibuffer2[i + 1];
    }
    tmp = (float)rand_r(&seed)/RAND_MAX*0.9+0.1;
    rand = (int)((tmp*10));
    humibuffer2[199] = rand;

    //printf("out of tabGraph\n");
}

// Flesh real time data for LcdNumer and graph
void tabGraph::flushBuff()
{
    int tmp;
    unsigned int seed;
    struct timeval now;
    float tmp1;
    int rand;
    int i=0,j;
    static int s1_showed = 0, f_showed = 0;
    int ready = 0, res;
    //PSensorNode psn;
    sensorData *sd;
    int enableS[6] = {0, 0, 0, 0, 0, 0};

    if(isStart){

        sd = get_sensordata();

        for (int i = 0; i < 6; ++i)
        {
            enableS[i] = sd->sensorinfo[i].isValiable;
        }
        /*for(psn; psn != 0;)
        {
            enableS[psn->node->sensorID] = psn->node->isEnabled;
            psn = psn->next;
            }*/
        //psn = NULL;
        //for test

        if(enableS[0])
        {
            if(s1_showed == 0)
            {
                s1_showed = 1;
                colorblue->setGeometry(QRect(80, 255, 10, 10));
                colorblue->setPalette( QPalette( blue/*QColor(192, 192, 192)*/ ) );
                colorblue->setFrameStyle( QFrame::Panel | QFrame::Sunken );
                cs1->setGeometry(QRect(91, 255, 50, 10));
                cs1->setText(tr(": sensor1"));
            }
            tmp = temptest[139];
            for(int i = 0; i < 139; i++)
            {
                //temptest[i] = temptest[i+1];
                temptest[i] = temptest[i+1];
            }
            //temptest[139] = tmp;
            //printf("temptest[199]: %d\n", temptest[199]);
            //temptest[199] = tmp;

            for(int i = 0; i < 139; i++)
            {
                humitest[i] = humitest[i + 1];
            }
            //printf("humitest[199]: %d\n", humitest[139]);

        }

    /*if (enableS[1])
        {
            gettimeofday(&now, NULL);
            seed = now.tv_sec ^ now.tv_usec;
            for(int i = 0; i < 200; i++)
            {
                tempbuffer2[i] = tempbuffer2[i + 1];
            }
            tmp1 = (float)rand_r(&seed)/RAND_MAX*0.9+0.1;
            rand = (int)((tmp1*10));
            tempbuffer2[199] = rand;

            gettimeofday(&now, NULL);
            seed = now.tv_sec ^ now.tv_usec;
            for(int i = 0; i < 200; i++)
            {
                humibuffer2[i] = humibuffer2[i + 1];
            }
            tmp1 = (float)rand_r(&seed)/RAND_MAX*0.9+0.1;
            rand = (int)((tmp1*10));
            humibuffer2[199] = rand;
        }

        if (enableS[2])
        {
            gettimeofday(&now, NULL);
            seed = now.tv_sec ^ now.tv_usec;
            for(int i = 0; i < 200; i++)
            {
                tempbuffer3[i] = tempbuffer3[i + 1];
            }
            tmp1 = (float)rand_r(&seed)/RAND_MAX*0.9+0.1;
            rand = (int)((tmp1*10));
            tempbuffer3[199] = rand;

            gettimeofday(&now, NULL);
            seed = now.tv_sec ^ now.tv_usec;
            for(int i = 0; i < 200; i++)
            {
                humibuffer3[i] = humibuffer3[i + 1];
            }
            tmp1 = (float)rand_r(&seed)/RAND_MAX*0.9+0.1;
            rand = (int)((tmp1*10));
            humibuffer3[199] = rand;
            }*/

        if (enableS[0] | enableS[1] | enableS[2])
        {
            if(f_showed == 0)
            {
                f_showed = 1;
                colorblack->setGeometry(QRect(10, 255, 10, 10));
                colorblack->setPalette( QPalette( black/*QColor(192, 192, 192)*/ ) );
                colorblack->setFrameStyle( QFrame::Panel | QFrame::Sunken );
                cf->setGeometry(QRect(21, 255, 50, 10));
                cf->setText(tr(": fused"));
            }
            for (int i = 0; i < 139; ++i)
            {
                tempbufferF[i] = tempbufferF[i + 1];
                humibufferF[i] = humibufferF[i + 1];
            }
        }
    }

    tempshow->repaint(0, 0, 270, 120);
    humishow->repaint(0, 0, 270, 120);

}

PixTemp::PixTemp(QWidget *parent):
QLabel(parent)
{

}

PixTemp::~PixTemp()
{}

void PixTemp::paintEvent(QPaintEvent *event)
{
    QLabel::paintEvent(event);
    erase(0, 0, 270, 120);
    QPainter painterS1(this);
    QPainter painterS2(this);
    QPainter painterS3(this);
    QPainter painterF(this);
    QPoint beginPointS1;
    QPoint endPointS1;
    QPoint beginPointS2;
    QPoint endPointS2;
    QPoint beginPointS3;
    QPoint endPointS3;
    QPoint beginPointF;
    QPoint endPointF;
    //PSensorNode psn;
    sensorData *sd;
    int enableS[6] = {0, 0, 0, 0, 0, 0};

    painterS1.setPen(blue);
    painterS2.setPen(red);
    painterS3.setPen(yellow);
    painterF.setPen(black);

    sd = get_sensordata();
    //psn = get_sensor_list();

    for (int i = 0; i < 6; ++i)
    {
        enableS[i] = sd->sensorinfo[i].isValiable;
    }

    /*for(psn; psn!=0;)
    {
        enableS[psn->node->sensorID] = psn->node->isEnabled;
        psn = psn->next;
    }
    psn = NULL;*/

    if(isStart == 1)
    {
        for(int i = 0; i < 139; i++)
        {
            if(enableS[0])
            {
                //printf("enableS[0]");
                beginPointS1.setX(2*i);
                beginPointS1.setY(91 - temptest[i]*2);
                endPointS1.setX(2*i + 1);
                endPointS1.setY(91 - temptest[i + 1]*2);
                painterS1.drawLine(beginPointS1, endPointS1);
//            printf("beginY %d, endY %d\n", 91 - temptest[i]*2, )

            }

            /*if (enableS[1])
              {
              beginPointS2.setX(2*i);
              beginPointS2.setY(tempbuffer2[i] + 60);
              endPointS2.setX(2*i + 1);
              endPointS2.setY(tempbuffer2[i+1] + 60);
              painterS2.drawLine(beginPointS2, endPointS2);
              }

              if (enableS[2])
              {
              beginPointS3.setX(2*i);
              beginPointS3.setY(tempbuffer3[i] + 70);
              endPointS3.setX(2*i + 1);
              endPointS3.setY(tempbuffer3[i+1] + 70);
              painterS3.drawLine(beginPointS3, endPointS3);
              }*/
            if(enableS[0] | enableS[1] | enableS[2])
            {
                beginPointF.setX(2*i);
                beginPointF.setY(91 - tempbufferF[i]*2);
                endPointF.setX(2*i + 1);
                endPointF.setY(91 - tempbufferF[i+1]*2);
                painterF.drawLine(beginPointF, endPointF);
            }

        }
    }
}

PixHumi::PixHumi(QWidget *parent):
QLabel(parent)
{}

PixHumi::~PixHumi()
{}

void PixHumi::paintEvent(QPaintEvent *event)
{
    QLabel::paintEvent(event);
    erase(0, 0, 270, 120);
    QPainter painterS1(this);
    QPainter painterS2(this);
    QPainter painterS3(this);
    QPainter painterF(this);
    QPoint beginPointS1;
    QPoint endPointS1;
    QPoint beginPointS2;
    QPoint endPointS2;
    QPoint beginPointS3;
    QPoint endPointS3;
    QPoint beginPointF;
    QPoint endPointF;
    //PSensorNode pn;
    sensorData *sd;
    int enableS[6] = { 0, 0, 0, 0, 0, 0};

    painterS1.setPen(blue);
    painterS2.setPen(red);
    painterS3.setPen(yellow);
    painterF.setPen(black);

    sd = get_sensordata();
    //pn = get_sensor_list();

    for (int i = 0; i < 6; ++i)
    {
        enableS[i] = sd->sensorinfo[i].isValiable;
    }

    /*for(pn; pn!=0;)
    {
        enableS[pn->node->sensorID] = pn->node->isEnabled;
        pn = pn->next;
        }*/

    if(isStart == 1)
    {
        for(int i = 0; i < 139; i++)
        {
            if(enableS[0])
            {
                //printf("enableS[0]\n");
                beginPointS1.setX(2*i);
                beginPointS1.setY(82 - humitest[i]*0.65);
                endPointS1.setX(2*i + 1);
                endPointS1.setY(82 - humitest[i + 1]*0.65);
                painterS1.drawLine(beginPointS1, endPointS1);
            }

            /*if (enableS[1])
              {
              beginPointS2.setX(2*i);
              beginPointS2.setY(humibuffer2[i] + 12);
              endPointS2.setX(2*i + 1);
              endPointS2.setY(humibuffer2[i+1] + 12);
              painterS2.drawLine(beginPointS2, endPointS2);
              }

              if (enableS[2])
              {
              beginPointS3.setX(2*i);
              beginPointS3.setY(humibuffer3[i] + 20);
              endPointS3.setX(2*i + 1);
              endPointS3.setY(humibuffer3[i+1] + 20);
              painterS3.drawLine(beginPointS3, endPointS3);
              }*/

            if(enableS[0] | enableS[1] | enableS[2])
            {
                beginPointF.setX(2*i);
                beginPointF.setY(82 - humibufferF[i]*0.65);
                endPointF.setX(2*i + 1);
                endPointF.setY(82 - humibufferF[i+1]*0.65);
                painterF.drawLine(beginPointF, endPointF);
            }
        }
    }
}
