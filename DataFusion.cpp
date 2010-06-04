#include <qpushbutton.h>
#include <qlabel.h>
#include <qtopia/qpeapplication.h>
//#include <qpaintevent.h>
#include <qpen.h>
#include <qpoint.h>
#include "DataFusion.h"

class PixPaint;
static int count = 0;

DataFusionForm::DataFusionForm(QWidget *parent, const char *name, WFlags fl):
DataFusionBaseForm(parent, name, fl)
{
	isStart = 0;
  //tabpic = new tabPic();
  tabpic = new tabGraph();
  TabWidget2->addTab(tabpic, tr("Test"));
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

/*void DataFusionForm::paintEvent(QPaintEvent *event)
{
	//PixmapPainter = new PixPaint;
	//PixPaint::paintEvent(QPaintEvent *event);
}*/

/*tabPic::tabPic(QWidget *parent):
DataFusionBaseForm(parent)
{
  ppp = new PixTemp(this);
  ppp->setGeometry(10, 10, 20, 100);
  ppp->setPalette( QPalette( QColor(192, 192, 192) ) );
  ppp->setFrameStyle( QFrame::Panel | QFrame::Sunken );

}

tabPic::~tabPic()
{
}*/

tabGraph::tabGraph(QWidget *parent):
QWidget(parent)
{
  for(int i = 0; i < 200; i++)
  {
    buffer[i] = (int)(sin((i*3.14)/100)*60);
  }

  for(int i = 0; i < 101; i++)
  {
    buffer2[i] = (int)i*1.7;
  }
  for(int i = 101; i < 200; i++)
  {
    buffer2[i] = (int)((200 - i));
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
}

tabGraph::~tabGraph()
{
}

void tabGraph::flushBuff()
{
  int tmp = buffer[0];

  for(int i = 0; i < 200; i++)
  {
    buffer[i] = buffer[i+1];
  }
  buffer[199] = tmp;

  tmp = buffer2[0];
  for(int j = 0; j < 20; j ++)
  {
    tmp = buffer2[0];
    for(int i = 0; i < 200; i++)
    {
      buffer2[i] = buffer2[i+1];
    }
    buffer2[199] = tmp;
  }
  tempshow->repaint(0, 0, 270, 120);

  count++;
  if(count == 17)
  {
    count = 0;
    humishow->repaint(0, 0, 270, 120);
  }
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
    for(int i = 0; i < 199; i++)
    {
      beginPoint.setX(2*i);
      beginPoint.setY(buffer[i] + 60);
      endPoint.setX(2*i + 1);
      endPoint.setY(buffer[i+1] + 60);
      painter.drawLine(beginPoint, endPoint);
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
  QPainter painter(this);
  QPoint beginPoint1;
  QPoint endPoint1;
  painter.setPen(red);
  for(int i = 0; i < 199; i++)
  {
      beginPoint1.setX(2*i);
      beginPoint1.setY(buffer2[i] + 12);
      endPoint1.setX(2*i + 1);
      endPoint1.setY(buffer2[i+1] + 12);
      painter.drawLine(beginPoint1, endPoint1);
  }
}
