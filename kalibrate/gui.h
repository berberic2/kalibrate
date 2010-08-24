/* -*- c++ -*- */
/*
 * Copyright (C) 2010 M G Berberich
 */

#ifndef GUI_H
#define GUI_H

#include <KMainWindow>
#include <KActionCollection>

#include "imagelist.h"

class ImageListModel;
class ImageDelegate;
class QListView;
class ImageView;

class KalibrateGui : public KMainWindow // QWidget
{
  Q_OBJECT;
public:
  KalibrateGui(QWidget *parent=0);
  virtual ~KalibrateGui();

  KActionCollection* actionCollection() { return &action_collection; }

  void load(const QString &filename);
  void save(const QString &filename);


public slots:
  virtual void load();
  virtual void save();
  virtual void load_images();

protected:
  virtual void saveProperties(KConfigGroup &);
  virtual void readProperties(const KConfigGroup &);

private:
  KActionCollection action_collection;
  QListView *theImageList;
  ImageView *theImageViewer;

  // Daten
  imageList images;
};

#endif
