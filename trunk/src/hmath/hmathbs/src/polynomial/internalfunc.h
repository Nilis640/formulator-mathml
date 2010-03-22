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

#if !defined(__ALMA_POLINOMS_INTERNAL_FUNC_H__)
#define __ALMA_POLINOMS_INTERNAL_FUNC_H__

#include "../cdsrnumbers/number.h"
#include "../cdsrnumbers/mathconst.h"
#include "../cdsrarrays/cdsrarray.h"
#include "../cdsrarrays/cdsrmatrix.h"

inline bool IsNoll(CDSRReal _arg)
{
	return (fabs(_arg) < MATH_REAL_EPSILON);
};

inline bool IsNoll(CDSRComplex _arg)
{
	return (abs(_arg) < MATH_REAL_EPSILON);
};

void GetHurwitzMatrix(CDSRArray<CDSRReal>& polynom, long order, CDSRMatrix<CDSRReal>& HurwitzMatrix);
void ComplexNumberNthRoot(CDSRComplex x, long power, CDSRArray<CDSRComplex>& roots);
double fact( long ii );
double C_nj( long n, long j);
void complexBinomNewton(CDSRArray<CDSRComplex>& nBinomial, long lPower, CDSRArray<CDSRComplex>& nResult);

class QString;
QString CreateComplexNumberString(CDSRComplex number);
QString CreatePolynomAnyPowerString(CDSRArray<CDSRComplex>& polynom);
#endif //__ALMA_POLINOMS_INTERNAL_FUNC_H__
