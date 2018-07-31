#include "diagramscene.h"
#include "arrow.h"
#include "myarrow.h"
#include <QTextCursor>
#include <QGraphicsSceneMouseEvent>

DiagramScene::DiagramScene(QMenu *itemMenu,QMenu *sceneMenu ,QObject *parent)
    : QGraphicsScene(parent)
{
    myItemMenu = itemMenu;
    mySceneMenu = sceneMenu;
    myMode = MoveItem;
    myItemType = DiagramItem::Step;
    line = 0;
    textItem = 0;
    myItemColor = Qt::white;
    myTextColor = Qt::black;
    myLineColor = Qt::black;
    //........
    radialGradient = new QRadialGradient(QPoint(35,20),100);
    radialGradient->setColorAt(0.0, Qt::white);
    radialGradient->setColorAt(0.5, myItemColor);
    radialGradient->setColorAt(1.0, Qt::black);
    //.........
    QPainterPath textPath;
    QFont timesFont("Times", 50);
    timesFont.setStyleStrategy(QFont::ForceOutline);
    //textPath.addText(10, 70, timesFont, tr("Home"));
    QGraphicsPathItem * test = new QGraphicsPathItem(textPath);
    test->setBrush(*radialGradient);
    test->setFlag(QGraphicsItem::ItemIsMovable);
    test->setPos(QPointF(0,0));
    //addItem(test);

}

void DiagramScene::setLineColor(const QColor &color)
{
    myLineColor = color;
    if (isItemChange(Arrow::Type)) {
        Arrow *item = qgraphicsitem_cast<Arrow *>(selectedItems().first());
        item->setColor(myLineColor);
        update();
    }
}

void DiagramScene::setTextColor(const QColor &color)
{
    myTextColor = color;
    if (isItemChange(DiagramTextItem::Type)) {
        DiagramTextItem *item = qgraphicsitem_cast<DiagramTextItem *>(selectedItems().first());
        item->setDefaultTextColor(myTextColor);
    }
}

void DiagramScene::setItemColor(const QColor &color)
{
    myItemColor = color;
    radialGradient->setColorAt(0.5, myItemColor);
    if (isItemChange(DiagramItem::Type)) {
        DiagramItem *item = qgraphicsitem_cast<DiagramItem *>(selectedItems().first());
        item->setBrush(*radialGradient);
    }
}

void DiagramScene::setFont(const QFont &font)
{
    myFont = font;

    if (isItemChange(DiagramTextItem::Type)) {
        QGraphicsTextItem *item = qgraphicsitem_cast<DiagramTextItem *>(selectedItems().first());
        //At this point the selection can change so the first selected item might not be a DiagramTextItem
        if (item)
            item->setFont(myFont);
    }
}

QColor DiagramScene::getColor()
{
    return this->myItemColor;
}

void DiagramScene::setMode(Mode mode)
{
    myMode = mode;
}

void DiagramScene::setItemType(DiagramItem::DiagramType type)
{
    myItemType = type;
}

void DiagramScene::editorLostFocus(DiagramTextItem *item)
{
    QTextCursor cursor = item->textCursor();
    cursor.clearSelection();
    item->setTextCursor(cursor);

    if (item->toPlainText().isEmpty()) {
        removeItem(item);
        item->deleteLater();
    }
}

void DiagramScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (mouseEvent->button() != Qt::LeftButton){
        if(this->selectedItems().isEmpty())
            mySceneMenu->exec(mouseEvent->screenPos());
        return;
    }


    DiagramItem *item;
    //myArrow *output;
    QPointF p1, p2, midPoint;
    QPen itemPen;
    itemPen.setJoinStyle(Qt::RoundJoin);
    itemPen.setCapStyle(Qt::RoundCap);
    switch (myMode) {
        case InsertItem:
            item = new DiagramItem(myItemType, myItemMenu);
            item->setPen(itemPen);
            item->setBrush(*radialGradient);
            addItem(item);
            item->setPos(mouseEvent->scenePos());
            //.....
            p1 = item->pos() + QPointF(0,item->height/2); //moves p1 to bottom of figure
            p2 = item->pos() + QPointF(0, (item->height/2) + 50); //moves p2 to bottom of figure + offset (50)
            midPoint = (p2 + p1) / 2;
            midPoint -= QPointF(4,10);
            item->myoutput->updatePosition( p2 , p1 );

            addItem(item->myoutput);

            addItem(item->myoutput->artype);
            item->myoutput->artype->setPos(midPoint);

            emit itemInserted(item);
            break;
        case InsertLine:
            line = new QGraphicsLineItem(QLineF(mouseEvent->scenePos(),
                                        mouseEvent->scenePos()));
            line->setPen(QPen(myLineColor, 2));
            addItem(line);
            break;
        case InsertText:
            textItem = new DiagramTextItem();
            textItem->setFont(myFont);
            textItem->setTextInteractionFlags(Qt::TextEditorInteraction);
            textItem->setZValue(1000.0);
            connect(textItem, SIGNAL(lostFocus(DiagramTextItem*)),
                    this, SLOT(editorLostFocus(DiagramTextItem*)));
            connect(textItem, SIGNAL(selectedChange(QGraphicsItem*)),
                    this, SIGNAL(itemSelected(QGraphicsItem*)));
            addItem(textItem);
            textItem->setDefaultTextColor(myTextColor);
            textItem->setPos(mouseEvent->scenePos());
            emit textInserted(textItem);
    default:
        ;
    }
    QGraphicsScene::mousePressEvent(mouseEvent);
}

void DiagramScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (myMode == InsertLine && line != 0) {
        QLineF newLine(line->line().p1(), mouseEvent->scenePos());
        line->setLine(newLine);
    } else if (myMode == MoveItem) {
        QGraphicsScene::mouseMoveEvent(mouseEvent);
    }
}

void DiagramScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (line != 0 && myMode == InsertLine) {
        QList<QGraphicsItem *> startItems = items(line->line().p1());
        if (startItems.count() && startItems.first() == line)
            startItems.removeFirst();
        QList<QGraphicsItem *> endItems = items(line->line().p2());
        if (endItems.count() && endItems.first() == line)
            endItems.removeFirst();

        removeItem(line);
        delete line;

        if (startItems.count() > 0 && endItems.count() > 0 &&
            startItems.first()->type() == DiagramItem::Type &&
            endItems.first()->type() == DiagramItem::Type &&
            startItems.first() != endItems.first()) {
            DiagramItem *startItem = qgraphicsitem_cast<DiagramItem *>(startItems.first());
            DiagramItem *endItem = qgraphicsitem_cast<DiagramItem *>(endItems.first());
            Arrow *arrow = new Arrow(startItem, endItem);
            arrow->setColor(myLineColor);
            startItem->addArrow(arrow);
            endItem->addArrow(arrow);
            arrow->setZValue(-1000.0);
            addItem(arrow);
            arrow->updatePosition();
        }
    }
    line = 0;
    QGraphicsScene::mouseReleaseEvent(mouseEvent);
}

bool DiagramScene::isItemChange(int type)
{
    foreach (QGraphicsItem *item, selectedItems()) {
        if (item->type() == type)
            return true;
    }
    return false;
}


void DiagramScene::addArrowScene(myArrow *arrow){
    sceneArrows.append(arrow);
}

void DiagramScene::genSceneInputPos()
{
    int f = 0, ar = 0;
    float div, pos;
    QPointF p, q, midPoint;

    p =  this->sceneRect().center() - QPointF(0,this->height()/2);
    q = p - QPointF(0,100);
    midPoint = (p+q)/2 - QPointF(4,0);

    if(sceneArrows.length() == 0)
        return;
    if(sceneArrows.length() % 2 == 0 ){
        div = floor(this->width() / sceneArrows.length());
        ar = sceneArrows.length() / 2;
     }
    else if (sceneArrows.length() > 1){
        div = floor(this->width() / (sceneArrows.length()-1));
        f = 1;
        ar = (sceneArrows.length() - 1) / 2;
    }

    int i = 0;
    pos = floor(ar) * div;

    QPointF sp = p - QPointF(pos,0);

    if(!f){
        foreach(myArrow *myarrow, sceneArrows){
            if(i==ar && ar != 0)
                sp += QPointF(div,0);
            midPoint = (sp + (sp - QPointF(0,100))) / 2;
            midPoint -= QPointF(4,10);
            myarrow->artype->setPos(midPoint);
            myarrow->updatePosition(sp, sp - QPointF(0,100));
            sp += QPointF(div,0);
            ++i;
        }
    }
    else{
        foreach(myArrow *myarrow, sceneArrows){
            midPoint = (sp + (sp - QPointF(0,100))) / 2;
            midPoint -= QPointF(4,10);
            myarrow->artype->setPos(midPoint);
            myarrow->updatePosition(sp, sp - QPointF(0,100));
            sp += QPointF(div,0);
            ++i;
        }
    }
}

void DiagramScene::removeArrow(myArrow *arrow)
{
    int index = sceneArrows.indexOf(arrow);

    if (index != -1)
        sceneArrows.removeAt(index);
}
