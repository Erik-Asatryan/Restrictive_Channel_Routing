#include "IntervalGraph.h"

#include "IntervalData.h"
#include "Algorithms.h"

#include <QString>
#include <QDebug>

IntervalGraph::IntervalGraph( const VecOfValues& vecTop, const VecOfValues& vecBottom )
	: m_vecTop( vecTop )
	, m_vecBottom( vecBottom )
{
	// Interval graph is being constructed using top and bottom vertex vectors
	
	// if( vecTop.size() != vecBottom.size() )
	//	@TODO throw exception or expand small one with zeros

	// Set to 0 all the top values, which are not appear in bottom vector
	// and all the bottom values, which do not appear in top vector  
	preprocessTopAndBottomVectors();
	
	// Create vector of intervals based on the top and bottom vectors of vertexes
	InitializeIntervals();

	// Seperate intervals by their rank (length of the longest path)
	VecOfMultisetIntervals vecOfSetOfIntervals = separateIntervalsByRank();
	
	// Place intervals with same ranks on corresponding rows
	// Non-intersecting intervals should be placed on the same row
	determineRowsOfIntervals( vecOfSetOfIntervals );
}

void IntervalGraph::preprocessTopAndBottomVectors()
{
	SetOfValues setTop( m_vecTop.begin(), m_vecTop.end() ),
				setBottom( m_vecBottom.begin(), m_vecBottom.end() );
	
	for( size_t nVertexVal : setTop )
	{
		// Remove value ${nVertexVal} from the ${setBottom},
		// if value appears in both top and bottom vectors
		if( setBottom.find( nVertexVal ) != setBottom.end() )
			setBottom.erase( nVertexVal );
		// If the value ${nVertexVal} has not been found in the bottom vector
		// replace those values with 0 in the top vector to not process them.
		else
			std::replace( m_vecTop.begin(), m_vecTop.end(), nVertexVal, size_t(0) );
	}

	// At this moment in the setBottom appear only those vertex values
	// which do not appear in the top vector, 
	// so replace them with zeros in the bottom vector to not process them.
	for( size_t nVertexVal : setBottom )
	{
		std::replace( m_vecBottom.begin(), m_vecBottom.end(), nVertexVal, size_t(0) );
	}
}

void IntervalGraph::InitializeIntervals()
{
	// Processed interval vertex values should be saved here
	// to not process already processed intervals
	SetOfValues setProcessedCols = { 0 };

	for( size_t i = 0; i < m_vecTop.size(); ++i )
	{
		int nVertexVal = m_vecTop[i];

		// Not process already processed intervals
		if( setProcessedCols.find( nVertexVal ) != setProcessedCols.end() )
			continue;

		// Remember that the interval is processed
		setProcessedCols.emplace( nVertexVal );

		size_t nIndex = m_vecIntervals.size();
		IntervalData intervalCur( nIndex, nVertexVal );

		// Add all the top vertexes with value ${nVertexVal} to set of columns
		addTopVertexes( intervalCur, i );
		
		// Add all the bottom vertexes with value ${nVertexVal} to set of columns
		bool bAdd = addBottomVertexes( intervalCur );

		// Add the interval only in case, when there is at least one vertex
		// with value ${nVertexVal} in the bottom vector
		if( bAdd )
			m_vecIntervals.emplace_back( intervalCur );
	}
}

void IntervalGraph::addTopVertexes( IntervalData& intervalCur, size_t nCurIndex )
{
	// Search vertex with same value in the top vector.
	// Start from ${nCurIndex} element, 
	// as the previous ones from the ${m_vecTop} are already processed
	for( size_t i = nCurIndex; i < m_vecTop.size(); ++i )
	{
		if ( m_vecTop[i] == intervalCur.getVertexValue() )
		{
			// Interval has vertex at point i
			intervalCur.addCol( i );

			// Add information about out connection in the VC graph
			if ( m_vecBottom[i] != 0 && m_vecBottom[i] != m_vecTop[i] )
			{
				intervalCur.addOutConnection( m_vecBottom[i] );
			}
		}
	}
}

bool IntervalGraph::addBottomVertexes( IntervalData& intervalCur )
{
	bool bThereIsAtLeastOneBottom = false;

	// Search vertex with same value in the bottom vector.
	for( size_t i = 0; i < m_vecBottom.size(); ++i )
	{
		if( m_vecBottom[i] == intervalCur.getVertexValue() )
		{
			bThereIsAtLeastOneBottom = true;
			// Interval has vertex at point ${i}
			intervalCur.addCol( i );
		}
	}

	return bThereIsAtLeastOneBottom;
}

IntervalGraph::MatrixOfValues IntervalGraph::getAdjacencyMatrix()
{
	// At this moment we can create adjacency matrix
	std::map< size_t, size_t > mapVertexValToIndex;
	for( const IntervalData& interval : m_vecIntervals )
	{
		mapVertexValToIndex.emplace( interval.getVertexValue(), interval.getIndex() );
	}

	size_t nIntervalCount = m_vecIntervals.size();

	// Create matrix of zeros
	MatrixOfValues matrixAdjacency( nIntervalCount, VecOfValues( nIntervalCount, 0 ) );

	for( size_t i = 0; i < m_vecIntervals.size(); ++i )
	{
		// Set element[i][j] to 1 if there is connection 
		// from interval[i] -> interval[j] in VC
		for( size_t nToVertexVal : m_vecIntervals[i].getOutConnections() )
		{
			size_t nTemp = mapVertexValToIndex[ nToVertexVal ];
			matrixAdjacency[ nTemp ][ i ] = 1;
		}
	}

	return matrixAdjacency;
}

