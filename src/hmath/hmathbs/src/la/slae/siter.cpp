/****************************************************************************
**
** Copyright (C) 2010 Andriy Kovalchuk, Vyacheslav Levytskyy,
** Igor Samolyuk, Valentyn Yanchuk (aka "Hermitech Laboratory")
**
** All rights reserved.
** Contact: Hermitech Laboratory (info@mmlsoft.com)
**
** This file is a part of the Formulator MathML Editor project
** (http://www.mmlsoft.com).
**
** Commercial Usage
** Licensees holding valid Formulator Commercial licenses may use this 
** file in accordance with the Formulator Commercial License Agreement 
** provided with the Software or, alternatively, in accordance with the 
** terms contained in a written agreement between you and 
** Hermitech Laboratory.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
** If you have questions regarding the use of this file, please contact
** Hermitech Laboratory at info@mmlsoft.com.
**
****************************************************************************/

#include "siter.h"
#include "slae_internal.h"

//////////////////////////////////////////////////////////////////////

MATHBASE_API 
void _SimpleIterations( CDSRMMatrix<CDSRReal> *pA, CDSRMVector<CDSRReal> *pB, CDSRMVector<CDSRReal> *pX, CDSRReal rlPrecision)
{
	if( pA->n_row() != pA->n_column() )
		throw "The matrix is not square";
	if( pA->n_row() != (long) pB->size() )
		throw "Internal problem - sizes of the source and destination matrix are not equal";

	long i, j, counter = 0;
	bool bPrecision = false;
	CDSRReal value;
	CDSRMVector<CDSRReal> vector;

	for( i = 0; i < pA->n_row(); i++ )
	{
		if( IsNull( (*pA)( i, i ) ) )
			throw "SLAE cannot be solved by this method or matrix is singular";

		value = fabs( (*pA)( i, i ) );
		for( j = 0; j < pA->n_row(); j++ )
			if( i != j && value < fabs( (*pA)( i, j ) ) )
				throw "SLAE cannot be solved by iteration method or matrix is singular";
	}

	pX->resize( pA->n_row() );
	vector.resize( pA->n_row(), 1.0 );

	while( !bPrecision )
	{
		for( i = 0; i < pA->n_row(); i++ )
		{
			(*pX)[ i ] = -(*pB)[ i ];
			for( j = 0; j < pA->n_row(); j++ )
				(*pX)[ i ] += (*pA)( i, j ) * vector[ j ];

			(*pX)[ i ] = vector[ i ] - (*pX)[ i ] / (*pA)( i, i );
		}

		counter++;
		bPrecision = true;
		for( i = 0; i < pA->n_row(); i++ )
			if( fabs( (*pX)[ i ] - vector[ i ] ) > rlPrecision )
			{
				bPrecision = false;
				vector = (*pX);
				break;
			}

		if( counter > ALMASLAE_MAX_ITER_COUNT )
			throw "Iteration counter limit exceeded";
	}
}

//////////////////////////////////////////////////////////////////////

MATHBASE_API 
void _SimpleIterationsC( CDSRMMatrix<CDSRReal> *pA, CDSRMVector<CDSRReal> *pB, CDSRMVector<CDSRReal> *pX, CDSRReal rlPrecision)
{
	if( pA->n_row() != pA->n_column() )
		throw "The matrix is not square";
	if( pA->n_row() != (long) pB->size() )
		throw "Internal problem - sizes of the source and destination matrix are not equal";

	long i, j, counter = 0;
	bool bPrecision = false;
	CDSRReal value;
	CDSRMVector<long> col( pA->n_row() );
	CDSRMVector<CDSRReal> vector;

	for( i = 0; i < pA->n_row(); i++ )
		col[ i ] = i;

	for( i = 0; i < pA->n_row(); i++ )
	{
		_IndexC( i, pA, &col );

		value = fabs( (*pA)( i, col[ i ] ) );
		for( j = 0; j < pA->n_row(); j++ )
			if( i != j && value < fabs( (*pA)( i, col[ j ] ) ) )
				throw "SLAE cannot be solved by iteration method or matrix is singular";
	}

	pX->resize( pA->n_row() );
	vector.resize( pA->n_row(), 1.0 );

	while( !bPrecision )
	{
		for( i = 0; i < pA->n_row(); i++ )
		{
			(*pX)[ col[ i ] ] = -(*pB)[ i ];
			for( j = 0; j < pA->n_row(); j++ )
				(*pX)[ col[ i ] ] += (*pA)( i, col[ j ] ) * vector[ col[ j ] ];

			(*pX)[ col[ i ] ] = vector[ col[ i ] ] - (*pX)[ col[ i ] ] / (*pA)( i, col[ i ] );
		}

		counter++;
		bPrecision = true;
		for( i = 0; i < pA->n_row(); i++ )
			if( fabs( (*pX)[ col[ i ] ] - vector[ col[ i ] ] ) > rlPrecision )
			{
				bPrecision = false;
				vector = (*pX);
				break;
			}

		if( counter > ALMASLAE_MAX_ITER_COUNT )
			throw "Iteration counter limit exceeded";
	}
}

