/* -*- c++ -*- */
/*
 * Copyright (C) 2010 M G Berberich
 */

#ifndef LINEAR_H
#define LINEAR_H

#include <iostream>

#include <cmath>

/**
 * 2-dimensional point
 */
struct Point2
{
  double x, y;
  
  Point2() {}
  Point2(double a, double b) : x(a), y(b) {}
  void set(double a, double b) { x = a; y = b; }
  Point2 operator- (const Point2 &b) const { return Point2(x-b.x, y-b.y); } 
  Point2 operator+ (const Point2 &b) const { return Point2(x+b.x, y+b.y); }
  Point2 operator/ (double s) const { return Point2(x/s, y/s); }
  Point2 operator* (double s) const { return Point2(x*s, y*s); }
};

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

#endif
