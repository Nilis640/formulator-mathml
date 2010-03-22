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

#if !defined(__ALMA_RATIONAL_FUNCTIONS_H__)
#define __ALMA_RATIONAL_FUNCTIONS_H__

#include "../hmathbs_global.h"
#include "../cdsrnumbers/number.h"

//////////////////////////////////////////////////////////////////////


MATHBASE_API 
void _calcFraction(CDSRArray<CDSRReal>* pNumerator, CDSRArray<CDSRReal>* pDenominator, CDSRReal rlArg, CDSRReal& rlResult);
MATHBASE_API 
void _calcFraction(CDSRArray<CDSRReal>* pNumerator, CDSRArray<CDSRReal>* pDenominator, CDSRComplex rlArg, CDSRComplex& rlResult);
MATHBASE_API 
void _calcFraction(CDSRArray<CDSRComplex>* pNumerator, CDSRArray<CDSRComplex>* pDenominator, CDSRComplex rlArg, CDSRComplex& rlResult);


MATHBASE_API
void _partFractDecompos(CDSRArray<CDSRReal>* pNumerator, CDSRArray<CDSRReal>* pDenominator, CDSRArray<CDSRArray<CDSRReal>*>& partNums, CDSRArray<CDSRArray<CDSRReal>*>& partDenoms, CDSRArray<unsigned long>& MultiplicityArray);
MATHBASE_API
void _partFractDecomposComplex(CDSRArray<CDSRReal>* pNumerator, CDSRArray<CDSRReal>* pDenominator, CDSRArray<CDSRComplex>& numCoefs, CDSRArray<CDSRComplex>& denomRoots, CDSRArray<unsigned long>& MultiplicityArray);


MATHBASE_API
void _rationalFuncDeriv(CDSRArray<CDSRReal>* pNum, CDSRArray<CDSRReal>* pDen, CDSRArray<CDSRReal>* pNumRes, CDSRArray<CDSRReal>* pDenRes);
MATHBASE_API
void _fractDeriativeComplex(CDSRArray<CDSRComplex>* pNumerator, CDSRArray<CDSRComplex>* pDenominator, CDSRArray<CDSRComplex>* pResNum, CDSRArray<CDSRComplex>* pResDenom);
MATHBASE_API
void _fractDeriativeComplex(CDSRArray<CDSRComplex>* pNumerator, CDSRArray<CDSRComplex>* pDenominator, long nDerivative, CDSRComplex arg, CDSRComplex& res);


MATHBASE_API
void _rationalFuncsSum(CDSRArray<CDSRReal>* pNum1, CDSRArray<CDSRReal>* pDen1, CDSRArray<CDSRReal>* pNum2, CDSRArray<CDSRReal>* pDen2, CDSRArray<CDSRReal>* pNumRes, CDSRArray<CDSRReal>* pDenRes);
MATHBASE_API
void _rationalFuncsSum(CDSRArray<CDSRComplex>* pNum1, CDSRArray<CDSRComplex>* pDen1, CDSRArray<CDSRComplex>* pNum2, CDSRArray<CDSRComplex>* pDen2, CDSRArray<CDSRComplex>* pNumRes, CDSRArray<CDSRComplex>* pDenRes);


MATHBASE_API
void _rationalFuncsMult(CDSRArray<CDSRReal>* pNum1, CDSRArray<CDSRReal>* pDen1, CDSRArray<CDSRReal>* pNum2, CDSRArray<CDSRReal>* pDen2, CDSRArray<CDSRReal>* pNumRes, CDSRArray<CDSRReal>* pDenRes);

MATHBASE_API
void _rationalFuncReduce(CDSRArray<CDSRReal>* pNum, CDSRArray<CDSRReal>* pDen, CDSRArray<CDSRReal>* pNumRes, CDSRArray<CDSRReal>* pDenRes);

//////////////////////////////////////////////////////////////////////
#endif //__ALMA_RATIONAL_FUNCTIONS_H__
