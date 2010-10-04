/*
 * Copyright (C) 2010 M G Berberich
 */

#ifndef OPENCV_H
#define OPENCV_H

#include <QWidget>
#include <QSpinBox>

#include "imagelist.h"
#include "kalib_plugin.h"

/**
 * this should be a subclass of OpenCV, but this is not possible with Qt :(
 * Error: Meta object features not supported for nested classes
 */
class OpenCVGui : public QWidget
{
  Q_OBJECT
public:
  int width, height;

  OpenCVGui();
  ~OpenCVGui();
  void dimension(int x, int y);

private:
  QSpinBox *widthWidget, *heightWidget;
private slots:
  void widthChanged(int i) { width = i; }
  void heightChanged(int i) { height = i; }
};

/**
 *
 */
class OpenCV : public Extractor {
public:
  OpenCV();
  ~OpenCV();
  QWidget *getParamGui();
  bool extractPlate(ImageNode &node) const;
  void dimension(int x, int y);

private:
  OpenCVGui *theGui;
};

inline QWidget *OpenCV::getParamGui()
{
  if(!theGui) 
    theGui = new OpenCVGui;
  return theGui;
}

#endif
