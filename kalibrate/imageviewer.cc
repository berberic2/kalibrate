/*
 * Copyright (C) 2010 M G Berberich
 */

#include <iostream>
#include <cmath>

#include <QAbstractScrollArea>
#include <QPainter>
#include <QWheelEvent>

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
  // performance? buffer scaled image?
  painter.drawImage(0, 0, theImage);
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
