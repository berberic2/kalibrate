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
  ~OpenCVExtractorGui() { }

private:
  // QSpinBox *widthWidget, *heightWidget;

private slots:
  void widthChanged(int i) { width = i; }
  void heightChanged(int i) { height = i; }
  void distxChanged(double i) { dist_w = i; }
  void distyChanged(double i) { dist_h = i; }
};

/**
 * Exctractor-class for the OpenCV chessboard-method.
 */
class OpenCVExtractor : public Extractor {
public:
 OpenCVExtractor() : theGui(0) { }
  ~OpenCVExtractor()  { if (theGui) delete theGui; }
  QWidget *getParamGui();
  bool operator() (const QImage &node, Plate &grid) const;
  QString getName() { return i18n("OpenCV chessboard"); }

  void dimension(int x, int y);

  static Extractor *newExtractor() { return new OpenCVExtractor; }
private:
  OpenCVExtractorGui *theGui;
};

class OpenCVCamera : public Camera
{
 public:
  Point2 distort(Point2 p) {}
  Point2 undistort(Point2 p) {}

  Point2 worldToImage(Point3 p) {}
  Line3 imageToRay(Point2 p) {}

  QString getName() { return i18n("OpenCV pinhole-camera"); } 

 private:
  //CvMat* cameraMatrix;
  //CvMat* distCoeffs;
};

class OpenCVOptimizer : public Optimizer
{
public:
 OpenCVOptimizer() : camera(0) { }
  ~OpenCVOptimizer() { }
  QWidget *getParamGui() { return 0; }
  double optimize(std::vector<Plate> &plates) { }

  /**
   * Give the name of this optimizer.
   * @returns the name of the optimizer in UTF-8 encoding.
   */
  QString getName() { return i18n("OpenCV Calibration"); } 
  static Optimizer *newOptimizer() { return new OpenCVOptimizer; }

private:
  Camera *camera;
};

#endif
