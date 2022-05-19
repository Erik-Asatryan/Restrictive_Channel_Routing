#pragma once
#include <vector>
#include <set>

namespace Algorithms
{
	using VecOfValues = std::vector< size_t >;
	using MatrixOfValues = std::vector< VecOfValues >;

	bool haveSameElements( const std::vector<int>& vecTop, const std::vector<int>& vecBottom )
	{
		std::set<int> setTop( vecTop.begin(), vecTop.end() ),
					  setBottom( vecBottom.begin(), vecBottom.end() );

		// Need to ignore 0s, so add them to both sets
		setTop.emplace( 0 );
		setBottom.emplace( 0 );

		return setTop == setBottom;
	}
	
	bool isMatrixOfZeros( const MatrixOfValues& matrix )
	{
		for( size_t nRow = 0; nRow < matrix.size(); ++nRow )
		{
			for( size_t nCol = 0; nCol < matrix.size(); ++nCol )
			{
				if( matrix[ nRow ][ nCol ] != 0 )
				{
					return false;
				}
			}
		}

		return true;
	}

	MatrixOfValues multiplyMatrixes( const MatrixOfValues& matrixFirst, 
									 const MatrixOfValues& matrixSecond )
	{
		size_t nRows = matrixFirst.size();

		if( nRows != matrixSecond.size() )
		{
			return MatrixOfValues();
		}

		for( size_t i = 0; i < matrixFirst.size(); ++i )
		{
			if( matrixFirst[i].size() != matrixSecond[i].size() )
			{
				return MatrixOfValues();
			}
		}

		MatrixOfValues matrixRes( nRows, VecOfValues( nRows, 0 ) );

		for( size_t i = 0; i < nRows; ++i )
		{
			for( size_t j = 0; j < nRows; ++j )
			{
				matrixRes[i][j] = 0;
				for( size_t k = 0; k < nRows; ++k )
				{
					matrixRes[i][j] += matrixFirst[i][k] * matrixSecond[k][j];	
				}
			}
		}

		return matrixRes;
	}
}
