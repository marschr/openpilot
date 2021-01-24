#include "player.hpp"

#include <QDebug>

//TODO: better rounding/ceiling on timestamps for longer videos
//TODO: scrollable timeline

Cursor::Cursor(QWidget* parent) : QWidget(parent){
  qDebug() << "Cursor ctor";

  QHBoxLayout* layout = new QHBoxLayout;
  layout->setContentsMargins(0,10,0,0);
  layout->setSpacing(0);

  QLabel* cts = new QLabel; //current timestamp
  // cts->setText(QString::number(current_ts));
  cts->setText("12:34");
  cts->setAlignment(Qt::AlignCenter);
  
  layout->addWidget(cts, 1 , Qt::AlignTop | Qt::AlignHCenter );
  setFixedSize(90,200);
  setLayout(layout);
  setStyleSheet(R"( background-color: transparent; font-size: 27px; font-weight: 600; )");
}

void Cursor::paintEvent(QPaintEvent *e){
  qDebug() << "Cursor paintEvent()";
  QPainter p(this);
  p.setRenderHint(QPainter::Antialiasing, true);
  p.setPen(Qt::NoPen);
  p.setBrush(QColor(0xc8a71a));
  p.drawRoundedRect(QRectF(0,5,90,45), 10, 10); //current timestamp
  p.drawRect(QRectF(43,5,3,190)); //needle

  // p.setBrush(Qt::black);
  // p.setBrush(QColor(0,0,0,128));
  // p.drawRoundedRect(QRectF(3,8,84,39), 7, 7);

}

Timeline::Timeline(float duration, QWidget* parent) : QWidget(parent),
_duration(duration) {
  qDebug() << "Timeline ctor";

  QGridLayout* layout = new QGridLayout;
  Cursor* cursor = new Cursor(this);
  cursor->current_ts = 12.3;

  layout->setContentsMargins(0,0,0,0);
  layout->setSpacing(0);
  layout->addWidget(cursor,0,0,Qt::AlignHCenter|Qt::AlignTop);

  // setStyleSheet(R"( background-color: red; )"); //TODO: remove

  setLayout(layout);
}

void Timeline::paintEvent(QPaintEvent *e){
  qDebug() << "Timeline paintEvent()";
  QPainter p(this);
  //New settings btn stuff
  p.setRenderHint(QPainter::Antialiasing, true);
  p.setPen(QPen(QColor(0xb2b2b2), 3, Qt::SolidLine, Qt::FlatCap));
  // p.setBrush(Qt::black);
  // origin at 1.5,1.5 because qt issues with pixel perfect borders
  p.drawRoundedRect(QRectF(10.5, 55.5, size().width()-13, 128), 20, 20);

  qDebug() << "tl size: " << size();

  float spacing = size().width() / _duration;


  qDebug() << "tl spacing: " << spacing;
  
  //setup time stamps painter
  QFont font = p.font();
  font.setPixelSize(25);
  p.setFont(font);
  p.setPen(QPen(QColor(0xb2b2b2), 2, Qt::SolidLine, Qt::FlatCap));
  //draw 0th ts  
  p.drawText(QRect(3,15,100,25), Qt::AlignLeft | Qt::AlignBottom, "0:00");
  p.drawLine(10,40,10,50);
  
  for (int i = 5; i < ((int)_duration - 5); i+=5)
  {
    QString ts = QString::number(i) + ":00";
    p.drawText(QRect(i*spacing,15,100,25), Qt::AlignBottom | Qt::AlignCenter, ts);
    p.drawLine(50+(i*spacing),40,50+(i*spacing),50);
  }

  //TODO: properly parse float minutes into text
  p.drawText(QRect(size().width()-100,15,100,25), Qt::AlignRight | Qt::AlignBottom, QString::number(_duration) + ":00");
  p.drawLine(size().width()-3,40,size().width()-3,50);

}

Player::Player(QWidget* parent) : QFrame(parent){
  qDebug() << "Player ctor";
  QVBoxLayout* main_layout = new QVBoxLayout(this);
  main_layout->setContentsMargins(10,10,10,10);
  main_layout->setSpacing(10);

  
  Timeline* tline = new Timeline(38,this);
  

  main_layout->addStretch(3);
  main_layout->addWidget(tline,1);

  

  // setFixedSize(300,300);

  setStyleSheet(R"( Player { background-color: #444444; }; )");
  setLayout(main_layout);
  
}