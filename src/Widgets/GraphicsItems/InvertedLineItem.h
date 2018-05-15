#ifndef INVERTEDLINEITEM_H
#define INVERTEDLINEITEM_H

#include <QGraphicsLineItem>
#include <QPainter>
#include <QPen>


class InvertedLineItem : public QGraphicsLineItem
{
public:
	InvertedLineItem(qreal x1, qreal y1, qreal x2, qreal y2, QGraphicsItem *i=nullptr) : QGraphicsLineItem(x1,y1,x2,y2,i){}
	InvertedLineItem(const QLineF &l, QGraphicsItem *i=nullptr) : QGraphicsLineItem(l,i){}

protected:
	void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*);
};

#endif // INVERTEDLINEITEM_H
