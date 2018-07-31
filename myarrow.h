

#ifndef MYARROW_H
#define MYARROW_H

#include <QGraphicsLineItem>
#include "diagramitem.h"
#include "diagramscene.h"
#include <QtWidgets>
#include <QGraphicsPixmapItem>

class CustomRect;

#include <QGraphicsPathItem>

class myArrow : public QGraphicsPathItem
{
public:
    enum arrowType {Circle, Square, Triangle, Cond};
    myArrow(QGraphicsPolygonItem *item, arrowType arType = Circle);
    void updatePosition(QPointF p1, QPointF p2);
    QGraphicsPolygonItem *type;
    QPolygonF typePolygon;
private:
    QGraphicsPolygonItem *myitem;
    arrowType myType;
};

#endif // MYARROW_H
