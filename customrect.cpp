#include "customrect.h"

int CustomRect::count = 0;

CustomRect::CustomRect()
{
    count++;
    this->setFlag(QGraphicsItem::ItemIsMovable);
}

QRectF CustomRect::boundingRect() const
{
    QRectF rec(10,10,100,100);
    return rec;
}

void CustomRect::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QRectF rec = this->boundingRect();
    QBrush brush(Qt::black);
    QPen pen(Qt::red);
    pen.setWidth(3);
    QString str = QString("Box: %1").arg(count);
    painter->setPen(pen);
    painter->fillRect( rec, brush );
    painter->drawRect(rec);
    painter->drawText(rec,Qt::AlignCenter,str);
}
