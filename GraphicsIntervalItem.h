#pragma once
#include <QGraphicsItem>
#include <QPainter>
#include <set>

class GraphicsIntervalItem : public QGraphicsItem
{
public:
	GraphicsIntervalItem( const std::set<size_t>& setCols, 
						  double dDeltaX, double dRadius, 
						  QGraphicsItem* pParent = nullptr ) 
	: QGraphicsItem( pParent )
	, m_dDeltaX( dDeltaX )
	, m_dRadius( dRadius )
	{
		m_setCols = setCols;
		processColumnSet();
	}

	QRectF boundingRect() const override
	{
		qreal penWidth = 1;
		
		size_t dCol1 = *( m_setCols.begin() ),
			   dCol2 = *( m_setCols.rbegin() );

		// determines the rectangle where will be put the item
		return QRectF( -m_dRadius - penWidth / 2, -m_dRadius - penWidth / 2, 
					  ( dCol2 - dCol1 ) * m_dDeltaX + penWidth, m_dRadius + penWidth );
	}

	void paint(QPainter* painter,
		const QStyleOptionGraphicsItem* option,
		QWidget* widget) override
	{
		QPen pen( Qt::black, 2 );
		QBrush brush( pen.color() );
		painter->setPen( pen );
		painter->setBrush( brush );

		size_t dCol1 = *( m_setCols.begin() ),
			   dCol2 = *( m_setCols.rbegin() );

		QPoint pointStart( m_dRadius, m_dRadius ),
			   pointEnd( m_dRadius + ( dCol2 - dCol1 ) * m_dDeltaX, m_dRadius );
			   
		painter->drawLine( pointStart, pointEnd );

		for( size_t dCol : m_setCols )
		{
			QPointF center( m_dRadius + dCol * m_dDeltaX, m_dRadius );
			painter->drawEllipse( center, m_dRadius, m_dRadius );
		}
	}

protected:
	void processColumnSet()
	{
		// We do not care about column index,
		// so offset all indexes to make first index 0
		
		// Offset value needs to be equal to first index,
		// so firstIndex - offset = 0
		size_t dOff = *( m_setCols.begin() ),
			   dCurVal;

		if( dOff == 0 )
			return;

		// As the set elements are not editable,
		// make a copy vector and edit elements in that vector
		std::vector<size_t> vecCols( m_setCols.begin(), m_setCols.end() );
		for( size_t& dCol : vecCols )
		{
			dCol -= dOff;
		}

		// Now clear the set and re-initialize it with vector's elements
		m_setCols.clear();
		m_setCols.insert( vecCols.begin(), vecCols.end() );
	}

protected:
	std::set<size_t> m_setCols;
	double m_dRow;
	double m_dDeltaX;
	double m_dRadius;
};

