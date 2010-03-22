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

#include "rotation.h"
#include "slae_internal.h"

//////////////////////////////////////////////////////////////////////

MATHBASE_API 
void _Rotation( CDSRMMatrix<CDSRReal> *pA, CDSRMVector<CDSRReal> *pB, CDSRMVector<CDSRReal> *pX )
{
	if( pA->n_row() != pA->n_column() )
		throw "The matrix is not square";
	if( pA->n_row() != (long) pB->size() )
		throw "Internal problem - sizes of the source and destination matrix are not equal";

	long i, j, k, lm;
	CDSRReal M = 0.0, L = 0.0, value_1, value_2;
	CDSRMMatrix<CDSRReal> matrix( (*pA) );
	CDSRMVector<CDSRReal> vector( (*pB) );
	
	pX->resize( pB->size() );

	//
	for( i = 0; i < pA->n_row() - 1; i++ )
	{
		for( k = i + 1; k < pA->n_row(); k++ )
		{
			value_1 = matrix( i, i );
			value_2 = matrix( k, i );

			lm = (IsNull( value_1 ) ? 2 : 0) + (IsNull( value_2 ) ? 1 : 0);
			switch( lm )
			{
			case 3:
				M = 1.0;
				L = 0.0;
				break;

			case 2:
				M = 0.0;
				L =  - 1.0;
				break;

			case 1:
				M = 1.0;
				L = 0.0;
				break;

			case 0:
				M = sqrt( value_1 * value_1 + value_2 * value_2 );
				L = -value_2 / M;
				M = value_1 / M;
				break;
			}

			for( j = i; j < pA->n_row(); j++ )
			{
				value_1 = matrix( i, j );
				value_2 = matrix( k, j );
        			matrix( i, j ) = M * value_1 - L * value_2;
				matrix( k, j ) = L * value_1 + M * value_2;
			}

			value_1 = vector[ i ];
			value_2 = vector[ k ];
			vector[ i ] = M * value_1 - L * value_2;
			vector[ k ] = L * value_1 + M * value_2;
		}
	}

	for( i = 0; i < pA->n_row(); i++ )
		if( IsNull( matrix( i, i ) ) )
			throw "SLAE cannot be solved by this method or matrix is singular";

	for( i = pA->n_row() - 1 ; i > 0; i-- )
	{
		(*pX)[ i ] = vector[ i ];
		for( j = i + 1; j < pA->n_row(); j++ )
			(*pX)[ i ] -= matrix( i, j ) * (*pX)[ j ];
		(*pX)[ i ] /= matrix( i, i );
	}

	(*pX)[ 0 ] = vector[ 0 ];
	for( j = 1; j < pA->n_row(); j++ )
	   	(*pX)[ 0 ] -= matrix( 0, j ) * (*pX)[ j ];
	(*pX)[ 0 ] /= matrix( 0, 0 );
}

//////////////////////////////////////////////////////////////////////
