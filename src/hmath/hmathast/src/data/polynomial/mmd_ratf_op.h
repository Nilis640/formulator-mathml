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

#ifndef __PARSEDSR_DATA_MMD_RATF_OP_H__
#define __PARSEDSR_DATA_MMD_RATF_OP_H__

#include "../uniword/uniword_op.h"
#include "mmd_poly_op.h"
#include "mmd_ratf.h"

///////////////////////////////////////////////////////////////////////////////

__HMATHLNGDLL__
MMD_RationalFun* _dsrop_RationalFunAdd_integer( MMD_RationalFun *a, MMD_RationalFun *b );
__HMATHLNGDLL__
MMD_RationalFun* _dsrop_RationalFunAdd_real( MMD_RationalFun *a, MMD_RationalFun *b );
__HMATHLNGDLL__
MMD_RationalFun* _dsrop_RationalFunAdd_complex( MMD_RationalFun *a, MMD_RationalFun *b );
__HMATHLNGDLL__
MMD_RationalFun* _dsrop_RationalFunPlus_integer( MMD_RationalFun *a );
__HMATHLNGDLL__
MMD_RationalFun* _dsrop_RationalFunPlus_real( MMD_RationalFun *a );
__HMATHLNGDLL__
MMD_RationalFun* _dsrop_RationalFunPlus_complex( MMD_RationalFun *a );
__HMATHLNGDLL__
MMD_RationalFun* _dsrop_RationalFunSub_integer( MMD_RationalFun *a, MMD_RationalFun *b );
__HMATHLNGDLL__
MMD_RationalFun* _dsrop_RationalFunSub_real( MMD_RationalFun *a, MMD_RationalFun *b );
__HMATHLNGDLL__
MMD_RationalFun* _dsrop_RationalFunSub_complex( MMD_RationalFun *a, MMD_RationalFun *b );
__HMATHLNGDLL__
MMD_RationalFun* _dsrop_RationalFunMinus_integer( MMD_RationalFun *a );
__HMATHLNGDLL__
MMD_RationalFun* _dsrop_RationalFunMinus_real( MMD_RationalFun *a );
__HMATHLNGDLL__
MMD_RationalFun* _dsrop_RationalFunMinus_complex( MMD_RationalFun *a );
__HMATHLNGDLL__
MMD_RationalFun* _dsrop_RationalFunMul_integer( MMD_RationalFun *a, MMD_RationalFun *b );
__HMATHLNGDLL__
MMD_RationalFun* _dsrop_RationalFunMul_real( MMD_RationalFun *a, MMD_RationalFun *b );
__HMATHLNGDLL__
MMD_RationalFun* _dsrop_RationalFunMul_complex( MMD_RationalFun *a, MMD_RationalFun *b );
__HMATHLNGDLL__
MMD_RationalFun* _dsrop_RationalFunDiv_integer( MMD_RationalFun *a, MMD_RationalFun *b );
__HMATHLNGDLL__
MMD_RationalFun* _dsrop_RationalFunDiv_real( MMD_RationalFun *a, MMD_RationalFun *b );
__HMATHLNGDLL__
MMD_RationalFun* _dsrop_RationalFunDiv_complex( MMD_RationalFun *a, MMD_RationalFun *b );
__HMATHLNGDLL__
MMD_RationalFun* _dsrop_RationalFunPow_integer( MMD_RationalFun *a, UniWord& b );
__HMATHLNGDLL__
MMD_RationalFun* _dsrop_RationalFunPow_real( MMD_RationalFun *a, UniWord& b );
__HMATHLNGDLL__
MMD_RationalFun* _dsrop_RationalFunPow_complex( MMD_RationalFun *a, UniWord& b );
__HMATHLNGDLL__
UniWord _dsrop_RationalFunEqual_anytype( MMD_RationalFun *a, MMD_RationalFun *b );
__HMATHLNGDLL__
UniWord _dsrop_RationalFunEqual_integer( MMD_RationalFun *a, MMD_RationalFun *b );
__HMATHLNGDLL__
UniWord _dsrop_RationalFunEqual_real( MMD_RationalFun *a, MMD_RationalFun *b );
__HMATHLNGDLL__
UniWord _dsrop_RationalFunEqual_complex( MMD_RationalFun *a, MMD_RationalFun *b );
__HMATHLNGDLL__
UniWord _dsrop_RationalFunNotEqual_integer( MMD_RationalFun *a, MMD_RationalFun *b );
__HMATHLNGDLL__
UniWord _dsrop_RationalFunNotEqual_real( MMD_RationalFun *a, MMD_RationalFun *b );
__HMATHLNGDLL__
UniWord _dsrop_RationalFunNotEqual_complex( MMD_RationalFun *a, MMD_RationalFun *b );
__HMATHLNGDLL__
MMD_RationalFun* _dsrop_RationalFunAssign_anytype( MMD_RationalFun *a, MMD_RationalFun *b );
__HMATHLNGDLL__
MMD_RationalFun* _dsrop_RationalFunAssign_integer( MMD_RationalFun *a, MMD_RationalFun *b );
__HMATHLNGDLL__
MMD_RationalFun* _dsrop_RationalFunAssign_real( MMD_RationalFun *a, MMD_RationalFun *b );
__HMATHLNGDLL__
MMD_RationalFun* _dsrop_RationalFunAssign_complex( MMD_RationalFun *a, MMD_RationalFun *b );
__HMATHLNGDLL__
MMD_RationalFun* _dsrop_Convert_natural2RationalFun_integer( UniWord& a, CParseDsrSymbolTable& _smbtable );
__HMATHLNGDLL__
MMD_RationalFun* _dsrop_Convert_natural2RationalFun_real( UniWord& a, CParseDsrSymbolTable& _smbtable );
__HMATHLNGDLL__
MMD_RationalFun* _dsrop_Convert_natural2RationalFun_complex( UniWord& a, CParseDsrSymbolTable& _smbtable );
__HMATHLNGDLL__
MMD_RationalFun* _dsrop_Convert_integer2RationalFun_integer( UniWord& a, CParseDsrSymbolTable& _smbtable );
__HMATHLNGDLL__
MMD_RationalFun* _dsrop_Convert_integer2RationalFun_real( UniWord& a, CParseDsrSymbolTable& _smbtable );
__HMATHLNGDLL__
MMD_RationalFun* _dsrop_Convert_integer2RationalFun_complex( UniWord& a, CParseDsrSymbolTable& _smbtable );
__HMATHLNGDLL__
MMD_RationalFun* _dsrop_Convert_rational2RationalFun_real( UniWord& a, CParseDsrSymbolTable& _smbtable );
__HMATHLNGDLL__
MMD_RationalFun* _dsrop_Convert_rational2RationalFun_complex( UniWord& a, CParseDsrSymbolTable& _smbtable );
__HMATHLNGDLL__
MMD_RationalFun* _dsrop_Convert_real2RationalFun_real( UniWord& a, CParseDsrSymbolTable& _smbtable );
__HMATHLNGDLL__
MMD_RationalFun* _dsrop_Convert_real2RationalFun_complex( UniWord& a, CParseDsrSymbolTable& _smbtable );
__HMATHLNGDLL__
MMD_RationalFun* _dsrop_Convert_complex2RationalFun_complex( UniWord& a, CParseDsrSymbolTable& _smbtable );
__HMATHLNGDLL__
MMD_RationalFun* _dsrop_Convert_RationalFun_integer2RationalFun_real( MMD_RationalFun* a );
__HMATHLNGDLL__
MMD_RationalFun* _dsrop_Convert_RationalFun_integer2RationalFun_complex( MMD_RationalFun* a );
__HMATHLNGDLL__
MMD_RationalFun* _dsrop_Convert_RationalFun_real2RationalFun_complex( MMD_RationalFun* a );

///////////////////////////////////////////////////////////////////////////////

#endif
