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

#ifndef __PARSEDSR_DATA_MMD_VEC_OP_H__
#define __PARSEDSR_DATA_MMD_VEC_OP_H__

#include "../uniword/uniword_op.h"
#include "mmd_vec.h"

__HMATHLNGDLL__
void _dsrop_abs_Vector_integer( UniWord* resultDet, MMD_Vector *a, CParseDsrSymbolTable& smbtable );
__HMATHLNGDLL__
void _dsrop_abs_Vector_real( UniWord* resultDet, MMD_Vector *a, CParseDsrSymbolTable& smbtable );
__HMATHLNGDLL__
void _dsrop_abs_Vector_complex( UniWord* resultDet, MMD_Vector *a, CParseDsrSymbolTable& smbtable );
__HMATHLNGDLL__
void _dsrop_VectorInverse_real( UniWord *resultDet, MMD_Vector *a, CParseDsrSymbolTable& smbtable );
__HMATHLNGDLL__
void _dsrop_VectorInverse_complex( UniWord *resultDet, MMD_Vector *a, CParseDsrSymbolTable& smbtable );
__HMATHLNGDLL__
MMD_Vector* _dsrop_VectorTranspose_natural( MMD_Vector *a );
__HMATHLNGDLL__
MMD_Vector* _dsrop_VectorTranspose_integer( MMD_Vector *a );
__HMATHLNGDLL__
MMD_Vector* _dsrop_VectorTranspose_rational( MMD_Vector *a );
__HMATHLNGDLL__
MMD_Vector* _dsrop_VectorTranspose_real( MMD_Vector *a );
__HMATHLNGDLL__
MMD_Vector* _dsrop_VectorTranspose_complex( MMD_Vector *a );

