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

OpenCVGui::OpenCVGui() :
  width(11), height(11)
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

OpenCVGui::~OpenCVGui()
{
}

OpenCV::OpenCV() :
  theGui(0)
{
}

OpenCV::~OpenCV()
{
  if (theGui)
    delete theGui;
}

bool OpenCV::extractPlate(ImageNode &node) const
{
  if (!theGui) return false;

  QImage &image = node.image;
  int h = image.height();
  int w = image.width();

  IplImage img;
  cvInitImageHeader(&img, cvSize(w, h), IPL_DEPTH_8U, 1);
  cvCreateData(&img);
  for(int y=0; y<h; ++y)
    for(int x=0; x<w; ++x)
      img.imageData[y*img.widthStep+x] = qGray(image.pixel(x, y));

  CvPoint2D32f points[theGui->width*theGui->height];
  int corners = 0;

  int r = cvFindChessboardCorners(&img, cvSize(theGui->width, theGui->height), points, &corners);

  cvReleaseData(&img);

  if (r != 0) {
    node.grid.points.resize(corners);
    for(int i=0; i<corners; ++i) {
      node.grid.points[i].set(points[i].x, points[i].y);
      //    std::cout << "  corners:" << corners << "\n";
    }
    node.grid.dimension(theGui->width, theGui->height);
    node.active = true;
    node.extrinsic = false;
    return true;
  } else {
    node.grid.points.clear();
    node.active = false;
    node.extrinsic = false;
    return false;
  }
}

void OpenCV::dimension(int x, int y)
{
  if (!theGui) 
    theGui = new OpenCVGui;
  theGui->dimension(x, y);
}

void OpenCVGui::dimension(int x, int y)
{
  width = x; 
  height = y;
  widthWidget->setValue(width);
  heightWidget->setValue(height);
}



