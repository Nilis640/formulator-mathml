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

#if !defined( __PARSEDSR_OPERATOR_OPERATOR_H__ )
#define __PARSEDSR_OPERATOR_OPERATOR_H__

#include "../hmathast_global.h"
#include "../type/typetable.h"

///////////////////////////////////////////////////////////////////////////////

class CParseDsrSymbol;
class CParseDsrSymbolTable;
class UniWordVector;

enum CDsrFunctionListID
{
	FUNCTION_STD_NONE = 0,
	OPERATOR_op_setadd,
	OPERATOR_op_setmul,
	OPERATOR_op_rel_in,
	OPERATOR_op_rel_not_in,
	OPERATOR_op_rel_set_in,
	OPERATOR_op_rel_set_in_inv,
	OPERATOR_op_rel_set_in_eq,
	OPERATOR_op_rel_set_in_eq_inv,
	OPERATOR_op_rel_set_not_in,
	OPERATOR_round_brackets,
	OPERATOR_square_brackets,
	OPERATOR_square_brackets_byaddr,
	OPERATOR_curly_brackets,
	OPERATOR_op_add,
	OPERATOR_op_sub,
	OPERATOR_op_mul,
	OPERATOR_op_div,
	OPERATOR_op_mod,
	OPERATOR_op_pow,
	OPERATOR_op_rel_l,
	OPERATOR_op_rel_le,
	OPERATOR_op_rel_g,
	OPERATOR_op_rel_ge,
	OPERATOR_op_bool_and,
	OPERATOR_op_bool_or,
	OPERATOR_op_bool_not,
	OPERATOR_op_compare_ne,
	OPERATOR_op_assign,
	OPERATOR_op_return,
	OPERATOR_op_create_x,
	OPERATOR_op_equation_e,
	OPERATOR_op_equation_l,
	OPERATOR_op_equation_le,
	OPERATOR_op_equation_g,
	OPERATOR_op_equation_ge,
	OPERATOR_op_compare_e,
	OPERATOR_op_type_spec,
	OPERATOR_op_convert_NATURAL_2_INTEGER,
	OPERATOR_op_convert_NATURAL_2_BITS,
	OPERATOR_op_convert_NATURAL_2_RATIONAL,
	OPERATOR_op_convert_NATURAL_2_REAL,
	OPERATOR_op_convert_NATURAL_2_COMPLEX,
	OPERATOR_op_convert_INTEGER_2_BITS,
	OPERATOR_op_convert_INTEGER_2_RATIONAL,
	OPERATOR_op_convert_INTEGER_2_REAL,
	OPERATOR_op_convert_INTEGER_2_COMPLEX,
	OPERATOR_op_convert_RATIONAL_2_REAL,
	OPERATOR_op_convert_RATIONAL_2_COMPLEX,
	OPERATOR_op_convert_REAL_2_COMPLEX,
	OPERATOR_op_convert_NATURAL_2_POLYNOM_INTEGER,
	OPERATOR_op_convert_NATURAL_2_POLYNOM_REAL,
	OPERATOR_op_convert_NATURAL_2_POLYNOM_COMPLEX,
	OPERATOR_op_convert_NATURAL_2_RATIONAL_FUN_INTEGER,
	OPERATOR_op_convert_NATURAL_2_RATIONAL_FUN_REAL,
	OPERATOR_op_convert_NATURAL_2_RATIONAL_FUN_COMPLEX,
	OPERATOR_op_convert_INTEGER_2_POLYNOM_INTEGER,
	OPERATOR_op_convert_INTEGER_2_POLYNOM_REAL,
	OPERATOR_op_convert_INTEGER_2_POLYNOM_COMPLEX,
	OPERATOR_op_convert_INTEGER_2_RATIONAL_FUN_INTEGER,
	OPERATOR_op_convert_INTEGER_2_RATIONAL_FUN_REAL,
	OPERATOR_op_convert_INTEGER_2_RATIONAL_FUN_COMPLEX,
	OPERATOR_op_convert_RATIONAL_2_POLYNOM_REAL,
	OPERATOR_op_convert_RATIONAL_2_POLYNOM_COMPLEX,
	OPERATOR_op_convert_RATIONAL_2_RATIONAL_FUN_REAL,
	OPERATOR_op_convert_RATIONAL_2_RATIONAL_FUN_COMPLEX,
	OPERATOR_op_convert_REAL_2_POLYNOM_REAL,
	OPERATOR_op_convert_REAL_2_POLYNOM_COMPLEX,
	OPERATOR_op_convert_REAL_2_RATIONAL_FUN_REAL,
	OPERATOR_op_convert_REAL_2_RATIONAL_FUN_COMPLEX,
	OPERATOR_op_convert_COMPLEX_2_POLYNOM_COMPLEX,
	OPERATOR_op_convert_COMPLEX_2_RATIONAL_FUN_COMPLEX,
	OPERATOR_op_convert_POLYNOM_INTEGER_2_POLYNOM_REAL,
	OPERATOR_op_convert_POLYNOM_INTEGER_2_POLYNOM_COMPLEX,
	OPERATOR_op_convert_POLYNOM_REAL_2_POLYNOM_COMPLEX,
	OPERATOR_op_convert_POLYNOM_INTEGER_2_RATIONAL_FUN_INTEGER,
	OPERATOR_op_convert_POLYNOM_INTEGER_2_RATIONAL_FUN_REAL,
	OPERATOR_op_convert_POLYNOM_INTEGER_2_RATIONAL_FUN_COMPLEX,
	OPERATOR_op_convert_POLYNOM_REAL_2_RATIONAL_FUN_REAL,
	OPERATOR_op_convert_POLYNOM_REAL_2_RATIONAL_FUN_COMPLEX,
	OPERATOR_op_convert_POLYNOM_COMPLEX_2_RATIONAL_FUN_COMPLEX,
	OPERATOR_op_convert_RATIONAL_FUN_INTEGER_2_RATIONAL_FUN_REAL,
	OPERATOR_op_convert_RATIONAL_FUN_INTEGER_2_RATIONAL_FUN_COMPLEX,
	OPERATOR_op_convert_RATIONAL_FUN_REAL_2_RATIONAL_FUN_COMPLEX,
	OPERATOR_op_convert_NATURAL_2_INTEGER_VECTOR,
	OPERATOR_op_convert_NATURAL_2_RATIONAL_VECTOR,
	OPERATOR_op_convert_NATURAL_2_REAL_VECTOR,
	OPERATOR_op_convert_NATURAL_2_COMPLEX_VECTOR,
	OPERATOR_op_convert_INTEGER_2_RATIONAL_VECTOR,
	OPERATOR_op_convert_INTEGER_2_REAL_VECTOR,
	OPERATOR_op_convert_INTEGER_2_COMPLEX_VECTOR,
	OPERATOR_op_convert_RATIONAL_2_REAL_VECTOR,
	OPERATOR_op_convert_RATIONAL_2_COMPLEX_VECTOR,
	OPERATOR_op_convert_REAL_2_COMPLEX_VECTOR,
	FUNCTION_std_abs,
	FUNCTION_std_arch,
	FUNCTION_std_arccos,
	FUNCTION_std_arccosec,
	FUNCTION_std_arcsec,
	FUNCTION_std_arcsin,
	FUNCTION_std_arctg,
	FUNCTION_std_arcth,
	FUNCTION_std_arsh,
	FUNCTION_std_ch,
	FUNCTION_std_cos,
	FUNCTION_std_cosec,
	FUNCTION_std_cosech,
	FUNCTION_std_ctg,
	FUNCTION_std_cth,
	FUNCTION_std_sec,
	FUNCTION_std_sech,
	FUNCTION_std_sin,
	FUNCTION_std_sh,
	FUNCTION_std_sqrt,
	FUNCTION_std_tg,
	FUNCTION_std_th,
	FUNCTION_std_exp,
	FUNCTION_std_int,
	FUNCTION_std_lg,
	FUNCTION_std_ln,
	FUNCTION_std_log,
	FUNCTION_std_pow,
	FUNCTION_std_root,
	FUNCTION_std_atan2,
	FUNCTION_std_fmod,
	FUNCTION_std_polar,
	FUNCTION_std_ceil,
	FUNCTION_std_floor,
	FUNCTION_std_imag,
	FUNCTION_std_pow10,
	FUNCTION_std_real,
	FUNCTION_std_random,
	FUNCTION_std_identity,
	FUNCTION_std_primes,
	FUNCTION_std_isprime,
	FUNCTION_std_gcd,
	FUNCTION_std_lcm,
	FUNCTION_std_polyroots,
	FUNCTION_std_polyder,
	FUNCTION_std_polyint,
	FUNCTION_std_polygcd,
	FUNCTION_std_polylcm,
	FUNCTION_std_lsolve,
	FUNCTION_std_ratfpart,
	FUNCTION_std_ratfder,
	FUNCTION_std_det,
	FUNCTION_std_inverse,
	FUNCTION_std_eigenvals,
	FUNCTION_std_eigenvec,
	FORMULA_box_lt,
	FORMULA_box_rt,
	FORMULA_box_lb,
	FORMULA_box_rb,
	FORMULA_box,
	FORMULA_abs,
	FORMULA_norm,
	FORMULA_floor,
	FORMULA_ceil,
	FORMULA_empty_frame,
	FORMULA_fraction,
	FORMULA_diagonal_fraction,
	FORMULA_sqrt,
	FORMULA_root,
	FORMULA_plusminus,
	FORMULA_times,
	FORMULA_circle_plus,
	FORMULA_circle_times,
	FORMULA_minus,
	FORMULA_center_dot,
	FORMULA_bullet,
	FORMULA_lang,
	FORMULA_rang,
	FORMULA_identical,
	FORMULA_almost_equal,
	FORMULA_proportional,
	FORMULA_therefore,
	FORMULA_exist,
	FORMULA_forall,
	FORMULA_in,
	FORMULA_not_in,
	FORMULA_sadd,
	FORMULA_smul,
	FORMULA_set_in,
	FORMULA_set_in_inv,
	FORMULA_set_in_eq,
	FORMULA_set_in_eq_inv,
	FORMULA_set_not_in,
	FORMULA_set_empty,
	FORMULA_sup,
	FORMULA_sub,
	FORMULA_subsup,
	FORMULA_over,
	FORMULA_under,
	FORMULA_underover,
	FORMULA_table,
	FORMULA_transpose,
	FORMULA_prime,
	CONST_polynom_x_integer,
	CONST_polynom_x_real,
	CONST_polynom_x_complex,
	CONST_true,
	CONST_false,
	CONST_TIME,
	FORMULA_exponentiale,
	FORMULA_eulergamma,
	FORMULA_imaginaryi,
	FORMULA_pi,
	FORMULA_p_g,
	FORMULA_integers,
	FORMULA_reals,
	FORMULA_rationals,
	FORMULA_naturalnumbers,
	FORMULA_complexes,
	OPERATOR_VAR_ask,
	VARIABLE_USER,
	LOCAL_VARIABLE_USER,
	VARIABLE_USER_INDIRECT,
	LOCAL_VARIABLE_USER_INDIRECT,
	FUNCTION_USER,
	FUNCTION_USER_POLYNOMIAL,
	FUNCTION_USER_RATIONAL_FUN,
	FUNCTION_STD_LAST
};

