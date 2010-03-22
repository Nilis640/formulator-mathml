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

#include "HUtils/ihkstring.h"
#include "HUtils/iustring.h"

#include "cmmlCalcTag.h"

///////////////////////////////////////////////////////////////////////////////

int aMapMMLTag2DsrOp[ TAG2MML_LAST ] =
{
	FUNCTION_STD_NONE,						//FBL_TAG_math,
	FUNCTION_STD_NONE,						//FBL_TAG_mrow,
	FUNCTION_STD_NONE,						//FBL_TAG_malignmark,
	FUNCTION_STD_NONE,						//FBL_TAG_maligngroup,
	FUNCTION_STD_NONE,						//FBL_TAG_mlabeledtr,
	FUNCTION_STD_NONE,						//FBL_TAG_mn,
	FUNCTION_STD_NONE,						//FBL_TAG_mo,
	FUNCTION_STD_NONE,						//FBL_TAG_mi,
	FUNCTION_STD_NONE,						//FBL_TAG_mtext,
	FUNCTION_STD_NONE,						//FBL_TAG_ms,
	FUNCTION_STD_NONE,						//FBL_TAG_mspace,
	FUNCTION_STD_NONE,						//FBL_TAG_mtable,
	FUNCTION_STD_NONE,						//FBL_TAG_mtr,
	FUNCTION_STD_NONE,						//FBL_TAG_mtd,
	FUNCTION_STD_NONE,						//FBL_TAG_mfenced,
	FUNCTION_STD_NONE,						//FBL_TAG_mfrac,
	FUNCTION_STD_NONE,						//FBL_TAG_mstyle,
	FUNCTION_STD_NONE,						//FBL_TAG_mglyph,
	FUNCTION_STD_NONE,						//FBL_TAG_msqrt,
	FUNCTION_STD_NONE,						//FBL_TAG_merror,
	FUNCTION_STD_NONE,						//FBL_TAG_menclose,
	FUNCTION_STD_NONE,						//FBL_TAG_mpadded,
	FUNCTION_STD_NONE,						//FBL_TAG_mphantom,
	FUNCTION_STD_NONE,						//FBL_TAG_maction,
	FUNCTION_STD_NONE,						//FBL_TAG_munder,
	FUNCTION_STD_NONE,						//FBL_TAG_mover,
	FUNCTION_STD_NONE,						//FBL_TAG_munderover,
	FUNCTION_STD_NONE,						//FBL_TAG_msup,
	FUNCTION_STD_NONE,						//FBL_TAG_msub,
	FUNCTION_STD_NONE,						//FBL_TAG_mmultiscripts,
	_CONSTRUCT_FUNCTION_CN,					//FBL_TAG_cn,
	_CONSTRUCT_FUNCTION_CI,					//FBL_TAG_ci,
	_CONSTRUCT_FUNCTION_CSYMBOL,			//FBL_TAG_csymbol,
	_CONSTRUCT_FUNCTION_APPLY,				//FBL_TAG_apply,
	_CONSTRUCT_FUNCTION_APPLY,				//FBL_TAG_reln,
	_CONSTRUCT_FUNCTION_APPLY,				//FBL_TAG_fn,
	FUNCTION_STD_NONE,						//FBL_TAG_interval,
	_PROCESS_FUNCTION_INVERSE,				//FBL_TAG_inverse,
	FUNCTION_STD_NONE,						//FBL_TAG_sep,
	FUNCTION_STD_NONE,						//FBL_TAG_condition,
	FUNCTION_STD_NONE,						//FBL_TAG_declare,
	_CONSTRUCT_FUNCTION_LAMBDA,				//FBL_TAG_lambda,
	_CONSTRUCT_FUNCTION_COMPOSE,			//FBL_TAG_compose,
	FUNCTION_STD_NONE,						//FBL_TAG_ident,
	FUNCTION_STD_NONE,						//FBL_TAG_domain,
	FUNCTION_STD_NONE,						//FBL_TAG_codomain,
	FUNCTION_STD_NONE,						//FBL_TAG_image,
	FUNCTION_STD_NONE,						//FBL_TAG_domainofapplication,
	FUNCTION_STD_NONE,						//FBL_TAG_piecewise,
	FUNCTION_STD_NONE,						//FBL_TAG_piece,
	FUNCTION_STD_NONE,						//FBL_TAG_otherwise,
	_TODO_FUNCTION_QUOTIENT,				//FBL_TAG_quotient,
	FUNCTION_std_exp,						//FBL_TAG_exp,
	_TODO_FUNCTION_FACTORIAL,				//FBL_TAG_factorial,
	OPERATOR_op_div,						//FBL_TAG_divide,
	_TODO_FUNCTION_MAX,						//FBL_TAG_max,
	_TODO_FUNCTION_MIN,						//FBL_TAG_min,
	OPERATOR_op_sub,						//FBL_TAG_minus,
	OPERATOR_op_add,						//FBL_TAG_plus,
	OPERATOR_op_pow,						//FBL_TAG_power,
	OPERATOR_op_mod,						//FBL_TAG_rem,
	OPERATOR_op_mul,						//FBL_TAG_times,
	FUNCTION_std_root,						//FBL_TAG_root,
	FUNCTION_std_gcd,						//FBL_TAG_gcd,
	OPERATOR_op_bool_and,					//FBL_TAG_and,
	OPERATOR_op_bool_or,					//FBL_TAG_or,
	_TODO_FUNCTION_XOR,						//FBL_TAG_xor,
	OPERATOR_op_bool_not,					//FBL_TAG_not,
	_NOTYET_SYMBOLIC,						//FBL_TAG_implies,
	_NOTYET_SYMBOLIC,						//FBL_TAG_forall,
	_NOTYET_SYMBOLIC,						//FBL_TAG_exists,
	FUNCTION_std_abs,						//FBL_TAG_abs,
	_TODO_FUNCTION_COMPLEX_CONJUGATE,		//FBL_TAG_conjugate,
	_TODO_FUNCTION_COMPLEX_ARG,				//FBL_TAG_arg,
	FUNCTION_std_real,						//FBL_TAG_real,
	FUNCTION_std_imag,						//FBL_TAG_imaginary,
	FUNCTION_std_lcm,						//FBL_TAG_lcm,
	FUNCTION_std_floor,						//FBL_TAG_floor,
	FUNCTION_std_ceil,						//FBL_TAG_ceiling,
	OPERATOR_op_compare_e,					//FBL_TAG_eq,	// former OPERATOR_op_assign
	OPERATOR_op_compare_ne,					//FBL_TAG_neq,
	OPERATOR_op_rel_g,						//FBL_TAG_gt,
	OPERATOR_op_rel_l,						//FBL_TAG_lt,
	OPERATOR_op_rel_ge,						//FBL_TAG_geq,
	OPERATOR_op_rel_le,						//FBL_TAG_leq,
	OPERATOR_op_compare_e,					//FBL_TAG_equivalent,
	FUNCTION_STD_NONE,						//FBL_TAG_approx,
	FUNCTION_STD_NONE,						//FBL_TAG_factorof,
	_TODO_FUNCTION_INTEGRAL,				//FBL_TAG_int,
	_TODO_FUNCTION_DIFF,					//FBL_TAG_diff,
	_TODO_FUNCTION_PARTDIFF,				//FBL_TAG_partialdiff,
	FUNCTION_STD_NONE,						//FBL_TAG_lowlimit,
	FUNCTION_STD_NONE,						//FBL_TAG_uplimit,
	FUNCTION_STD_NONE,						//FBL_TAG_bvar,
	FUNCTION_STD_NONE,						//FBL_TAG_degree,
	FUNCTION_STD_NONE,						//FBL_TAG_logbase,
	_TODO_FUNCTION_DIVERGENCE,				//FBL_TAG_divergence,
	_TODO_FUNCTION_GRAD,					//FBL_TAG_grad,
	_TODO_FUNCTION_CURL,					//FBL_TAG_curl,
	_TODO_FUNCTION_LAPLASIAN,				//FBL_TAG_laplacian,
	FUNCTION_STD_NONE,						//FBL_TAG_set,
	FUNCTION_STD_NONE,						//FBL_TAG_list,
	OPERATOR_op_setadd,						//FBL_TAG_union,
	OPERATOR_op_setmul,						//FBL_TAG_intersect,
	OPERATOR_op_rel_in,						//FBL_TAG_in,
	OPERATOR_op_rel_not_in,					//FBL_TAG_notin,
	OPERATOR_op_rel_set_in_eq,				//FBL_TAG_subset,
	OPERATOR_op_rel_set_in,					//FBL_TAG_prsubset,
	_TODO_FUNCTION_set_not_in_eq,			//FBL_TAG_notsubset,
	OPERATOR_op_rel_set_not_in,				//FBL_TAG_notprsubset,
	_TODO_FUNCTION_setdiff,					//FBL_TAG_setdiff,
	_TODO_FUNCTION_set_card,				//FBL_TAG_card,
	_TODO_FUNCTION_set_cartesianproduct,	//FBL_TAG_cartesianproduct,
	_TODO_FUNCTION_SUM,						//FBL_TAG_sum,
	_TODO_FUNCTION_PRODUCT,					//FBL_TAG_product,
	_NOTYET_SYMBOLIC,						//FBL_TAG_limit,
	FUNCTION_STD_NONE,						//FBL_TAG_tendsto,
	FUNCTION_std_ln,						//FBL_TAG_ln,
	FUNCTION_std_log,						//FBL_TAG_log,
	FUNCTION_std_sin,						//FBL_TAG_sin,
	FUNCTION_std_cos,						//FBL_TAG_cos,
	FUNCTION_std_tg,						//FBL_TAG_tan,
	FUNCTION_std_sec,						//FBL_TAG_sec,
	FUNCTION_std_cosec,						//FBL_TAG_csc,
	FUNCTION_std_ctg,						//FBL_TAG_cot,
	FUNCTION_std_sh,						//FBL_TAG_sinh,
	FUNCTION_std_ch,						//FBL_TAG_cosh,
	FUNCTION_std_th,						//FBL_TAG_tanh,
	FUNCTION_std_sech,						//FBL_TAG_sech,
	FUNCTION_std_cosech,					//FBL_TAG_csch,
	FUNCTION_std_cth,						//FBL_TAG_coth,
	FUNCTION_std_arcsin,					//FBL_TAG_arcsin,
	FUNCTION_std_arccos,					//FBL_TAG_arccos,
	FUNCTION_std_arctg,						//FBL_TAG_arctan,
	FUNCTION_std_arch,						//FBL_TAG_arccosh,
	_TODO_FUNCTION_arccot,					//FBL_TAG_arccot,
	_TODO_FUNCTION_arccoth,					//FBL_TAG_arccoth,
	FUNCTION_std_arccosec,					//FBL_TAG_arccsc,
	_TODO_FUNCTION_arccosech,				//FBL_TAG_arccsch,
	FUNCTION_std_arcsec,					//FBL_TAG_arcsec,
	_TODO_FUNCTION_arcsech,					//FBL_TAG_arcsech,
	FUNCTION_std_arsh,						//FBL_TAG_arcsinh,
	FUNCTION_std_arcth,						//FBL_TAG_arctanh,
	_NOTYET_STAT,							//FBL_TAG_mean,
	_NOTYET_STAT,							//FBL_TAG_sdev,
	_NOTYET_STAT,							//FBL_TAG_variance,
	_NOTYET_STAT,							//FBL_TAG_median,
	_NOTYET_STAT,							//FBL_TAG_mode,
	_NOTYET_STAT,							//FBL_TAG_moment,
	_NOTYET_STAT,							//FBL_TAG_momentabout,
	FUNCTION_STD_NONE,						//FBL_TAG_vector,
	FUNCTION_STD_NONE,						//FBL_TAG_matrix,
	FUNCTION_STD_NONE,						//FBL_TAG_matrixrow,
	FUNCTION_std_det,						//FBL_TAG_determinant,
	FORMULA_transpose,						//FBL_TAG_transpose,
	OPERATOR_square_brackets,				//FBL_TAG_selector,
	_TODO_FUNCTION_LA_vectorproduct,		//FBL_TAG_vectorproduct,
	_TODO_FUNCTION_LA_scalarproduct,		//FBL_TAG_scalarproduct,
	_TODO_FUNCTION_LA_outerproduct,			//FBL_TAG_outerproduct,
	FUNCTION_STD_NONE,						//FBL_TAG_annotation,
	FUNCTION_STD_NONE,						//FBL_TAG_semantics,
	FUNCTION_STD_NONE,						//FBL_TAG_annotation_xml,
	FORMULA_integers,						//FBL_TAG_integers,
	FORMULA_reals,							//FBL_TAG_reals,
	FORMULA_rationals,						//FBL_TAG_rationals,
	FORMULA_naturalnumbers,					//FBL_TAG_naturalnumbers,
	FORMULA_complexes,						//FBL_TAG_complexes,
	FUNCTION_std_primes,					//FBL_TAG_primes,
	FORMULA_exponentiale,					//FBL_TAG_exponentiale,
	FORMULA_imaginaryi,						//FBL_TAG_imaginaryi,
	FUNCTION_STD_NONE,						//FBL_TAG_notanumber,
	CONST_true,								//FBL_TAG_true,
	CONST_false,							//FBL_TAG_false,
	FORMULA_set_empty,						//FBL_TAG_emptyset,
	FORMULA_pi,								//FBL_TAG_pi,
	FORMULA_eulergamma,						//FBL_TAG_eulergamma,
	FUNCTION_STD_NONE,						//FBL_TAG_infinity,
	FUNCTION_STD_NONE						//FBL_TAG_nop
};

