#ifndef DATA_FUSION_H
#define DATA_FUSION_H

#include "Data_Fusion_Base.h"
#include <qpainter.h>
#include <qlabel.h>
#include <qtabwidget.h>
#include <qtimer.h>
#include <math.h>

class PixTemp;
class PixHumi;
class tabPic;
class tabGraph;

static int buffer[200];
static int buffer2[200];

class DataFusionForm:public DataFusionBaseForm
{
	Q_OBJECT
public:
	DataFusionForm(QWidget *parent = 0, const char *name = 0, WFlags fl = 0);
	~DataFusionForm();
	int isStart;
  //tabPic *tabpic;
  tabGraph *tabpic;
public slots:
	virtual void paint(); 
protected:
//	void paintEvent(QPaintEvent *event);
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
	~tabGraph();
  PixTemp *tempshow;
  PixHumi *humishow;
  QLabel *temptext;
  QLabel *humitext;
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
