#include "CanvasGraphicsView.h"

#include "CanvasGraphicsScene.h"

#include <QWheelEvent>
#include <QInputDialog>

#include <QVBoxLayout>
#include <QLabel>

CanvasGraphicsView::CanvasGraphicsView(QWidget* pParent)
	: GraphicsView( pParent )
{
	size_t nRows = 5,
		   nCols = 30;

	QInputDialog qInputDialog;
	qInputDialog.setWindowFlags(qInputDialog.windowFlags() & (~Qt::WindowContextHelpButtonHint) & (~Qt::WindowCloseButtonHint));
	qInputDialog.setWindowTitle( "Welcome" );
	qInputDialog.setLabelText( "Please input number of pins:");
	qInputDialog.setCancelButtonText( "Default (30)" );
	qInputDialog.setInputMode( QInputDialog::IntInput );
	qInputDialog.setIntMinimum( 5 );
	qInputDialog.setIntMaximum( 1000 );

	if( qInputDialog.exec() == QDialog::DialogCode::Accepted )
		nCols = qInputDialog.intValue();
	
	m_pCanvasScene = new CanvasGraphicsScene( nRows, nCols, 0, 0, 
											  nCols * 50, nRows * 40, this );
	setScene( m_pCanvasScene );
}

void CanvasGraphicsView::refreshCanvasScene()
{
	m_pCanvasScene->refresh();
	repaint();
}

void CanvasGraphicsView::wheelEvent(QWheelEvent* event)
{
	setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
	static const double scaleFactor = 1.25;
	static double currentScale = 1.0;  // stores the current scale value.
	static const double scaleMin = .1; // defines the min scale limit.

	if (event->modifiers().testFlag(Qt::ControlModifier))
	{
		if (event->delta() > 0)
			scale( scaleFactor, scaleFactor );
		else
			scale( 1 / scaleFactor, 1 / scaleFactor );
	}
	else
	{
		Base::GraphicsView::wheelEvent(event);
	}
}
