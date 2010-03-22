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

#include "gauss.h"
#include "slae_internal.h"

//////////////////////////////////////////////////////////////////////

MATHBASE_API 
void _Gauss( CDSRMMatrix<CDSRReal> *pA, CDSRMVector<CDSRReal> *pB, CDSRMVector<CDSRReal> *pX )
{
	if( pA->n_row() != pA->n_column() )
		throw "The matrix is not square";
	if( pA->n_row() != (long) pB->size() )
		throw "Internal problem - sizes of the source and destination matrix are not equal";

	long i, j, k;
	CDSRReal value;
	CDSRMMatrix<CDSRReal> matrix( ( *pA ) );
	CDSRMVector<CDSRReal> vector( ( *pB ) );

	pX->resize( pB->size() );
	
	for( i = 0; i < pA->n_row(); i++ )
	{
		if( IsNull( matrix( i, i ) ) )
			throw "SLAE cannot be solved by this method or matrix is singular";

		for( k = i; k < pA->n_row(); k++ )
		{
			value = matrix( k, i );
			if( IsNotNull( value ) && IsNotOne( value ) )
			{
				vector[ k ] /= value;
				for( j = i + 1; j < pA->n_row(); j++ )
					matrix( k, j ) /= value;
				matrix( k, i ) = 1.0;
			}
		}

		for( k = i + 1; k < pA->n_row(); k++ )
			if( IsNotNull( matrix( k, i ) ) )
			{
				vector[ k ] -= vector[ i ];
				for( j = i + 1; j < pA->n_row(); j++ )
					matrix( k, j ) -= matrix( i, j );
				matrix( k, i ) = 0.0;
			}
	}

	for( i = pA->n_row() - 1 ; i > 0; i-- )
	{
		(*pX)[ i ] = vector[ i ];
		for( j = i + 1; j < pA->n_row(); j++ )
			(*pX)[ i ] -= matrix( i, j ) * (*pX)[ j ];
	}

	(*pX)[ 0 ] = vector[ 0 ];
	for( j = 1; j < pA->n_row(); j++ )
		(*pX)[ 0 ] -= matrix( 0, j ) * (*pX)[ j ];
}

//////////////////////////////////////////////////////////////////////

MATHBASE_API 
void _GaussC( CDSRMMatrix<CDSRReal> *pA, CDSRMVector<CDSRReal> *pB, CDSRMVector<CDSRReal> *pX )
{
	if( pA->n_row() != pA->n_column() )
		throw "The matrix is not square";
	if( pA->n_row() != (long) pB->size() )
		throw "Internal problem - sizes of the source and destination matrix are not equal";

	long i, j, k;
	CDSRReal value;
	CDSRMMatrix<CDSRReal> matrix( ( *pA ) );
	CDSRMVector<CDSRReal> vector( ( *pB ) );
	CDSRMVector<long> col( pA->n_row() );

	for( i = 0; i < pA->n_row(); i++ )
		col[ i ] = i;
	
	pX->resize( pB->size() );

	for( i = 0; i < pA->n_row(); i++ )
	{
		if( IsNull( matrix( i, col[ i ] ) ) )
			_IndexC( i, &matrix, &col );

		for( k = i; k < pA->n_row(); k++ )
		{
			value = matrix( k, col[ i ] );
			if( IsNotNull( value ) && IsNotOne( value ) )
			{
				vector[ k ] /= value;
				for( j = i + 1; j < pA->n_row(); j++ )
					matrix( k, col[ j ] ) /= value;
				matrix( k, col[ i ] ) = 1.0;
			}
		}

		for( k = i + 1; k < pA->n_row(); k++ )
			if( IsNotNull( matrix(k, col[ i ] ) ) )
			{
				vector[ k ] -= vector[ i ];
				for( j = i + 1; j < pA->n_row(); j++ )
					matrix( k, col[ j ] ) -= matrix( i, col[ j ] );
				matrix( k, col[ i ] ) = 0.0;
			}
	}

	for( i = pA->n_row() - 1 ; i > 0; i-- )
	{
		(*pX)[ col[ i ] ] = vector[ i ];
		for( j = i + 1; j < pA->n_row(); j++ )
			(*pX)[ col[ i ] ] -= matrix( i, col[ j ] ) * (*pX)[ col[ j ] ];
	}

	(*pX)[ col[ 0 ] ] = vector[ 0 ];
	for( j = 1; j < pA->n_row(); j++ )
		(*pX)[ col[ 0 ] ] -= matrix( 0, col[ j ] ) * (*pX)[ col[ j ] ];
}

//////////////////////////////////////////////////////////////////////

