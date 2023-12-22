#ifndef IMAGEDELEGATE_H
#define IMAGEDELEGATE_H

#include <QStyledItemDelegate>
#include <QPixmap>
#include <QBuffer>
#include <QDebug>


class ImageDelegate : public QStyledItemDelegate
{
  Q_OBJECT

public:
  ImageDelegate(QObject *parent = 0) : QStyledItemDelegate(parent) {}

  void paint(QPainter *painter, const QStyleOptionViewItem &option,
             const QModelIndex &index) const;

};


#endif // IMAGEDELEGATE_H
