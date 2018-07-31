#include "myarrow.h"
#include "diagramitem.h"
#include <QPainter>
#include <cmath>

myArrow::myArrow(QGraphicsPolygonItem *item, parentType ptype, arrowType arType){
    myitem = item;
    myType = arType;
    parentObj = ptype;
    QPointF p = item->polygon().boundingRect().center();
    updatePosition(p /*- QPointF(0, 50)*/, p /*- QPointF(0, 250)*/);
    setFlag(QGraphicsLineItem::ItemIsSelectable);
    artype = new QGraphicsPolygonItem(QRectF(0,0,8,8));

    QRectF temp(0,0,8,8);
    QPainterPath path;
    switch(myType){
        case Circle:
            path.moveTo(temp.center() + QPoint(4,0));
            path.arcTo(temp, 0.0, 360.0);
            typePolygon = path.toFillPolygon();
            artype->setBrush(Qt::blue);
            break;
        case Square:
            typePolygon << QPointF(0,0)  << QPointF(8,0)
                        << QPointF(8, 8) << QPointF(0,8)
                        << QPointF(0,0);
            artype->setBrush(Qt::green);
            break;

        case Triangle:
            typePolygon << QPointF(4,0) << QPointF(8,8)
                        << QPointF(0,8)
                        << QPointF(4,0);
            artype->setBrush(Qt::red);
            break;
        case Cond:
            typePolygon << QPointF(0,4) << QPointF(4,8)
                        << QPointF(8,4) << QPointF(4,0)
                        << QPointF(0,4);
            artype->setBrush(Qt::yellow);
            break;
    }


    artype->setPolygon(typePolygon);
    //type->setBrush(Qt::green);
    setBrush(Qt::black);
}


void myArrow::updatePosition(QPointF p1, QPointF p2)
{
    const qreal arrowSize = 10;

    QPainterPath path;

    path.moveTo(p1);
    path.lineTo(p2);

    QPointF diff = p2-p1;
    double angle = std::atan2(-diff.y(), diff.x());

    QPointF arrowP1 = p1 + QPointF(sin(angle + M_PI / 3) * arrowSize,
                                    cos(angle + M_PI / 3) * arrowSize);
    QPointF arrowP2 = p1 + QPointF(sin(angle + M_PI - M_PI / 3) * arrowSize,
                                    cos(angle + M_PI - M_PI / 3) * arrowSize);
    QPolygonF arrowHead;
    arrowHead << p1 << arrowP1 << arrowP2 << p1;
    path.moveTo(p1);
    path.addPolygon(arrowHead);

    setPath(path);
    update();
}
