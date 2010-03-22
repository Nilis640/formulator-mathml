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

#ifndef __PARSEDSR_DATA_MMD_POLY_OP_H__
#define __PARSEDSR_DATA_MMD_POLY_OP_H__

#include "../uniword/uniword_op.h"
#include "mmd_poly.h"
#include "mmd_ratf.h"

///////////////////////////////////////////////////////////////////////////////

__HMATHLNGDLL__
MMD_Polynomial* _dsrop_PolynomialAdd_integer( MMD_Polynomial *a, MMD_Polynomial *b );
__HMATHLNGDLL__
MMD_Polynomial* _dsrop_PolynomialAdd_real( MMD_Polynomial *a, MMD_Polynomial *b );
__HMATHLNGDLL__
MMD_Polynomial* _dsrop_PolynomialAdd_complex( MMD_Polynomial *a, MMD_Polynomial *b );
__HMATHLNGDLL__
MMD_Polynomial* _dsrop_PolynomialPlus_integer( MMD_Polynomial *a );
__HMATHLNGDLL__
MMD_Polynomial* _dsrop_PolynomialPlus_real( MMD_Polynomial *a );
__HMATHLNGDLL__
MMD_Polynomial* _dsrop_PolynomialPlus_complex( MMD_Polynomial *a );
__HMATHLNGDLL__
MMD_Polynomial* _dsrop_PolynomialSub_integer( MMD_Polynomial *a, MMD_Polynomial *b );
__HMATHLNGDLL__
MMD_Polynomial* _dsrop_PolynomialSub_real( MMD_Polynomial *a, MMD_Polynomial *b );
__HMATHLNGDLL__
MMD_Polynomial* _dsrop_PolynomialSub_complex( MMD_Polynomial *a, MMD_Polynomial *b );
__HMATHLNGDLL__
MMD_Polynomial* _dsrop_PolynomialMinus_integer( MMD_Polynomial *a );
__HMATHLNGDLL__
MMD_Polynomial* _dsrop_PolynomialMinus_real( MMD_Polynomial *a );
__HMATHLNGDLL__
MMD_Polynomial* _dsrop_PolynomialMinus_complex( MMD_Polynomial *a );
__HMATHLNGDLL__
MMD_Polynomial* _dsrop_PolynomialMul_integer( MMD_Polynomial *a, MMD_Polynomial *b );
__HMATHLNGDLL__
MMD_Polynomial* _dsrop_PolynomialMul_real( MMD_Polynomial *a, MMD_Polynomial *b );
__HMATHLNGDLL__
MMD_Polynomial* _dsrop_PolynomialMul_complex( MMD_Polynomial *a, MMD_Polynomial *b );
__HMATHLNGDLL__
MMD_RationalFun* _dsrop_PolynomialDiv_anytype( MMD_Polynomial *a, MMD_Polynomial *b );
__HMATHLNGDLL__
MMD_RationalFun* _dsrop_PolynomialDiv_integer( MMD_Polynomial *a, MMD_Polynomial *b );
__HMATHLNGDLL__
MMD_RationalFun* _dsrop_PolynomialDiv_real( MMD_Polynomial *a, MMD_Polynomial *b );
__HMATHLNGDLL__
MMD_RationalFun* _dsrop_PolynomialDiv_complex( MMD_Polynomial *a, MMD_Polynomial *b );
__HMATHLNGDLL__
MMD_Polynomial* _dsrop_PolynomialDiv_integer( MMD_Polynomial *a, UniWord& b );
__HMATHLNGDLL__
MMD_Polynomial* _dsrop_PolynomialDiv_real( MMD_Polynomial *a, UniWord& b );
__HMATHLNGDLL__
MMD_Polynomial* _dsrop_PolynomialDiv_complex( MMD_Polynomial *a, UniWord& b );
__HMATHLNGDLL__
MMD_Polynomial* _dsrop_PolynomialPow_anytype( MMD_Polynomial *a, UniWord& b );
__HMATHLNGDLL__
MMD_Polynomial* _dsrop_PolynomialPow_integer( MMD_Polynomial *a, UniWord& b );
__HMATHLNGDLL__
MMD_Polynomial* _dsrop_PolynomialPow_real( MMD_Polynomial *a, UniWord& b );
__HMATHLNGDLL__
MMD_Polynomial* _dsrop_PolynomialPow_complex( MMD_Polynomial *a, UniWord& b );
__HMATHLNGDLL__
UniWord _dsrop_PolynomialEqual_anytype( MMD_Polynomial *a, MMD_Polynomial *b );
__HMATHLNGDLL__
UniWord _dsrop_PolynomialEqual_integer( MMD_Polynomial *a, MMD_Polynomial *b );
__HMATHLNGDLL__
UniWord _dsrop_PolynomialEqual_real( MMD_Polynomial *a, MMD_Polynomial *b );
__HMATHLNGDLL__
UniWord _dsrop_PolynomialEqual_complex( MMD_Polynomial *a, MMD_Polynomial *b );
__HMATHLNGDLL__
UniWord _dsrop_PolynomialNotEqual_integer( MMD_Polynomial *a, MMD_Polynomial *b );
__HMATHLNGDLL__
UniWord _dsrop_PolynomialNotEqual_real( MMD_Polynomial *a, MMD_Polynomial *b );
__HMATHLNGDLL__
UniWord _dsrop_PolynomialNotEqual_complex( MMD_Polynomial *a, MMD_Polynomial *b );
__HMATHLNGDLL__
MMD_Polynomial* _dsrop_PolynomialAssign_anytype( MMD_Polynomial *a, MMD_Polynomial *b );
__HMATHLNGDLL__
MMD_Polynomial* _dsrop_PolynomialAssign_integer( MMD_Polynomial *a, MMD_Polynomial *b );
__HMATHLNGDLL__
MMD_Polynomial* _dsrop_PolynomialAssign_real( MMD_Polynomial *a, MMD_Polynomial *b );
__HMATHLNGDLL__
MMD_Polynomial* _dsrop_PolynomialAssign_complex( MMD_Polynomial *a, MMD_Polynomial *b );
__HMATHLNGDLL__
MMD_Polynomial* _dsrop_Convert_natural2Polynomial_integer( UniWord& a, CParseDsrSymbolTable& _smbtable );
__HMATHLNGDLL__
MMD_Polynomial* _dsrop_Convert_natural2Polynomial_real( UniWord& a, CParseDsrSymbolTable& _smbtable );
__HMATHLNGDLL__
MMD_Polynomial* _dsrop_Convert_natural2Polynomial_complex( UniWord& a, CParseDsrSymbolTable& _smbtable );
__HMATHLNGDLL__
MMD_Polynomial* _dsrop_Convert_integer2Polynomial_integer( UniWord& a, CParseDsrSymbolTable& _smbtable );
__HMATHLNGDLL__
MMD_Polynomial* _dsrop_Convert_integer2Polynomial_real( UniWord& a, CParseDsrSymbolTable& _smbtable );
__HMATHLNGDLL__
MMD_Polynomial* _dsrop_Convert_integer2Polynomial_complex( UniWord& a, CParseDsrSymbolTable& _smbtable );
__HMATHLNGDLL__
MMD_Polynomial* _dsrop_Convert_rational2Polynomial_real( UniWord& a, CParseDsrSymbolTable& _smbtable );
__HMATHLNGDLL__
MMD_Polynomial* _dsrop_Convert_rational2Polynomial_complex( UniWord& a, CParseDsrSymbolTable& _smbtable );
__HMATHLNGDLL__
MMD_Polynomial* _dsrop_Convert_real2Polynomial_real( UniWord& a, CParseDsrSymbolTable& _smbtable );
__HMATHLNGDLL__
MMD_Polynomial* _dsrop_Convert_real2Polynomial_complex( UniWord& a, CParseDsrSymbolTable& _smbtable );
__HMATHLNGDLL__
MMD_Polynomial* _dsrop_Convert_complex2Polynomial_complex( UniWord& a, CParseDsrSymbolTable& _smbtable );
__HMATHLNGDLL__
MMD_Polynomial* _dsrop_Convert_Polynomial_integer2Polynomial_real( MMD_Polynomial* a );
__HMATHLNGDLL__
MMD_Polynomial* _dsrop_Convert_Polynomial_integer2Polynomial_complex( MMD_Polynomial* a );
__HMATHLNGDLL__
MMD_Polynomial* _dsrop_Convert_Polynomial_real2Polynomial_complex( MMD_Polynomial* a );
__HMATHLNGDLL__
MMD_RationalFun* _dsrop_Convert_Polynomial_integer2RationalFun_integer( MMD_Polynomial* a );
__HMATHLNGDLL__
MMD_RationalFun* _dsrop_Convert_Polynomial_integer2RationalFun_real( MMD_Polynomial* a );
__HMATHLNGDLL__
MMD_RationalFun* _dsrop_Convert_Polynomial_integer2RationalFun_complex( MMD_Polynomial* a );
__HMATHLNGDLL__
MMD_RationalFun* _dsrop_Convert_Polynomial_real2RationalFun_real( MMD_Polynomial* a );
__HMATHLNGDLL__
MMD_RationalFun* _dsrop_Convert_Polynomial_real2RationalFun_complex( MMD_Polynomial* a );
__HMATHLNGDLL__
MMD_RationalFun* _dsrop_Convert_Polynomial_complex2RationalFun_complex( MMD_Polynomial* a );

///////////////////////////////////////////////////////////////////////////////

#endif
