#pragma once

#include <QtWidgets>

#include "FrameReader.hpp"


class CursorSliderStyle : public QProxyStyle {
public:
  CursorSliderStyle();
  void drawComplexControl(ComplexControl cc, const QStyleOptionComplex *opt, QPainter *p, const QWidget *widget = nullptr) const;
  QRect subControlRect(ComplexControl cc, const QStyleOptionComplex *opt, SubControl sc, const QWidget *widget) const;
  int styleHint(StyleHint hint, const QStyleOption *opt = nullptr, const QWidget *widget = nullptr, QStyleHintReturn *returnData = nullptr) const;

};


class Cursor : public QSlider {
  Q_OBJECT

public:
  Cursor(Qt::Orientation o, QWidget* parent = 0);
  float current_ts = 12.3;
  int _loBound = -1;
  int _hiBound = -1;
  int _tl_padSides = -1;

public slots:
  void checkBounds(int val);
  // void updateBounds(QSize parentSize);

protected:
  void resizeEvent(QResizeEvent *event) override;
//   void paintEvent(QPaintEvent*) override;
};


class Timeline : public QFrame {
  Q_OBJECT

public:
  Timeline(float duration, QWidget* parent = 0);
  Cursor* cursor = 0;
  int getSize(); //need to wait till paint

  float _height = 200;
  float _duration = 10.0;

  float _box_height = 100;
  float _box_width = 1200;
  float _box_padTop = 55;
  float _box_padSides = -1;

  void setThumbs(QList<QImage> thumbs);

private:
  QList<QImage> _thumbs;

protected:
  void paintEvent(QPaintEvent*) override;
  void resizeEvent(QResizeEvent *event) override;

};





class Video: public QFrame { //TODO: maybe QWidget
  Q_OBJECT

public:
  Video(QWidget* parent = 0);

  int _width = 1164;
  int _height = 800;

  QImage fimg;

// protected:
//   void paintEvent(QPaintEvent*) override;

  
};



class Player: public QFrame {
  Q_OBJECT

public:
  Player(QWidget* parent = 0);

  QString _route;

  QList<QImage> getThumbs(int start, int end);

  QList<QString> getLocalLogs(QString path);

  QList<QImage> thumbs;

  bool setRoute(QString route);
  bool addSegment(int i);
  // bool peekSegment(int i); //only reads enough info for painting UI

  // QMap<int, LogReader*> lrs;
  QMap<int, FrameReader*> frs;

  int _tl_height;

  Timeline* tline;
  Video* vid;

signals:
  void updateTimeline(QSize psize);
  
};



