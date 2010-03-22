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

#include "../vmevallink.h"
#include "execop_call.h"

///////////////////////////////////////////////////////////////////////////////

CDsrOperationCall CDsrOperationCallArray[ FUNCTION_STD_LAST ] =
{









	__Call_Nop,								//"nop"
	__Call_SetAdd,
	__Call_SetMul,
	__Call_SetRelIn,
	__Call_SetRelNotIn,
	__Call_SetRelSetIn,
	__Call_SetRelSetInInv,
	__Call_SetRelSetInEq,
	__Call_SetRelSetInEqInv,
	__Call_SetRelSetNotIn,
	__Call_Nop, 							//"()",
	__Call_Square,
	__Call_SquareLValue,
	__Call_ConstructSet,
	__Call_Add,
	__Call_Sub,
	__Call_Mul,
	__Call_Div,
	__Call_Mod,
	__Call_Pow,
	__Call_Less,
	__Call_LessEq,
	__Call_Greater,
	__Call_GreaterEq,
	__Call_BoolAnd,
	__Call_BoolOr,
	__Call_BoolNot,
	__Call_NotEqual,
	__Call_Assign,
	__Call_Nop,								//"return a value"
	__Call_Nop, 							//"independent variable"
	__Call_Nop, 							//"equation ="
	__Call_Nop, 							//"equation <"
	__Call_Nop, 							//"equation <="
	__Call_Nop, 							//"equation >"
	__Call_Nop, 							//"equation >="
	__Call_Equal,
	__Call_TypeSpec,
	__Call_Convert_natural2integer,
	__Call_Convert_natural2bits,
	__Call_Convert_natural2rational,
	__Call_Convert_natural2real,
	__Call_Convert_natural2complex,
	__Call_Convert_integer2bits,
	__Call_Convert_integer2rational,
	__Call_Convert_integer2real,
	__Call_Convert_integer2complex,
	__Call_Convert_rational2real,
	__Call_Convert_rational2complex,
	__Call_Convert_real2complex,
	__Call_Convert_natural2Polynomial_integer,
	__Call_Convert_natural2Polynomial_real,
	__Call_Convert_natural2Polynomial_complex,
	__Call_Convert_natural2RationalFun_integer,
	__Call_Convert_natural2RationalFun_real,
	__Call_Convert_natural2RationalFun_complex,
	__Call_Convert_integer2Polynomial_integer,
	__Call_Convert_integer2Polynomial_real,
	__Call_Convert_integer2Polynomial_complex,
	__Call_Convert_integer2RationalFun_integer,
	__Call_Convert_integer2RationalFun_real,
	__Call_Convert_integer2RationalFun_complex,
	__Call_Convert_rational2Polynomial_real,
	__Call_Convert_rational2Polynomial_complex,
	__Call_Convert_rational2RationalFun_real,
	__Call_Convert_rational2RationalFun_complex,
	__Call_Convert_real2Polynomial_real,
	__Call_Convert_real2Polynomial_complex,
	__Call_Convert_real2RationalFun_real,
	__Call_Convert_real2RationalFun_complex,
	__Call_Convert_complex2Polynomial_complex,
	__Call_Convert_complex2RationalFun_complex,
	__Call_Convert_Polynomial_integer2Polynomial_real,
	__Call_Convert_Polynomial_integer2Polynomial_complex,
	__Call_Convert_Polynomial_real2Polynomial_complex,
	__Call_Convert_Polynomial_integer2RationalFun_integer,
	__Call_Convert_Polynomial_integer2RationalFun_real,
	__Call_Convert_Polynomial_integer2RationalFun_complex,
	__Call_Convert_Polynomial_real2RationalFun_real,
	__Call_Convert_Polynomial_real2RationalFun_complex,
	__Call_Convert_Polynomial_complex2RationalFun_complex,
	__Call_Convert_RationalFun_integer2RationalFun_real,
	__Call_Convert_RationalFun_integer2RationalFun_complex,
	__Call_Convert_RationalFun_real2RationalFun_complex,
	__Call_VectorConvert_natural2integer,
	__Call_VectorConvert_natural2rational,
	__Call_VectorConvert_natural2real,
	__Call_VectorConvert_natural2complex,
	__Call_VectorConvert_integer2rational,
	__Call_VectorConvert_integer2real,
	__Call_VectorConvert_integer2complex,
	__Call_VectorConvert_rational2real,
	__Call_VectorConvert_rational2complex,
	__Call_VectorConvert_real2complex,
	__Call_abs,
	__Call_arch,
	__Call_arccos,
	__Call_arccosec,
	__Call_arcsec,
	__Call_arcsin,
	__Call_arctg,
	__Call_arcth,
	__Call_arsh,
	__Call_ch,
	__Call_cos,
	__Call_cosec,
	__Call_cosech,
	__Call_ctg,
	__Call_cth,
	__Call_sec,
	__Call_sech,
	__Call_sin,
	__Call_sh,
	__Call_sqrt,
	__Call_tg,
	__Call_th,
	__Call_exp,
	__Call_int,
	__Call_lg,
	__Call_ln,
	__Call_log,
	__Call_pow,
	__Call_root,
	__Call_atan2,
	__Call_fmod,
	__Call_polar,
	__Call_ceil,
	__Call_floor,
	__Call_imag,
	__Call_pow10,
	__Call_real,
	__Call_random,
	__Call_identity,
	__Call_primes,
	__Call_isprime,
	__Call_gcd,
	__Call_lcm,
	__Call_polyroots,
	__Call_polyder,
	__Call_polyint,
	__Call_polygcd,
	__Call_polylcm,
	__Call_lsolve,
	__Call_ratfpart,
	__Call_ratfder,
	__Call_det,
	__Call_inverse,
	__Call_eigenvals,
	__Call_Nop,//eigenvec
	__Call_Nop, 							//"@box_lt",
	__Call_Nop, 							//"@box_rt",
	__Call_Nop, 							//"@box_lb",
	__Call_Nop, 							//"@box_rb",
	__Call_Nop, 							//"@box",
	__Call_Nop, 							//"@abs",
	__Call_Nop,
	__Call_Nop, 							//"@floor",
	__Call_Nop, 							//"@ceil",
	__Call_Nop, 							//"@empty_frame",
	__Call_Nop, 							//"@fraction",
	__Call_Nop, 							//"@diagonal_fraction",
	__Call_Nop, 							//"@sqrt",
	__Call_Nop, 							//"@root",
	__Call_Nop, 							//"@plusminus",
	__Call_Nop, 							//"@times",
	__Call_Nop, 							//"@circle_plus",
	__Call_Nop, 							//"@circle_times",
	__Call_Nop, 							//"@minus",
	__Call_Nop, 							//"@center_dot",
	__Call_Nop, 							//"@bullet",
	__Call_Nop, 							//"@lang",
	__Call_Nop, 							//"@rang",
	__Call_Nop, 							//"@identical",
	__Call_Nop, 							//"@almost_equal",
	__Call_Nop, 							//"@proportional",
	__Call_Nop, 							//"@therefore",
	__Call_Nop, 							//"@exist",
	__Call_Nop, 							//"@forall",
	__Call_SetRelIn,
	__Call_SetRelNotIn,
	__Call_SetAdd,
	__Call_SetMul,
	__Call_SetRelSetIn,
	__Call_SetRelSetInInv,
	__Call_SetRelSetInEq,
	__Call_SetRelSetInEqInv,
	__Call_SetRelNotIn,
	__Call_ConstructSetEmpty,
	__Call_Nop, 							//"@sup",
	__Call_Nop, 							//"@sub",
	__Call_Nop, 							//"@subsup",
	__Call_Nop, 							//"@over",
	__Call_Nop, 							//"@under",
	__Call_Nop, 							//"@underover",
	__Call_ConstructVector,					//"@table"
	__Call_VectorTranspose,					//"@transpose"
	__Call_Nop, 							//"@prime",
	__Call_Nop, //__Call_PushPolynomIntegerX,
	__Call_Nop, //__Call_PushPolynomRealX,
	__Call_Nop, //__Call_PushPolynomComplexX,
	__Call_Nop, //__Call_PushTrue,
	__Call_Nop, //__Call_PushFalse,
	__Call_Nop, //__Call_PushTime,
	__Call_Nop, //__Call_PushE,
	__Call_Nop, //__Call_PushEuler,
	__Call_Nop, //__Call_PushI,
	__Call_Nop, //__Call_PushPi,
	__Call_Nop, //__Call_PushPi,
	__Call_Nop, //__Call_PushSetIntegers,
	__Call_Nop, //__Call_PushSetReals,
	__Call_Nop, //__Call_PushSetRationals,
	__Call_Nop, //__Call_PushSetNaturalnumbers,
	__Call_Nop, //__Call_PushSetComplexes,
	__Call_Nop, 							//"?",
	__Call_Nop, //__Call_PushVariable,
	__Call_Nop, //__Call_PushLocalVariable,
	__Call_Nop, //__Call_PushVariableIndirect,
	__Call_Nop, //__Call_PushLocalVariableIndirect,
	__Call_UserCall,
	__Call_UserCallPolynomial,
	__Call_UserCallRationalFun
};

///////////////////////////////////////////////////////////////////////////////

long __ml_OperationCall( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg )
{
	if( (enum CDsrFunctionListID)(addr.param1) >= FUNCTION_STD_NONE && 
		(enum CDsrFunctionListID)(addr.param1) < FUNCTION_STD_LAST )
		return CDsrOperationCallArray[ (enum CDsrFunctionListID)(addr.param1) ]( vm, addr, arg );
	return 0;
}

///////////////////////////////////////////////////////////////////////////////
