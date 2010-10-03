/*
 * Copyright (C) 2010 M G Berberich, Peter Barth
 */

#include <QWidget>

class Extractor : public QObject
{
public:
  virtual QWidget *getParamGui() = 0;
  virtual bool extractPlate(ImageNode &node) const = 0;
};
