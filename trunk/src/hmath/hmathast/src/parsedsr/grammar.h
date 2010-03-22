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

#if !defined( __GRAMMAR_H__ )
#define __GRAMMAR_H__

#define	LEX_NONE	                                        0 
#define	LEX_Main	                                        1 
#define	LEX_StmtList	                                    2 
#define	LEX_Stmt		                                    3 
#define	LEX_Compound	                                    4 
#define	LEX_IfStmt		                                    5 
#define	LEX_ArgList		                                    6 
#define	LEX_ObjDef		                                    7 
#define	LEX_IfHead		                                    8
#define	LEX_Else		                                    9 
#define	LEX_Expression	                                    10
#define	LEX_BoolOrExpr	                                    11
#define	LEX_AssignOP	                                    12
#define	LEX_BoolOrOP	                                    13
#define	LEX_BoolAndExpr	                                    14
#define	LEX_BoolAndOP	                                    15
#define	LEX_EqualExpr	                                    16
#define	LEX_EqualOP		                                    17
#define	LEX_RelExpr		                                    18
#define	LEX_RelOP		                                    19
#define	LEX_AddExpr		                                    20
#define	LEX_AddOP		                                    21
#define	LEX_MulExpr		                                    22
#define	LEX_MulOP		                                    23
#define	LEX_PowExpr		                                    24
#define	LEX_PowOP		                                    25
#define	LEX_UnaryExpr	                                    26
#define	LEX_UnaryOP		                                    27
#define	LEX_PostFixExpr	                                    28
#define	LEX_Primary		                                    29
#define	LEX_IdxList		                                    30
#define	LEX_ConstStringList                                 31
#define	LEX_CompoundId	                                    32
#define	LEX_TypeSpec	                                    33
#define	LEX_PrimaryId	                                    34
#define	LEX_SpecialOP	                                    35
#define	LEX_ObjHead		                                    36
#define	LEX_ObjModifier	                                    37
#define	LEX_ObjArg		                                    38
#define	LEX_ObjParamList                                    39
#define	LEX_dot                                             40
#define	LEX_ask		                                        41
#define	LEX_coma	                                        42
#define	LEX_eol		                                        43
#define	LEX_lb		                                        44
#define	LEX_rb		                                        45
#define	LEX_lab		                                        46
#define	LEX_rab		                                        47
#define	LEX_lfb		                                        48
#define	LEX_rfb		                                        49
#define	LEX_op_assign                                       50
#define	LEX_op_bool_or                                      51
#define	LEX_op_bool_and                                     52
#define	LEX_op_compare_e                                    53
#define	LEX_op_compare_ne                                   54
#define	LEX_op_setadd	                                    55
#define	LEX_op_setmul	                                    56
#define	LEX_op_rel_g	                                    57
#define	LEX_op_rel_ge	                                    58
#define	LEX_op_rel_l	                                    59
#define	LEX_op_rel_le	                                    60
#define	LEX_op_rel_in	                                    61
#define	LEX_op_rel_not_in                                   62
#define	LEX_op_rel_set_in                                   63
#define	LEX_op_rel_set_in_inv	                            64
#define	LEX_op_rel_set_in_eq	                            65
#define	LEX_op_rel_set_in_eq_inv                            66
#define	LEX_op_rel_set_not_in	                            67
#define	LEX_op_add	                                        68
#define	LEX_op_mul	                                        69
#define	LEX_op_pow	                                        70
#define	LEX_op_bool_not	                                    71
#define	LEX_op_arrow	                                    72
#define	LEX_const_int	                                    73
#define	LEX_const_big_int	                                74
#define	LEX_const_real	                                    75
#define	LEX_const_string                                    76
#define	LEX_identifier	                                    77
#define	LEX_sp_identifier                                   78
#define	LEX_kw_begin	                                    79
#define	LEX_kw_end	                                        80
#define	LEX_kw_if	                                        81
#define	LEX_kw_then	                                        82
#define	LEX_kw_else	                                        83
#define	LEX_kw_function	                                    84
#define	LEX_kw_group	                                    85
#define	LEX_kw_polynomial	                                86
#define	LEX_kw_rational_function	                        87
#define	LEX_ignore_remark	                                88
#define	LEX_ignore_blank	                                89
#define	LEX_ignore_0d0a	                                    90
#define	LEX_error_or_unknown	                            91
#define	LEX_eof		                                        92
                                                             
#endif
