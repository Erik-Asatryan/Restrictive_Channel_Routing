#include "CanvasGraphicsScene.h"

#include "GraphicsIntervalItem.h"
#include "IntervalGraph.h"
#include "IntervalData.h"

#include <QWidget>
#include <QLineEdit>
#include <QIntValidator>

#include <QGraphicsTextItem>
#include <QGraphicsProxyWidget>

#include <QColor>
#include <QPen>
#include <QBrush>

#include <vector>

// experiments
#include <QDebug>

CanvasGraphicsScene::CanvasGraphicsScene( size_t nRows, size_t nCols, 
										  qreal x, qreal y, qreal width, qreal height, 
										  QWidget* pParent)
	: GraphicsScene( x, y, width, height, pParent )
	, m_nRows( 1 ) //@TODO we don't need nRows passed here
	, m_nCols( nCols )
{
	//setBackgroundBrush(Qt::darkGreen);

	/* DRAW TERMINALS 
	for (size_t i = 0; i < m_nRows; ++i)
	{
		QPen pen(Qt::gray, 1);
		// draw terminals
		drawLine( i, 0, i, m_nCols - 1, pen );
	}
	*/

	QColor golden( 163, 136, 49 );
	QBrush brush( golden );
	QPen pen;//( golden );

	for( size_t i = 0; i < m_nCols; ++i )
	{
		// draw top pins
		drawPin( -1, i, 2, true, pen );

		// draw bottom pins
		drawPin( m_nRows, i, 2, false, pen );
	}
	
	double dDeltaX = sceneRect().width() / ( m_nCols + 1 );

	//std::vector<size_t> vecTop    = { 1, 2, 1, 4, 3, 0, 5 },
	//					vecBottom = { 3, 3, 2, 1, 5, 4, 0 };
	//std::vector<size_t> vecTop =    { 1, 0, 0, 2, 0, 3, 0, 4, 0, 5, 0, 0, 6, 0, 7, 0, 0, 0, 8, 0, 0 },
	//					vecBottom = { 0, 0, 1, 0, 2, 0, 3, 0, 4, 0, 0, 5, 0, 6, 0, 0, 0, 7, 0, 0, 8 };
	//std::vector<size_t> vecTop    = { 1, 2, 1, 1, 0, 2, 0, 3, 4, 5, 6 },
	//					vecBottom = { 2, 3, 3, 6, 1, 4, 2, 4, 6, 6, 0 };

	refresh();

	/*

	// @TODO add dialog to ask user for vertex vectors.
	IntervalGraph intervalGraph( vecTop, vecBottom );
	std::multimap< size_t, IntervalData > mapIntervals( intervalGraph.getMapIntervals() );
	
	qDebug() << "Interval count: " << mapIntervals.size();
	for( auto& it : mapIntervals )
	{
		drawInterval( it.second.getCols(), it.first, dDeltaX, 2, pen );
	}
	*/
}

bool CanvasGraphicsScene::saveTopItemsInfo( std::vector<size_t>& vecTopInfo )
{
	// If there is nothing to save, then return
	if( m_vecTopItems.empty() )
		return false;

	QLineEdit* pCurLineEdit;

	// Go through the top items and save their values in the "vecTopInfo" vector
	for( auto it = m_vecTopItems.begin(); it != m_vecTopItems.end(); ++it )
	{
		// line edit should exist
		assert( pCurLineEdit = *it );

		const QString& sText = pCurLineEdit->text();

		bool ok;
		size_t nVertexVal = sText.toUInt( &ok );

		// If the value is not integer reset it to 0
		if( !ok )
		{
			nVertexVal = 0;
			pCurLineEdit->setText( std::to_string( nVertexVal ).c_str() );
		}

		// Save the value in the vector in corresponding index
		vecTopInfo.emplace_back( nVertexVal );
	}
	
	return true;
}

bool CanvasGraphicsScene::saveBottomItemsInfo( std::vector<size_t>& vecBottomInfo )
{
	// If there is nothing to save, then return
	if( m_vecBottomItems.empty() )
		return false;

	QLineEdit* pCurLineEdit;

	// Go through the top items and save their values in the "vecTopInfo" vector
	for( auto it = m_vecBottomItems.begin(); it != m_vecBottomItems.end(); ++it )
	{
		// line edit should exist
		assert( pCurLineEdit = *it );

		const QString& sText = pCurLineEdit->text();
		
		bool ok;
		size_t nVertexVal = sText.toUInt( &ok );

		// If the value is not integer reset it to 0
		if( !ok )
		{
			nVertexVal = 0;
			pCurLineEdit->setText( std::to_string( nVertexVal ).c_str() );
		}

		vecBottomInfo.emplace_back( nVertexVal );
	}

	return true;
}

void CanvasGraphicsScene::clearCanvas()
{
	clear();
	m_vecTopItems.clear();
	m_vecBottomItems.clear();
}

void CanvasGraphicsScene::drawMagistrals( const QPen& pen )
{
	for ( size_t i = 0; i < m_nRows; ++i )
	{
		// draw magistrals 
		drawLine( i, 0, i, m_nCols - 1, pen );
	}
}

