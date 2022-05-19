#pragma once
#include <vector>
#include <set>
#include <map>

class IntervalData;
struct compareByRank;

class IntervalGraph
{
public:
	using VecOfValues = std::vector< size_t >;
	using SetOfValues = std::set< size_t >;
	using VecOfIntervals = std::vector< IntervalData >;
	using MatrixOfValues = std::vector< VecOfValues >;
	using MultisetIntervals = std::multiset< IntervalData >;
	using IterMultisetIntervals = MultisetIntervals::iterator;
	using MultimapRowInterval = std::multimap< size_t, IntervalData >;
	using VecOfMultisetIntervals = std::vector< MultisetIntervals >;

public:
	IntervalGraph( const VecOfValues& vecTop, const VecOfValues& vecBottom );

	const MultimapRowInterval& getMapIntervals() const { return m_mapRowInterval; }

protected:
	void preprocessTopAndBottomVectors();
	void InitializeIntervals();

	void addTopVertexes( IntervalData& intervalCur, size_t nCurIndex );
	bool addBottomVertexes( IntervalData& intervalCur );
	
	MatrixOfValues getAdjacencyMatrix();
	MatrixOfValues getIdentityMatrix();
	SetOfValues getSetOfIntervalIndexes();

	IterMultisetIntervals findFirstAbsoluteBiggerThan( const IterMultisetIntervals& itB,
													   const IterMultisetIntervals& itE,
													   const IntervalData& interval);

	void determineRowsOfIntervals( VecOfMultisetIntervals& vecOfSetIntervals );
	VecOfMultisetIntervals separateIntervalsByRank();


protected:
	VecOfValues m_vecTop;
	VecOfValues m_vecBottom;
	VecOfIntervals m_vecIntervals;	
	MultimapRowInterval m_mapRowInterval;
};
