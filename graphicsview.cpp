#include "graphicsview.h"

void GraphicsView::resizeEvent(QResizeEvent *event)
{
    //fitInView( sceneRect() ,Qt::KeepAspectRatio);
    QGraphicsView::resizeEvent(event);

}
