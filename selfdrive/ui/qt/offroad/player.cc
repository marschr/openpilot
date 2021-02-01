#include "player.hpp"

#include <QDebug>

//TODO: better rounding/ceiling on timestamps for longer videos
//TODO: make cursor scrollable
//TODO: overflow scrollable timeline
//TODO: read frames into images/pixmaps; ffmpeg?? generate + cache sprites?
//TODO: optimizations with QPixmapCache?
//TODO: render video playback inside video widget
//TODO: option to rescale video/fullscreen (scale from avcodec?)


CursorSliderStyle::CursorSliderStyle() : QProxyStyle(){
}

QRect CursorSliderStyle::subControlRect(ComplexControl cc, const QStyleOptionComplex *opt, SubControl sc, const QWidget *widget) const {
  QRect rect = QCommonStyle::subControlRect(cc, opt, sc, widget);
  if (const QStyleOptionSlider *slider = qstyleoption_cast<const QStyleOptionSlider *>(opt)) {
    // int tickSize = proxy()->pixelMetric(PM_SliderTickmarkOffset, opt, widget);
    // if (sc == SC_SliderHandle){
    //   // rect.moveTop(0); //why move top/y axis?!
    //   // QRect rectr(rect.left(), rect.top(), 10, 10);
    //   // return rectr;
    //   return rect;
    // }

    // if (sc == SC_SliderGroove){
    //   QPoint grooveCenter = slider->rect.center();
    //   const int grooveThickness = 10;
    //   rect.setHeight(grooveThickness);
    //   // QRect rectr(rect.left()+150, rect.top(), rect.width()-300, 100);
    //   // rect = rectr;
    //   rect.moveCenter(grooveCenter);
    //   qDebug() << "subControlRect groove after" << rect;
    // }
  }
  
  
  return rect;
}

int CursorSliderStyle::styleHint(StyleHint hint, const QStyleOption *opt, const QWidget *widget, QStyleHintReturn *returnData) const {
	if (hint == QStyle::SH_Slider_AbsoluteSetButtons)
	{
		return (Qt::LeftButton | Qt::MidButton);
	}

	return QProxyStyle::styleHint(hint, opt, widget, returnData);
}


void CursorSliderStyle::drawComplexControl(ComplexControl cc, const QStyleOptionComplex *opt, QPainter *p, const QWidget *widget) const {

  qDebug() << "drawComplexControlsize " << widget->size();
  if (cc == CC_Slider){
    if (const QStyleOptionSlider *slider = qstyleoption_cast<const QStyleOptionSlider *>(opt)) {      
      if (!slider) return;
      const QSlider* w = qobject_cast<const QSlider *>(widget);
      // QRect groove = proxy()->subControlRect(CC_Slider, opt, SC_SliderGroove, widget);
      // if (slider->subControls & SC_SliderGroove) {
      //   qDebug() << "groove rect: " << groove;
      //   QRect pixmapRect(groove.left(), groove.top(), groove.width(), groove.height());
      //   p->setRenderHint(QPainter::Antialiasing, true);
      //   p->setPen(Qt::NoPen);
      //   p->setBrush(Qt::darkRed);
      //   // p->drawRoundedRect(pixmapRect.adjusted(0, 10, 0, 10), 1, 1);

      //   p->drawRect(groove); //groove
        
      // }
      QRect handle = proxy()->subControlRect(CC_Slider, opt, SC_SliderHandle, widget);
      if (slider->subControls & SC_SliderHandle) {
        qDebug() << "handle rect: " << handle;
        p->setRenderHint(QPainter::Antialiasing, true);
        p->setPen(Qt::NoPen);
        p->setBrush(QColor(0xc8a71a));
        // p->drawRoundedRect(handle.adjusted(1, 1, -1, -1), 2, 2);
        // p->drawRoundedRect(handle.adjusted(0,5,90,45), 10, 10); //current timestamp
        // p->drawRoundedRect(handle.left() - (handle.width() / 2), handle.top(), 90, 45, 10, 10); //current timestamp
        // p->drawRect(handle.adjusted(43,5,3,190)); //needle
        QPointF points[3] = {
            QPointF(handle.center().x()-14, handle.top()+40),
            QPointF(handle.center().x()+16, handle.top()+40),
            QPointF(handle.center().x()+1, handle.top()+55),
        };
        p->drawConvexPolygon(points, 3);
        p->drawRoundedRect(QRect(handle.center().x()-45, handle.top(), 90, 45), 10, 10); //current timestamp
        p->drawRect(handle.center().x(), handle.top(), 3, 165); //needle

        QFont font = p->font();
        font.setPixelSize(30);
        p->setFont(font);
        p->setPen(QPen(QColor(Qt::white), 2, Qt::SolidLine, Qt::FlatCap));
        //draw 0th ts  
        QString elap = QString::number(w->value());
        p->drawText(QRect(handle.center().x()-45, handle.top()+7, 90, 35), Qt::AlignHCenter | Qt::AlignBottom, elap);
      }
      
    }
  }
}

