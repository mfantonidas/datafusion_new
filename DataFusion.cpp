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

    //tabpic = new tabPic();
    tabpic = new tabGraph(this);
    TabWidget2->addTab(tabpic, tr("Test"));
    connect(PushButtonStart, SIGNAL(clicked()), this, SLOT(start_catch()));
    connect(PushButtonStop, SIGNAL(clicked()), this, SLOT(stop_catch()));
    lcdtimer = new QTimer(this, "lcdtimer");
    connect(lcdtimer, SIGNAL(timeout()), this, SLOT(lcd_show()));
    connect(RadioButtonAuto, SIGNAL(clicked()), this, SLOT(mode_change()));
    connect(RadioButtonManual, SIGNAL(clicked()), this, SLOT(mode_change()));
    connect(RadioButtonOne, SIGNAL(clicked()), this, SLOT(mode_change()));
    connect(RadioButtonMany, SIGNAL(clicked()), this, SLOT(mode_change()));
    connect(PushButtonChooseS, SIGNAL(clicked()), this, SLOT(sensor_choose_ok()));

    PushButtonChooseS->setDisabled(true);
    create_sensor_list();
    s = get_sensor_list();

    for(s; s!=0; )
    {
        if (s->node->avalible == 1)
        {
            itoa(s->node->sensorID, buffer);
            ListBoxSensors->insertItem(QString(s->node->name)+QString(buffer));
            qs = ListBoxSensors->text(s->node->sensorID);
        }
        s = s->next;
    }
    //lcdtimer->start(200);
}

DataFusionForm::~DataFusionForm()
{
    isStart = 0;
    s = get_sensor_list();
    del_sensor_list(&s);
    s = NULL;
}

void DataFusionForm::paint()
{
    isStart = 1;
    setBackgroundColor( QColor(255, 255, 102) );
}

void DataFusionForm::start_catch()
{
    check_radio();
    isStart = 1;
    lcdtimer->start(200);
}

void DataFusionForm::stop_catch()
{
    isStart = 0;
    lcdtimer->stop();
}

void DataFusionForm::check_radio()
{
    if (RadioButtonAuto->isChecked())
    {
        isAuto = 1;
    }
    else
        isAuto = 0;
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
    if (isStart)
    {
        LCDNumberST1->display(tempbuffer1[199]);
        LCDNumberST2->display(tempbuffer2[199]);
        LCDNumberST3->display(tempbuffer3[199]);
        LCDNumberFT->display(tempbuffer1[199]);

        LCDNumberSH1->display(humibuffer1[199]);
        LCDNumberSH2->display(humibuffer2[199]);
        LCDNumberSH3->display(humibuffer3[199]);
        LCDNumberFH->display(humibuffer1[199]);
    }
    else
    {
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

void DataFusionForm::sensor_choose_ok()
{
    QString qs;
    char buf[5];
    int i, counts;
    PSensorNode psn;

    psn = get_sensor_list();
    qs = ListBoxSensors->currentText();
    counts = ListBoxSensors->count();

    for (i = 0; i < counts; ++i)
    {
        itoa(i, buf);
        init_sensor_stat(&psn);
        if(qs == (QString("sensor") + QString(buf)))
        {
            singleSensor[i] = 1;
            set_sensor_stat(i, &psn, 1);
            TextLabelSingleS->setText(QString("sensor") + QString(buf));
            break;
        }
    }
}

tabGraph::tabGraph(QWidget *parent):
QWidget(parent)
{
    for(int i = 0; i < 200; i++)
    {
        tempbuffer1[i] = (int)(sin((i*3.14)/100)*60);
    }

    for (int i = 0; i < 200; ++i)
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
    }

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
    drawtimer->start(30);

    /*
     *  Random data for test
     * */
    //flushtimer = new QTimer(this, "flushtimer");
    //connect(flushtimer, SIGNAL(timerout()), this, SLOT(flush_test_buff()));
    //flushtimer->start(10);
}

tabGraph::tabGraph(DataFusionForm *parent)
{
    for(int i = 0; i < 200; i++)
    {
        tempbuffer1[i] = (int)(sin((i*3.14)/100)*60);
    }

    for (int i = 0; i < 200; ++i)
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
    }

    temptext = new QLabel( this, "temptext" );
    temptext->setGeometry( QRect(10, 10, 70, 10) );
    temptext->setText( tr( "Tempture:  " ) );

    tempshow = new PixTemp(this);
    tempshow->setGeometry( QRect(10, 30, 270, 120) );
    tempshow->setPalette( QPalette( QColor(192, 192, 192) ) );
    tempshow->setFrameStyle( QFrame::Panel | QFrame::Sunken );


    humitext = new QLabel( this, "humitext" );
    humitext->setGeometry( QRect(10, 160, 70, 10) );
    humitext->setText( tr( "Humidity:  " ) );

    humishow = new PixHumi(this);
    humishow->setGeometry(QRect(10, 180, 270, 120));
    humishow->setPalette( QPalette( QColor(192, 192, 192) ) );
    humishow->setFrameStyle( QFrame::Panel | QFrame::Sunken );

    drawtimer = new QTimer(this, "drawtimer");
    connect(drawtimer, SIGNAL(timeout()), this, SLOT(flushBuff()));
    drawtimer->start(30);

    /*
     *  Random data for test
     * */
    //flushtimer = new QTimer(this, "flushtimer");
    //connect(flushtimer, SIGNAL(timerout()), this, SLOT(flush_test_buff()));
    //flushtimer->start(10);
}

