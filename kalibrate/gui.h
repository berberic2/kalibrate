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
class Extractor;
class Optimizer;
class QComboBox;
class QVBoxLayout;

class KalibrateGui : public KMainWindow // QWidget
{
  Q_OBJECT;
public:
  KalibrateGui(QWidget *parent=0);
  virtual ~KalibrateGui();
  
  KActionCollection* actionCollection() { return &action_collection; }
  
  void load(const QString &filename);
  void save(const QString &filename);
				    
  void addExtractor(Extractor *(*f)());
  void addOptimizer(Optimizer *(*f)());

public slots:
  virtual void load();
  virtual void save();
  virtual void load_images();
  virtual void imageSelected(const QModelIndex & index);
  virtual void extractorChanged(int i);
  
protected:
  virtual void saveProperties(KConfigGroup &);
  virtual void readProperties(const KConfigGroup &);
  
private:
  KActionCollection action_collection;
  QListView *theImageList;
  ImageView *theImageViewer;
  QComboBox *extractorSelector, *optimizerSelector;
  QVBoxLayout *settingsVBox;

  // Daten
  imageList images;
  std::vector<Extractor *> extractors;
  std::vector<Optimizer *> optimizers;

  QWidget *settingsWidget(QWidget *parent);

};

#endif
