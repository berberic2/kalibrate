
#include <QPainter>
#include <QPixmap>

#include "imagelist.h"

void ImageDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
      const QModelIndex &index) const
{
  painter->save();
  ImageNode node = index.model()->data(index, Qt::DisplayRole).value<ImageNode>();
  painter->drawPixmap(option.rect.x(), option.rect.y(), node.thumb);
  painter->restore();
}

QSize ImageDelegate::sizeHint(const QStyleOptionViewItem &option,
      const QModelIndex &index) const
{
  ImageNode node = index.model()->data(index, Qt::DisplayRole).value<ImageNode>();
  int w = 70;
  int h = qMax(40, node.thumb.height()+4);
  return QSize(w, h);
}


ImageListModel::ImageListModel(QObject *parent) : 
  QAbstractListModel(parent) 
{ 
  ImageNode i; 
  i.image.load("/tmp/p0000095.jpg");
  // bmg: wise to do so
  i.thumb = QPixmap::fromImage(i.image.scaledToWidth(150, Qt::SmoothTransformation));
  i.points = 221;
  i.active = true;
  i.extrinsic = false;
  i.name = "p0000095.jpg";
  images.push_back(i); 
}

int ImageListModel::rowCount(const QModelIndex &parent) const 
{
  return 50;
}

QVariant ImageListModel::data(const QModelIndex &index, int role) const
{
  if (!index.isValid() || index.row() >= 50)
    return QVariant();
  
  if (role == Qt::DisplayRole) {
    return QVariant::fromValue(images[0]);
  } else
    return QVariant();
}

QVariant ImageListModel::headerData(int section, Qt::Orientation orientation, 
      int role) const
{
  if (role != Qt::DisplayRole)
    return QVariant();
  
  return QVariant();
} 
