#ifndef CUSTOMRECT_H
#define CUSTOMRECT_H
#include <QtCore>
#include <QtGui>
#include <QGraphicsItem>

class CustomRect : public QGraphicsItem
{


public:
    static int count;
    static void init() { count = 0; }
    CustomRect();

private:
    QRectF boundingRect() const;
    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option, QWidget *widget);

};



#endif // CUSTOMRECT_H
