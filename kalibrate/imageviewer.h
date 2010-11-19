/* -*- c++ -*- */
/*
 * Copyright (C) 2010 M G Berberich
 */

#include <QWidget>
#include <QScrollArea>

#include "imagelist.h"

class QImage;

class ImageWidget : public QWidget
{
public:
  ImageWidget(QWidget *parent = 0);
  ImageWidget(const QImage &i, QWidget *parent = 0);
  void image(const QImage &i);
  void grid(const Plate &g);

  void scale(double s);
  double scale() const { return theScale; };

protected:
  virtual void paintEvent(QPaintEvent *event);

private:
  QImage theImage;
  Plate theGrid;
  double theScale;

  void drawAxisLabel(QPainter &painter, const Point2 &a, const Point2 &b,
	bool left, const char *label);
};

class ImageView : public QScrollArea
{
  Q_OBJECT;
public:
  ImageView(QWidget *parent = 0) : theImage(parent) {
    setWidget(&theImage);
    setAlignment(Qt::AlignCenter);
  }
  ImageWidget &imageWidget() { return theImage; }
  const ImageWidget &imageWidget() const { return theImage; }

protected:
    virtual void wheelEvent(QWheelEvent *event);

private:
  ImageWidget theImage;
};
