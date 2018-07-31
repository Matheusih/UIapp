

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
    enum { Type = UserType + 30 };
    int type() const override { return Type;}
    enum arrowType {Circle, Square, Triangle, Cond};
    enum parentType {DiagramItem , DiagramScene};
    myArrow(QGraphicsPolygonItem *item, parentType ptype, arrowType arType = Circle);
    void updatePosition(QPointF p1, QPointF p2);
    QGraphicsPolygonItem *artype;
    QPolygonF typePolygon;
    parentType parentObj;
    QGraphicsPolygonItem *myitem;
private:
    arrowType myType;

};

#endif // MYARROW_H