void Cursor::checkBounds(int pos) {
  qDebug() << "Cursor checkBounds " << pos;
  if (pos < 0){
    this->setSliderPosition(0);
  }
  if (pos > _hiBound){
    this->setSliderPosition(_hiBound);
  }
}

void Cursor::resizeEvent(QResizeEvent *event){
  qDebug() << "Cursor resizeEvent " << parentWidget()->size();
  const Timeline* w = qobject_cast<const Timeline *>(parentWidget());

  qDebug() << "Cursor tl padsides " << w->_box_padSides;
  _tl_padSides = w->_box_padSides;

  _loBound = _tl_padSides;
  _hiBound = w->_box_width;

  qDebug() << "Cursor tl _hiBound " << _hiBound;

  setMinimum(-_tl_padSides); //negative min so 0 is on tl x axis origin
  setMaximum(_hiBound+_tl_padSides);
  setValue(0);
}


Cursor::Cursor(Qt::Orientation o, QWidget* parent) : QSlider(o, parent){
  qDebug() << "Cursor ctor";
  qDebug() << "Cursor ctor size" << size();

  setStyle(new CursorSliderStyle);


  
  // QHBoxLayout* layout = new QHBoxLayout;
  // layout->setContentsMargins(0,10,0,0);
  // layout->setSpacing(0);

  // QLabel* cts = new QLabel; //current timestamp
  // cts->setText(QString::number(current_ts));
  // cts->setText("12:34");
  // cts->setAlignment(Qt::AlignCenter);
  
  // int _pxRange = tline->_box_width;
  // _tl_sidePads = (width() - tline->_box_width)/2;

  // qDebug() << "Cursor ctor _pxRange" << _pxRange;

  // qDebug() << "_tl_sidePads: " << _tl_sidePads;
  
  // layout->addWidget(cts, 1 , Qt::AlignTop | Qt::AlignHCenter );
  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  // setFixedSize(_pxRange,185);
  // setLayout(layout);
  setStyleSheet(R"( background-color: transparent; font-size: 27px; font-weight: 600; )");

  connect(this,SIGNAL(valueChanged(int)),this,SLOT(checkBounds(int)));
}

// void Cursor::paintEvent(QPaintEvent *e){
  // qDebug() << "Cursor paintEvent()";
  // QPainter p(this);
  // p.setRenderHint(QPainter::Antialiasing, true);
  // p.setPen(Qt::NoPen);
  // p.setBrush(QColor(0xc8a71a));
  // p.drawRoundedRect(QRectF(0,5,90,45), 10, 10); //current timestamp
  // p.drawRect(QRectF(43,5,3,190)); //needle

  // //TODO: enable background for better readability?
  // // p.setBrush(Qt::black);
  // // p.setBrush(QColor(0,0,0,128));
  // // p.drawRoundedRect(QRectF(3,8,84,39), 7, 7);


// }

