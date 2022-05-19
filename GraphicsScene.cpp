#include "GraphicsScene.h"

using namespace Base;

#include <QWidget>

GraphicsScene::GraphicsScene( qreal x, qreal y, qreal width, qreal height, 
							  QWidget* pParent )
	: QGraphicsScene( x, y, width, height, pParent )
{
}
