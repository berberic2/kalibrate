/* -*- c++ -*- */
/*
 * Copyright (C) 2010 M G Berberich
 */

#ifndef IMAGELIST_H
#define IMAGELIST_H

#include <vector>

#include <cmath>

#include <QAbstractItemDelegate>
#include <QAbstractListModel>
#include <QFontMetrics>
#include <QListView>
#include <QPointF>

/**
 * 2-dimensional point
 */
struct Point2
{
  double x, y;
  
  Point2() {}
  Point2(double a, double b) : x(a), y(b) {}
  void set(double a, double b) { x = a; y = b; }
  operator QPointF() const { return QPointF(x, y); } 
  Point2 operator- (const Point2 &b) const { return Point2(x-b.x, y-b.y); } 
  Point2 operator/ (double s) const { return Point2(x/s, y/s); }
   Point2 operator* (double s) const { return Point2(x*s, y*s); }
 double len() const { return sqrt(x*x+y*y); }
};

/** 
 * Grid-information 
 */
struct Grid
{
  std::vector<Point2> points;
  bool rectangular;
  int width, height;

  typedef std::vector<Point2>::iterator pointiterator;

  bool isRectangular() { return rectangular; }
  bool dimension(int w, int h) { rectangular = true; width = w; height = h; }
  size_t size() { return points.size(); }
};

/**
 * Information structure for an Image
 */
struct ImageNode
{
  QImage image;			/**< image */
  QPixmap thumb;		/**< thumbnail of image */
  bool active;			/**< image is used for calibration */
  bool extrinsic;		/**< save extrinsic calibration */
  QString name;			/**< filename of the image */
  Grid grid; 		        /**< grid found in this image */

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
  ImageListModel(const imageList *i, QObject *parent = 0);

  virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
  virtual QVariant data(const QModelIndex &index, 
	int role = Qt::DisplayRole) const;
  virtual QVariant headerData(int section, Qt::Orientation orientation, 
	int role = Qt::DisplayRole) const;
  void images(imageList *i) { theImages = i; }

private:
  const imageList *theImages;
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

class ImageListView : public QListView
{
public:
  ImageListView(const imageList *images, QWidget *parent = 0);
private:
  ImageDelegate theDelegate;
  ImageListModel theModel;
};

inline ImageListModel::ImageListModel(QObject *parent) : 
  QAbstractListModel(parent), theImages(0) {}

inline ImageListModel::ImageListModel(const imageList *i, QObject *parent) : 
  QAbstractListModel(parent), theImages(i) {}

inline int ImageListModel::rowCount(const QModelIndex &parent) const 
{
  return theImages ? theImages->size() : 0;
}


inline ImageDelegate::ImageDelegate(QWidget *parent) : 
  QAbstractItemDelegate(parent) {}


#endif