///////////////////////////////////////////////////////////////////////////////

__HMATHLNGDLL__
extern QString CDsrFunctionListName[ FUNCTION_STD_LAST ];
__HMATHLNGDLL__
QString ml_Op2String( long id );

///////////////////////////////////////////////////////////////////////////////

#define OP_IS_LINEAR_FEATURE__no		0
#define OP_IS_LINEAR_FEATURE__ignore	1
#define OP_IS_LINEAR_FEATURE__one_c		2
#define OP_IS_LINEAR_FEATURE__left_c	3
#define OP_IS_LINEAR_FEATURE__right_c	4

__HMATHLNGDLL__
extern int CDsrFunctionListID_islinear[ FUNCTION_STD_LAST ];
__HMATHLNGDLL__
int ml_getOpLinearFeature( long id );

///////////////////////////////////////////////////////////////////////////////

__HMATHLNGDLL__
CDsrDataType_Node* __ml_makeAssignNodeSet( CDsrDataTypeTable& typetable, enum CDsrDataTypes arg );
__HMATHLNGDLL__
CDsrDataType_Node* __ml_makeAssignNode_Polynomial( CDsrDataTypeTable& typetable, enum CDsrDataTypes arg );
__HMATHLNGDLL__
CDsrDataType_Node* __ml_makeAssignNode_RationalFun( CDsrDataTypeTable& typetable, enum CDsrDataTypes arg );
__HMATHLNGDLL__
CDsrDataType_Node* __ml_makeAssignNode_Vector( CDsrDataTypeTable& typetable, enum CDsrDataTypes arg );
__HMATHLNGDLL__
CDsrDataType_Node* __ml_makeAssignNode( CDsrDataTypeTable& typetable, enum CDsrDataTypes arg );
__HMATHLNGDLL__
CDsrDataType_Node* __ml_makeFunctionNode_Poly2Fun( CDsrDataTypeTable& typetable, enum CDsrDataTypes arg );
__HMATHLNGDLL__
CDsrDataType_Node* __ml_makeFunctionNode_Polynomial( CDsrDataTypeTable& typetable, enum CDsrDataTypes arg, enum CDsrDataTypes res );
__HMATHLNGDLL__
CDsrDataType_Node* __ml_makeFunctionNode_RationalFun( CDsrDataTypeTable& typetable, enum CDsrDataTypes arg, enum CDsrDataTypes res );
__HMATHLNGDLL__
CDsrDataType_Node* __ml_makeFunctionNode( CDsrDataTypeTable& typetable, CDsrDataType_Node* arg1, CDsrDataType_Node* arg2, CDsrDataType_Node* arg3, CDsrDataType_Node* res );
__HMATHLNGDLL__
CDsrDataType_Node* __ml_makeFunctionNode( CDsrDataTypeTable& typetable, enum CDsrDataTypes arg1, enum CDsrDataTypes arg2, enum CDsrDataTypes arg3, enum CDsrDataTypes res );
__HMATHLNGDLL__
CDsrDataType_Node* __ml_makeFunctionNode( CDsrDataTypeTable& typetable, CDsrDataType_Node* arg1, CDsrDataType_Node* arg2, CDsrDataType_Node* res );
__HMATHLNGDLL__
CDsrDataType_Node* __ml_makeFunctionNode( CDsrDataTypeTable& typetable, enum CDsrDataTypes arg1, enum CDsrDataTypes arg2, enum CDsrDataTypes res );
__HMATHLNGDLL__
CDsrDataType_Node* __ml_makeFunctionNode( CDsrDataTypeTable& typetable, CDsrDataType_Node* arg1, CDsrDataType_Node* res );
__HMATHLNGDLL__
CDsrDataType_Node* __ml_makeFunctionNode( CDsrDataTypeTable& typetable, enum CDsrDataTypes arg1, enum CDsrDataTypes res );
__HMATHLNGDLL__
CDsrDataType_Node* __ml_makeFunctionNode_2Vector( CDsrDataTypeTable& typetable, enum CDsrDataTypes arg );
__HMATHLNGDLL__
CDsrDataType_Node* __ml_makeFunctionNode_3Vector( CDsrDataTypeTable& typetable, enum CDsrDataTypes arg );
__HMATHLNGDLL__
CDsrDataType_Node* __ml_makeFunctionNode_2Polynomial( CDsrDataTypeTable& typetable, enum CDsrDataTypes arg );
__HMATHLNGDLL__
CDsrDataType_Node* __ml_makeFunctionNode_3Polynomial( CDsrDataTypeTable& typetable, enum CDsrDataTypes arg );
__HMATHLNGDLL__
CDsrDataType_Node* __ml_makeFunctionNode_2RationalFun( CDsrDataTypeTable& typetable, enum CDsrDataTypes arg );
__HMATHLNGDLL__
CDsrDataType_Node* __ml_makeFunctionNode_3RationalFun( CDsrDataTypeTable& typetable, enum CDsrDataTypes arg );