Timeline::Timeline(float duration, QWidget* player) : QFrame((QFrame*) player)
,_duration(duration) { //TODO: access duration from player
  qDebug() << "Timeline ctor";
  qDebug() << "Timeline playerhint: " << player->sizeHint();

  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
  setFixedSize(1330,_height);

  QHBoxLayout* layout = new QHBoxLayout;

  cursor = new Cursor(Qt::Horizontal, this);
  // cursor->current_ts = 12.3;

  // cursor->setMaximum(1000+_box_padSides);
  // cursor->setMinimum(-_box_padSides);
  // cursor->setValue(0);

  layout->setContentsMargins(0,0,0,0);
  layout->setSpacing(0);
  
  layout->addWidget(cursor, 0);

  setLayout(layout);

  // setStyleSheet(R"( background-color: red; )"); //TODO: remove
  // setFixedHeight(_height); //185
}

void Timeline::setThumbs(QList<QImage> thumbs){
  _thumbs = thumbs;
}

void Timeline::resizeEvent(QResizeEvent *event){
  qDebug() << "Timeline resizeEvent()";
  //set _box_padSides so that Cursor will have it's parent
  // alread measured on Cursor::showEvent()
  _box_padSides = (width() - _box_width)/2;
  // cursor->layout()->update();
}

void Timeline::paintEvent(QPaintEvent *e){
  qDebug() << "Timeline paintEvent()";
  qDebug() << "Timeline widget pe size " << size();
  QPainter p(this);

  
  // _box_width = (width() - (_box_padSides * 2));

  // qDebug() << "Timeline _box_padSides " << _box_padSides;

  // qDebug() << "Timeline _box_padSides" << _box_padSides;

  int _twidth = _box_height*1.33;
  int len = _thumbs.size();


  //TODO: calculate spacing if % > 6px;
  float next = (float)len/(_box_width/_twidth);
  int i = 0;
  float fi = 0;
  while(1){
    QImage img = _thumbs[(int)fi];
    //TODO: scale it on bitmap creation time with avscale?
    img.scaledToHeight(_box_height,Qt::SmoothTransformation);
    p.drawImage(QRect(_box_padSides + (_twidth*i), _box_padTop, _twidth, _box_height), img);
    fi = fi + next;
    i++;
    if (fi > len-1) break;
  }

  // p.setBrush(Qt::darkGreen);
  // p.drawRect(QRectF(0,0,width()-1,height()-1));


  p.setRenderHint(QPainter::Antialiasing, true);
  p.setPen(QPen(QColor(0xb2b2b2), 3, Qt::SolidLine, Qt::FlatCap));
  //TODO: round rect boxes are easy, need to figure out masking/clipping for thumbs
  // p.drawRoundedRect(QRectF(10.5, 55.5, size().width()-13, _box_height), 20, 20);
  p.drawRect(QRectF(1.5+_box_padSides, 1.5+_box_padTop, _box_width-3, _box_height));


  //TODO: FIXME: not properly calculating ruler spacing
  float spacing = _box_width / (int)_duration;
  qDebug() << "tl size: " << _box_width;
  qDebug() << "tl spacing: " << spacing;
  
  //setup time stamps painter
  QFont font = p.font();
  font.setPixelSize(25);
  p.setFont(font);
  p.setPen(QPen(QColor(0xb2b2b2), 2, Qt::SolidLine, Qt::FlatCap));
  //draw 0th ts  
  p.drawText(QRect(_box_padSides-50, 15, 100, 25), Qt::AlignHCenter | Qt::AlignBottom, "0:00");
  p.drawLine(_box_padSides, 40, _box_padSides, 50);
  
  for (int i = 5; i < ((int)_duration - 5); i+=5)
  {
    QString ts = QString::number(i) + ":00";
    p.drawText(QRect(i*spacing,15,100,25), Qt::AlignBottom | Qt::AlignCenter, ts);
    p.drawLine(50+(i*spacing),40,50+(i*spacing),50);
  }

  //TODO: properly parse float minutes into text
  p.drawText(QRect(size().width()-_box_padSides-50,15,100,25), Qt::AlignHCenter | Qt::AlignBottom, QString::number(_duration) + ":00");
  p.drawLine(size().width()-_box_padSides, 40, size().width()-_box_padSides, 50);

}

Video::Video(QWidget* parent) : QFrame(parent) {
  qDebug() << "Video ctor";

  QHBoxLayout* layout = new QHBoxLayout;
  setLayout(layout);
  setFixedSize(_width,_height);
  setStyleSheet(R"( background-color: #101010; )"); //TODO: remove

}

