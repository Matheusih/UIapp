

#ifndef MYARROW_H
#define MYARROW_H

#include <QGraphicsLineItem>
#include "diagramitem.h"
#include <QtWidgets>
#include <QGraphicsPixmapItem>

class CustomRect;

#include <QGraphicsPathItem>

class myArrow : public QGraphicsPathItem
{
public:
    enum arrowType {Circle, Square, Triangle, Cond};
    myArrow(DiagramItem *item, arrowType arType = Circle);
    void updatePosition(QPointF p1, QPointF p2);
    QGraphicsPolygonItem *type;
    QPolygonF typePolygon;
private:
    DiagramItem *myitem;
    arrowType myType;
};

#endif // MYARROW_H