MATHBASE_API 
void _GaussR( CDSRMMatrix<CDSRReal> *pA, CDSRMVector<CDSRReal> *pB, CDSRMVector<CDSRReal> *pX )
{
	if( pA->n_row() != pA->n_column() )
		throw "The matrix is not square";
	if( pA->n_row() != (long) pB->size() )
		throw "Internal problem - sizes of the source and destination matrix are not equal";

	long i, j, k;
	CDSRReal value;
	CDSRMMatrix<CDSRReal> matrix( ( *pA ) );
	CDSRMVector<CDSRReal> vector( ( *pB ) );
	CDSRMVector<long> row( pA->n_row() );

	pX->resize( pB->size() );

	for( i = 0; i < pA->n_row(); i++ )
		row[ i ] = i;

	for( i = 0; i < pA->n_row(); i++ )
	{
		if( IsNull( matrix( row[ i ], i) ) )
			_IndexR( i, &matrix, &row );

		for( k = i; k < pA->n_row(); k++ )
		{
			value = matrix( row[ k ], i );
			if( IsNotNull( value ) && IsNotOne( value ) )
			{
				vector[ row[ k ] ] /= value;
				for( j = i + 1; j < pA->n_row(); j++ )
					matrix( row[ k ], j ) /= value;
				matrix( row[ k ], i ) = 1.0;
			}
		}

		for( k = i + 1; k < pA->n_row(); k++ )
			if( IsNotNull( matrix( row[ k ], i ) ) )
			{
				vector[ row[ k ] ] -= vector[ row[ i ] ];
				for( j = i + 1; j < pA->n_row(); j++ )
					matrix( row[ k ], j ) -= matrix( row[ i ], j );
				matrix( row[ k ], i ) = 0.0;
			}
	}

	for( i = pA->n_row() - 1; i > 0; i-- )
	{
		(*pX)[ i ] = vector[ row[ i ] ];
		for( j = i + 1; j < pA->n_row(); j++ )
			(*pX)[ i ] -= matrix( row[ i ], j ) * (*pX)[ j ];
	}

	(*pX)[ 0 ] = vector[ row[ 0 ] ];
	for( j = 1; j < pA->n_row(); j++ )
	   	(*pX)[ 0 ] -= matrix( row[ 0 ], j ) * (*pX)[ j ];
}

//////////////////////////////////////////////////////////////////////

MATHBASE_API 
void _GaussCR( CDSRMMatrix<CDSRReal> *pA, CDSRMVector<CDSRReal> *pB, CDSRMVector<CDSRReal> *pX )
{
	if( pA->n_row() != pA->n_column() )
		throw "The matrix is not square";
	if( pA->n_row() != (long) pB->size() )
		throw "Internal problem - sizes of the source and destination matrix are not equal";

	long i, j, k;
	CDSRReal value;
	CDSRMMatrix<CDSRReal> matrix( ( *pA ) );
	CDSRMVector<CDSRReal> vector( ( *pB ) );
	CDSRMVector<long> row( pA->n_row() );
	CDSRMVector<long> col( pA->n_row() );

	for( i = 0; i < pA->n_row(); i++ )
		col[ i ] = row[ i ] = i;

	pX->resize( pB->size() );

	for( i = 0; i < pA->n_row(); i++ )
	{
		if( IsNull( matrix( row[ i ], col[ i ] ) ) )
			_IndexCR( i, &matrix, &row, &col );

		for( k = i; k < pA->n_row(); k++ )
		{
			value = matrix( row[ k ], col[ i ] );
			if( IsNotNull( value ) && IsNotOne( value ) )
			{
				vector[ row[ k ] ] /= value;
				for( j = i + 1; j < pA->n_row(); j++ )
					matrix(row[ k ], col[ j ] ) /= value;
				matrix( row[ k ], col[ i ] ) = 1.0;
			}
		}

		for( k = i + 1; k < pA->n_row(); k++ )
			if( IsNotNull( matrix( row[ k ], col[ i ] ) ) )
			{
				vector[ row[ k ] ] -= vector[ row[ i ] ];
				for( j = i + 1; j < pA->n_row(); j++ )
					matrix( row[ k ], col[ j ] ) -= matrix( row[ i ], col[ j ] );
				matrix( row[ k ], col[ i ] ) = 0.0;
			}
	}

	for( i = pA->n_row() - 1 ; i > 0; i-- )
	{
		(*pX)[ col[ i ] ] = vector[ row[ i ] ];
		for( j = i + 1; j < pA->n_row(); j++ )
			(*pX)[ col[ i ] ] -= matrix( row[ i ], col[ j ] ) * (*pX)[ col[ j ] ];
	}

	(*pX)[ col[ 0 ] ] = vector[ row[ 0 ] ];
	for( j = 1; j < pA->n_row(); j++ )
		(*pX)[ col[ 0 ] ] -= matrix( row[ 0 ], col[ j ] ) * (*pX)[ col[ j ] ];
}

//////////////////////////////////////////////////////////////////////
