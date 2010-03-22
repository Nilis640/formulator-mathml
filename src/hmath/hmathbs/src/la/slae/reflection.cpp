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

#include "reflection.h"
#include "slae_internal.h"

//////////////////////////////////////////////////////////////////////

MATHBASE_API void _Reflection( CDSRMMatrix<CDSRReal> *pA, CDSRMVector<CDSRReal> *pB, CDSRMVector<CDSRReal> *pX )
{
	if( pA->n_row() != pA->n_column() )
		throw "The matrix is not square";
	if( pA->n_row() != (long) pB->size() )
		throw "Internal problem - sizes of the source and destination matrix are not equal";

	long i, j, r;
	CDSRReal sum, k;
	CDSRMMatrix<CDSRReal> matrix( (*pA) );
	CDSRMVector<CDSRReal> vector( (*pB) );
	CDSRMVector<CDSRReal> a3( pA->n_row() );

	pX->resize( pB->size() );

	for( r = 0; r < pA->n_row() - 1; r++ )
	{
		sum = 0.0;
		for( i  = r; i < pA->n_row(); i++ )
			sum += matrix( i, r ) * matrix( i, r );

		sum = matrix( r, r ) > 0.0 ? -sqrt( sum ) : sqrt( sum );

		k = 1 / ( sum * sum - sum * matrix( r, r ) );
		(*pX)[ r ] = matrix( r, r ) - sum;

		for( i = r + 1; i < pA->n_row(); i++ )
			(*pX)[ i ] = matrix( i, r );

		for( i = r; i < pA->n_row(); i++ )
		{
			sum = 0.0;
			for( j = r; j < pA->n_row(); j++ )
				sum += matrix( j, i ) * (*pX)[ j ];
			a3[ i ] = k * sum;
		}

		for( i = r; i < pA->n_row(); i++ )
			for( j = r; j < pA->n_row(); j++ )
				matrix( i, j ) -= (*pX)[ i ] * a3[ j ];

		sum = 0.0;
		for( i = r; i < pA->n_row(); i++ )
			sum = sum + vector[ i ] * (*pX)[ i ];
		for( i = r; i < pA->n_row(); i++ )
			vector[ i ] -= (*pX)[ i ] * sum * k;
	}

	for( i = 0; i < pA->n_row(); i++ )
		if( IsNull(matrix( i, i ) ) )
			throw "SLAE cannot be solved by this method or matrix is singular";

	for( i = pA->n_row() - 1 ; i > 0; i-- )
	{
		(*pX)[ i ] = vector[ i ];
		for( j = i + 1; j < pA->n_row(); j++ )
			(*pX)[ i ] -= matrix( i, j ) * (*pX)[ j ];
		(*pX)[ i ] /= matrix(i, i);
	}

	(*pX)[ 0 ] = vector[ 0 ];
	for( j = 1; j < pA->n_row(); j++ )
		(*pX)[ 0 ] -= matrix( 0 , j ) * (*pX)[ j ];
	(*pX)[ 0 ] /= matrix( 0, 0 );
}

//////////////////////////////////////////////////////////////////////
