#ifndef DATA_FUSION_H
#define DATA_FUSION_H

#include "Data_Fusion_Base.h"
#include <qpainter.h>
#include <qlabel.h>
#include <qtabwidget.h>
class PixTemp;
class PixHumi;
class tabPic;
class tab1;

class DataFusionForm:public DataFusionBaseForm
{
	Q_OBJECT
public:
	DataFusionForm(QWidget *parent = 0, const char *name = 0, WFlags fl = 0);
	~DataFusionForm();
	int isStart;
  //tabPic *tabpic;
  tab1 *tabpic;
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

class tab1:public QWidget
{
	Q_OBJECT
public:
	tab1(QWidget *parent = 0);
	~tab1();
  PixTemp *tempshow;
  PixHumi *humishow;
  QLabel *temptext;
  QLabel *humitext;
};

class PixTemp : public QLabel
{
	Q_OBJECT
public:
	PixTemp(QWidget *parent = 0);
	~PixTemp();
protected:
	void paintEvent(QPaintEvent *);
};

class PixHumi : public QLabel
{
	Q_OBJECT
public:
	PixHumi(QWidget *parent = 0);
	~PixHumi();
protected:
	void paintEvent(QPaintEvent *);
};

#endif //DATA_FUSION_H
