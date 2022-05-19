#pragma once
#include "GraphicsScene.h"

#include <set>

class GraphicsIntervalItem;
class QLineEdit;

class CanvasGraphicsScene : public Base::GraphicsScene
{
public:
	CanvasGraphicsScene(size_t nRows, size_t nCols, 
						qreal x, qreal y, qreal width, qreal height, 
						QWidget* pParent = nullptr);

	void refresh();

protected:
	QPoint getOffsetPosFromIndex( double dRow, double dCol, 
								  double dOffX, double dOffY );
	QPoint getOffsetPosFromIndex( double dRow, double dCol, double dOff );
	QPoint getPosFromIndex( double dRow, double dCol );
	
	
	QLine getLine( double dRow1, double dCol1, 
				   double dRow2, double dCol2 );

	QGraphicsLineItem* drawLine( double dRow1, double dCol1, 
								 double dRow2, double dCol2, 
								 const QPen& pen = QPen() );
	QGraphicsEllipseItem* drawPin( double dRow, double dCol,
									  double dRad, bool bIsTop, const QPen& pen = QPen() );
	GraphicsIntervalItem* drawInterval( const std::set<size_t>& setCols, double dRow, double dDeltaX,
									    double dRad, const QPen& pen = QPen() );

	bool saveTopItemsInfo( std::vector<size_t>& vecTopInfo );
	bool saveBottomItemsInfo( std::vector<size_t>& vecBottomInfo );
	void clearCanvas();
	void drawMagistrals( const QPen& pen = QPen() );
	void drawPins( const std::vector<size_t>& vecTopInfo, 
				   const std::vector<size_t>& vecBottomInfo, const QPen& pen = QPen() );

protected:
	size_t m_nRows;
	size_t m_nCols;
	std::vector< QLineEdit* > m_vecTopItems;
	std::vector< QLineEdit* > m_vecBottomItems;
};

