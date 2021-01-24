#pragma once

#include <QtWidgets>


class Cursor : public QWidget {
  Q_OBJECT

public:
  Cursor(QWidget* parent = 0);

protected:
  void paintEvent(QPaintEvent*) override;
};

class Timeline : public QWidget {
  Q_OBJECT

public:
  Timeline(float duration, QWidget* parent = 0);

protected:
  void paintEvent(QPaintEvent*) override;

private:
  float _height = 100;
  float _duration = 10.0;
};

class Player: public QFrame {
  Q_OBJECT

public:
  Player(QWidget* parent = 0);

  
};