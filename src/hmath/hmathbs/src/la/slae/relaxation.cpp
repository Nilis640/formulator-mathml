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

#include <algorithm>
#include "relaxation.h"
#include "slae_internal.h"

//////////////////////////////////////////////////////////////////////

MATHBASE_API 
void _Relaxation( CDSRMMatrix<CDSRReal> *pA, CDSRMVector<CDSRReal> *pB, CDSRMVector<CDSRReal> *pX, unsigned long nRelaxCount, CDSRReal rlPrecision )
{
	if( pA->n_row() != pA->n_column() )
		throw "The matrix is not square";
	if( pA->n_row() != (long) pB->size() )
		throw "Internal problem - sizes of the source and destination matrix are not equal";

	long i, j, k, counter = 0;
	bool bPrecision = false;
	CDSRReal value;
	CDSRMVector<CDSRReal> vector;
	CDSRMVector<long> relitems( std::min( (unsigned long) pA->n_row(), std::max( (unsigned long) 1, nRelaxCount ) ) );
	CDSRMVector<long> othitems( pA->n_row() - (long) relitems.size() );

	for( i = 0; i < pA->n_row(); i++ )
	{
		if( IsNull( (*pA)( i, i ) ) )
			throw "SLAE cannot be solved by this method or matrix is singular";

		value = fabs( (*pA)( i, i ) );
		for( j = 0; j < pA->n_row(); j++ )
			if( i != j && value < fabs( (*pA)( i, j ) ) )
				throw "SLAE cannot be solved by iteration method or matrix is singular";
	}

	for( i = 0; i < (long) relitems.size(); i++ )
		relitems[ i ] = i;
	for( i = 0; i < (long) othitems.size(); i++ )
		othitems[ i ] = relitems.size() + i;

	pX->resize( pA->n_row() );
	vector.resize( pA->n_row(), 1.0 );

	while( !bPrecision )
	{
		for( i = 0; i < pA->n_row(); i++ )
		{
			vector[ i ] = -(*pB)[ i ];
			for( j = 0; j < pA->n_row(); j++ )
 				vector[ i ] += (*pA)( i, j ) * (*pX)[ j ];
		}

		for( j = 0; j < (long) relitems.size(); j++ )
			for( i = 0; i < (long) othitems.size(); i++ )
				if( fabs( vector[ othitems[ i ] ] ) > fabs( vector[ relitems[ j ] ] ) )
				{
					k = relitems[ j ];
					relitems[ j ] = othitems[ i ];
					othitems[ i ] = k;
				}

		for( j = 0; j < (long) relitems.size(); j++ )
			(*pX)[ relitems[ j ] ] -= vector[ relitems[ j ] ] / 
				(*pA)( relitems[ j ], relitems[ j ] );

		counter++;
		bPrecision = true;
		for( i = 0; i < pA->n_row(); i++ )
			if( fabs(vector[ i ] / (*pA)( i, i ) ) > rlPrecision )
			{
				bPrecision = false;
				break;
			}

		if( counter > ALMASLAE_MAX_ITER_COUNT )
			throw "Iteration counter limit exceeded";
	}
}

//////////////////////////////////////////////////////////////////////

