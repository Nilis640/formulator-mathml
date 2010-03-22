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

#if !defined( __FORMULATOR_DICTIONARY_OP_IDDEFS_H__ )
#define __FORMULATOR_DICTIONARY_OP_IDDEFS_H__

#include "HUtils/ihkstring.h"

///////////////////////////////////////////////////////////////////////////////

#define __GLOBAL_INIT_MODE				-1
#define __GLOBAL_WORK_MODE				-2
#define __GLOBAL_USER_MODE1				-3
#define __GLOBAL_USER_MODE2				-4
#define __GLOBAL_USER_MODE3				-5
#define __GLOBAL_USER_MODE4				-6
#define __GLOBAL_USER_MODE5				-7

///////////////////////////////////////////////////////////////////////////////

#define HF_BTNID_SQRT						_T("@sqrt")
#define HF_BTNID_DEFAULT_DIVISION_SP		_T("@fraction")
#define HF_BTNID_DEFAULT_TABLE_ASK_X_ASK	_T("@table ask x ask")
#define HF_BTNID_DEFAULT_DASHED_BOX 		_T("@box_dash")
#define HF_BTNID_DEFAULT_SOLID_BOX 			_T("@box")
#define HF_BTNID_DEFAULT_BEVELLED_MFRAC 	_T("@diagonal_fraction")
#define HF_BTNID_EMPTY_FRAME				_T("@empty_frame")
#define HF_BTNID_SUP						_T("@sup")
#define HF_BTNID_SUB						_T("@sub")
#define HF_BTNID_CN							_T("@cn")
#define HF_BTNID_CI							_T("@ci")
#define HF_BTNID_CN_RATIONAL				_T("@cn_rational")
#define HF_BTNID_CN_INPUT					_T("@cn_input")
#define HF_BTNID_CI_INPUT					_T("@ci_input")
#define HF_BTNID_SELECTOR_INPUT				_T("@selector_apply_ask")
#define HF_BTNID_APPLY_INPUT				_T("@apply_ask")
#define HF_BTNID_DECLARE_INPUT				_T("@declare_ask")
#define HF_BTNID_COMPOSE_INPUT				_T("@compose_ask")
#define HF_BTNID_PIECEWISE_INPUT			_T("@piecewise1_ask")
#define HF_BTNID_PIECEWISEOTHER_INPUT		_T("@piecewise1_ask_1")
#define HF_BTNID_AND_INPUT			    	_T("@and_ask")
#define HF_BTNID_OR_INPUT					_T("@or_ask")
#define HF_BTNID_XOR_INPUT			    	_T("@xor_ask")
#define HF_BTNID_PLUS_INPUT			    	_T("@cm_plus_ask")
#define HF_BTNID_TIMES_INPUT				_T("@cm_times_ask")
#define HF_BTNID_GCD_INPUT			    	_T("@cm_gcd_ask")
#define HF_BTNID_LCM_INPUT			    	_T("@cm_lcm_ask")
#define HF_BTNID_MAX_INPUT			    	_T("@cm_max_ask")
#define HF_BTNID_MIN_INPUT			    	_T("@cm_min_ask")
#define HF_BTNID_MAX_BVAR_INPUT		    	_T("@cm_max_bvar_ask")
#define HF_BTNID_MIN_BVAR_INPUT		    	_T("@cm_min_bvar_ask")
#define HF_BTNID_FORALL_BVAR_INPUT			_T("@cm_forall_bvar_ask")
#define HF_BTNID_FORALL_BVARCOND_INPUT		_T("@cm_forall_bvar_cond_ask")
#define HF_BTNID_EXISTS_BVAR_INPUT			_T("@cm_exists_bvar_ask")
#define HF_BTNID_EXISTS_BVARCOND_INPUT		_T("@cm_exists_bvar_cond_ask")
#define HF_BTNID_EQ_INPUT					_T("@cm_eq_ask")
#define HF_BTNID_EQUIVALENT_INPUT			_T("@cm_equivalent_ask")
#define HF_BTNID_GT_INPUT					_T("@cm_gt_ask")
#define HF_BTNID_LT_INPUT					_T("@cm_lt_ask")
#define HF_BTNID_GEQ_INPUT					_T("@cm_geq_ask")
#define HF_BTNID_LEQ_INPUT					_T("@cm_leq_ask")
#define HF_BTNID_PD1_INPUT					_T("@cm_partialdiff1_ask")
#define HF_BTNID_PD2_INPUT					_T("@cm_partialdiff2_ask")
#define HF_BTNID_PD3_INPUT					_T("@cm_partialdiff3_ask")
#define HF_BTNID_PDLIST_INPUT				_T("@cm_partialdiff4_list_ask")
#define HF_BTNID_SET_INPUT					_T("@cm_set_ask")
#define HF_BTNID_SET_BVAR_INPUT				_T("@cm_set_bvar_ask")
#define HF_BTNID_LIST_INPUT					_T("@cm_list_ask")
#define HF_BTNID_LIST_BVAR_INPUT			_T("@cm_list_bvar_ask")
#define HF_BTNID_UNION_INPUT				_T("@cm_union_ask")
#define HF_BTNID_INTERSECT_INPUT			_T("@cm_intersect_ask")
#define HF_BTNID_CARTESIAN_INPUT			_T("@cm_cartesianproduct_ask")
#define HF_BTNID_MEAN_INPUT					_T("@cm_mean_ask")
#define HF_BTNID_SDEV_INPUT					_T("@cm_sdev_ask")
#define HF_BTNID_VARIANCE_INPUT				_T("@cm_variance_ask")
#define HF_BTNID_MEDIAN_INPUT				_T("@cm_median_ask")
#define HF_BTNID_MODE_INPUT					_T("@cm_mode_ask")
#define HF_BTNID_MATHML_CODE				_T("@cm_mathml_code")
#define HF_BTNID_PLUS_MINUS					_T("@cm_plus_minus2")
#define HF_BTNID_EQUAL						"@assign_eq"

