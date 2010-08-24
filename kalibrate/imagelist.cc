/* -*- c++ -*- */
/*
 * Copyright (C) 2010 M G Berberich
 */

#include <QPainter>
#include <QPixmap>
#include <QFont>
#include <QFileInfo>

#include <KLocale>

#include "imagelist.h"


/** 
 * initializes the ImageNode from a filename
 * 
 * @param filename 
 */
void ImageNode::set(const QString &filename)
{
  image.load(filename);
  thumb = QPixmap::fromImage(image.scaledToWidth(150, Qt::SmoothTransformation));
  points = 0;
  active = true;
  extrinsic = false;
  name = filename;
}


/** 
 * returns the @a ImageNode for the image number @a index.
 * 
 * does not differentiate for roles
 * 
 * @param index 
 * @param role 
 * 
 * @return a Variant containing a pointer to the @a ImageNode
 */
QVariant ImageListModel::data(const QModelIndex &index, int role) const
{
  if (!index.isValid() || index.row() >= rowCount())
    return QVariant();
  
  if (role == Qt::DisplayRole) {
    const ImageNode *node = &theImages->at(index.row());
    return QVariant::fromValue(node);
  } else
    return QVariant();
}

/** 
 * return header-datas (none needed here)
 * 
 * @param section 
 * @param orientation 
 * @param role 
 * 
 * @return 
 */
QVariant ImageListModel::headerData(int section, Qt::Orientation orientation, 
      int role) const
{
  if (role != Qt::DisplayRole)
    return QVariant();
  
  return QVariant();
} 

/** 
 * Generates a string containing ✓ or ✗ on state of @a b
 * 
 * @param b 
 * 
 * @return 1-Character-String (✓ or ✗)
 */
QString ImageDelegate::QStringFromBool(bool b) const
{
  static const QString check = QString::fromUtf8("✓");
  static const QString cross = QString::fromUtf8("✗");
  return b ? check : cross;
}

/** 
 * Generate info-text
 *
 * Generates the info-text that is shown right to the image. It is a 
 * multiline-text
 * 
 * @param node The image 
 * 
 * @return produced string
 */
QString ImageDelegate::genText(const ImageNode &node) const
{
  QString text = i18n("Grid: %1\nPoints: %2\nExtrinsic: %3\nName: %4")
    .arg(QStringFromBool(node.active))
    .arg(node.points)
    .arg(QStringFromBool(node.extrinsic))
    .arg(QFileInfo(node.name).fileName());
  return text;
}

/** 
 * does all the painting
 *
 * thumbnail of image left, info-text to the right, 2px border around
 * 
 * @param painter 
 * @param option 
 * @param index 
 */
void ImageDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
      const QModelIndex &index) const
{
  const ImageNode *node = index.data(Qt::DisplayRole).value<const ImageNode*>();
  QString text = genText(*node);
  QSize textsize = option.fontMetrics.size(0, text);

  painter->save();
  painter->drawPixmap(option.rect.x()+2, option.rect.y()+2, node->thumb);
  painter->drawText(option.rect.x()+node->thumb.width()+4, option.rect.y()+2, 
	textsize.width(), textsize.height(), Qt::AlignLeft|Qt::AlignTop, text);
  painter->restore();
}

/** 
 * calculates size nedded to paint this entry
 * 
 * @param option 
 * @param index 
 * 
 * @return Size of the entry
 */
QSize ImageDelegate::sizeHint(const QStyleOptionViewItem &option,
      const QModelIndex &index) const
{
  const ImageNode *node = index.data(Qt::DisplayRole).value<const ImageNode*>();
  QSize textsize = option.fontMetrics.size(0, genText(*node));
  int w = 4 + node->thumb.width() + textsize.width();
  int h = qMax(textsize.height(), node->thumb.height()) + 4 ;
  return QSize(w, h);
}

/** 
 * Constructor for convinience-wrapper-class ImageListView 
 * 
 * @param images list of images to display
 * @param parent parent-widget
 */
ImageListView::ImageListView(const imageList *images, QWidget *parent) : 
  theModel(images) 
{
  setModel(&theModel);
  setItemDelegate(&theDelegate);
}
 