tabGraph::~tabGraph()
{
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
    for(int i = 0; i < 200; i++)
    {
        humibuffer2[i] = humibuffer2[i + 1];
    }
    tmp = (float)rand_r(&seed)/RAND_MAX*0.9+0.1;
    rand = (int)((tmp*10));
    humibuffer2[199] = rand;
}

// Flesh real time data for LcdNumer and graph
void tabGraph::flushBuff()
{
    int tmp = tempbuffer1[0];
    unsigned int seed;
    struct timeval now;
    float tmp1;
    int rand;
    int i=0,j;
    PSensorNode psn;
    int enableS[3] = {0, 0, 0};

    if(isStart){

        psn = get_sensor_list();
        for(psn; psn != 0;)
        {
            enableS[psn->node->sensorID] = psn->node->isEnabled;
            psn = psn->next;
        }
        //for test

        for(int i = 0; i < 200; i++)
        {
            tempbuffer1[i] = tempbuffer1[i+1];
        }
        tempbuffer1[199] = tmp;

        gettimeofday(&now, NULL);
        seed = now.tv_sec ^ now.tv_usec;
        for(int i = 0; i < 200; i++)
        {
            humibuffer1[i] = humibuffer1[i + 1];
        }
        tmp1 = (float)rand_r(&seed)/RAND_MAX*0.9+0.1;
        rand = (int)((tmp1*10));
        humibuffer1[199] = rand;
    }

        tempshow->repaint(0, 0, 270, 120);
        humishow->repaint(0, 0, 270, 120);

    //for(int i = 0; i < 200; i++)
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
    PSensorNode psn;
    int enableS[3] = {0, 0, 0};

    painterS1.setPen(blue);
    painterS2.setPen(red);
    painterS3.setPen(yellow);
    painterF.setPen(black);

    psn = get_sensor_list();
    for(psn; psn!=0;)
    {
        enableS[psn->node->sensorID] = psn->node->isEnabled;
        psn = psn->next;
    }

    for(int i = 0; i < 199; i++)
    {
        if(enableS[0]){
            beginPointS1.setX(2*i);
            beginPointS1.setY(tempbuffer1[i] + 60);
            endPointS1.setX(2*i + 1);
            endPointS1.setY(tempbuffer1[i+1] + 60);
            painterS1.drawLine(beginPointS1, endPointS1);
        }

        if (enableS[1])
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
        }
        if(enableS[0] | enableS[1] | enableS[2])
        {
            beginPointF.setX(2*i);
            beginPointF.setY(tempbuffer1[i] + 60);
            endPointF.setX(2*i + 1);
            endPointF.setY(tempbuffer1[i+1] + 60);
            painterF.drawLine(beginPointF, endPointF);
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
    PSensorNode pn;
    int enableS[3] = { 0, 0, 0,};

    painterS1.setPen(blue);
    painterS2.setPen(red);
    painterS3.setPen(yellow);
    painterF.setPen(black);

    pn = get_sensor_list();
    for(pn; pn!=0;)
    {
        enableS[pn->node->sensorID] = pn->node->isEnabled;
        pn = pn->next;
    }

    for(int i = 0; i < 199; i++)
    {
        if(enableS[0])
        {
            beginPointS1.setX(2*i);
            beginPointS1.setY(humibuffer1[i] + 12);
            endPointS1.setX(2*i + 1);
            endPointS1.setY(humibuffer1[i+1] + 12);
            painterS1.drawLine(beginPointS1, endPointS1);
        }

        if (enableS[1])
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
        }

        if(enableS[0] | enableS[1] | enableS[2])
        {
            beginPointF.setX(2*i);
            beginPointF.setY(humibuffer1[i] + 12);
            endPointF.setX(2*i + 1);
            endPointF.setY(humibuffer1[i+1] + 12);
            painterF.drawLine(beginPointF, endPointF);
        }
    }
}