MATHBASE_API 
void _RelaxationC( CDSRMMatrix<CDSRReal> *pA, CDSRMVector<CDSRReal> *pB, CDSRMVector<CDSRReal> *pX, unsigned long nRelaxCount, CDSRReal rlPrecision )
{
	if( pA->n_row() != pA->n_column() )
		throw "The matrix is not square";
	if( pA->n_row() != (long) pB->size() )
		throw "Internal problem - sizes of the source and destination matrix are not equal";

	long i, j, k, counter = 0;
	bool bPrecision = false;
	CDSRReal value;
	CDSRMVector<CDSRReal> vector;
	CDSRMVector<long> col( pA->n_row() );
	CDSRMVector<long> relitems( std::min( (unsigned long) pA->n_row(), std::max( (unsigned long) 1, nRelaxCount ) ) );
	CDSRMVector<long> othitems( pA->n_row() - (long) relitems.size() );

	for( i = 0; i < pA->n_row(); i++ )
		col[ i ] = i;

	for( i = 0; i < pA->n_row(); i++ )
	{
		// тест главной диагонали
		_IndexC( i, pA, &col );

		// проверка на сходимость
		value = fabs( (*pA)( i, col[ i ] ) );
		for( j = 0; j < pA->n_row(); j++ )
			if( i != j && value < fabs( (*pA)( i, col[ j ] ) ) )
				throw "SLAE cannot be solved by iteration method or matrix is singular";
	}

	for( i = 0; i < (long) relitems.size(); i++ )
		relitems[ i ] = i;
	for( i = 0; i < (long) othitems.size(); i++ )
		othitems[ i ] = relitems.size() + i;

	pX->resize( pA->n_row() );
	vector.resize( pA->n_row(), 1.0 );

	while( !bPrecision )
	{
		for( i = 0; i < pA->n_row(); i++ )
		{
			vector[ col[ i ] ] = -(*pB)[ i ];
			for( j = 0; j < pA->n_row(); j++ )
 				vector[ col[ i ] ] += (*pA)( i, col[ j ] ) * (*pX)[ col[ j ] ];
		}

		for( j = 0; j < (long) relitems.size(); j++ )
			for( i = 0; i < (long) othitems.size(); i++ )
				if( fabs( vector[ col[ othitems[ i ] ] ] ) > fabs( vector[ col[ relitems[ j ] ] ] ) )
				{
					k = relitems[ j ];
					relitems[ j ] = othitems[ i ];
					othitems[ i ] = k;
				}

		for( j = 0; j < (long) relitems.size(); j++ )
			(*pX)[ col[ relitems[ j ] ] ] -= vector[ col[ relitems[ j ] ] ] / 
				(*pA)( relitems[ j ], col[ relitems[ j ] ] );

		counter++;
		bPrecision = true;
		for( i = 0; i < pA->n_row(); i++ )
			if( fabs( vector[ col[ i ] ] / (*pA)( i, col[ i ] ) ) > rlPrecision )
			{
				bPrecision = false;
				break;
			}

		if( counter > ALMASLAE_MAX_ITER_COUNT )
			throw "Iteration counter limit exceeded";
	}
}

//////////////////////////////////////////////////////////////////////

MATHBASE_API 
void _RelaxationR( CDSRMMatrix<CDSRReal> *pA, CDSRMVector<CDSRReal> *pB, CDSRMVector<CDSRReal> *pX, unsigned long nRelaxCount, CDSRReal rlPrecision )
{
	if( pA->n_row() != pA->n_column() )
		throw "The matrix is not square";
	if( pA->n_row() != (long) pB->size() )
		throw "Internal problem - sizes of the source and destination matrix are not equal";

	long i, j, k, counter = 0;
	bool bPrecision = false;
	CDSRReal value;
	CDSRMVector<CDSRReal> vector;
	CDSRMVector<long> row( pA->n_row() );
	CDSRMVector<long> relitems( std::min( (unsigned long) pA->n_row(), std::max( (unsigned long) 1, nRelaxCount ) ) );
	CDSRMVector<long> othitems( pA->n_row() - (long) relitems.size() );

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

	for( i = 0; i < (long) relitems.size(); i++ )
		relitems[ i ] = i;
	for( i = 0; i < (long) othitems.size(); i++ )
		othitems[ i ] = relitems.size() + i;

	pX->resize( pA->n_row() );
	vector.resize( pA->n_row(), 1.0 );

	while( !bPrecision )
	{
		for( i = 0; i < pA->n_row(); i++ )
		{
			vector[ i ] = -(*pB)[ row[ i ] ];
			for( j = 0; j < pA->n_row(); j++ )
 				vector[ i ] += (*pA)( row[ i ], j ) * (*pX)[ j ];
		}

		for( j = 0; j < (long) relitems.size(); j++ )
			for( i = 0; i < (long) othitems.size(); i++ )
				if( fabs( vector[ othitems[ i ] ] ) > fabs( vector[ relitems[ j ] ] ) )
				{
					k = relitems[ j ];
					relitems[ j ] = othitems[ i ];
					othitems[ i ] = k;
				}

		for( j = 0; j < (long) relitems.size(); j++ )
			(*pX)[ relitems[ j ] ] -= vector[ relitems[ j ] ] / 
				(*pA)( row[ relitems[ j ] ], relitems[ j ] );

		counter++;
		bPrecision = true;
		for( i = 0; i < pA->n_row(); i++ )
			if( fabs( vector[ i ] / (*pA)( row[ i ], i ) ) > rlPrecision )
			{
				bPrecision = false;
				break;
			}

		if( counter > ALMASLAE_MAX_ITER_COUNT )
			throw "Iteration counter limit exceeded";
	}
}

