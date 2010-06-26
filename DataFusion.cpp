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

    tabpic = new tabGraph(this);
    TabWidget2->addTab(tabpic, tr("Graph"));
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
    s = NULL;
    //lcdtimer->start(200);*/
}

DataFusionForm::~DataFusionForm()
{
    isStart = 0;
    lcdtimer->stop();
    del_sensor_list();
}

void DataFusionForm::paint()
{
}

void DataFusionForm::start_catch()
{
    PSensorNode psn = NULL;
    char buf[10];
    QString qs;

    check_radio();
    if(isAuto)
    {
        psn = get_sensor_list();
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
        }
        LineEditMode->setText(QString("Auto, using sensors: ") + qs);
    }

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
    PSensorNode psn = NULL;
    int enableS[3] = {0, 0, 0};

    if (isStart)
    {
        psn = get_sensor_list();
        for(psn; psn != 0;)
        {
            enableS[psn->node->sensorID] = psn->node->isEnabled;
            psn = psn->next;
        }

        if (enableS[0])
        {
            LCDNumberST1->display(tempbuffer1[199]);
            LCDNumberSH1->display(humibuffer1[199]);
        }

        if (enableS[1])
        {
            LCDNumberST2->display(tempbuffer2[199]);
            LCDNumberSH2->display(humibuffer2[199]);
        }

        if (enableS[2])
        {
            LCDNumberST3->display(tempbuffer3[199]);
            LCDNumberSH3->display(humibuffer3[199]);
        }

        if (enableS[0] | enableS[1] | enableS[2])
        {
            LCDNumberFT->display(tempbufferF[199]);
            LCDNumberFH->display(humibufferF[199]);
        }
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
    init_sensor_stat(&psn);

    for (i = 0; i < counts; ++i)
    {
        itoa(i, buf);
        if(qs == (QString("sensor") + QString(buf)))
        {
            singleSensor[i] = 1;
            set_sensor_stat(i, &psn, 1);
            TextLabelSingleS->setText(QString("sensor") + QString(buf));
            LineEditMode->setText(QString("Manual, using sensor: ") + QString(buf));
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
    }

    tempp1 = new QLabel(this, "tempp1");
    tempp1->setGeometry(QRect(0, 30, 10, 10));
    tempp1->setText(tr("40"));
    tempp2 = new QLabel(this, "tempp2");
    tempp2->setGeometry(QRect(0, 50, 10, 10));
    tempp2->setText(tr("30"));
    tempp3 = new QLabel(this, "tempp3");
    tempp3->setGeometry(QRect(0, 70, 10, 10));
    tempp3->setText(tr("20"));
    tempp4 = new QLabel(this, "tempp4");
    tempp4->setGeometry(QRect(0, 90, 10, 10));
    tempp4->setText(tr("10"));
    tempp5 = new QLabel(this, "tempp5");
    tempp5->setGeometry(QRect(0, 110, 10, 10));
    tempp5->setText(tr("0"));
    sect = new QLabel(this, "sect");
    sect->setGeometry(QRect(230, 150, 40, 10));
    sect->setText(tr("time: s"));

    humip1 = new QLabel(this, "humip1");
    humip1->setGeometry(QRect(0, 175, 10, 10));
    humip1->setText(tr("60"));
    humip2 = new QLabel(this, "humip2");
    humip2->setGeometry(QRect(0, 195, 10, 10));
    humip2->setText(tr("50"));
    humip3 = new QLabel(this, "humip3");
    humip3->setGeometry(QRect(0, 215, 10, 10));
    humip3->setText(tr("40"));
    humip4 = new QLabel(this, "humip4");
    humip4->setGeometry(QRect(0, 235, 10, 10));
    humip4->setText(tr("30"));
    humip5 = new QLabel(this, "humip5");
    humip5->setGeometry(QRect(0, 255, 10, 10));
    humip5->setText(tr("20"));
    sech = new QLabel(this, "sech");
    sech->setGeometry(QRect(230, 295, 40, 10));
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

    xs1->setGeometry(QRect(51, 145, 1, 3));
    xs1->setFrameStyle(QFrame::VLine | QFrame::Plain);
    xs2->setGeometry(QRect(97, 145, 1, 3));
    xs2->setFrameStyle(QFrame::VLine | QFrame::Plain);
    xs3->setGeometry(QRect(143, 145, 1, 3));
    xs3->setFrameStyle(QFrame::VLine | QFrame::Plain);
    xs4->setGeometry(QRect(189, 145, 1, 3));
    xs4->setFrameStyle(QFrame::VLine | QFrame::Plain);
    xs5->setGeometry(QRect(235, 145, 1, 3));
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

    xsh1->setGeometry(QRect(51, 290, 1, 3));
    xsh1->setFrameStyle(QFrame::VLine | QFrame::Plain);
    xsh2->setGeometry(QRect(97, 290, 1, 3));
    xsh2->setFrameStyle(QFrame::VLine | QFrame::Plain);
    xsh3->setGeometry(QRect(143, 290, 1, 3));
    xsh3->setFrameStyle(QFrame::VLine | QFrame::Plain);
    xsh4->setGeometry(QRect(189, 290, 1, 3));
    xsh4->setFrameStyle(QFrame::VLine | QFrame::Plain);
    xsh5->setGeometry(QRect(235, 290, 1, 3));
    xsh5->setFrameStyle(QFrame::VLine | QFrame::Plain);

    ysh1->setGeometry(QRect(11, 181, 3, 1));
    ysh1->setFrameStyle(QFrame::HLine | QFrame::Plain);
    ysh2->setGeometry(QRect(11, 201, 3, 1));
    ysh2->setFrameStyle(QFrame::HLine | QFrame::Plain);
    ysh3->setGeometry(QRect(11, 221, 3, 1));
    ysh3->setFrameStyle(QFrame::HLine | QFrame::Plain);
    ysh4->setGeometry(QRect(11, 241, 3, 1));
    ysh4->setFrameStyle(QFrame::HLine | QFrame::Plain);
    ysh5->setGeometry(QRect(11, 261, 3, 1));
    ysh5->setFrameStyle(QFrame::HLine | QFrame::Plain);

    scale_t_x = new QFrame(this, "scale_t_x");
    scale_t_y = new QFrame(this, "scale_t_y");
    scale_t_x->setGeometry(QRect(5, 148, 282, 1));
    scale_t_x->setFrameStyle(QFrame::HLine | QFrame::Plain);
    scale_t_y->setGeometry(QRect(11, 16, 1, 135));
    scale_t_y->setFrameStyle(QFrame::VLine | QFrame::Plain);
    scale_h_x = new QFrame(this, "scale_h_x");
    scale_h_y = new QFrame(this, "scale_h_y");
    scale_h_x->setGeometry(QRect(5, 293, 282, 1));
    scale_h_x->setFrameStyle(QFrame::HLine | QFrame::Plain);
    scale_h_y->setGeometry(QRect(11, 161, 1, 135));
    scale_h_y->setFrameStyle(QFrame::VLine | QFrame::Plain);

    x1 = new QFrame(this, "x1");
    x2 = new QFrame(this, "x2");
    y1 = new QFrame(this, "y1");
    y2 = new QFrame(this, "y2");
    x1->setGeometry(QRect(12, 18, 1, 2));
    x1->setFrameStyle(QFrame::VLine | QFrame::Plain);
    x2->setGeometry(QRect(13, 19, 1, 1));
    x2->setFrameStyle(QFrame::VLine | QFrame::Plain);
    y1->setGeometry(QRect(284, 147, 2, 1));
    y1->setFrameStyle(QFrame::HLine | QFrame::Plain);
    y2->setGeometry(QRect(284, 146, 1, 1));
    y2->setFrameStyle(QFrame::VLine | QFrame::Plain);
    //xs1->setGeometry(QRect(12, 17, 1, 1));
    //xs1->setFrameStyle(QFrame::VLine | QFrame::Plain);

    temptext = new QLabel( this, "temptext" );
    temptext->setGeometry( QRect(20, 7, 70, 10) );
    temptext->setText( tr( "Tempture: 'C " ) );

    tempshow = new PixTemp(this);
    tempshow->setGeometry( QRect(15, 25, 270, 120) );
    tempshow->setPalette( QPalette( white/*QColor(192, 192, 192)*/ ) );
    tempshow->setFrameStyle( QFrame::Panel | QFrame::Sunken );

    humitext = new QLabel( this, "humitext" );
    humitext->setGeometry( QRect(20, 155, 70, 10) );
    humitext->setText( tr( "Humidity:  %" ) );

    humishow = new PixHumi(this);
    humishow->setGeometry(QRect(15, 170, 270, 120));
    humishow->setPalette( QPalette( white/*QColor(192, 192, 192)*/ ) );
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
    int tmp;
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
        psn = NULL;
        //for test

        if(enableS[0])
        {
            tmp = tempbuffer1[0];
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

        if (enableS[1])
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
        }

        if (enableS[0] | enableS[1] | enableS[2])
        {
            for (int i = 0; i < 200; ++i)
            {
                tempbufferF[i] = tempbufferF[i + 1];
                humibufferF[i] = humibufferF[i + 1];
            }
            tempbufferF[199] = (tempbuffer1[199] + tempbuffer2[199] + tempbuffer3[199]) / 3;
            humibufferF[199] = (humibuffer1[199] + humibuffer2[199] + humibuffer3[199]) / 3;
        }
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
    psn = NULL;

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
            beginPointF.setY(tempbufferF[i] + 60);
            endPointF.setX(2*i + 1);
            endPointF.setY(tempbufferF[i+1] + 60);
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
            beginPointF.setY(humibufferF[i] + 12);
            endPointF.setX(2*i + 1);
            endPointF.setY(humibufferF[i+1] + 12);
            painterF.drawLine(beginPointF, endPointF);
        }
    }
}