//////////////////////////////////////////////////////////////////////

MATHBASE_API 
void _SimpleIterationsR( CDSRMMatrix<CDSRReal> *pA, CDSRMVector<CDSRReal> *pB, CDSRMVector<CDSRReal> *pX, CDSRReal rlPrecision)
{
	if( pA->n_row() != pA->n_column() )
		throw "The matrix is not square";
	if( pA->n_row() != (long) pB->size() )
		throw "Internal problem - sizes of the source and destination matrix are not equal";

	long i, j, counter = 0;
	bool bPrecision = false;
	CDSRReal value;
	CDSRMVector<long> row( pA->n_row() );
	CDSRMVector<CDSRReal> vector;

	for( i = 0; i < pA->n_row(); i++ )
		row[ i ] = i;

	for( i = 0; i < pA->n_row(); i++ )
	{
		_IndexR( i, pA, &row );

		value = fabs( (*pA)( row[ i ], i ) );
		for( j = 0; j < pA->n_row(); j++ )
			if( i != j && value < fabs( (*pA)( row[ i ], j ) ) )
				throw "SLAE cannot be solved by iteration method or matrix is singular";
	}

	pX->resize( pA->n_row() );
	vector.resize( pA->n_row(), 1.0 );

	while( !bPrecision )
	{
		for( i = 0; i < pA->n_row(); i++ )
		{
			(*pX)[ i ] = -(*pB)[ row[ i ] ];
			for( j = 0; j < pA->n_row(); j++ )
				(*pX)[ i ] += (*pA)( row[ i ], j ) * vector[ j ];

			(*pX)[ i ] = vector[ i ] - (*pX)[ i ] / (*pA)(row[ i ], i );
		}

		counter++;
		bPrecision = true;
		for( i = 0; i < pA->n_row(); i++ )
			if( fabs( (*pX)[ i ] - vector[ i ] ) > rlPrecision )
			{
				bPrecision = false;
				vector = (*pX);
				break;
			}

		if( counter > ALMASLAE_MAX_ITER_COUNT )
			throw "Iteration counter limit exceeded";
	}
}

//////////////////////////////////////////////////////////////////////

MATHBASE_API 
void _SimpleIterationsCR( CDSRMMatrix<CDSRReal> *pA, CDSRMVector<CDSRReal> *pB, CDSRMVector<CDSRReal> *pX, CDSRReal rlPrecision)
{
	if( pA->n_row() != pA->n_column() )
		throw "The matrix is not square";
	if( pA->n_row() != (long) pB->size() )
		throw "Internal problem - sizes of the source and destination matrix are not equal";

	long i, j, counter = 0;
	bool bPrecision = false;
	CDSRReal value;
	CDSRMVector<long> row( pA->n_row() );
	CDSRMVector<long> col( pA->n_row() );
	CDSRMVector<CDSRReal> vector;

	for( i = 0; i < pA->n_row(); i++ )
		row[ i ] = col[ i ] = i;

	for( i = 0; i < pA->n_row(); i++ )
	{
		_IndexCR( i, pA, &row, &col);

		value = fabs( (*pA)( row[ i ], col[ i ] ) );
		for( j = 0; j < pA->n_row(); j++ )
			if( i != j && value < fabs( (*pA)( row[ i ], col[ j ] ) ) )
				throw "SLAE cannot be solved by iteration method or matrix is singular";
	}

	pX->resize( pA->n_row() );
	vector.resize( pA->n_row(), 1.0 );

	while( !bPrecision )
	{
		for( i = 0; i < pA->n_row(); i++ )
		{
			(*pX)[ col[ i ] ] = -(*pB)[ row[ i ] ];
			for( j = 0; j < pA->n_row(); j++ )
				(*pX)[ col[ i ] ] += (*pA)( row[ i ], col[ j ] ) * vector[ col[ j ] ];

			(*pX)[ col[ i ] ] = vector[ col[ i ] ] - (*pX)[ col[ i ] ] / (*pA)( row[ i ], col[ i ] );
		}

		counter++;
		bPrecision = true;
		for( i = 0; i < pA->n_row(); i++ )
			if( fabs( (*pX)[ col[ i ] ] - vector[ col[ i ] ] ) > rlPrecision )
			{
				bPrecision = false;
				vector = (*pX);
				break;
			}

		if( counter > ALMASLAE_MAX_ITER_COUNT )
			throw "Iteration counter limit exceeded";
	}
}

//////////////////////////////////////////////////////////////////////
