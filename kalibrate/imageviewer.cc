#include <iostream>

#include <QAbstractScrollArea>
#include <QPainter>

#include "imageviewer.h"

ImageWidget::ImageWidget(QWidget *parent)
{
  std::cout << "Hey\n";
}

void ImageWidget::paintEvent(QPaintEvent * event)
{
  QWidget::paintEvent(event);
  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing);
  painter.drawLine(0, 0, width(), height());
  painter.drawLine(0, height(), width(), 0);
  painter.end();
  std::cout << "Hallo\n";
}

ImageView::ImageView(QWidget *parent) 
{
  theImage = 0;
  setViewport(new ImageWidget(this));
}

void ImageView::paintEvent(QPaintEvent * event)
{
  QAbstractScrollArea::paintEvent(event);
  QPainter painter(viewport());
  painter.setRenderHint(QPainter::Antialiasing);
  if (theImage) painter.drawImage(0, 0, *theImage);
  painter.drawLine(0, 0, viewport()->width(), viewport()->height());
  painter.drawLine(0, viewport()->height(), viewport()->width(), 0);
  painter.drawRect(0, 0, viewport()->width()-1, viewport()->height()-1);
  painter.end();
  std::cout << "Hallo2 " << width() << "\n";
}
