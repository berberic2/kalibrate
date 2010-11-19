/* -*- c++ -*- */
/*
 * Copyright (C) 2010 M G Berberich
 */

#ifndef LINEAR_H
#define LINEAR_H

#include <iostream>

#include <cmath>

/**
 * 2-dimensional point.
 * a Point from @f$\mathbf R^2@f$ with some common operations.
 */
struct Point2
{
  double x, y;

  Point2() : x(0), y(0) {}
  Point2(double a, double b) : x(a), y(b) {}
  void set(double a, double b) { x = a; y = b; }
  bool isnan() const { return std::isnan(x); }
  Point2 operator- (const Point2 &b) const { return Point2(x-b.x, y-b.y); }
  Point2 operator+ (const Point2 &b) const { return Point2(x+b.x, y+b.y); }
  Point2 operator/ (double s) const { return Point2(x/s, y/s); }
  Point2 operator* (double s) const { return Point2(x*s, y*s); }
};

/// @relates Point2 @{ 
inline Point2 operator* (double s, const Point2 &p)
{
  return Point2(p.x*s, p.y*s);
}

inline double norm(const Point2 &p)
{
  return sqrt(p.x*p.x+p.y*p.y);
}

inline Point2 normalize(const Point2 &p)
{
  return Point2(p/norm(p));
}

// @} 

/**
 * 3-dimensional point.
 * a Point from @f$\mathbf R^3@f$ with some common operations.
 */
struct Point3
{
  double x, y, z;

  Point3() : x(0), y(0), z(0) {}
  Point3(double a, double b, double c) : x(a), y(b), z(b) {}
  void set(double a, double b, double c) { x = a; y = b; z = c; }
  bool isnan() const { return std::isnan(x); }
};

/**
 * A line in R³.
 * given in the form @f$ \{ p+\lambda v\mid\lambda\in\mathbf R \} @f$
 */
struct Line3
{
  Point3 p;  //!< point on line
  Point3 v;  //!< direction of line
};

#endif
