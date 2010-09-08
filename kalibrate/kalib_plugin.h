
#ifndef KALIB_PLUGIN_H
#define KALIB_PLUGIN_H


class Extractor
{
 public:
  virtual gui getParamGui();
  virtual plate extractPlate(image image);
}


class Optimizer
{
 public:
  virtual Optimizer(Camera &c);
  virtual gui getParamGui();
  virtual double optimize(std::vector<plate> &plates);
}


class Camera
{
 public:
  virtual gui getParamGui();

  virtual fpoint2 distort(fpoint2 p);
  virtual fpoint2 undistort(fpoint2 p);

  virtual fpoint2 worldToImage(fpoint3 p);
  virtual line2d imageToRay(fpoint2 p);
}
#endif
