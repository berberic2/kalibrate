/* -*- c++ -*- */
/*
 * Copyright (C) 2010 M G Berberich
 */

#ifndef IMAGELIST_H
#define IMAGELIST_H

#include <QAbstractItemDelegate>
#include <QFontMetrics>


/**
 * Information structure for an Imaga
 */
struct ImageNode
{
  QImage image;			/**< image */
  QPixmap thumb;		/**< thumbnail of image */
  int points;			/**< number of points found */
  bool active;			/**< image is used for calibration */
  bool extrinsic;		/**< save extrinsic calibration */
  std::string name;		/**< filename of the image */

  void set(const QString &filename);
};
typedef std::vector<ImageNode> imageList;


Q_DECLARE_METATYPE(const ImageNode*)
/**
 * Model for Qt to display ImageLists
 */
class ImageListModel : public QAbstractListModel
{
  Q_OBJECT
public:
  ImageListModel(QObject *parent = 0);
  ImageListModel(imageList *i, QObject *parent = 0);

  virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
  virtual QVariant data(const QModelIndex &index, 
	int role = Qt::DisplayRole) const;
  virtual QVariant headerData(int section, Qt::Orientation orientation, 
	int role = Qt::DisplayRole) const;
  void images(imageList *i) { theImages = i; }

private:
  imageList *theImages;
};

class ImageDelegate : public QAbstractItemDelegate
{
  Q_OBJECT
public:
  ImageDelegate(QWidget *parent = 0);
  
  void paint(QPainter *painter, const QStyleOptionViewItem &option,
	const QModelIndex &index) const;
  QSize sizeHint(const QStyleOptionViewItem &option,
	const QModelIndex &index) const;

private:
  QString QStringFromBool(bool b) const;
  QString genText(const ImageNode &node) const;
};

inline ImageListModel::ImageListModel(QObject *parent) : 
  QAbstractListModel(parent), theImages(0) {}

inline ImageListModel::ImageListModel(imageList *i, QObject *parent) : 
  QAbstractListModel(parent), theImages(i) {}

inline int ImageListModel::rowCount(const QModelIndex &parent) const 
{
  return theImages ? theImages->size() : 0;
}


inline ImageDelegate::ImageDelegate(QWidget *parent) : 
  QAbstractItemDelegate(parent) {}


#endif
