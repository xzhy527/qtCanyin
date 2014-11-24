#ifndef DISHESSCALE_H
#define DISHESSCALE_H

#include <QItemDelegate>
#include <QLabel>
#include <QPainter>
#include <QDebug>
#include <QApplication>
class DishesScale : public QItemDelegate
{
    Q_OBJECT
public:
    explicit DishesScale(QObject *parent = 0);

signals:

public slots:

public:
    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const; 
};



#endif // DISHESSCALE_H
