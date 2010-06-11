#include <qpushbutton.h>
#include <qlabel.h>
#include <qtopia/qpeapplication.h>
//#include <qpaintevent.h>
#include <qpen.h>
#include <qpoint.h>
#include "DataFusion.h"
#include "TestData.h"

class PixPaint;
static int count = 0;

DataFusionForm::DataFusionForm(QWidget *parent, const char *name, WFlags fl):
DataFusionBaseForm(parent, name, fl)
{
    isStart = 0;
    //tabpic = new tabPic();
    tabpic = new tabGraph(this);
    TabWidget2->addTab(tabpic, tr("Test"));
    connect(PushButtonStart, SIGNAL(clicked()), this, SLOT(start_catch()));
    connect(PushButtonStop, SIGNAL(clicked()), this, SLOT(stop_catch()));
    lcdtimer = new QTimer(this, "lcdtimer");
    connect(lcdtimer, SIGNAL(timeout()), this, SLOT(lcd_show()));
    connect(RadioButtonAuto, SIGNAL(clicked()), this, SLOT(mode_change()));
    connect(RadioButtonManual, SIGNAL(clicked()), this, SLOT(mode_change()));
    //lcdtimer->start(200);
}

DataFusionForm::~DataFusionForm()
{
	isStart = 0;
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
    }
    else
    {
        RadioButtonOne->setEnabled(true);
        RadioButtonMany->setEnabled(true);
    }
}

void DataFusionForm::lcd_show()
{
    if (isStart)
    {
        LCDNumber2->display(tempbuffer1[199]);
        LCDNumber3->display(tempbuffer2[199]);
        LCDNumber4->display(tempbuffer3[199]);
        LCDNumber5->display(tempbuffer1[199]);

        LCDNumber2_2->display(humibuffer1[199]);
        LCDNumber3_2->display(humibuffer2[199]);
        LCDNumber4_2->display(humibuffer3[199]);
        LCDNumber5_6->display(humibuffer1[199]);
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

void tabGraph::flushBuff()
{
    int tmp = tempbuffer1[0];
    unsigned int seed;
    struct timeval now;
    float tmp1;
    int rand;
    int i=0,j;

    if(isStart){
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
    QPainter painter(this);
    QPoint beginPoint;
    QPoint endPoint;
    painter.setPen(blue);

    //if(isStart){
        for(int i = 0; i < 199; i++)
        {
            beginPoint.setX(2*i);
            beginPoint.setY(tempbuffer1[i] + 60);
            endPoint.setX(2*i + 1);
            endPoint.setY(tempbuffer1[i+1] + 60);
            painter.drawLine(beginPoint, endPoint);
        }
        //}
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
    QPainter painter(this);
    QPoint beginPoint1;
    QPoint endPoint1;
    painter.setPen(red);

    //if (isStart)
    //{
        for(int i = 0; i < 199; i++)
        {
            beginPoint1.setX(2*i);
            beginPoint1.setY(humibuffer1[i] + 12);
            endPoint1.setX(2*i + 1);
            endPoint1.setY(humibuffer1[i+1] + 12);
            painter.drawLine(beginPoint1, endPoint1);
        }
        //}
}
