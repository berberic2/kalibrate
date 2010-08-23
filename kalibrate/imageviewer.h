/* -*- c++ -*- */
/*
 * Copyright (C) 2010 M G Berberich
 */

#include <QWidget>
#include <QAbstractScrollArea>


class QImage;

class ImageWidget : public QWidget
{
public:
  ImageWidget(QWidget *parent = 0);
protected:
  virtual void paintEvent(QPaintEvent *event);
};

class ImageView : public QAbstractScrollArea
{
  Q_OBJECT;
public:
  ImageView(QWidget *parent = 0);
  void Image(const QImage *i) { theImage = i; update(); };

protected:
  virtual void paintEvent(QPaintEvent *event);

private:
  const QImage *theImage;
};
