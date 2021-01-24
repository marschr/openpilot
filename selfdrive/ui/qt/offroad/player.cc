#include "player.hpp"

#include <QDebug>

//TODO: better rounding/ceiling on timestamps for longer videos
//TODO: make cursor scrollable
//TODO: overflow scrollable timeline
//TODO: read frames into images/pixmaps; ffmpeg?? generate + cache sprites?
//TODO: render video playback inside video widget

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

  //TODO: enable background for better readability?
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
  p.setRenderHint(QPainter::Antialiasing, true);
  p.setPen(QPen(QColor(0xb2b2b2), 3, Qt::SolidLine, Qt::FlatCap));
  p.drawRoundedRect(QRectF(10.5, 55.5, size().width()-13, 128), 20, 20);

  float spacing = size().width() / (int)_duration;
  // qDebug() << "tl size: " << size();
  // qDebug() << "tl spacing: " << spacing;
  
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

Video::Video(QWidget* parent) : QFrame(parent){
  qDebug() << "Video ctor";

  QHBoxLayout* layout = new QHBoxLayout;
  setLayout(layout);
  setFixedSize(1000,800);
  setStyleSheet(R"( background-color: #101010; )"); //TODO: remove
}

Player::Player(QWidget* parent) : QFrame(parent){
  qDebug() << "Player ctor";
  QVBoxLayout* main_layout = new QVBoxLayout(this);
  main_layout->setContentsMargins(10,10,10,10);
  main_layout->setSpacing(10);

  
  Timeline* tline = new Timeline(38,this);
  Video* vid = new Video(this);
  

  main_layout->addWidget(vid, 0, Qt::AlignCenter);
  main_layout->addWidget(tline);

  setLayout(main_layout);
  
}
