/* -*- c++ -*- */
/*
 * Copyright (C) 2010 M G Berberich
 */

#include <QWidget>
#include <QScrollArea>


class QImage;

class ImageWidget : public QWidget
{
public:
  ImageWidget(QWidget *parent = 0);
  void image(const QImage *i);

  void scale(double s);
protected:
  virtual void paintEvent(QPaintEvent *event);
  virtual void wheelEvent(QWheelEvent *event);
private:
  const QImage *theImage;
  double theScale;
};

class ImageView : public QScrollArea
{
  Q_OBJECT;
public:
  ImageView(QWidget *parent = 0) : theImage(parent) { setWidget(&theImage); }
  ImageWidget &imageWidget() { return theImage; }
  const ImageWidget &imageWidget() const { return theImage; }
private:
  ImageWidget theImage;
};
