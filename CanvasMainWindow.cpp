#include "CanvasMainWindow.h"

#include "CanvasGraphicsView.h"
#include "ToolBar.h"

#include <QDebug>
#include <QTimer>

CanvasMainWindow::CanvasMainWindow(QWidget* pParent) : 
	MainWindow( pParent )
{
	Base::ToolBar* pToolBar = new Base::ToolBar();
	QAction* act = pToolBar->addAction( "Refresh" );
	pToolBar->setMovable( false );

	addToolBar( Qt::TopToolBarArea, pToolBar );
	setWindowFlags( Qt::Widget );

	CanvasGraphicsView* pGraphicsView = new CanvasGraphicsView( this );
	connect( act, SIGNAL( triggered() ),
			 pGraphicsView, SLOT( refreshCanvasScene() ));

 	setCentralWidget( pGraphicsView );

	repaint();
	show();
	
	QTimer::singleShot( 3000, pGraphicsView, SLOT( zoomIn()));
}
