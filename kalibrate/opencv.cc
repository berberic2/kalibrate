/*
 * Copyright (C) 2010 M G Berberich
 */

#include <opencv/cv.h>

#include <QWidget>
#include <QSpinBox>
#include <QLabel>
#include <QLayout>
#include <QFormLayout>

#include <KLocale>

#include "imagelist.h"
#include "opencv.h"

OpenCVExtractorGui::OpenCVExtractorGui() :
  width(11), height(11), dist_w(1.0), dist_h(1.0)
{
  // Widget
  QFormLayout *formLayout = new QFormLayout(this);

  // Label
  QLabel *label = new QLabel(i18n("Chessboard-size:"), this);
  formLayout->setWidget(0, QFormLayout::LabelRole, label);

  // w×h Eingabe
  QHBoxLayout *horizontalLayout = new QHBoxLayout();
  widthWidget = new QSpinBox(this);
  horizontalLayout->addWidget(widthWidget);
  QLabel *labelx = new QLabel(QString::fromUtf8("×"), this);
  horizontalLayout->addWidget(labelx);
  heightWidget = new QSpinBox(this);
  horizontalLayout->addWidget(heightWidget);

  // connect actions to widgets
  connect(widthWidget, SIGNAL(valueChanged(int)), SLOT(widthChanged(int)));
  connect(heightWidget, SIGNAL(valueChanged(int)), SLOT(heightChanged(int)));
}

OpenCVExtractorGui::~OpenCVExtractorGui()
{
}

OpenCVExtractor::OpenCVExtractor() :
  theGui(0)
{
}

OpenCVExtractor::~OpenCVExtractor()
{
  if (theGui)
    delete theGui;
}

/**
 * Get GUI-elements for the openCV-Extractor.
 * @return a pointer to a qwidget containing the gui
 */
QWidget *OpenCVExtractor::getParamGui()
{
  if(!theGui) 
    theGui = new OpenCVExtractorGui;
  return theGui;
}

/**
 * Does the extraction of a grid from image.
 *
 * Extract a grid from the image using OpenCVs @a cvFindChessboardCorners
 * and @a cvFindCornerSubPix functions.
 *
 * @param image @a Qimage from which points/grid are extracted
 * @param grid reference to a @a Plate that is filled with the points/grid
 * @return true if a grid was found, false otherwise
 */
bool OpenCVExtractor::operator() (const QImage &image, Plate &grid) const
{
  if (!theGui) return false;

  int h = image.height();
  int w = image.width();

  // create and copy image → IplImage
  cv::Ptr<IplImage> img = cvCreateImage(cvSize(w, h), IPL_DEPTH_8U, 1);
  for(int y=0; y<h; ++y)
    for(int x=0; x<w; ++x)
      img->imageData[y*img->widthStep+x] = qGray(image.pixel(x, y));

  // find chessboard
  CvPoint2D32f points[theGui->width*theGui->height];
  int corners = 0;

  int r = cvFindChessboardCorners(img, cvSize(theGui->width, theGui->height),
	points, &corners);

  // if nothing found, clean up and return
  if (r == 0) throw 1;

  // refine points
  cvFindCornerSubPix(img, points, corners, cvSize(5, 5), cvSize(1, 1),
	cvTermCriteria(CV_TERMCRIT_EPS|CV_TERMCRIT_ITER, 100, 0.001));

  // copy grid to node
  grid.points.resize(corners);
  for(int i=0; i<corners; ++i) {
    grid.points[i].image.set(points[i].x, points[i].y);
    grid.points[i].space.set(i%theGui->width * theGui->dist_w, 
	  i/theGui->width * theGui->dist_h, 0.0);
    // std::cout << grid.points[i] << "\n";
  }
  grid.dimension(theGui->width, theGui->height);
  return true;
}

void OpenCVExtractor::dimension(int x, int y)
{
  if (!theGui)
    theGui = new OpenCVExtractorGui;
  theGui->dimension(x, y);
}

void OpenCVExtractorGui::dimension(int x, int y)
{
  width = x;
  height = y;
  widthWidget->setValue(width);
  heightWidget->setValue(height);
}



