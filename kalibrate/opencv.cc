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

OpenCV::OpenCV() :
  theGui(0), theWidth(11), theHeight(11)
{
}

OpenCV::~OpenCV()
{
  if (theGui)
    delete theGui;
}

bool OpenCV::extractPlate(ImageNode &node) const
{
  QImage &image = node.image;
  int h = image.height();
  int w = image.width();

  IplImage img;
  cvInitImageHeader(&img, cvSize(w, h), IPL_DEPTH_8U, 1);
  cvCreateData(&img);
  for(int y=0; y<h; ++y)
    for(int x=0; x<w; ++x)
      img.imageData[y*img.widthStep+x] = qGray(image.pixel(x, y));

  CvPoint2D32f points[theWidth*theHeight];
  int corners = 0;

  int r = cvFindChessboardCorners(&img, cvSize(theWidth, theHeight), points, &corners);

  cvReleaseData(&img);

  if (r != 0) {
    node.grid.points.resize(corners);
    for(int i=0; i<corners; ++i) {
      node.grid.points[i].set(points[i].x, points[i].y);
      //    std::cout << "  corners:" << corners << "\n";
    }
    node.grid.dimension(theWidth, theHeight);
    node.active = true;
    node.extrinsic = false;
  } else {
    node.grid.points.clear();
    node.active = false;
    node.extrinsic = false;
  }
}

void OpenCV::setupGui()
{
  if (theGui) return;

  // Widget
  theGui = new QWidget();
  QFormLayout *formLayout = new QFormLayout(theGui);

  // Label
  QLabel *label = new QLabel(i18n("Chessboard-size:"), theGui);
  formLayout->setWidget(0, QFormLayout::LabelRole, label);

  // w×h Eingabe
  QHBoxLayout *horizontalLayout = new QHBoxLayout();
  widthWidget = new QSpinBox(theGui);
  horizontalLayout->addWidget(widthWidget);
  QLabel *labelx = new QLabel(QString::fromUtf8("×"), theGui);
  horizontalLayout->addWidget(labelx);
  heightWidget = new QSpinBox(theGui);
  horizontalLayout->addWidget(heightWidget);

  connect(widthWidget, SIGNAL(valueChanged(int)), SLOT(widthChanged(int)));
}
