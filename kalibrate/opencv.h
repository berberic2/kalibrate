/*
 * Copyright (C) 2010 M G Berberich
 */

#ifndef OPENCV_H
#define OPENCV_H

#include <QWidget>
#include <QSpinBox>
#include <QString>

#include <KLocale>

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
  double dist_w; //!< width of chessboard-tile
  double dist_h; //!< height of chessboard-tile

  OpenCVExtractorGui();
  ~OpenCVExtractorGui();

  void dimension(int x, int y); //!< set dimension of chessboard manually
  void distance(double w, double h) { dist_w = w; dist_h = h; }

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
  QString getName() { return i18n("OpenCV chessboard"); }

  void dimension(int x, int y);

private:
  OpenCVExtractorGui *theGui;
};

class OpenCVCamera : public Camera
{
  QString getName() { return i18n("OpenCV pinhole-camera"); } 
};

#endif