#define HF_BTNID_NOP_VALUE					QChar(0x25AE)

#define HF_BTNID_COLONEQ_VALUE				MML_OPERATOR_BODY_ASSIGN
#define HF_BTNID_COLONEQ_ENTITY				MML_OPERATOR_BODY_ASSIGN_ENTITY
#define HF_BTNID_ASSIGN_ID					"@assign"
#define HF_BTNID_CMASSIGN_ID				"@cm_assign"
#define HF_BTNID_CMASSIGN2_ID				"@cm_assign2"

#define HF_BTNID_QUALIFIER1_INPUT			_T("@cm_qualifier_q1_ask")
#define HF_BTNID_QUALIFIER2_INPUT			_T("@cm_qualifier_q2_ask")
#define HF_BTNID_QUALIFIER3_INPUT			_T("@cm_qualifier_q3_ask")
#define HF_BTNID_QUALIFIER4_INPUT			_T("@cm_qualifier_q4_ask")

#define HF_BTNID_DEFAULT_BRACKET_LR 		_T("()")
#define HF_BTNID_DEFAULT_BRACKET_LRB 		_T("[]")
#define HF_BTNID_DEFAULT_BRACKET_LRF 		_T("{}")
#define HF_BTNID_DEFAULT_BRACKET_LRA 		_T("<>")
#define HF_BTNID_DEFAULT_BRACKET_ABS 		_T("@abs")
#define HF_BTNID_DEFAULT_BRACKET_NORM 		_T("@norm")
#define HF_BTNID_DEFAULT_BRACKET_FLOOR		_T("@floor")
#define HF_BTNID_DEFAULT_BRACKET_CEIL		_T("@ceil")
#define HF_BTNID_DEFAULT_BRACKET_L 			_T("(")
#define HF_BTNID_DEFAULT_BRACKET_R 			_T(")")
#define HF_BTNID_DEFAULT_BRACKET_LB 		_T("[")
#define HF_BTNID_DEFAULT_BRACKET_RB 		_T("]")
#define HF_BTNID_DEFAULT_BRACKET_LF 		_T("{")
#define HF_BTNID_DEFAULT_BRACKET_RF 		_T("}")
#define HF_BTNID_DEFAULT_BRACKET_LA 		_T("<")
#define HF_BTNID_DEFAULT_BRACKET_RA 		_T(">")
#define HF_BTNID_DEFAULT_BRACKET_ABS_L		_T("| left")
#define HF_BTNID_DEFAULT_BRACKET_ABS_R		_T("| right")
#define HF_BTNID_DEFAULT_BRACKET_NORM_L		_T("|| left")
#define HF_BTNID_DEFAULT_BRACKET_NORM_R		_T("|| right")

///////////////////////////////////////////////////////////////////////////////

#endif
