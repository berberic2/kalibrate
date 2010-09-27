/*
 * Copyright (C) 2010 M G Berberich
 */

#include <iostream>
#include <cmath>

#include <QAbstractScrollArea>
#include <QPainter>
#include <QWheelEvent>

#include "imagelist.h"
#include "imageviewer.h"


/**
 * constructor
 *
 * @param parent parent-widget
 */
ImageWidget::ImageWidget(QWidget *parent) :
  theScale(1.0)
{
}

/**
 * constructor
 *
 * @param parent parent-widget
 */
ImageWidget::ImageWidget(const QImage &i, QWidget *parent) :
  theImage(i), theScale(1.0)
{
  update();
}

/** 
 * Set zoom/scale
 * 
 * Set the scale-parameter for the widgets painter
 *
 * @param s 
 */
void ImageWidget::scale(double s)
{
  theScale = s;
  QSize i = theImage.size();
  i *= theScale;
  setFixedSize(i);
}

/** 
 * Paint the image
 * 
 *
 *
 * @param event 
 */
void ImageWidget::paintEvent(QPaintEvent * event)
{
  QWidget::paintEvent(event);
  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing);
  painter.scale(theScale, theScale);
  painter.translate(0.5, 0.5);
  // performance? buffer scaled image?
  // draw image
  painter.drawImage(QPointF(-0.5, -0.5), theImage);

  if (theGrid.points.size() == 0) return;

  QPen p;
  p.setWidth(2);
  
  // if there is grid: draw it
  if (theGrid.isRectangular()) {
    p.setColor("yellow");
    painter.setPen(p);
    const int w = theGrid.width;
    const int h = theGrid.height;
    for(int y=1; y<w; ++y)
      for(int x=0; x<w; ++x) {
	Point2 &a = theGrid.points[y*w+x];
	Point2 &b = theGrid.points[y*w+x-w];
	Point2 d = (b-a)/(b-a).len()*7.0;
	painter.drawLine(a+d, b-d);
      }
    for(int y=0; y<w; ++y)
      for(int x=1; x<w; ++x) {
	Point2 &a = theGrid.points[y*w+x];
	Point2 &b = theGrid.points[y*w+x-1];
      Point2 d = (b-a)/(b-a).len()*7.0;
      painter.drawLine(a+d, b-d);
      }
  }

  // draw points
  p.setColor("red");
  painter.setPen(p);
  foreach(Point2 i, theGrid.points) {
    painter.drawEllipse(i, 5.0, 5.0);
  }
  painter.drawEllipse(theGrid.points[0], 9.0, 9.0);
}

/** 
 * Handle wheel-events
 * 
 * Wheel allows to zooms in/out (changes the scale)
 *
 * @param event 
 */
void ImageWidget::wheelEvent(QWheelEvent *event)
{
  if(event->orientation() == Qt::Vertical) {
    double s = theScale*pow(M_SQRT2, event->delta()/(15.0*8.0));
    if (s > 0.1 && s < 10) {
      scale(s);
      // ToDo set scroll so that center stays fixed
    }
  }
}

/** 
 * Set the image
 * 
 * Set the image that is displayed. The image gets copied
 *
 * @param i 
 */
void ImageWidget::image(const QImage &i) 
{ 
  theImage = i; 
  scale(theScale);
  update(); 
};

/** 
 * Set the grid
 * 
 * Set the grid that is displayed. The grid gets copied
 *
 * @param i 
 */
void ImageWidget::grid(const Grid &g) 
{ 
  theGrid = g; 
  update(); 
};