inline
CDsrDataType_Node* __ml_makeBinaryOpRecord( 
	CDsrDataTypeTable& typetable, enum CDsrDataTypes _type )
{
	return __ml_makeFunctionNode( typetable, _type, _type, _type );
}

inline
CDsrDataType_Node* __ml_makeUnaryOpRecord( 
	CDsrDataTypeTable& typetable, enum CDsrDataTypes _type )
{
	return __ml_makeFunctionNode( typetable, _type, _type );
}

///////////////////////////////////////////////////////////////////////////////

__HMATHLNGDLL__
void ml_InitFunctionSmbTable( CParseDsrSymbolTable *smbtable );
__HMATHLNGDLL__
void ml_InitOperatorSmbTable( CParseDsrSymbolTable *smbtable );
__HMATHLNGDLL__
void ml_InitFormulaSmbTable( CParseDsrSymbolTable *smbtable );
__HMATHLNGDLL__
void ml_InitConstSmbTable( CParseDsrSymbolTable *smbtable );
__HMATHLNGDLL__
void ml_InitConstSmbTable_Value( CParseDsrSymbolTable& smbtable, UniWordVector& var_tbl );

#define __CDSRTypeConversionOpVector_Length 64
class __CDSRTypeConversionOpVector : public MArray<enum CDsrFunctionListID>
{
public:
	__CDSRTypeConversionOpVector( long cb = __CDSRTypeConversionOpVector_Length ) :
		MArray<enum CDsrFunctionListID>( cb, 0, cb )
	{}
};

__HMATHLNGDLL__
extern enum CDsrDataTypes getRetType4Op( enum CDsrFunctionListID op, CParseDsrSymbolTable& smbtable );
__HMATHLNGDLL__
enum CDsrFunctionListID getTypeConvOp( CDsrDataType_Node *dest, CDsrDataType_Node *src );
__HMATHLNGDLL__
extern enum CDsrFunctionListID getTypeConvOp( enum CDsrDataTypes dest, enum CDsrDataTypes src );

///////////////////////////////////////////////////////////////////////////////

#endif
