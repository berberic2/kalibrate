/*
 * Copyright (C) 2010 M G Berberich
 */

#include <opencv/cv.h>

#include <QWidget>
#include <QSpinBox>
#include <QLabel>
#include <QLayout>
#include <QFormLayout>
#include <QCheckBox>

#include <KLocale>

#include "imagelist.h"
#include "opencv.h"

OpenCVExtractorGui::OpenCVExtractorGui() :
  width(11), height(11), dist_w(1.0), dist_h(1.0)
{
  // Widget
  QVBoxLayout *widgetLayout = new QVBoxLayout(this);
  QFormLayout *formLayout = new QFormLayout(this);

  // Label
  QLabel *formlabel = new QLabel(i18n("Chessboard-size:"), this);
 
  // w×h Eingabe
  QHBoxLayout *horizontalLayout = new QHBoxLayout();
  QSpinBox *widthWidget = new QSpinBox(this);
  horizontalLayout->addWidget(widthWidget);
  QLabel *label = new QLabel(QString::fromUtf8("×"), this);
  horizontalLayout->addWidget(label);
  QSpinBox *heightWidget = new QSpinBox(this);
  horizontalLayout->addWidget(heightWidget);
  formLayout->addRow(formlabel, horizontalLayout);

  // Label
  formlabel = new QLabel(i18n("Chessboard-dimension:"), this);

  // dimension Eingabe
  horizontalLayout = new QHBoxLayout();
  QDoubleSpinBox *distxWidget = new QDoubleSpinBox(this);
  horizontalLayout->addWidget(distxWidget);
  label = new QLabel(QString::fromUtf8("×"), this);
  horizontalLayout->addWidget(label);
  QDoubleSpinBox *distyWidget = new QDoubleSpinBox(this);
  horizontalLayout->addWidget(distyWidget);
  formLayout->addRow(formlabel, horizontalLayout);

  widgetLayout->addLayout(formLayout);

  // more-box
  QCheckBox  *moreWidget = new QCheckBox("More", this);
  widgetLayout->addWidget(moreWidget);

  // more-group
  moreGroup = new QWidget(this);
  QFormLayout *moreLayout = new QFormLayout(moreGroup);

  // Label
  formlabel = new QLabel(i18n("½ window-size:"), moreGroup);
  // window input (x, y)
  horizontalLayout = new QHBoxLayout();
  QSpinBox *windowxWidget = new QSpinBox(moreGroup);
  horizontalLayout->addWidget(windowxWidget);
  label = new QLabel(QString::fromUtf8("×"), moreGroup);
  horizontalLayout->addWidget(label);
  QSpinBox *windowyWidget = new QSpinBox(moreGroup);
  horizontalLayout->addWidget(windowyWidget);
  moreLayout->addRow(formlabel, horizontalLayout);

  formlabel = new QLabel(i18n("½ zero-zone:"), moreGroup);
  // zero-zone input (x, y)
  horizontalLayout = new QHBoxLayout();
  QSpinBox *zeroxWidget = new QSpinBox(moreGroup);
  horizontalLayout->addWidget(zeroxWidget);
  label = new QLabel(QString::fromUtf8("×"), moreGroup);
  horizontalLayout->addWidget(label);
  QSpinBox *zeroyWidget = new QSpinBox(moreGroup);
  horizontalLayout->addWidget(zeroyWidget);
  moreLayout->addRow(formlabel, horizontalLayout);

  widgetLayout->addWidget(moreGroup);
  moreGroup->hide();

  // connect actions to widgets
  connect(widthWidget, SIGNAL(valueChanged(int)), SLOT(widthChanged(int)));
  connect(heightWidget, SIGNAL(valueChanged(int)), SLOT(heightChanged(int)));
  connect(distxWidget, SIGNAL(valueChanged(double)), SLOT(distyChanged(double)));
  connect(distyWidget, SIGNAL(valueChanged(double)), SLOT(distxChanged(double)));
  connect(distyWidget, SIGNAL(valueChanged(double)), SLOT(distxChanged(double)));
  connect(moreWidget, SIGNAL(stateChanged(int)), SLOT(moreChanged(int)));
  connect(windowxWidget, SIGNAL(valueChanged(int)), SLOT(windowxChanged(int)));
  connect(windowyWidget, SIGNAL(valueChanged(int)), SLOT(windowyChanged(int)));
  connect(zeroxWidget, SIGNAL(valueChanged(int)), SLOT(zeroxChanged(int)));
  connect(zeroyWidget, SIGNAL(valueChanged(int)), SLOT(zeroyChanged(int)));

  // defaults
  distxWidget->setValue(1.0);
  distyWidget->setValue(1.0);
  windowxWidget->setValue(5);
  windowyWidget->setValue(5);
  zeroxWidget->setValue(1);
  zeroyWidget->setValue(1);
}

/**
 * Handle show/hide of the experts configuration group
 */
void OpenCVExtractorGui::moreChanged(int state)
{
  switch(state) {
  case Qt::Unchecked:
    moreGroup->hide();
    break;
  case Qt::Checked:
    moreGroup->show();
    break;
  }
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
  CvSize window, zero;
  if (theGui->moreGroup->isHidden()) {
    window = cvSize(5, 5);
    zero = cvSize(1, 1);
  } else {
    window = cvSize(theGui->window_w, theGui->window_h);
    zero = cvSize(theGui->zero_w, theGui->zero_h);
  }
  cvFindCornerSubPix(img, points, corners, window, zero,
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