__HMATHLNGDLL__
MMD_Vector* _dsrop_VectorAdd_natural( MMD_Vector *a, MMD_Vector *b );
__HMATHLNGDLL__
MMD_Vector* _dsrop_VectorAdd_integer( MMD_Vector *a, MMD_Vector *b );
__HMATHLNGDLL__
MMD_Vector* _dsrop_VectorAdd_rational( MMD_Vector *a, MMD_Vector *b );
__HMATHLNGDLL__
MMD_Vector* _dsrop_VectorAdd_real( MMD_Vector *a, MMD_Vector *b );
__HMATHLNGDLL__
MMD_Vector* _dsrop_VectorAdd_complex( MMD_Vector *a, MMD_Vector *b );
__HMATHLNGDLL__
MMD_Vector* _dsrop_VectorPlus_natural( MMD_Vector *a );
__HMATHLNGDLL__
MMD_Vector* _dsrop_VectorPlus_integer( MMD_Vector *a );
__HMATHLNGDLL__
MMD_Vector* _dsrop_VectorPlus_rational( MMD_Vector *a );
__HMATHLNGDLL__
MMD_Vector* _dsrop_VectorPlus_real( MMD_Vector *a );
__HMATHLNGDLL__
MMD_Vector* _dsrop_VectorPlus_complex( MMD_Vector *a );
__HMATHLNGDLL__
MMD_Vector* _dsrop_VectorSub_integer( MMD_Vector *a, MMD_Vector *b );
__HMATHLNGDLL__
MMD_Vector* _dsrop_VectorSub_rational( MMD_Vector *a, MMD_Vector *b );
__HMATHLNGDLL__
MMD_Vector* _dsrop_VectorSub_real( MMD_Vector *a, MMD_Vector *b );
__HMATHLNGDLL__
MMD_Vector* _dsrop_VectorSub_complex( MMD_Vector *a, MMD_Vector *b );
__HMATHLNGDLL__
MMD_Vector* _dsrop_VectorMinus_integer( MMD_Vector *a );
__HMATHLNGDLL__
MMD_Vector* _dsrop_VectorMinus_rational( MMD_Vector *a );
__HMATHLNGDLL__
MMD_Vector* _dsrop_VectorMinus_real( MMD_Vector *a );
__HMATHLNGDLL__
MMD_Vector* _dsrop_VectorMinus_complex( MMD_Vector *a );
__HMATHLNGDLL__
MMD_Vector* _dsrop_VectorMul_natural( MMD_Vector *a, UniWord& b );
__HMATHLNGDLL__
MMD_Vector* _dsrop_VectorMul_integer( MMD_Vector *a, UniWord& b );
__HMATHLNGDLL__
MMD_Vector* _dsrop_VectorMul_rational( MMD_Vector *a, UniWord& b );
__HMATHLNGDLL__
MMD_Vector* _dsrop_VectorMul_real( MMD_Vector *a, UniWord& b );
__HMATHLNGDLL__
MMD_Vector* _dsrop_VectorMul_complex( MMD_Vector *a, UniWord& b );
__HMATHLNGDLL__
UniWord _dsrop_VectorScalarMul_natural( MMD_Vector *a, MMD_Vector *b );
__HMATHLNGDLL__
UniWord _dsrop_VectorScalarMul_integer( MMD_Vector *a, MMD_Vector *b );
__HMATHLNGDLL__
UniWord _dsrop_VectorScalarMul_rational( MMD_Vector *a, MMD_Vector *b );
__HMATHLNGDLL__
UniWord _dsrop_VectorScalarMul_real( MMD_Vector *a, MMD_Vector *b );
__HMATHLNGDLL__
UniWord _dsrop_VectorScalarMul_complex( MMD_Vector *a, MMD_Vector *b );
__HMATHLNGDLL__
MMD_Vector* _dsrop_VectorMul_natural( MMD_Vector *a, MMD_Vector *b );
__HMATHLNGDLL__
MMD_Vector* _dsrop_VectorMul_integer( MMD_Vector *a, MMD_Vector *b );
__HMATHLNGDLL__
MMD_Vector* _dsrop_VectorMul_rational( MMD_Vector *a, MMD_Vector *b );
__HMATHLNGDLL__
MMD_Vector* _dsrop_VectorMul_real( MMD_Vector *a, MMD_Vector *b );
__HMATHLNGDLL__
MMD_Vector* _dsrop_VectorMul_complex( MMD_Vector *a, MMD_Vector *b );
__HMATHLNGDLL__
MMD_Vector* _dsrop_VectorDiv_natural( MMD_Vector *a, UniWord& b );
__HMATHLNGDLL__
MMD_Vector* _dsrop_VectorDiv_integer( MMD_Vector *a, UniWord& b );
__HMATHLNGDLL__
MMD_Vector* _dsrop_VectorDiv_rational( MMD_Vector *a, UniWord& b );
__HMATHLNGDLL__
MMD_Vector* _dsrop_VectorDiv_real( MMD_Vector *a, UniWord& b );
__HMATHLNGDLL__
MMD_Vector* _dsrop_VectorDiv_complex( MMD_Vector *a, UniWord& b );
__HMATHLNGDLL__
MMD_Vector* _dsrop_VectorConvert_natural2integer( MMD_Vector *a );
__HMATHLNGDLL__
MMD_Vector* _dsrop_VectorConvert_natural2rational( MMD_Vector *a );
__HMATHLNGDLL__
MMD_Vector* _dsrop_VectorConvert_natural2real( MMD_Vector *a );
__HMATHLNGDLL__
MMD_Vector* _dsrop_VectorConvert_natural2complex( MMD_Vector *a );
__HMATHLNGDLL__
MMD_Vector* _dsrop_VectorConvert_integer2rational( MMD_Vector *a );
__HMATHLNGDLL__
MMD_Vector* _dsrop_VectorConvert_integer2real( MMD_Vector *a );
__HMATHLNGDLL__
MMD_Vector* _dsrop_VectorConvert_integer2complex( MMD_Vector *a );
__HMATHLNGDLL__
MMD_Vector* _dsrop_VectorConvert_rational2real( MMD_Vector *a );
__HMATHLNGDLL__
MMD_Vector* _dsrop_VectorConvert_rational2complex( MMD_Vector *a );
__HMATHLNGDLL__
MMD_Vector* _dsrop_VectorConvert_real2complex( MMD_Vector *a );
__HMATHLNGDLL__
UniWord _dsrop_VectorEqual_anytype( MMD_Vector *a, MMD_Vector *b );
__HMATHLNGDLL__
UniWord _dsrop_VectorEqual_natural( MMD_Vector *a, MMD_Vector *b );
__HMATHLNGDLL__
UniWord _dsrop_VectorEqual_integer( MMD_Vector *a, MMD_Vector *b );
__HMATHLNGDLL__
UniWord _dsrop_VectorEqual_rational( MMD_Vector *a, MMD_Vector *b );
__HMATHLNGDLL__
UniWord _dsrop_VectorEqual_real( MMD_Vector *a, MMD_Vector *b );
__HMATHLNGDLL__
UniWord _dsrop_VectorEqual_complex( MMD_Vector *a, MMD_Vector *b );
__HMATHLNGDLL__
MMD_Vector* _dsrop_VectorPow_natural( MMD_Vector *a, UniWord& p );
__HMATHLNGDLL__
MMD_Vector* _dsrop_VectorPow_integer( MMD_Vector *a, UniWord& p );
__HMATHLNGDLL__
MMD_Vector* _dsrop_VectorPow_rational( MMD_Vector *a, UniWord& p );
__HMATHLNGDLL__
MMD_Vector* _dsrop_VectorPow_real( MMD_Vector *a, UniWord& p );
__HMATHLNGDLL__
MMD_Vector* _dsrop_VectorPow_complex( MMD_Vector *a, UniWord& p );
__HMATHLNGDLL__
MMD_Vector* _dsrop_VectorAssign_natural( MMD_Vector *a, MMD_Vector *b );
__HMATHLNGDLL__
MMD_Vector* _dsrop_VectorAssign_integer( MMD_Vector *a, MMD_Vector *b );
__HMATHLNGDLL__
MMD_Vector* _dsrop_VectorAssign_rational( MMD_Vector *a, MMD_Vector *b );
__HMATHLNGDLL__
MMD_Vector* _dsrop_VectorAssign_real( MMD_Vector *a, MMD_Vector *b );
__HMATHLNGDLL__
MMD_Vector* _dsrop_VectorAssign_complex( MMD_Vector *a, MMD_Vector *b );
__HMATHLNGDLL__
UniWord _dsrop_VectorNotEqual_natural( MMD_Vector *a, MMD_Vector *b );
__HMATHLNGDLL__
UniWord _dsrop_VectorNotEqual_integer( MMD_Vector *a, MMD_Vector *b );
__HMATHLNGDLL__
UniWord _dsrop_VectorNotEqual_rational( MMD_Vector *a, MMD_Vector *b );
__HMATHLNGDLL__
UniWord _dsrop_VectorNotEqual_real( MMD_Vector *a, MMD_Vector *b );
__HMATHLNGDLL__
UniWord _dsrop_VectorNotEqual_complex( MMD_Vector *a, MMD_Vector *b );

__HMATHLNGDLL__
MMD_Vector* _dsrop_ConstructVector_natural( CParseDsrSymbolTable& smbtable, UniWord *first_arg );
__HMATHLNGDLL__
MMD_Vector* _dsrop_ConstructVector_integer( CParseDsrSymbolTable& smbtable, UniWord *first_arg );
__HMATHLNGDLL__
MMD_Vector* _dsrop_ConstructVector_rational( CParseDsrSymbolTable& smbtable, UniWord *first_arg );
__HMATHLNGDLL__
MMD_Vector* _dsrop_ConstructVector_real( CParseDsrSymbolTable& smbtable, UniWord *first_arg );
__HMATHLNGDLL__
MMD_Vector* _dsrop_ConstructVector_complex( CParseDsrSymbolTable& smbtable, UniWord *first_arg );
__HMATHLNGDLL__
MMD_Vector* _dsrop_ConstructVector_anytype( CParseDsrSymbolTable& smbtable, UniWord *first_arg, enum CDsrDataTypes nested_type );
__HMATHLNGDLL__
MMD_Vector* _dsrop_ConstructVector_identity( CParseDsrSymbolTable& smbtable, long dim );

#endif

