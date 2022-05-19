#pragma once
#include <set>

class IntervalData
{
public:
	IntervalData( size_t nIndex, size_t nVertexValue );
	IntervalData( const std::set<size_t>& setCols,
				  const std::set<size_t>& setOutConnections, 
				  size_t nIndex, size_t nVertexValue );
	IntervalData( const IntervalData& other);
	IntervalData& operator=( const IntervalData& other);

	friend bool operator==( const IntervalData& interval1, const IntervalData& interval2 );
	friend bool operator<( const IntervalData& interval1, const IntervalData& interval2 );
	friend bool operator>( const IntervalData& interval1, const IntervalData& interval2 );
	friend bool operator<<( const IntervalData& interval1, const IntervalData& interval2 );
	friend bool operator>>( const IntervalData& interval1, const IntervalData& interval2 );

	std::set< size_t > getCols() const { return m_setCols; }
	std::set< size_t > getOutConnections() const { return m_setOutConnections; }
	size_t getIndex() const { return m_nIndex; }
	size_t getVertexValue() const { return m_nVertexValue; }
	size_t getRank() const { return m_nRank; }

	void setRank( size_t nRank ){ m_nRank = nRank; }
	inline void addCol( size_t nCol );
	inline void addOutConnection( size_t nOutConnection );

protected:
	std::set< size_t > m_setCols;
	std::set< size_t > m_setOutConnections;
	size_t m_nIndex;
	size_t m_nVertexValue;
	size_t m_nRank;
};

void IntervalData::addCol( size_t nCol )
{
	m_setCols.emplace( nCol );
}

void IntervalData::addOutConnection( size_t nOutConnection )
{
	m_setOutConnections.emplace( nOutConnection );
}