///////////////////////////////////////////////////////////////////////////////

enum CDsrDataTypes mapMMLValueTypeAttr2Dsr( QString typeName )
{
	if( !xml_strcmp( typeName, _T("integer") ) )
		return DSRDATA_TYPE_INTEGER;
	else if( !xml_strcmp( typeName, _T("real") ) )
		return DSRDATA_TYPE_REAL;
	else if( !xml_strcmp( typeName, _T("rational") ) )
		return DSRDATA_TYPE_RATIONAL;
	else if( !xml_strcmp( typeName, _T("complex-cartesian") ) )
		return DSRDATA_TYPE_COMPLEX;
	else if( !xml_strcmp( typeName, _T("complex-polar") ) )
		return DSRDATA_TYPE_COMPLEX;
	else if( !xml_strcmp( typeName, _T("complex") ) )
		return DSRDATA_TYPE_COMPLEX;
	else if( !xml_strcmp( typeName, _T("constant") ) )
		return DSRDATA_TYPE_REAL;
	else if( !xml_strcmp( typeName, _T("function") ) )
		return DSRDATA_TYPE_FUNCTION;
	else if( !xml_strcmp( typeName, _T("interval") ) )
		return DSRDATA_TYPE_RANGE;
	else if( !xml_strcmp( typeName, _T("list") ) )
		return DSRDATA_TYPE_ABS_SET;
	else if( !xml_strcmp( typeName, _T("matrix") ) )
		return DSRDATA_TYPE_VECTOR;
	else if( !xml_strcmp( typeName, _T("set") ) )
		return DSRDATA_TYPE_ABS_SET;
	else if( !xml_strcmp( typeName, _T("vector") ) )
		return DSRDATA_TYPE_VECTOR;

	return DSRDATA_TYPE_REAL;
}

///////////////////////////////////////////////////////////////////////////////
