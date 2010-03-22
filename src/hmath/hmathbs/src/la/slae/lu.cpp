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

#include "lu.h"
#include "slae_internal.h"

//////////////////////////////////////////////////////////////////////

MATHBASE_API 
void _LU( CDSRMMatrix<CDSRReal> *pA )
{
	if( pA->n_row() != pA->n_column() )
		throw "The matrix is not square";

	long i, r, k;
	CDSRReal sum;
	
	for( r = 0; r < pA->n_row(); r++ )
	{
		for( i = r; i < pA->n_row(); i++ )
		{
			sum = 0.0;
			for( k = 0; k < r; k++ )
				sum += (*pA)( i, k ) * (*pA)( k, r );
			(*pA)( i, r ) -= sum;
		}

		if( IsNull( (*pA)( r, r ) ) )
			throw "SLAE cannot be solved by LU method or matrix is singular";

		for( i = r + 1; i < pA->n_row(); i++ )
		{
			sum = 0.0;
			for( k = 0; k < r; k++ )
				sum += (*pA)( r, k ) * (*pA)( k, i );
			(*pA)( r, i ) -= sum;
			(*pA)( r, i ) /= (*pA)( r, r );
		}
	}
};

//////////////////////////////////////////////////////////////////////

MATHBASE_API 
void _LU_Result( CDSRMMatrix<CDSRReal> *pA, CDSRMVector<CDSRReal> *pB, CDSRMVector<CDSRReal> *pX )
{
	if( pA->n_row() != pA->n_column() )
		throw "The matrix is not square";
	if( pA->n_row() != (long) pB->size() )
		throw "Internal problem - sizes of the source and destination matrix are not equal";

	long i, r;
	pX->resize( pB->size() );

	// go down ...
	for( i = 0; i < pA->n_row(); i++ )
	{
		(*pX)[ i ] = (*pB)[ i ];
		for( r = 0; r < i; r++ )
			(*pX)[ i ] -= (*pA)( i, r ) * (*pX)[ r ];
		(*pX)[ i ] /= (*pA)( i, i );
	}

	for( i = pA->n_row() - 1; i > 0; i-- )
		for( r = i + 1; r < pA->n_row(); r++ )
			(*pX)[ i ] -= (*pA)( i, r ) * (*pX)[ r ];

	for( r = 1; r < pA->n_row(); r++ )
		(*pX)[ 0 ] -= (*pA)( 0, r ) * (*pX)[ r ];
};

//////////////////////////////////////////////////////////////////////

MATHBASE_API 
void _LU( CDSRMMatrix<CDSRReal> *pA, CDSRMVector<CDSRReal> *pB, CDSRMVector<CDSRReal> *pX )
{
	if( pA->n_row() != pA->n_column() )
		throw "The matrix is not square";
	if( pA->n_row() != (long) pB->size() )
		throw "Internal problem - sizes of the source and destination matrix are not equal";

	CDSRMMatrix<CDSRReal> matrix( (*pA) );
	_LU( &matrix );
	_LU_Result( &matrix, pB, pX );
};

