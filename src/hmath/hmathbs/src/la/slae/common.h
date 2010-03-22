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

#ifndef __ALMASLAE_COMMONSLAE_H__
#define __ALMASLAE_COMMONSLAE_H__

#include "HMathBS/iarray.h"
#include "HMathBS/inumber.h"
#include "../../hmathbs_global.h"

//////////////////////////////////////////////////////////////////////

MATHBASE_API void _Inverse( CDSRMMatrix<CDSRComplex> *pA, CDSRMMatrix<CDSRComplex> *pInv );
MATHBASE_API void _DeterminantCR( CDSRMMatrix<CDSRInteger> *pA, CDSRInteger& rlDet );
MATHBASE_API void _DeterminantCR( CDSRMMatrix<CDSRComplex> *pA, CDSRComplex& rlDet );

MATHBASE_API void _Inverse( CDSRMMatrix<CDSRReal> *pA, CDSRMMatrix<CDSRReal> *pInv );
MATHBASE_API void _InverseC( CDSRMMatrix<CDSRReal> *pA, CDSRMMatrix<CDSRReal> *pInv );
MATHBASE_API void _InverseR( CDSRMMatrix<CDSRReal> *pA, CDSRMMatrix<CDSRReal> *pInv );
MATHBASE_API void _InverseCR( CDSRMMatrix<CDSRReal> *pA, CDSRMMatrix<CDSRReal> *pInv );

MATHBASE_API void _Determinant( CDSRMMatrix<CDSRReal> *pA, CDSRReal& rlDet );
MATHBASE_API void _DeterminantC( CDSRMMatrix<CDSRReal> *pA, CDSRReal& rlDet );
MATHBASE_API void _DeterminantR( CDSRMMatrix<CDSRReal> *pA, CDSRReal& rlDet );
MATHBASE_API void _DeterminantCR( CDSRMMatrix<CDSRReal> *pA, CDSRReal& rlDet );

MATHBASE_API void _LSolve(CDSRMMatrix<CDSRReal> *pA, CDSRMVector<CDSRReal> *pB, CDSRMVector<CDSRReal> *pX, std::string _method = "" );

//////////////////////////////////////////////////////////////////////

#endif // __ALMASLAE_COMMONSLAE_H__
