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

#if !defined( __HMATH_CMMCALC_CMMCALCTAG__ )
#define __HMATH_CMMCALC_CMMCALCTAG__

///////////////////////////////////////////////////////////////////////////////

#include "HMathML/immldefs.h"
#include "../operator/operator.h"

///////////////////////////////////////////////////////////////////////////////

#define CMMLCALC_KW_FUNCTION	_T("function")

///////////////////////////////////////////////////////////////////////////////

enum aMapMMLTag2DsrOpValue
{
	_PROCESS_FUNCTION_INVERSE = FUNCTION_STD_LAST + 1,

	_NOTYET_SYMBOLIC,
	_NOTYET_STAT,

	_TODO_FUNCTION_QUOTIENT,
	_TODO_FUNCTION_FACTORIAL,
	_TODO_FUNCTION_MAX,
	_TODO_FUNCTION_MIN,
	_TODO_FUNCTION_XOR,

	_TODO_FUNCTION_set_not_in_eq,
	_TODO_FUNCTION_setdiff,
	_TODO_FUNCTION_set_card,
	_TODO_FUNCTION_set_cartesianproduct,

	_TODO_FUNCTION_arccot,
	_TODO_FUNCTION_arccoth,
	_TODO_FUNCTION_arcsech,
	_TODO_FUNCTION_arccosech,

	_TODO_FUNCTION_COMPLEX_CONJUGATE,
	_TODO_FUNCTION_COMPLEX_ARG,

	_TODO_FUNCTION_LA_vectorproduct,
	_TODO_FUNCTION_LA_scalarproduct,
	_TODO_FUNCTION_LA_outerproduct,

	_TODO_FUNCTION_SUM,
	_TODO_FUNCTION_PRODUCT,

	_TODO_FUNCTION_INTEGRAL,
	_TODO_FUNCTION_DIFF,
	_TODO_FUNCTION_PARTDIFF,
	_TODO_FUNCTION_DIVERGENCE,
	_TODO_FUNCTION_GRAD,
	_TODO_FUNCTION_CURL,
	_TODO_FUNCTION_LAPLASIAN,

	_CONSTRUCT_FUNCTION_CN,
	_CONSTRUCT_FUNCTION_CI,
	_CONSTRUCT_FUNCTION_CSYMBOL,
	_CONSTRUCT_FUNCTION_APPLY,
	_CONSTRUCT_FUNCTION_LAMBDA,
	_CONSTRUCT_FUNCTION_COMPOSE,

	aMapMMLTag2DsrOpValue_LAST
};

///////////////////////////////////////////////////////////////////////////////

__HMATHLNGDLL__
extern int aMapMMLTag2DsrOp[ TAG2MML_LAST ];

///////////////////////////////////////////////////////////////////////////////

inline
int mapMMLTag2DsrOp( enum TAG2MML tagID )
{
	if( tagID < 0 || tagID >= TAG2MML_LAST ) return FUNCTION_STD_NONE;
	return aMapMMLTag2DsrOp[ tagID ];
}

inline
int mapMMLTag2DsrOp( QString tagName )
{
	return ::mapMMLTag2DsrOp( ::mml_getTag2MML( tagName ) );
}

///////////////////////////////////////////////////////////////////////////////

__HMATHLNGDLL__ enum CDsrDataTypes mapMMLValueTypeAttr2Dsr( QString typeName );
__HMATHLNGDLL__ enum CDsrDataTypes mapMMLValueType2Dsr( long typeidx );

#endif
