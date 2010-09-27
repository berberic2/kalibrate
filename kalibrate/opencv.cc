/*
 * Copyright (C) 2010 M G Berberich
 */

#include <opencv/cv.h>

#include "imagelist.h"

bool extract(ImageNode &node, int pw, int ph)
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

  CvPoint2D32f points[pw*ph];
  int corners = 0;
  
  int r = cvFindChessboardCorners(&img, cvSize(pw, ph), points, &corners);

  cvReleaseData(&img);

  if (r != 0) {
    node.grid.points.resize(corners);
    for(int i=0; i<corners; ++i) {
      node.grid.points[i].set(points[i].x, points[i].y);
      //    std::cout << "  corners:" << corners << "\n";
    }
    node.grid.dimension(pw, ph);
  } else {
    node.grid.points.clear();
  }
}
