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

#if !defined( __PARSEDSR_OPERATOR_EXECOP_EXECOP_CALL_H__ )
#define __PARSEDSR_OPERATOR_EXECOP_EXECOP_CALL_H__

#include "execop.h"
#include "../datalink.h"
class CDSR_VMEval;

///////////////////////////////////////////////////////////////////////////////
__HMATHLNGDLL__ long __Call_Nop( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg );
__HMATHLNGDLL__ long __Call_SetAdd( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg );
__HMATHLNGDLL__ long __Call_SetMul( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg );
__HMATHLNGDLL__ long __Call_SetRelIn( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg );
__HMATHLNGDLL__ long __Call_SetRelNotIn( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg );
__HMATHLNGDLL__ long __Call_SetRelSetIn( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg );
__HMATHLNGDLL__ long __Call_SetRelSetInInv( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg );
__HMATHLNGDLL__ long __Call_SetRelSetInEq( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg );
__HMATHLNGDLL__ long __Call_SetRelSetInEqInv( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg );
__HMATHLNGDLL__ long __Call_SetRelSetNotIn( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg );
__HMATHLNGDLL__ long __Call_Square( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg );
__HMATHLNGDLL__ long __Call_SquareLValue( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg );
__HMATHLNGDLL__ long __Call_ConstructSet( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg );
__HMATHLNGDLL__ long __Call_Add( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg );
__HMATHLNGDLL__ long __Call_Sub( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg );
__HMATHLNGDLL__ long __Call_Mul( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg );
__HMATHLNGDLL__ long __Call_Div( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg );
__HMATHLNGDLL__ long __Call_Mod( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg );
__HMATHLNGDLL__ long __Call_Pow( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg );
__HMATHLNGDLL__ long __Call_Less( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg );
__HMATHLNGDLL__ long __Call_LessEq( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg );
__HMATHLNGDLL__ long __Call_Greater( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg );
__HMATHLNGDLL__ long __Call_GreaterEq( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg );
__HMATHLNGDLL__ long __Call_BoolAnd( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg );
__HMATHLNGDLL__ long __Call_BoolOr( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg );
__HMATHLNGDLL__ long __Call_BoolNot( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg );
__HMATHLNGDLL__ long __Call_NotEqual( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg );
__HMATHLNGDLL__ long __Call_Assign( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg );
__HMATHLNGDLL__ long __Call_Equal( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg );
__HMATHLNGDLL__ long __Call_TypeSpec( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg );
__HMATHLNGDLL__ long __Call_Convert_natural2integer( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg );
__HMATHLNGDLL__ long __Call_Convert_natural2bits( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg );
__HMATHLNGDLL__ long __Call_Convert_natural2rational( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg );
__HMATHLNGDLL__ long __Call_Convert_natural2real( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg );
__HMATHLNGDLL__ long __Call_Convert_natural2complex( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg );
__HMATHLNGDLL__ long __Call_Convert_integer2bits( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg );
__HMATHLNGDLL__ long __Call_Convert_integer2rational( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg );
__HMATHLNGDLL__ long __Call_Convert_integer2real( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg );
__HMATHLNGDLL__ long __Call_Convert_integer2complex( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg );
__HMATHLNGDLL__ long __Call_Convert_rational2real( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg );
__HMATHLNGDLL__ long __Call_Convert_rational2complex( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg );
__HMATHLNGDLL__ long __Call_Convert_real2complex( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg );
__HMATHLNGDLL__ long __Call_Convert_natural2Polynomial_integer( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg );
__HMATHLNGDLL__ long __Call_Convert_natural2Polynomial_real( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg );
__HMATHLNGDLL__ long __Call_Convert_natural2Polynomial_complex( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg );
__HMATHLNGDLL__ long __Call_Convert_natural2RationalFun_integer( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg );
__HMATHLNGDLL__ long __Call_Convert_natural2RationalFun_real( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg );
__HMATHLNGDLL__ long __Call_Convert_natural2RationalFun_complex( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg );
__HMATHLNGDLL__ long __Call_Convert_integer2Polynomial_integer( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg );
__HMATHLNGDLL__ long __Call_Convert_integer2Polynomial_real( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg );
__HMATHLNGDLL__ long __Call_Convert_integer2Polynomial_complex( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg );
__HMATHLNGDLL__ long __Call_Convert_integer2RationalFun_integer( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg );
__HMATHLNGDLL__ long __Call_Convert_integer2RationalFun_real( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg );
__HMATHLNGDLL__ long __Call_Convert_integer2RationalFun_complex( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg );
__HMATHLNGDLL__ long __Call_Convert_rational2Polynomial_real( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg );
__HMATHLNGDLL__ long __Call_Convert_rational2Polynomial_complex( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg );
__HMATHLNGDLL__ long __Call_Convert_rational2RationalFun_real( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg );
__HMATHLNGDLL__ long __Call_Convert_rational2RationalFun_complex( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg );
__HMATHLNGDLL__ long __Call_Convert_real2Polynomial_real( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg );
__HMATHLNGDLL__ long __Call_Convert_real2Polynomial_complex( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg );
__HMATHLNGDLL__ long __Call_Convert_real2RationalFun_real( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg );
__HMATHLNGDLL__ long __Call_Convert_real2RationalFun_complex( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg );
__HMATHLNGDLL__ long __Call_Convert_complex2Polynomial_complex( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg );
__HMATHLNGDLL__ long __Call_Convert_complex2RationalFun_complex( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg );
__HMATHLNGDLL__ long __Call_Convert_Polynomial_integer2Polynomial_real( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg );
__HMATHLNGDLL__ long __Call_Convert_Polynomial_integer2Polynomial_complex( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg );
__HMATHLNGDLL__ long __Call_Convert_Polynomial_real2Polynomial_complex( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg );
__HMATHLNGDLL__ long __Call_Convert_Polynomial_integer2RationalFun_integer( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg );
__HMATHLNGDLL__ long __Call_Convert_Polynomial_integer2RationalFun_real( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg );
__HMATHLNGDLL__ long __Call_Convert_Polynomial_integer2RationalFun_complex( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg );
__HMATHLNGDLL__ long __Call_Convert_Polynomial_real2RationalFun_real( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg );
__HMATHLNGDLL__ long __Call_Convert_Polynomial_real2RationalFun_complex( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg );
__HMATHLNGDLL__ long __Call_Convert_Polynomial_complex2RationalFun_complex( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg );
__HMATHLNGDLL__ long __Call_Convert_RationalFun_integer2RationalFun_real( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg );
__HMATHLNGDLL__ long __Call_Convert_RationalFun_integer2RationalFun_complex( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg );
__HMATHLNGDLL__ long __Call_Convert_RationalFun_real2RationalFun_complex( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg );
__HMATHLNGDLL__ long __Call_VectorConvert_natural2integer( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg );
__HMATHLNGDLL__ long __Call_VectorConvert_natural2rational( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg );
__HMATHLNGDLL__ long __Call_VectorConvert_natural2real( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg );
__HMATHLNGDLL__ long __Call_VectorConvert_natural2complex( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg );
__HMATHLNGDLL__ long __Call_VectorConvert_integer2rational( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg );
__HMATHLNGDLL__ long __Call_VectorConvert_integer2real( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg );
__HMATHLNGDLL__ long __Call_VectorConvert_integer2complex( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg );
__HMATHLNGDLL__ long __Call_VectorConvert_rational2real( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg );
__HMATHLNGDLL__ long __Call_VectorConvert_rational2complex( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg );
__HMATHLNGDLL__ long __Call_VectorConvert_real2complex( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg );
__HMATHLNGDLL__ long __Call_abs( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg );
__HMATHLNGDLL__ long __Call_arch( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg );
__HMATHLNGDLL__ long __Call_arccos( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg );
__HMATHLNGDLL__ long __Call_arccosec( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg );
__HMATHLNGDLL__ long __Call_arcsec( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg );
__HMATHLNGDLL__ long __Call_arcsin( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg );
__HMATHLNGDLL__ long __Call_arctg( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg );
__HMATHLNGDLL__ long __Call_arcth( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg );
__HMATHLNGDLL__ long __Call_arsh( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg );
__HMATHLNGDLL__ long __Call_ch( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg );
__HMATHLNGDLL__ long __Call_cos( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg );
__HMATHLNGDLL__ long __Call_cosec( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg );
__HMATHLNGDLL__ long __Call_cosech( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg );
__HMATHLNGDLL__ long __Call_ctg( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg );
__HMATHLNGDLL__ long __Call_cth( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg );
__HMATHLNGDLL__ long __Call_sec( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg );
__HMATHLNGDLL__ long __Call_sech( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg );
__HMATHLNGDLL__ long __Call_sin( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg );
__HMATHLNGDLL__ long __Call_sh( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg );
__HMATHLNGDLL__ long __Call_sqrt( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg );
__HMATHLNGDLL__ long __Call_tg( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg );
__HMATHLNGDLL__ long __Call_th( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg );
__HMATHLNGDLL__ long __Call_exp( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg );
__HMATHLNGDLL__ long __Call_int( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg );
__HMATHLNGDLL__ long __Call_lg( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg );
__HMATHLNGDLL__ long __Call_ln( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg );
__HMATHLNGDLL__ long __Call_log( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg );
__HMATHLNGDLL__ long __Call_pow( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg );
__HMATHLNGDLL__ long __Call_root( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg );
__HMATHLNGDLL__ long __Call_atan2( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg );
__HMATHLNGDLL__ long __Call_fmod( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg );
__HMATHLNGDLL__ long __Call_polar( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg );
__HMATHLNGDLL__ long __Call_ceil( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg );
__HMATHLNGDLL__ long __Call_floor( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg );
__HMATHLNGDLL__ long __Call_imag( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg );
__HMATHLNGDLL__ long __Call_pow10( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg );
__HMATHLNGDLL__ long __Call_real( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg );
__HMATHLNGDLL__ long __Call_random( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg );
__HMATHLNGDLL__ long __Call_identity( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg );
__HMATHLNGDLL__ long __Call_primes( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg );
__HMATHLNGDLL__ long __Call_isprime( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg );
__HMATHLNGDLL__ long __Call_gcd( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg );
__HMATHLNGDLL__ long __Call_lcm( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg );
__HMATHLNGDLL__ long __Call_polyroots( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg );
__HMATHLNGDLL__ long __Call_polyder( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg );
__HMATHLNGDLL__ long __Call_polyint( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg );
__HMATHLNGDLL__ long __Call_polygcd( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg );
__HMATHLNGDLL__ long __Call_polylcm( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg );
__HMATHLNGDLL__ long __Call_lsolve( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg );
__HMATHLNGDLL__ long __Call_ratfpart( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg );
__HMATHLNGDLL__ long __Call_ratfder( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg );
__HMATHLNGDLL__ long __Call_det( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg );
__HMATHLNGDLL__ long __Call_inverse( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg );
__HMATHLNGDLL__ long __Call_eigenvals( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg );
__HMATHLNGDLL__ long __Call_SetRelIn( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg );
__HMATHLNGDLL__ long __Call_SetRelNotIn( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg );
__HMATHLNGDLL__ long __Call_SetAdd( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg );
__HMATHLNGDLL__ long __Call_SetMul( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg );
__HMATHLNGDLL__ long __Call_SetRelSetIn( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg );
__HMATHLNGDLL__ long __Call_SetRelSetInInv( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg );
__HMATHLNGDLL__ long __Call_SetRelSetInEq( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg );
__HMATHLNGDLL__ long __Call_SetRelSetInEqInv( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg );
__HMATHLNGDLL__ long __Call_SetRelNotIn( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg );
__HMATHLNGDLL__ long __Call_ConstructSetEmpty( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg );
__HMATHLNGDLL__ long __Call_ConstructVector( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg );
__HMATHLNGDLL__ long __Call_VectorTranspose( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg );
__HMATHLNGDLL__ long __Call_UserCall( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg );
__HMATHLNGDLL__ long __Call_UserCallPolynomial( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg );
__HMATHLNGDLL__ long __Call_UserCallRationalFun( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg );

///////////////////////////////////////////////////////////////////////////////

#endif
