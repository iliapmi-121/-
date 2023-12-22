#include "imagedelegate.h"

#include <QByteArray>
#include <QPainter>
#include <QPixmap>
#include <QStyleOptionViewItem>

void ImageDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                        const QModelIndex &index) const
{
    if (index.data().canConvert<QByteArray>()) {
        QByteArray imageData = index.data().toByteArray();

        // Предположим, что данные включают расширение файла (например, ".jpg")
        QPixmap pixmap;
        pixmap.loadFromData(imageData);

        if (!pixmap.isNull()) {
            painter->drawPixmap(option.rect, pixmap);
        } else {
            QStyledItemDelegate::paint(painter, option, index);
        }
    } else {
        QStyledItemDelegate::paint(painter, option, index);
    }
}