//////////////////////////////////////////////////////////////////////

MATHBASE_API 
void _RelaxationCR( CDSRMMatrix<CDSRReal> *pA, CDSRMVector<CDSRReal> *pB, CDSRMVector<CDSRReal> *pX, unsigned long nRelaxCount, CDSRReal rlPrecision )
{
	if( pA->n_row() != pA->n_column() )
		throw "The matrix is not square";
	if( pA->n_row() != (long) pB->size() )
		throw "Internal problem - sizes of the source and destination matrix are not equal";

	long i, j, k, counter = 0;
	bool bPrecision = false;
	CDSRReal value;
	CDSRMVector<CDSRReal> vector;
	CDSRMVector<long> row( pA->n_row() );
	CDSRMVector<long> col( pA->n_row() );
	CDSRMVector<long> relitems( std::min( (unsigned long) pA->n_row(), std::max( (unsigned long) 1, nRelaxCount ) ) );
	CDSRMVector<long> othitems( pA->n_row() - (long) relitems.size() );

	for( i = 0; i < pA->n_row(); i++ )
		row[ i ] = col[ i ] = i;

	for( i = 0; i < pA->n_row(); i++ )
	{
		_IndexCR( i, pA, &row, &col );

		value = fabs( (*pA)( row[ i ], col[ i ] ) );
		for( j = 0; j < pA->n_row(); j++ )
			if( i != j && value < fabs( (*pA)( row[ i ], col[ j ] ) ) )
				throw "SLAE cannot be solved by iteration method or matrix is singular";
	}

	for( i = 0; i < (long) relitems.size(); i++ )
		relitems[ i ] = i;
	for( i = 0; i < (long) othitems.size(); i++ )
		othitems[ i ] = relitems.size() + i;

	pX->resize( pA->n_row() );
	vector.resize( pA->n_row(), 1.0 );

	while( !bPrecision )
	{
		for( i = 0; i < pA->n_row(); i++ )
		{
			vector[ col[ i ] ] = -(*pB)[ row[ i ] ];
			for( j = 0; j < pA->n_row(); j++ )
 				vector[ col[ i ] ] += (*pA)( row[ i ], col[ j ] ) * (*pX)[ col[ j ] ];
		}

		for( j = 0; j < (long) relitems.size(); j++ )
			for( i = 0; i < (long) othitems.size(); i++ )
				if( fabs( vector[ col[ othitems[ i ] ] ] ) > fabs( vector[ col[ relitems[ j ] ] ] ) )
				{
					k = relitems[ j ];
					relitems[ j ] = othitems[ i ];
					othitems[ i ] = k;
				}

		for( j = 0; j < (long) relitems.size(); j++ )
			(*pX)[ col[ relitems[ j ] ] ] -= vector[ col[ relitems[ j ] ] ] / 
				(*pA)( row[ relitems[ j ] ], col[ relitems[ j ] ] );

		counter++;
		bPrecision = true;
		for( i = 0; i < pA->n_row(); i++ )
			if( fabs( vector[ col[ i ] ] / (*pA)( row[ i ], col[ i ] ) ) > rlPrecision )
			{
				bPrecision = false;
				break;
			}

		if( counter > ALMASLAE_MAX_ITER_COUNT )
			throw "Iteration counter limit exceeded";
	}
}

//////////////////////////////////////////////////////////////////////
