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

#ifndef __ODE_SOLDE_H__
#define __ODE_SOLDE_H__

#include "../../hmathast_global.h"
#include "HMathBS/inumber.h"
#include "HMathBS/imathconst.h"
#include "HMathBS/iarray.h"
#include "../vmeval.h"

__HMATHLNGDLL__
int _solde( CDSR_VMEval *pOdeRTI, 
	CDSRArray<CDSRReal> *pInitials, 
	CDSRArray<CDSRReal> *pArg, 
	CDSRMatrix<CDSRReal > *pFunc, 
	CDSRReal rlPrec = ALMA_ODE_PREC, 
	unsigned long nMaxIteration = 2048 );

__HMATHLNGDLL__
void _solde( CDSR_VMEval *pOdeRTI, 
	CDSRArray<CDSRReal>* pInitials, 
	CDSRArray<CDSRReal> *pArg, 
	CDSRArray<CDSRArray<CDSRReal>*> *pX, 
	CDSRArray<CDSRArray<CDSRReal>*> *pDerX = NULL, 
	CDSRArray<CDSRArray<CDSRReal>*> *pMis = NULL, 
	CDSRReal rlPrec = ALMA_ODE_PREC, 
	unsigned long nMaxIteration = 2048 );

/////////////////////////////////////////////////////////////////////////////
#endif //__ODE_SOLDE_H__