IntervalGraph::MatrixOfValues IntervalGraph::getIdentityMatrix()
{
	size_t nIntervalCount = m_vecIntervals.size();
	MatrixOfValues matrixIdentity( nIntervalCount, VecOfValues( nIntervalCount, 0 ) );

	for( size_t i = 0; i < nIntervalCount; ++i )
	{
		matrixIdentity[i][i] = 1;
	}

	return matrixIdentity;
}

IntervalGraph::SetOfValues IntervalGraph::getSetOfIntervalIndexes()
{
	size_t nIntervalCount = m_vecIntervals.size();
	SetOfValues setIntervalIndexes;

	for( size_t i = 0; i < nIntervalCount; ++i )
	{
		setIntervalIndexes.emplace( i );
	}

	return setIntervalIndexes;
}


IntervalGraph::IterMultisetIntervals
IntervalGraph::findFirstAbsoluteBiggerThan( const IntervalGraph::IterMultisetIntervals& itB, 
											const IntervalGraph::IterMultisetIntervals& itE,
											const IntervalData& interval )
{
	return std::find_if( itB, itE, [=]( const IntervalData& interval1 )
				{ 
					return interval1 >> interval; 
				});
}

IntervalGraph::VecOfMultisetIntervals IntervalGraph::separateIntervalsByRank()
{
	MatrixOfValues matrixAdjacency = getAdjacencyMatrix();

	// Get set of indexes { 1, 2, 3, ..., n }
	SetOfValues setNonZeroIndexes = getSetOfIntervalIndexes();
	
	// Create vector of zeros to check lines of the matrix 
	VecOfValues vecOfZeros = VecOfValues( m_vecIntervals.size(), 0 );

	// First assign identity matrix to get adjacency matrix on the first power
	MatrixOfValues matrixPowAdjacency = getIdentityMatrix();

	VecOfMultisetIntervals vecOfSetOfIntervals;
	int nRank = -1;

	// Length of the longest path to the interval[i] in the VC graph equals to 
	// the power of ${nRank} of the adjacency matrix.
	while( !Algorithms::isMatrixOfZeros( matrixPowAdjacency ) )
	{
		++nRank;
		vecOfSetOfIntervals.emplace_back();
		matrixPowAdjacency = Algorithms::multiplyMatrixes( matrixPowAdjacency, matrixAdjacency );

		// Check all the intervals, corresponding rows of which were not equal to 
		// { 0, 0, ..., 0 } on the previous step.
		for( auto it = setNonZeroIndexes.begin(); it != setNonZeroIndexes.end(); )
		{
			size_t nCurIndex = *it;
			// If it became { 0, 0, ..., 0 }, set its rank and 
			// remove from the set of the unprocessed intervals
			if( matrixPowAdjacency[ nCurIndex ] == vecOfZeros )
			{
				m_vecIntervals[ nCurIndex ].setRank( nRank );
				vecOfSetOfIntervals[ nRank ].emplace( m_vecIntervals[ nCurIndex ] );
				// Same as ++it and remove (it - 1), 
				// so do not need to increment iterator in this case
				it = setNonZeroIndexes.erase( it );
			}
			else
				++it;
		}
	}

	return vecOfSetOfIntervals;
}

void IntervalGraph::determineRowsOfIntervals(VecOfMultisetIntervals& vecOfSetIntervals )
{
	int nRow = 0;

	while( !vecOfSetIntervals.empty() )
	{
		auto& setIntervals = vecOfSetIntervals[0];

		// Go through each element and determine its' row
		while ( !setIntervals.empty() )
		{
			IntervalData processedInterval = *( setIntervals.begin() );
			// At this point there is no interval on this row, so we assign
			// current interval to ${nRow}
			m_mapRowInterval.emplace( nRow, processedInterval );
			setIntervals.erase( processedInterval );

			IterMultisetIntervals it = setIntervals.begin();

			// Assign all the fitting intervals to the ${nRow}.
			// Intervals on the same row should not intersect, so each next interval 
			// on the row should be absolute bigger than previous. To fit as much intervals 
			// as possible, that interval should be smallest of the absolute bigger ones.
			// As they are sorted in ascending order, in each step search absolute bigger
			// interval not from the start, but from the curernt interval to the end.
			while( ( it = findFirstAbsoluteBiggerThan( it, setIntervals.end(), processedInterval ) )
				!= setIntervals.end() )
			{
				m_mapRowInterval.emplace( nRow, *it );
				processedInterval = *it;
				it = setIntervals.erase( it );
			}
			++nRow;
		}

		vecOfSetIntervals.erase( vecOfSetIntervals.begin() );
	}
}
