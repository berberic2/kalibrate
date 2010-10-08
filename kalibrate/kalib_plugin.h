/*
 * Copyright (C) 2010 M G Berberich, Peter Barth
 */

#ifndef KALIB_PLUGIN_H
#define KALIB_PLUGIN_H

#include <string>

/**
 * Grid-information
 */
struct Plate
{
  std::vector<Point2> points;
  bool rectangular;
  int width, height;

  typedef std::vector<Point2>::iterator pointiterator;

  bool isRectangular() { return rectangular; }
  bool dimension(int w, int h) { rectangular = true; width = w; height = h; }
  size_t size() { return points.size(); }
};


/**
 * extract point-sets from images
 */
class Extractor
{
public:
  /**
   * @return a pointer to a qwidget containing the gui
   */
  virtual QWidget *getParamGui() = 0;

  /**
   * Does the extraction of a grid from image.
   *
   * Hint: throw any exception if something goes wrong.
   *
   * @param image @a Qimage from which points/grid are extracted
   * @param grid reference to a @a Plate that is filled with the points/grid
   * @return true if a grid was found, false otherwise
   */
  virtual bool operator() (const QImage &image, Plate &grid) const = 0;

  /**
   * @returns the name of the extractor in UTF-8
   */
  virtual std::string getName() = 0;
};

/**
 * a calibrated camera-model
 */
class Camera
{
public:
  // virtual QWidget *getParamGui() = 0;

  virtual Point2 distort(Point2 p) = 0;
  virtual Point2 undistort(Point2 p) = 0;

  virtual Point2 worldToImage(Point3 p);
  virtual Line3 imageToRay(Point2 p);
};


class Optimizer
{
public:
  Optimizer() {};
  Optimizer(Camera &c) {};
  virtual QWidget *getParamGui() = 0;
  virtual double optimize(std::vector<Plate> &plates) = 0;

private:
  Camera *camera;
};


#endif
