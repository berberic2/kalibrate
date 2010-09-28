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

inline QPointF toQP(const Point2 &p) { return QPointF(p.x, p.y); }

static void drawAxisLabel(QPainter &painter, 
      const Point2 &a, const Point2 &b, 
      bool left, const char *label)
{
  Point2 d = normalize(b-a);
  QRect r = QFontMetrics(painter.font()).boundingRect(label);
  Point2 c;
  
  if (left) {
    Point2 n(d.y, -d.x);
    if (d.x > 0) 
      if (d.y > 0) 
	c = a+10.0*d+2.0*n+Point2(r.left(), -r.bottom()); 
      else
	c = a+10.0*d+2.0*n+Point2(-r.right(), -r.bottom());
    else
      if (d.y > 0)
	c = a+10.0*d+2.0*n+Point2(r.left(), -r.top());
      else
	c = a+10.0*d+2.0*n+Point2(-r.right(), -r.top());
  } else {
    Point2 n(-d.y, d.x);
    if (d.x > 0) 
      if (d.y > 0) 
	c = a+10.0*d+2.0*n+Point2(-r.right(), -r.top()); 
      else
	c = a+10.0*d+2.0*n+Point2(r.left(), -r.top());
    else
      if (d.y > 0)
	c = a+10.0*d+2.0*n+Point2(-r.right(), -r.bottom());
      else
	c = a+10.0*d+2.0*n+Point2(r.left(), -r.bottom()); 
  }
  painter.drawText(toQP(c), label);
}

/** 
 * Paint the image
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
  // draw image
  // TODO: performance? buffer scaled image?
  painter.drawImage(QPointF(-0.5, -0.5), theImage);

  if (theGrid.points.size() == 0) return;

  QPen p;
  p.setWidthF(2.0/theScale);
  const double radius = 5.0;
  
  // if there is grid: draw it
  if (theGrid.isRectangular()) {
    p.setColor("yellow");
    painter.setPen(p);
    const int w = theGrid.width;
    const int h = theGrid.height;
    for(int y=1; y<w; ++y)
      for(int x=0; x<w; ++x) {
	const Point2 &a = theGrid.points[y*w+x];
	const Point2 &b = theGrid.points[y*w+x-w];
	Point2 d = normalize(b-a)*radius;
	painter.drawLine(toQP(a+d), toQP(b-d));
      }
    for(int y=0; y<w; ++y)
      for(int x=1; x<w; ++x) {
	const Point2 &a = theGrid.points[y*w+x];
	const Point2 &b = theGrid.points[y*w+x-1];
	Point2 d = normalize(b-a)*radius;
	painter.drawLine(toQP(a+d), toQP(b-d));
      }
    // draw x/y markers
    // TODO: bmg
    p.setColor("red");
    painter.setPen(p);
    drawAxisLabel(painter, theGrid.points[0], theGrid.points[1], true, "x");
    drawAxisLabel(painter, theGrid.points[0], theGrid.points[w], false, "y");
  }

  // draw points
  p.setColor("red");
  painter.setPen(p);
  foreach(Point2 i, theGrid.points) {
    painter.drawEllipse(toQP(i), radius, radius);
  }
  painter.drawEllipse(toQP(theGrid.points[0]), radius+4, radius+4);

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
