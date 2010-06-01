#include <qpushbutton.h>
#include <qlabel.h>
#include <qtopia/qpeapplication.h>
//#include <qpaintevent.h>
#include <qpen.h>
#include <qpoint.h>
#include "DataFusion.h"

class PixPaint;

DataFusionForm::DataFusionForm(QWidget *parent, const char *name, WFlags fl):
DataFusionBaseForm(parent, name, fl)
{
	isStart = 0;
  //tabpic = new tabPic();
  tabpic = new tab1();
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

tab1::tab1(QWidget *parent):
QWidget(parent)
{
  temptext =  new QLabel( this, "temptext" );
  temptext->setGeometry( 10, 10, 70, 10 );
  temptext->setText( tr( "Tempture:  " ) );

  tempshow = new PixTemp(this);
  tempshow->setGeometry(10, 30, 270, 120);
  tempshow->setPalette( QPalette( QColor(192, 192, 192) ) );
  tempshow->setFrameStyle( QFrame::Panel | QFrame::Sunken );

  humitext =  new QLabel( this, "humitext" );
  humitext->setGeometry( 10, 160, 70, 10 );
  humitext->setText( tr( "Humidity:  " ) );

  humishow = new PixHumi(this);
  humishow->setGeometry(10, 180, 270, 120);
  humishow->setPalette( QPalette( QColor(192, 192, 192) ) );
  humishow->setFrameStyle( QFrame::Panel | QFrame::Sunken );
}

tab1::~tab1()
{
}

PixTemp::PixTemp(QWidget *parent):
QLabel(parent)
{}

PixTemp::~PixTemp()
{}

void PixTemp::paintEvent(QPaintEvent *event)
{
    QLabel::paintEvent(event);
    QPainter painter(this);
    QPoint start(0,0);
    QPoint end(50,50);
    QPen pen(qRgb(255,0,0));
    painter.setPen(pen);
    painter.drawLine(start, end);
}

PixHumi::PixHumi(QWidget *parent):
QLabel(parent)
{}

PixHumi::~PixHumi()
{}

void PixHumi::paintEvent(QPaintEvent *event)
{
    QLabel::paintEvent(event);
    QPainter painter(this);
    QPoint start(50,0);
    QPoint end(50,50);
    QPen pen(qRgb(0,255,0));
    painter.setPen(pen);
    painter.drawLine(start, end);
}
