#include "IntervalData.h"

IntervalData::IntervalData( size_t nIndex, size_t nVertexValue )
	: m_setCols()
	, m_setOutConnections()
	, m_nIndex( nIndex )
	, m_nVertexValue( nVertexValue )
	, m_nRank( 0 )
{
}

IntervalData::IntervalData( const std::set<size_t>& setCols,
						    const std::set<size_t>& setOutConnections, 
							size_t nIndex, size_t nVertexValue )
	: m_setCols( setCols )
	, m_setOutConnections( setOutConnections )
	, m_nIndex( nIndex )
	, m_nVertexValue( nVertexValue )
	, m_nRank( 0 )
{
}

IntervalData::IntervalData(const IntervalData& other)
{
	m_setCols = other.m_setCols;
	m_setOutConnections = other.m_setOutConnections;
	m_nIndex = other.m_nIndex;
	m_nVertexValue = other.m_nVertexValue;
	m_nRank = other.m_nRank;
}

IntervalData& IntervalData::operator=(const IntervalData& other)
{
	m_setCols = other.m_setCols;
	m_setOutConnections = other.m_setOutConnections;
	m_nIndex = other.m_nIndex;
	m_nVertexValue = other.m_nVertexValue;
	m_nRank = other.m_nRank;

	return *this;
}

bool operator==(const IntervalData& interval1, const IntervalData& interval2)
{
	return interval1.getRank() == interval2.getRank() &&
		   interval1.getCols() == interval2.getCols();
}

bool operator<( const IntervalData& interval1, const IntervalData& interval2 )
{
	// Determines whether start of the interval1 appears lefter,
	// than start of the interval2 or not.
	size_t nStart1 = *( interval1.m_setCols.begin() ),
		   nStart2 = *( interval2.m_setCols.begin() );

	return nStart1 < nStart2;
}
	
bool operator>( const IntervalData& interval1, const IntervalData& interval2 )
{
	// Determines whether start of the interval1 is righter,
	// than start of the interval2 or not.
	return !( interval1 < interval2 );
}

bool operator<<( const IntervalData& interval1, const IntervalData& interval2 )
{
	// Determines whether interval1 is absolutely smaller than interval2 or not,
	// meaning that start of the interval2 appears righter than end of the interval1
	size_t nEnd1 = *( interval1.m_setCols.rbegin() ),
		   nStart2 = *( interval2.m_setCols.begin() );
	
	return nEnd1 < nStart2;
}

bool operator>>( const IntervalData& interval1, const IntervalData& interval2 )
{
	// Determines whether interval1 is absolutely greater than interval2 or not,
	// meaning that start of the interval1 appears righter than end of the interval2
	size_t nStart1 = *( interval1.m_setCols.begin() ),
		   nEnd2 = *( interval2.m_setCols.rbegin() );
	
	return nStart1 > nEnd2;
}