void CanvasGraphicsScene::drawPins( const std::vector<size_t>& vecTopInfo, 
									const std::vector<size_t>& vecBottomInfo, 
									const QPen& pen )
{
	for( size_t i = 0; i < m_nCols; ++i )
	{
		// draw top pins
		drawPin( -1, i, 2, true, pen );
		m_vecTopItems[i]->setText( std::to_string( vecTopInfo[i] ).c_str() );

		// draw bottom pins
		drawPin( m_nRows, i, 2, false, pen );
		m_vecBottomItems[i]->setText( std::to_string( vecBottomInfo[i] ).c_str() );
	}
}



void CanvasGraphicsScene::refresh()
{
	std::vector<size_t> vecTop, vecBottom;

	saveTopItemsInfo( vecTop );
	saveBottomItemsInfo( vecBottom );

	//std::vector<size_t> vecTop = { 1, 2, 1, 1, 0, 2, 0, 3, 4, 5, 6 },
	//					vecBottom = { 2, 3, 3, 6, 1, 4, 2, 4, 6, 6, 0 };
	
	clearCanvas();

	QPen penPins,
		 penMagistrals( Qt::gray, 1 ),
		 penIntervals;

	IntervalGraph intervalGraph( vecTop, vecBottom );
	using MultimapRowInterval = IntervalGraph::MultimapRowInterval;
	MultimapRowInterval multimapIntervals( intervalGraph.getMapIntervals() );
	m_nRows = multimapIntervals.empty() ? 1 : multimapIntervals.rbegin()->first + 1;

	drawPins( vecTop, vecBottom, penPins );

	drawMagistrals( penMagistrals );

	double dDeltaX = sceneRect().width() / ( m_nCols + 1 );

	for( auto& it : multimapIntervals )
	{
		drawInterval( it.second.getCols(), it.first, dDeltaX, 2, penIntervals );
		qDebug() << it.second.getVertexValue();
	}

	update( sceneRect() );
} 

QPoint CanvasGraphicsScene::getOffsetPosFromIndex(double dRow, double dCol, double dOffX, double dOffY)
{
	double dWidth = sceneRect().width(), 
		   dDeltaX = dWidth / ( m_nCols + 1 ),
		   dX = dDeltaX + dCol * dDeltaX + dOffX,
		   dHeight = sceneRect().height(),
		   dDeltaY = dHeight / ( m_nRows + 3 ),
		   dY = 2 * dDeltaY + dRow * dDeltaY + dOffY;

	return { (int)dX, (int)dY };
}

QLine CanvasGraphicsScene::getLine( double dRow1, double dCol1, double dRow2, double dCol2 )
{
	const QPoint& pt1 = getPosFromIndex( dRow1, dCol1 ),
				  pt2 = getPosFromIndex( dRow2, dCol2 );

	return { pt1, pt2 };
}

QGraphicsLineItem* 
CanvasGraphicsScene::drawLine( double dRow1, double dCol1, 
							   double dRow2, double dCol2, 
							   const QPen& pen /* = QPen() */ )
{
	return addLine( getLine( dRow1, dCol1, dRow2, dCol2 ), pen );
}

QGraphicsEllipseItem* 
CanvasGraphicsScene::drawPin( double dRow, double dCol, 
								 double dRad, bool bIsTop, const QPen& pen /* = QPen() */ )
{
	// Add ellipse at index [dRow][dCol]
	// Left top angle of rectangle containing the circle should be
	// offset ${radius} to left and up, so the center of the circle
	// will be right on the position from index.
	const QPoint& leftTopAngle = getOffsetPosFromIndex( dRow, dCol, -dRad );

	QLineEdit* textItemEdit = new QLineEdit("0");
	textItemEdit->setFixedWidth( 30 );
	textItemEdit->setStyleSheet( "border: 0px");
	textItemEdit->setValidator( new QIntValidator( 0, 100, textItemEdit ) );

	auto textItem = addWidget( textItemEdit );

	if( bIsTop )
	{
		m_vecTopItems.push_back( textItemEdit );
		textItem->setPos( leftTopAngle.x() - 3, leftTopAngle.y() - 25 );
	}
	else
	{
		m_vecBottomItems.push_back( textItemEdit );
		textItem->setPos( leftTopAngle.x() - 3, leftTopAngle.y() + 5 );
	}

	QBrush brush( pen.color() );
	return addEllipse( leftTopAngle.x(), leftTopAngle.y(), 
					   2 * dRad, 2 * dRad, pen, brush );
}

GraphicsIntervalItem* 
CanvasGraphicsScene::drawInterval( const std::set<size_t>& setCols, double dRow, 
								   double dDeltaX, double dRad, const QPen& pen /* = QPen() */ )
{
	qDebug() << "Draw interval";
	GraphicsIntervalItem* pIntervalItem = new GraphicsIntervalItem( setCols, dDeltaX, dRad );
	double dCol = *( setCols.begin() );
	pIntervalItem->setPos( getOffsetPosFromIndex( dRow, dCol, -dRad ) );
	addItem( pIntervalItem );

	return pIntervalItem;
}

QPoint CanvasGraphicsScene::getOffsetPosFromIndex( double dRow, double dCol, double dOff )
{
	return getOffsetPosFromIndex( dRow, dCol, dOff, dOff );
}

QPoint CanvasGraphicsScene::getPosFromIndex( double dRow, double dCol )
{
	return getOffsetPosFromIndex( dRow, dCol, 0 );
}

