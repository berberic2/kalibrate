/*
 * Copyright (C) 2010 M G Berberich
 */

#include <QWidget>
#include <QSpinBox>

#include "imagelist.h"
#include "kalib_plugin.h"

class OpenCV : public Extractor {
  Q_OBJECT
public:
  OpenCV();
  ~OpenCV();
  QWidget *getParamGui();
  bool extractPlate(ImageNode &node) const;
  void dimension(int x, int y) { theWidth = x; theHeight = y; }

private:
  QWidget *theGui;
  QSpinBox *widthWidget, *heightWidget;
  int theWidth, theHeight;

  void setupGui();

private slots:
  void widthChanged(int i) { theWidth = i; }
  void heightChanged(int i) { theHeight = i; }

};

inline QWidget *OpenCV::getParamGui()
{
  if(!theGui) setupGui();
  return theGui;
}
