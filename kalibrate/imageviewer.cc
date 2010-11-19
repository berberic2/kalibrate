/*
 * Copyright (C) 2010 M G Berberich
 */

#include <iostream>
#include <cmath>

#include <QAbstractScrollArea>
#include <QPainter>
#include <QWheelEvent>
#include <QScrollBar>

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
  setFixedSize(QSize(800, 600));
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
#if 0
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
#else
void ImageWidget::drawAxisLabel(QPainter &painter,
      const Point2 &a, const Point2 &b,
      bool left, const char *label)
{
  Point2 m = (b+a)/2;
  Point2 d = normalize(b-a);
  QRectF r = QFontMetricsF(painter.font()).boundingRect(label[0]);
  Point2 size(r.right()-r.left(),r.bottom()-r.top());
  Point2 c;

  if (d.x > 0) {
    if (d.y > 0) {
      c = m + Point2(2*d.x,0.6*d.y*size.y);
    } else {
      c = m + Point2(2*d.x,2*d.y);
    }
  } else {
    if (d.y > 0) {
      c = m + Point2(d.x*(2+size.x),0) + Point2(0,0.6*d.y*size.y);
    } else {
      c = m + Point2(d.x*(2+size.x),2*d.y);
    }
  }
  const double angle = 2.8797933;
  const double cosa = cos(angle);
  const double sina = sin(angle);
  Point2 e1(cosa*d.x - sina*d.y, sina*d.x + cosa*d.y);
  Point2 e2(cosa*d.x + sina*d.y, -sina*d.x + cosa*d.y);
  QPointF arrow[3];
  arrow[0] = toQP(m);
  arrow[1] = toQP(m+e1*8);
  arrow[2] = toQP(m+e2*8);
  painter.drawLine(toQP(a), toQP(m));
  painter.save();
  painter.setBrush(QBrush(painter.pen().color(), Qt::SolidPattern));
  painter.drawPolygon(arrow, 3);
  painter.restore();
  painter.drawText(toQP(c), label);
}
#endif

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
#if 1
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
    for(int y=1; y<h; ++y)
      for(int x=0; x<w; ++x) {
	const Point2 &a = theGrid.points[y*w+x].image;
	const Point2 &b = theGrid.points[y*w+x-w].image;
	Point2 d = normalize(b-a)*radius;
	painter.drawLine(toQP(a+d), toQP(b-d));
      }
    for(int y=0; y<h; ++y)
      for(int x=1; x<w; ++x) {
	const Point2 &a = theGrid.points[y*w+x].image;
	const Point2 &b = theGrid.points[y*w+x-1].image;
	Point2 d = normalize(b-a)*radius;
	painter.drawLine(toQP(a+d), toQP(b-d));
      }
    // draw x/y markers
    // TODO: bmg
    p.setColor("red");
    painter.setPen(p);
    drawAxisLabel(painter, theGrid.points[0].image, theGrid.points[1].image, true, "x");
    drawAxisLabel(painter, theGrid.points[0].image, theGrid.points[w].image, false, "y");
    //drawArrow(painter, theGrid.points[0], theGrid.points[1]);
    //drawArrow(painter, theGrid.points[0], theGrid.points[w]);
  }

  // draw points
  p.setColor("red");
  painter.setPen(p);
  foreach(Plate::Point i, theGrid.points) {
    painter.drawEllipse(toQP(i.image), radius, radius);
  }
  painter.drawEllipse(toQP(theGrid.points[0].image), radius+4, radius+4);
#else
  QPen p;
  p.setWidthF(2.0/theScale);
  painter.setPen(p);
  for(double w=0; w<2*M_PI; w+=M_PI/40) {
    Point2 a(300+200*sin(w), 300+200*cos(w));
    Point2 b(300+250*sin(w), 300+250*cos(w));
    painter.drawLine(toQP(a), toQP(b));
    drawAxisLabel(painter, a, b, true, "x");
  }
#endif
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
void ImageWidget::grid(const Plate &g)
{
  theGrid = g;
  update();
};

/**
 * Handle wheel-events
 *
 * Wheel allows to zooms in/out (changes the scale)
 *
 * @param event
 */
void ImageView::wheelEvent(QWheelEvent *event)
{
  if(event->orientation() == Qt::Vertical) {
    double fx = double(event->x()-theImage.x())/theImage.width();
    double fy = double(event->y()-theImage.y())/theImage.height();
    double sf = pow(M_SQRT2, event->delta()/(15.0*8.0));
    double s = theImage.scale()*sf;
    if (s > 0.1 && s < 10) {
      theImage.scale(s);
      // zoom with fixed mousecursor
      horizontalScrollBar()->setValue(theImage.width()*fx-event->x());
      verticalScrollBar()->setValue(theImage.height()*fy-event->y());
    }
  }
}

