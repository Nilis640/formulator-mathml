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

#include "slae_internal.h"

//////////////////////////////////////////////////////////////////////

void _IndexCR( long i, CDSRMMatrix<CDSRReal> *pMatrix, CDSRMVector<long> *pRow, CDSRMVector<long> *pCol )
{
	long j, k;
	long J = i;
	long K = i;
	CDSRReal max = fabs( (*pMatrix)( (*pRow)[ i ], (*pCol)[ i ] ) );

	for( j = i; j < pMatrix->n_row(); j++ )
		for( k = i; k < pMatrix->n_row(); k++ )
			if( fabs( (*pMatrix)( (*pRow)[ j ], (*pCol)[ k ] )) > max )
			{
				max = fabs( (*pMatrix)( (*pRow)[ j ], (*pCol)[ k ] ) );
				J = j;
				K = k;
			}

	if( IsNotNull( max ) )
	{
		j = (*pRow)[ i ];
		k = (*pCol)[ i ];
		(*pRow)[ i ] = (*pRow)[ J ];
		(*pCol)[ i ] = (*pCol)[ K ];
		(*pRow)[ J ] = j;
		(*pCol)[ K ] = k;
	}
	else
		throw "Matrix is singular";
}

//////////////////////////////////////////////////////////////////////

void _IndexR( long i, CDSRMMatrix<CDSRReal> *pMatrix, CDSRMVector<long> *pRow )
{
	long j;
	long J = i;
	CDSRReal max = fabs( (*pMatrix)( (*pRow)[ i ], i ) );

	for( j = i + 1; j < pMatrix->n_row(); j++ )
		if( fabs( (*pMatrix)( (*pRow)[ j ], i ) ) > max )
		{
			max = fabs( (*pMatrix)( (*pRow)[ j ], i ) );
			J = j;
		}

	if( IsNotNull( max ) )
	{
		j = (*pRow)[ i ];
		(*pRow)[ i ] = (*pRow)[ J ];
		(*pRow)[ J ] = j;
	}
	else
		throw "Matrix is singular";
}

//////////////////////////////////////////////////////////////////////

void _IndexC( long i, CDSRMMatrix<CDSRReal> *pMatrix, CDSRMVector<long> *pCol )
{
	long j;
	long J = i;
	CDSRReal max = fabs( (*pMatrix)(i, (*pCol)[ i ]) );

	for( j = i + 1; j < pMatrix->n_row(); j++ )
		if( fabs( (*pMatrix)( i, (*pCol)[ j ] ) ) > max )
		{
			max = fabs( (*pMatrix)( i, (*pCol)[ j ] ) );
			J = j;
		}

	if( IsNotNull( max ) )
	{
		j = (*pCol)[ i ];
		(*pCol)[ i ] = (*pCol)[ J ];
		(*pCol)[ J ] = j;
	}
	else
		throw "Matrix is singular";
}

//////////////////////////////////////////////////////////////////////
