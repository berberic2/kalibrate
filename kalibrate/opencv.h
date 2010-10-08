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
 * The Gui for the OpenCV-Extractor.
 *
 * this should be a subclass of OpenCV, but this is not possible with Qt :(
 * Error: Meta object features not supported for nested classes
 */
class OpenCVExtractorGui : public QWidget
{
  Q_OBJECT
public:
  int width; //!< width of chessboard
  int height; //!< height of chessboard

  OpenCVExtractorGui();
  ~OpenCVExtractorGui();

  void dimension(int x, int y); //!< set dimension of chessboard manually

private:
  QSpinBox *widthWidget, *heightWidget;

private slots:
  void widthChanged(int i) { width = i; }
  void heightChanged(int i) { height = i; }
};

/**
 * Exctractor-class for the OpenCV chessboard-method.
 */
class OpenCVExtractor : public Extractor {
public:
  OpenCVExtractor();
  ~OpenCVExtractor();
  QWidget *getParamGui();
  bool operator() (const QImage &node, Plate &grid) const;
  std::string getName() { return "OpenCV Chessboard"; }

  void dimension(int x, int y);

private:
  OpenCVExtractorGui *theGui;
};

inline QWidget *OpenCVExtractor::getParamGui()
{
  if(!theGui) 
    theGui = new OpenCVExtractorGui;
  return theGui;
}

#endif
