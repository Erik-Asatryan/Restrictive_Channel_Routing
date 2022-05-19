#pragma once
#include <QGraphicsScene>

namespace Base
{
    class GraphicsScene : public QGraphicsScene
    {
    public:
	    GraphicsScene( qreal x, qreal y, qreal width, qreal height, 
					   QWidget* pParent = nullptr);
    };
}

