/*
 * Copyright (C) 2010 M G Berberich, Peter Barth
 */

#ifndef KALIB_PLUGIN_H
#define KALIB_PLUGIN_H

#include <string>

class Extractor
{
public:
  /**
   * returns a pointer to a qwidget containing the gui
   */
  virtual QWidget *getParamGui() = 0;

  /**
   * extract plate in image and add the grid/points to node
   */
  virtual bool extractPlate(ImageNode &node) const = 0;

  /**
   * return the name of the extractor in UTF-8
   */
  virtual std::string getName() = 0;
};

#endif