// void Video::paintEvent(QPaintEvent *e){
//   qDebug() << "Player paintEvent()";
//   QPainter p(this);
//   // int len = player->thumbs.size();

//   // qDebug() << "Player paintEvent() len:" << len;
//   //   QImage img = player->thumbs[1];
//   //   img.scaledToHeight(100,Qt::SmoothTransformation);
//   //   p.drawImage(QRect(i*100,0,100,100), img);
//   // }
  
  


//   // uint8_t * pFrame = frs.first()->get(1);


//   // uint8_t * pFrame = frs.first()->get(1);
//   // QPair<int,int> firstf(1,1);
//   // FrameReader *frn = frs[27];
//   // uint8_t *fptr = frn->get(1);

//   // fimg = QImage(_width,_height, QImage::Format_RGB888);
  
//   // for(int y=0;y<_height;y++)
//   //   memcpy(fimg.scanLine(y),fptr+y*_width*3,_width*3);
 
  

// }



Player::Player(QWidget* parent) : QFrame(parent){
  qDebug() << "Player ctor";
  qDebug() << "Player size: " << size();
  
  
  tline = new Timeline(38,this);
  vid = new Video(this);

  QGridLayout* main_layout = new QGridLayout(this);
  main_layout->setContentsMargins(10,10,10,10);
  main_layout->setSpacing(0);


  main_layout->addWidget(vid, 0, 0, Qt::AlignTop | Qt::AlignHCenter );
  main_layout->addWidget(tline, 0, 0, Qt::AlignBottom |  Qt::AlignHCenter);  
  vid->stackUnder(tline); //ensure vid is drawn behind timeline

  setLayout(main_layout);



  QList<QString> files = getLocalLogs("/home/marsch/Desktop/Chunk_1/b0c9d2329ad1606b_2018-08-17--12-07-08/");

  
  int start = files[0].toInt();
  int end = files[files.size()-1].toInt();

  setRoute("");
  for (size_t i = start; i <= end; i++)
  {
     addSegment(i);
  }
  
  tline->setThumbs(getThumbs(start, end));
  
}


QList<QString> Player::getLocalLogs(QString path){
  QDir dir(path);
  QFileInfoList dil = dir.entryInfoList(QDir::NoDotAndDotDot | QDir::Dirs);

  QList<QString> dirFiles;
  QList<QFileInfo>::iterator itr;  
  for (itr = dil.begin(); itr != dil.end(); ++itr)
  {
    QFileInfo file = *itr;
    qDebug() << file.fileName();
    dirFiles.append(file.fileName());
    
  }
  return dirFiles;
}

QList<QImage> Player::getThumbs(int start, int end){
  qDebug() << "getThumbs() start: " << start << " end: " << end;
  QList<QImage> thumbs;
  for (int i = start; i < end; i++) {
    
    FrameReader *frn = frs[i];
    uint8_t *fptr = frn->get(1);

    //TODO: hardcoded
    QImage fimg(1164,874, QImage::Format_RGB888);
    
    for(int y=0;y<874;y++)
      memcpy(fimg.scanLine(y),fptr+y*1164*3,1164*3);

    //TODO: scale before appending? calc scale?
    thumbs.append(fimg);
  }  
  return thumbs;
}

bool Player::setRoute(QString route){
  qDebug() << "setRoute()";
  //TODO: it's hardcoded  
  _route = QString("b0c9d2329ad1606b_2018-08-17--12-07-08");
  return false;
}

bool Player::addSegment(int i){
  qDebug() << "addSegment() " << i;
  if (frs.find(i) == frs.end()) {

    QString frn = QString("/home/marsch/Desktop/Chunk_1/%1/%2/video.hevc").arg(_route).arg(i);
    //TODO: hammering multiple calls to framereader ctor will result
    // in multiple asynchronous loader threads returning;
    // Does it matter? Use a FIFO queue maybe?
    frs.insert(i, new FrameReader(qPrintable(frn)));
    return true;
  }
  return false;
}























