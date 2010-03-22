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

#if !defined(__ALMA_EIGENVAL_H__)
#define __ALMA_EIGENVAL_H__

#include "../hmathbs_global.h"
#include "../cdsrnumbers/number.h"
#include "../cdsrnumbers/rootinfo.h"
#include "../cdsrnumbers/mathconst.h"
#include "../cdsrarrays/cdsrmmatrix.h"

//////////////////////////////////////////////////////////////////////

MATHBASE_API
void _eigenValLevere( CDSRMMatrix<CDSRReal>& A, CDSRArray<ROOT_INFO>& Lyambda, CDSRArray<CDSRReal>& polynom, CDSRReal rlPrec = EIGENVALUES_PRECISION);

MATHBASE_API
void _eigenValFadeev( CDSRMMatrix<CDSRReal>& A, CDSRArray<ROOT_INFO>& Lyambda, CDSRArray<CDSRReal>& polynom, CDSRReal rlPrec = EIGENVALUES_PRECISION);

MATHBASE_API
void _eigenValDanilev( CDSRMMatrix<CDSRReal>& A, CDSRArray<ROOT_INFO>& Lyambda, CDSRReal rlPrec = EIGENVALUES_PRECISION);

MATHBASE_API
void _eigenValKrilov( CDSRMMatrix<CDSRReal>& A, CDSRArray<ROOT_INFO>& Lyambda, CDSRArray<CDSRReal>& polynom, CDSRReal rlPrec = EIGENVALUES_PRECISION);

MATHBASE_API
void _eigenValJacobi( CDSRMMatrix<CDSRReal>& A, CDSRArray<CDSRReal>& Lyambda, CDSRReal rlPrec = EIGENVALUES_PRECISION);

//////////////////////////////////////////////////////////////////////

#endif //__ALMA_EIGENVAL_H__ corollary
