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
  theImage(0), theScale(1.0)
{
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
  QSize i = theImage->size();
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
  if (theImage) painter.drawImage(0, 0, *theImage);
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
    double s = theScale*pow(1.2, event->delta()/(15.0*8.0));
    if (s > 0.1 || s < 10) {
      scale(s);
      // ToDo set scroll so that center stays fixed
    }
  }
}

/** 
 * Set the image
 * 
 * Set the image that is displayed. The image does not get copied
 * so memory-management of the image is the job of the caller.
 * Consider it some kind of model-view-widget. 
 *
 * @param i 
 */
void ImageWidget::image(const QImage *i) 
{ 
  theImage = i; 
  scale(theScale);
  update(); 
};
