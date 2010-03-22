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

#include "solde.h"
#include "odeonestepparam.h"
#include "solde_internal.h"

/////////////////////////////////////////////////////////////////////////////

__HMATHLNGDLL__
int _solde( CDSR_VMEval *pOdeRTI, 
	CDSRArray<CDSRReal> *pInitials, 
	CDSRArray<CDSRReal> *pArg, 
	CDSRMatrix<CDSRReal > *pFunc, 
	CDSRReal rlPrec, unsigned long nMaxCount )
{
	unsigned long i, j, nCount;

	if ( pOdeRTI == 0 || pInitials == 0 || pArg == 0 || pFunc == 0 )
		throw "Internal error";
	pFunc->resize_matrix( pArg->size(), pInitials->size() );

	CDSRReal value;
	CDSRReal step;
	CDSRReal rlError;
	COdeOneStepParam param( -1.0, 2.0 );

	CDSRArray<CDSRReal> func( *pInitials );
	CDSRArray<CDSRReal> funcD( pInitials->size(), 0.0 );

	CDSRArray<CDSRReal> funcBack( pInitials->size(), 0.0 );
	CDSRArray<CDSRReal> funcBackD( pInitials->size(), 0.0 );

	//
	RunOneStep( pOdeRTI, (*pArg)[ 0 ], &func, &funcD );

	for ( j = 0; j < pInitials->size(); j++ )
		(*pFunc)( 0, j ) = func[ j ];

	//
	for ( i = 1; i < pArg->size(); i++ )
	{
		nCount = 0;
		step = (*pArg)[ i ] - (*pArg)[ i - 1 ];
		value = (*pArg)[ i - 1 ];

		while ( ((step > 0.0) ? value < (*pArg)[ i ] : value > (*pArg)[ i ]) )
		{
			funcBack = func;
			funcBackD = funcD;
			param.setStep( step );

			while ( !SoldeOneStep( pOdeRTI, value, &func, &funcD, param, rlError, rlPrec ) )
			{
				func = funcBack;
				funcD = funcBackD;
				step *= 0.5;
				param.setStep( step );
			}
			value += step;

			if ( rlError < rlPrec * ALMA_ODE_PREC_MUL )
				step *= 2.0;

			nCount++;
			if ( nCount > nMaxCount )
			{
				for ( j = 0; j < pInitials->size(); j++ )
					(*pFunc)( i, j ) = func[ j ];
				return -int(i);
			}
		}

		for ( j = 0; j < pInitials->size(); j++ )
			(*pFunc)( i, j ) = func[ j ];
	}
	return 0;
}

/////////////////////////////////////////////////////////////////////////////

__HMATHLNGDLL__
void _solde( CDSR_VMEval *pOdeRTI, 
	CDSRArray<CDSRReal> *pInitials, 
	CDSRArray<CDSRReal> *pArg, 
	CDSRArray<CDSRArray<CDSRReal>*> *pFunc, 
	CDSRArray<CDSRArray<CDSRReal>*> *pFuncD, 
	CDSRArray<CDSRArray<CDSRReal>*> *pFuncM, 
	CDSRReal rlPrec, 
	unsigned long nMaxCount )
{
	unsigned long i, j, nCount;

	pFunc->resize( pInitials->size() );
	if ( pFuncD )
		pFuncD->resize( pInitials->size() );
	if ( pFuncM )
		pFuncM->resize( pInitials->size() );

	for ( j = 0; j < pInitials->size(); j++ )
		(*pFunc)[ j ] = new CDSRArray<CDSRReal>( pArg->size() );
	if ( pFuncD )
		for ( j = 0; j < pInitials->size(); j++ )
			(*pFuncD)[ j ] = new CDSRArray<CDSRReal>( pArg->size() );
	if ( pFuncM )
		for ( j = 0; j < pInitials->size(); j++ )
			(*pFuncM)[ j ] = new CDSRArray<CDSRReal>( pArg->size() );

	CDSRReal rlValue;
	CDSRReal rlStep;
	CDSRReal rlError;
	COdeOneStepParam param( -1.0, 2.0 );

	CDSRArray<CDSRReal> func( *pInitials );
	CDSRArray<CDSRReal> funcD( pInitials->size(), 0.0 );
	CDSRArray<CDSRReal> funcM( pInitials->size(), 0.0 );

	CDSRArray<CDSRReal> funcBack( pInitials->size(), 0.0 );
	CDSRArray<CDSRReal> funcBackD( pInitials->size(), 0.0 );

	//
	RunOneStep( pOdeRTI, (*pArg)[ 0 ], &func, &funcD );

	for ( j = 0; j < pInitials->size(); j++ )
		(*((*pFunc)[ j ]))[ 0 ] = func[ j ];
	if ( pFuncD )
		for ( j = 0; j < pInitials->size(); j++ )
			(*((*pFuncD)[ j ]))[ 0 ] = funcD[ j ];
	if ( pFuncM )
		for ( j = 0; j < pInitials->size(); j++ )
			(*((*pFuncM)[ j ]))[ 0 ] = 0.0;

	//
	for ( i = 1; i < pArg->size(); i++ )
	{
		nCount = 0;
		rlStep = (*pArg)[ i ] - (*pArg)[ i - 1 ];
		rlValue = (*pArg)[ i - 1 ];

		while ( ((rlStep > 0.0) ? rlValue < (*pArg)[ i ] : rlValue > (*pArg)[ i ]) )
		{
			funcBack = func;
			funcBackD = funcD;
			param.setStep( rlStep );

			while ( !SoldeOneStep( pOdeRTI, rlValue, &func, &funcD, param, rlError, rlPrec ) )
			{
				func = funcBack;
				funcD = funcBackD;
				rlStep *= 0.5;
				param.setStep( rlStep );
			}
			rlValue += rlStep;

			if ( rlError < rlPrec * ALMA_ODE_PREC_MUL )
				rlStep *= 2.0;

			nCount++;
			if ( nCount > nMaxCount )
				throw "Iteration counter limit exceeded";
		}

		for ( j = 0; j < pInitials->size(); j++ )
			(*((*pFunc)[ j ]))[ i ] = func[ j ];
		if ( pFuncD )
			for ( j = 0; j < pInitials->size(); j++ )
				(*((*pFuncD)[ j ]))[ i ] = funcD[ j ];
		if ( pFuncM )
			for ( j = 0; j < pInitials->size(); j++ )
				(*((*pFuncM)[ j ]))[ i ] = funcM[ j ];
	}
}

/////////////////////////////////////////////////////////////////////////////
