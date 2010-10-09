/*
 * Copyright (C) 2010 M G Berberich, Peter Barth
 */

#ifndef KALIB_PLUGIN_H
#define KALIB_PLUGIN_H

#include <ostream>

#include <QString>

class QWidget;

/**
 * Grid/points-information.
 * contains information about image-points and corresponding space-points. 
 */
struct Plate
{
  /// An image to space correspondence
  struct Point {
    Point2 image; //!< point in the image in pixel-coordinates
    Point3 space; //!< point in space in plate-coordinate-system
  };

  std::vector<Point> points;  //!< Vector of points-correspondences
  bool rectangular;           //!< If it is a grid or just a bunch of points.
  int width, height;          //!< Width and height if it is a grid.

  typedef std::vector<Point>::iterator pointiterator;

  bool isRectangular() { return rectangular; }
  void dimension(int w, int h) { rectangular = true; width = w; height = h; }
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
   * Does the extraction of a grid from an image.
   *
   * Hint: throw any exception if something goes wrong.
   *
   * @param image @a Qimage from which points/grid are extracted
   * @param grid reference to a @a Plate that is filled with the points/grid
   * @return true if a grid was found, false otherwise
   */
  virtual bool operator() (const QImage &image, Plate &grid) const = 0;

  /**
   * Give the name of this extractor-type.
   * @returns the name of the extractor in UTF-8 encoding.
   */
  virtual QString getName() = 0;
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

  /**
   * Give the name of this camera-type.
   * @returns the name of the camera in UTF-8 encoding.
   */
  virtual QString getName() = 0;
};


class Optimizer
{
public:
  Optimizer() {};
  Optimizer(Camera &c) {};
  virtual QWidget *getParamGui() = 0;
  virtual double optimize(std::vector<Plate> &plates) = 0;

  /**
   * Give the name of this optimizer.
   * @returns the name of the optimizer in UTF-8 encoding.
   */
  virtual QString getName() = 0;

private:
  Camera *camera;
};


/// @relates Plate::Point @{
/**
 * output-operator for @a Plate::Point.
 */
inline std::ostream &operator<<(std::ostream &os, const Plate::Point &pt)
{
  os << "[(" <<  pt.image.x<< ", " << pt.image.y << "), ("
     << pt.space.x << ", " << pt.space.y << ", " << pt.space.z << ")]";
  return os;
}
// @}


#endif