/*

//////////////////////////////////////////////////////////////////////

MATHBASE_API 
void _LU( CDSRBlkDMatrix<CDSRReal> *pA )
{
	long i, r, k, l;
	CDSRReal sum;
	
	for( l = 0; l < pA->n_cntBlk(); l++ )
	{
		for( r = 0; r < pA->n_szBlk(); r++ )
		{
			for( i = r; i < pA->n_szBlk(); i++ )
			{
				sum = 0.0;
				for( k = 0; k < r; k++ )
					sum += (*pA)( l, i, k ) * (*pA)( l, k, r );
				(*pA)( l, i, r ) -= sum;
			}

			if( IsNull( (*pA)( l, r, r ) ) )
				throw "SLAE cannot be solved by LU method or matrix is singular";

			for( i = r + 1; i < pA->n_szBlk(); i++ )
			{
				sum = 0.0;
				for( k = 0; k < r; k++ )
					sum += (*pA)( l, r, k ) * (*pA)( l, k, i );
				(*pA)( l, r, i ) -= sum;
				(*pA)( l, r, i ) /= (*pA)( l, r, r );
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////

MATHBASE_API 
void _LU_Result( CDSRBlkDMatrix<CDSRReal> *pA, CDSRMVector<CDSRReal> *pB, CDSRMVector<CDSRReal> *pX )
{
	if( pA->n_szRowCol() != (long) pB->size() )
		throw "Internal problem - sizes of the source and destination matrix are not equal";

	long l, i, r, k;

	for( l = 0; l < pA->n_cntBlk(); l++ )
	{
		k = l * pA->n_szBlk();
		for( i = 0; i < pA->n_szBlk(); i++ )
		{
			(*pX)[ k + i ] = (*pB)[ k + i ];
			for( r = 0; r < i; r++ )
				(*pX)[ k + i ] -= (*pA)( l, i, r ) * (*pX)[ k + r ];
			(*pX)[ k + i ] /= (*pA)( l, i, i );
		}

		for( i = pA->n_szBlk() - 1; i > 0; i-- )
			for( r = i + 1; r < pA->n_szBlk(); r++ )
				(*pX)[ k + i ] -= (*pA)( l, i, r ) * (*pX)[ k + r ];

		for( r = 1; r < pA->n_szBlk(); r++ )
			(*pX)[ k ] -= (*pA)( l, 0, r ) * (*pX)[ k + r ];
	}
}

//////////////////////////////////////////////////////////////////////

MATHBASE_API 
void _LU( CDSRBlkDMatrix<CDSRReal> *pA, CDSRMVector<CDSRReal> *pB, CDSRMVector<CDSRReal> *pX )
{
	if( pA->n_szRowCol() != (long) pB->size() )
		throw "Internal problem - sizes of the source and destination matrix are not equal";

	CDSRBlkDMatrix<CDSRReal> matrix( (*pA) );

	_LU( &matrix );
	_LU_Result( &matrix, pB, pX );
}

//////////////////////////////////////////////////////////////////////

MATHBASE_API 
void _LU( CDSRSplineMatrix<CDSRReal> *pA )
{
	long i, r, k, l;
	CDSRReal sum;
	
	for( l = 0; l < pA->n_cntBlk(); l++ )
	{
		for( r = 0; r < pA->n_szBlk(); r++ )
		{
			for( i = r; i < pA->n_szBlk(); i++ )
			{
				sum = 0.0;
				for( k = 0; k < r; k++ )
					sum += (*pA)( l, i, k ) * (*pA)( l, k, r );
				(*pA)( l, i, r ) -= sum;
			}

			if( IsNull( (*pA)( l, r, r ) ) )
				throw "SLAE cannot be solved by LU method or matrix is singular";

			for( i = r + 1; i < pA->n_szBlk(); i++ )
			{
				sum = 0.0;
				for( k = 0; k < r; k++ )
					sum += (*pA)( l, r, k ) * (*pA)( l, k, i );
				(*pA)( l, r, i ) -= sum;
				(*pA)( l, r, i ) /= (*pA)( l, r, r );
			}
		}
	}

	for( l = 0; l < pA->n_cntDig(); l++ )
	{
		for( r = pA->n_szBlk() - pA->n_szDig(); r < pA->n_szBlk(); r++ )
		{
			for( i = pA->n_szBlk(); i < (pA->n_szBlk() << 1); i++ )
			{
				sum = 0.0;
				for( k = pA->n_szBlk() - pA->n_szDig(); k < r; k++ )
					sum += (*pA)( l, r, k ) * (*pA)( l, k, i );
				(*pA)( l, r, i ) -= sum;
				(*pA)( l, r, i ) /= (*pA)( l, r, r );
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////

MATHBASE_API 
void _LU_Result( CDSRSplineMatrix<CDSRReal> *pA, CDSRMVector<CDSRReal> *pB, CDSRMVector<CDSRReal> *pX )
{
	if( pA->n_szRowCol() != (long) pB->size() )
		throw "Internal problem - sizes of the source and destination matrix are not equal";

	long l, i, r, k, pos;

	for( l = 0; l < pA->n_cntBlk(); l++ )
	{
		k = l * pA->n_szBlk();
		for( i = 0; i < pA->n_szBlk(); i++ )
		{
			(*pX)[ k + i ] = (*pB)[ k + i ];
			for( r = 0; r < i; r++ )
				(*pX)[ k + i ] -= (*pA)( l, i, r ) * (*pX)[ k + r ];
			(*pX)[ k + i ] /= (*pA)( l, i, i );
		}
	}

	//
	l = pA->n_cntDig();
	k = l * pA->n_szBlk();
	for( i = pA->n_szBlk() - 1; i > 0; i-- )
		for( r = i + 1; r < pA->n_szBlk(); r++ )
			(*pX)[ k + i ] -= (*pA)( l, i, r ) * (*pX)[ k + r ];
	for( r = 1; r < pA->n_szBlk(); r++ )
		(*pX)[ k ] -= (*pA)( l, 0, r ) * (*pX)[ k + r ];
	for( pos = 1; pos < pA->n_cntBlk(); pos++ )
	{
		l = pA->n_cntDig() - pos;
		k = l * pA->n_szBlk();
		for( i = pA->n_szBlk() - 1; i > pA->n_szBlk() - pA->n_szDig() - 1; i-- )
			for( r = i + 1; r < (pA->n_szBlk() << 1); r++ )
				(*pX)[ k + i ] -= (*pA)( l, i, r ) * (*pX)[ k + r ];
		for( i = pA->n_szBlk() - pA->n_szDig() - 1; i > 0; i-- )
			for( r = i + 1; r < pA->n_szBlk(); r++ )
				(*pX)[ k + i ] -= (*pA)( l, i, r ) * (*pX)[ k + r ];
		for( r = 1; r < pA->n_szBlk(); r++ )
			(*pX)[ k ] -= (*pA)( l, 0, r ) * (*pX)[ k + r ];
	}
}

//////////////////////////////////////////////////////////////////////

MATHBASE_API 
void _LU( CDSRSplineMatrix<CDSRReal> *pA, CDSRMVector<CDSRReal> *pB, CDSRMVector<CDSRReal> *pX )
{
	if( pA->n_szRowCol() != (long) pB->size() )
		throw "Internal problem - sizes of the source and destination matrix are not equal";

	CDSRSplineMatrix<CDSRReal> matrix( (*pA) );

	_LU( &matrix );
	_LU_Result( &matrix, pB, pX );
}

//////////////////////////////////////////////////////////////////////

MATHBASE_API 
void _LU_Result( CDSRSplineMatrix<CDSRReal> *pA, CDSRMMatrix<CDSRReal> *pB, CDSRMMatrix<CDSRReal> *pX )
{
	if( pA->n_cntBlk() != pB->n_row() || pA->n_szBlk() != pB->n_column() || 
		pA->n_cntBlk() != pX->n_row() || pA->n_szBlk() != pX->n_column() )
		throw "Internal problem - sizes of the source and destination matrix are not equal";

	long l, i, r, pos;

	// go down ...
	for( l = 0; l < pA->n_cntBlk(); l++ )
	{
		for( i = 0; i < pA->n_szBlk(); i++ )
		{
			(*pX)( l, i ) = (*pB)( l, i );
			for( r = 0; r < i; r++ )
				(*pX)( l, i ) -= (*pA)( l, i, r ) * (*pX)( l, r );
			(*pX)( l, i ) /= (*pA)( l, i, i );
		}
	}

	//
	l = pA->n_cntDig();
	for( i = pA->n_szBlk() - 1; i > 0; i-- )
		for( r = i + 1; r < pA->n_szBlk(); r++ )
			(*pX)( l, i ) -= (*pA)( l, i, r ) * (*pX)( l, r );
	for( r = 1; r < pA->n_szBlk(); r++ )
		(*pX)( l, 0 ) -= (*pA)( l, 0, r ) * (*pX)( l, r );
	for( pos = 1; pos < pA->n_cntBlk(); pos++ )
	{
		l = pA->n_cntDig() - pos;
		for( i = pA->n_szBlk() - 1; i > pA->n_szBlk() - pA->n_szDig() - 1; i-- )
			for( r = i + 1; r < (pA->n_szBlk() << 1); r++ )
				(*pX)( l, i ) -= (*pA)( l, i, r ) * (*pX)( l, i );
		for( i = pA->n_szBlk() - pA->n_szDig() - 1; i > 0; i-- )
			for( r = i + 1; r < pA->n_szBlk(); r++ )
				(*pX)( l, i ) -= (*pA)( l, i, r ) * (*pX)( l, r );
		for( r = 1; r < pA->n_szBlk(); r++ )
			(*pX)( l, 0 ) -= (*pA)( l, 0, r ) * (*pX)( l, r );
	}
}

//////////////////////////////////////////////////////////////////////

MATHBASE_API 
void _LU( CDSRSplineMatrix<CDSRReal> *pA, CDSRMMatrix<CDSRReal> *pB, CDSRMMatrix<CDSRReal> *pX )
{
	if( pA->n_cntBlk() != pB->n_row() || pA->n_szBlk() != pB->n_column() || 
		pA->n_cntBlk() != pX->n_row() || pA->n_szBlk() != pX->n_column() )
		throw "Internal problem - sizes of the source and destination matrix are not equal";

	CDSRSplineMatrix<CDSRReal> matrix( (*pA) );

	_LU( &matrix );
	_LU_Result( &matrix, pB, pX );
}

//////////////////////////////////////////////////////////////////////

*/
