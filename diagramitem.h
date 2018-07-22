
#ifndef DIAGRAMITEM_H
#define DIAGRAMITEM_H

#include <QGraphicsPixmapItem>
#include <QList>
#include <QLabel>
#include <QGraphicsProxyWidget>

class QPixmap;
class QGraphicsItem;
class QGraphicsScene;
class QTextEdit;
class QGraphicsSceneMouseEvent;
class QMenu;
class QGraphicsSceneContextMenuEvent;
class QPainter;
class QStyleOptionGraphicsItem;
class QWidget;
class QPolygonF;
class Arrow;
class myArrow;

class DiagramItem : public QGraphicsPolygonItem
{
public:



    enum { Type = UserType + 15 };
    enum DiagramType { Step, Conditional, StartEnd, Io };

    DiagramItem(DiagramType diagramType, QMenu *contextMenu, QGraphicsItem *parent = nullptr);
    void removeArrow(Arrow *arrow);
    void removeArrows();
    DiagramType diagramType() const { return myDiagramType; }
    QPolygonF polygon() const { return myPolygon; }
    void addArrow(Arrow *arrow);
    void addMyArrow(myArrow *arrow);
    QPixmap image() const;
    int type() const override { return Type;}

    void setText(const QString & text);
    void genInputPos();
    void setSize(int height, int width);
    QList<myArrow *> myarrows;
    myArrow *myoutput;
    int height, width;

protected:
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

private:
    QLabel *label;
    QGraphicsProxyWidget* pMyProxy;
    DiagramType myDiagramType;
    QPolygonF myPolygon;
    QMenu *myContextMenu;
    QList<Arrow *> arrows;
    QString *myLabelStr;

};

#endif // DIAGRAMITEM_H
