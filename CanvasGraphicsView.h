#pragma once
#include "GraphicsView.h"

class CanvasGraphicsScene;

class CanvasGraphicsView : public Base::GraphicsView
{
	Q_OBJECT
public:
	CanvasGraphicsView(QWidget* pParent = nullptr);

public slots:
	void wheelEvent(QWheelEvent* event);
	void refreshCanvasScene();

protected:
	CanvasGraphicsScene* m_pCanvasScene;
};

