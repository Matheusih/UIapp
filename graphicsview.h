#ifndef GRAPHICSVIEW_H
#define GRAPHICSVIEW_H

#include <QGraphicsView>

class GraphicsView  : public QGraphicsView
{
public:

    using QGraphicsView::QGraphicsView;
    void resizeEvent(QResizeEvent *event)
;};

#endif // GRAPHICSVIEW_H
