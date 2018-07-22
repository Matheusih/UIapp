
#include "diagramitem.h"
#include "arrow.h"
#include "myarrow.h"

#include <QGraphicsScene>
#include <QGraphicsSceneContextMenuEvent>
#include <QMenu>
#include <QPainter>
#include <QLineEdit>
#include <QGraphicsProxyWidget>
#include <QLabel>

DiagramItem::DiagramItem(DiagramType diagramType, QMenu *contextMenu,QGraphicsItem *parent)
    : QGraphicsPolygonItem(parent)
{
    myDiagramType = diagramType;
    myContextMenu = contextMenu;
    setSize(50, 100);



    QPainterPath path;
    switch (myDiagramType) {
        case StartEnd:
            path.moveTo(200, 50);
            path.arcTo(150, 0, 50, 50, 0, 90);
            path.arcTo(50, 0, 50, 50, 90, 90);
            path.arcTo(50, 50, 50, 50, 180, 90);
            path.arcTo(150, 50, 50, 50, 270, 90);
            path.lineTo(200, 25);
            myPolygon = path.toFillPolygon();
            break;

        case Conditional:
            myPolygon << QPointF(-50, 0) << QPointF(0, 50)
                      << QPointF(50, 0) << QPointF(0, -50)
                      << QPointF(-50, 0);
            break;
        case Step:
            myPolygon << QPointF(-50, -25) << QPointF(50, -25)
                      << QPointF(50, 25) << QPointF(-50, 25)
                      << QPointF(-50, -25);
            break;
        default:
            myPolygon << QPointF(-120, -80) << QPointF(-70, 80)
                      << QPointF(120, 80) << QPointF(70, -80)
                      << QPointF(-120, -80);
            break;
    }
    setPolygon(myPolygon);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);


    //[1]** adds Label **//
    pMyProxy = new QGraphicsProxyWidget(this);
    label = new QLabel();

                    //label->setTextInteractionFlags(Qt::TextEditorInteraction);
    label->setStyleSheet("QLabel { background-color : white; color : black; }");
    label->setAttribute(Qt::WA_TranslucentBackground);
    pMyProxy->setWidget(label);
    setText(QString("Box"));
    myoutput = new myArrow(this);
    //[1]
}

void DiagramItem::removeArrow(Arrow *arrow)
{
    int index = arrows.indexOf(arrow);

    if (index != -1)
        arrows.removeAt(index);
}

void DiagramItem::removeArrows()
{
    foreach(myArrow *myarrow,myarrows){
        scene()->removeItem(myarrow->type);
        scene()->removeItem(myarrow);
        delete myarrow;
    }
    foreach (Arrow *arrow, arrows) {
        arrow->startItem()->removeArrow(arrow);
        arrow->endItem()->removeArrow(arrow);
        scene()->removeItem(arrow);
        delete arrow;
    }
    scene()->removeItem(this->myoutput->type);
    scene()->removeItem(this->myoutput);
}

void DiagramItem::addArrow(Arrow *arrow)
{
    arrows.append(arrow);
}

void DiagramItem::addMyArrow(myArrow *arrow)
{
    myarrows.append(arrow);
}

QPixmap DiagramItem::image() const
{
    QPixmap pixmap(250, 250);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    painter.setPen(QPen(Qt::black, 8));
    painter.translate(125, 125);
    painter.drawPolyline(myPolygon);

    return pixmap;
}

void DiagramItem::setText(const QString &text)
{
    label->setText(text);
    label->adjustSize();
    pMyProxy->setPos(boundingRect().center()-label->rect().center());
}




void DiagramItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    scene()->clearSelection();
    setSelected(true);
    myContextMenu->exec(event->screenPos());
}

QVariant DiagramItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == QGraphicsItem::ItemPositionChange) {
        QPointF newPos = value.toPointF();
        QPointF p1 = newPos + QPointF(0,this->height/2);
        QPointF p2 = newPos + QPointF(0,(this->height/2)+50);
        QPointF midPoint = (p1 + p2) / 2;
        QRectF rect = scene()->sceneRect();


        if (!rect.contains(newPos)) {
            // Keep the item inside the scene rect.
            newPos.setX(qMin(rect.right(), qMax(newPos.x(), rect.left())));
            newPos.setY(qMin(rect.bottom(), qMax(newPos.y(), rect.top())));

            p1 = newPos + QPointF(0,this->height/2);
            p2 = newPos + QPointF(0,(this->height/2)+50);
            midPoint = (p1 + p2) / 2;
            midPoint -= QPointF(4,10);

            genInputPos();
            this->myoutput->updatePosition( p2 , p1 );
            myoutput->type->setPos(midPoint);
            return newPos;
        }

        genInputPos();
        this->myoutput->updatePosition( p2, p1 );



        midPoint -= QPointF(4,10);
        myoutput->type->setPos(midPoint);

        foreach (Arrow *arrow, arrows) {
            arrow->updatePosition();
        }


    }
    return value;
}


void DiagramItem::genInputPos()
{
    if(myarrows.length() == 0)
        return;
    QPointF p, midPoint;
    int f = 0, ar = 0;
    float div, pos;
    if(myarrows.length() % 2 == 0 ){
        div = floor(50 / myarrows.length());
        ar = myarrows.length() / 2;
     }
    else if (myarrows.length() > 1){
        div = floor(50 / (myarrows.length()-1));
        f = 1;
        ar = (myarrows.length() - 1) / 2;
    }

    p = this->pos();
    p -= QPointF(0,(this->height/2));
    int i = 0;
    if(div < 10) div = 12;
    pos = floor(ar) * div;
    QPointF sp = p - QPointF(pos,0);
    if(!f){
        foreach(myArrow *myarrow, myarrows){
            if(i==ar && ar != 0)
                sp += QPointF(div,0);
            midPoint = (sp + (sp - QPointF(0,this->height))) / 2;
            midPoint -= QPointF(4,10);
            myarrow->type->setPos(midPoint);
            myarrow->updatePosition(sp, sp - QPointF(0,this->height));
            sp += QPointF(div,0);
            ++i;
        }
    }
    else{
        foreach(myArrow *myarrow, myarrows){
            midPoint = (sp + (sp - QPointF(0,this->height))) / 2;
            midPoint -= QPointF(4,10);
            myarrow->type->setPos(midPoint);
            myarrow->updatePosition(sp, sp - QPointF(0,this->height));
            sp += QPointF(div,0);
            ++i;
        }
    }
}

void DiagramItem::setSize(int height, int width)
{
    this->height = height;
    this->width = width;
}
