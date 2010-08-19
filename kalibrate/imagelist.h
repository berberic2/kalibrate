/* -*- c++ -*- */
/*
 * Copyright (C) 2010 M G Berberich
 */

#ifndef IMAGELIST_H
#define IMAGELIST_H

#include<QAbstractItemDelegate>

struct ImageNode
{
  QImage image;
  QPixmap thumb;
  int points;
  bool active;
  bool extrinsic;
  std::string name;
};
Q_DECLARE_METATYPE(ImageNode)


class ImageDelegate : public QAbstractItemDelegate
{
  Q_OBJECT
  
  public:
  ImageDelegate(QWidget *parent = 0) : QAbstractItemDelegate(parent) {} ;
  
  void paint(QPainter *painter, const QStyleOptionViewItem &option,
	const QModelIndex &index) const;
  QSize sizeHint(const QStyleOptionViewItem &option,
	const QModelIndex &index) const;
};

class ImageListModel : public QAbstractListModel
{
public:
  ImageListModel(QObject *parent = 0);
  virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
  virtual QVariant data(const QModelIndex &index, 
	int role = Qt::DisplayRole) const;
  virtual QVariant headerData(int section, Qt::Orientation orientation, 
	int role = Qt::DisplayRole) const;
private:
  std::vector<ImageNode> images;
};


#endif
