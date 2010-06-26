#ifndef DATA_FUSION_H
#define DATA_FUSION_H

#include "Data_Fusion_Base.h"
#include <qpainter.h>
#include <qlabel.h>
#include <qtabwidget.h>
#include <qtimer.h>
#include <math.h>
#include <qlcdnumber.h>
#include <qradiobutton.h>
#include <qlistbox.h>
#include "Sensors.h"
#include <stdlib.h>
#include <qlineedit.h>
//#include <QLine>

class PixTemp;
class PixHumi;
class tabPic;
class tabGraph;

static int tempbuffer1[200];
static int tempbuffer2[200];
static int tempbuffer3[200];
static int tempbufferF[200];
static int humibuffer1[200];
static int humibuffer2[200];
static int humibuffer3[200];
static int humibufferF[200];
static int isStart;

class DataFusionForm:public DataFusionBaseForm
{
    Q_OBJECT
public:
    DataFusionForm(QWidget *parent = 0, const char *name = 0, WFlags fl = 0);
    ~DataFusionForm();
    tabGraph *tabpic;
private:
    QTimer *lcdtimer;
    int isAuto;
    int isSingle;
    int isAbnormal;
    int singleSensor[3];

    PSensorNode s;

    virtual void check_radio();
public slots:
    virtual void paint();
    virtual void start_catch();
    virtual void stop_catch();
    virtual void lcd_show();
    virtual void mode_change();
    virtual void sensor_choose_ok();

protected:
//  void paintEvent(QPaintEvent *event);
};

/*class tabPic:public DataFusionBaseForm
{
    Q_OBJECT
public:
    tabPic(QWidget *parent = 0);
    ~tabPic();
  PixTemp *ppp;
};*/

class tabGraph:public QWidget
{
    Q_OBJECT
public:
    tabGraph(QWidget *parent = 0);
    tabGraph(DataFusionForm *parent = 0);
    ~tabGraph();
    PixTemp *tempshow;
    PixHumi *humishow;
    QLabel *temptext;
    QLabel *humitext;
    QLabel *tempp1;
    QLabel *tempp2;
    QLabel *tempp3;
    QLabel *tempp4;
    QLabel *tempp5;
    QLabel *humip1, *humip2, *humip3, *humip4, *humip5;
    QLabel *sect;
    QLabel *sech;
    QFrame *scale_t_x;
    QFrame *scale_t_y;
    QFrame *scale_h_x;
    QFrame *scale_h_y;
    QFrame *x1, *xh1;
    QFrame *x2, *xh2;
    QFrame *y1, *yh1;
    QFrame *y2, *yh2;
    QFrame *xs1, *xsh1;
    QFrame *xs2, *xsh2;
    QFrame *xs3, *xsh3;
    QFrame *xs4, *xsh4;
    QFrame *xs5, *xsh5;
    QFrame *xs6, *xsh6;
    QFrame *ys1, *ysh1;
    QFrame *ys2, *ysh2;
    QFrame *ys3, *ysh3;
    QFrame *ys4, *ysh4;
    QFrame *ys5, *ysh5;

//    QLine scaletem;
//    QLine scalehum;

protected slots:
    void flushBuff();

    void flush_test_buff();//for debug
private:
    QTimer* drawtimer;
/*
 *  Just for debug
 * */
    QTimer* flushtimer;
  //int buffer[200];
};

class PixTemp : public QLabel
{
    Q_OBJECT
public:
    PixTemp(QWidget *parent = 0);
    ~PixTemp();
    tabGraph* parentHandle;

protected:
    void paintEvent(QPaintEvent *);
protected slots:
//  void flushBuff();
private:
//  QFrame *
};

class PixHumi : public QLabel
{
    Q_OBJECT
public:
    PixHumi(QWidget *parent = 0);
    ~PixHumi();
    tabGraph *parentHandle;
protected:
    void paintEvent(QPaintEvent *);
};

#endif //DATA_FUSION_H
