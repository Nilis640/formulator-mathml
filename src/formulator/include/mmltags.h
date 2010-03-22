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

#if !defined( __FORMULATOR_INCLUDE_MMLTAGS_H__ )
#define __FORMULATOR_INCLUDE_MMLTAGS_H__

#include <QString>
#include "HUtils/ihkmap.h"

///////////////////////////////////////////////////////////////////////////////

#define FBL_ATTR_formulator_readonly	"formulator:readonly"
#define FBL_ATTR_autograph_readonly		"autograph:readonly"

#define FBL_ATTR_formulator_immovable	"formulator:immovable"
#define FBL_ATTR_autograph_immovable	"autograph:immovable"

///////////////////////////////////////////////////////////////////////////////

#define FBL_ATTR_class					"class"
#define FBL_ATTR_style					"style"
#define FBL_ATTR_id						"id"
#define FBL_ATTR_xref					"xref"
#define FBL_ATTR_xlink_href				"xlink:href"

///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////

#define FBL_TAG_mroot					"mroot"
#define FBL_TAG_mpadded					"mpadded"

#define FBL_TAG_math					"math"
#define FBL_TAG_mrow					"mrow"
#define FBL_TAG_mn						"mn"
#define FBL_TAG_mo						"mo"
#define FBL_TAG_mi						"mi"
#define FBL_TAG_mtext					"mtext"
#define FBL_TAG_ms						"ms"
#define FBL_TAG_mtable					"mtable"
#define FBL_TAG_mtr						"mtr"
#define FBL_TAG_mlabeledtr				"mlabeledtr"
#define FBL_TAG_mtd						"mtd"
#define FBL_TAG_mfenced					"mfenced"
#define FBL_TAG_mfrac					"mfrac"
#define FBL_TAG_mstyle					"mstyle"
#define FBL_TAG_mglyph					"mglyph"
#define FBL_TAG_msqrt					"msqrt"
#define FBL_TAG_merror					"merror"
#define FBL_TAG_menclose				"menclose"
#define FBL_TAG_mpadded					"mpadded"
#define FBL_TAG_mphantom				"mphantom"
#define FBL_TAG_mspace					"mspace"
#define FBL_TAG_maction					"maction"
#define FBL_TAG_munder					"munder"
#define FBL_TAG_mover					"mover"
#define FBL_TAG_munderover				"munderover"
#define FBL_TAG_msup					"msup"
#define FBL_TAG_msub					"msub"
#define FBL_TAG_msubsup					"msubsup"
#define FBL_TAG_mmultiscripts			"mmultiscripts"
#define FBL_TAG_mprescripts				"mprescripts"
#define FBL_TAG_malignmark				"malignmark"
#define FBL_TAG_maligngroup				"maligngroup"
#define FBL_TAG_none					"none"

///////////////////////////////////////////////////////////////////////////////

#define FBL_ATTR_scriptlevel			"scriptlevel"
#define FBL_ATTR_displaystyle			"displaystyle"
#define FBL_ATTR_scriptsizemultiplier	"scriptsizemultiplier"
#define FBL_ATTR_scriptminsize			"scriptminsize"
#define FBL_ATTR_veryverythinmathspace	"veryverythinmathspace"
#define FBL_ATTR_verythinmathspace		"verythinmathspace"
#define FBL_ATTR_thinmathspace			"thinmathspace"
#define FBL_ATTR_mediummathspace		"mediummathspace"
#define FBL_ATTR_thickmathspace	 		"thickmathspace"
#define FBL_ATTR_verythickmathspace 	"verythickmathspace"
#define FBL_ATTR_veryverythickmathspace	"veryverythickmathspace"
#define FBL_ATTR_background				"background"

#define FBL_ATTR_edge	 				"edge"

#define FBL_ATTR_overflow				"overflow"
#define FBL_ATTR_altimg					"altimg"
#define FBL_ATTR_alttext				"alttext"

#define FBL_ATTR_macros					"macros"
#define FBL_ATTR_mode					"mode"
#define FBL_ATTR_modeW					L"mode"
#define FBL_ATTR_display				"display"
#define FBL_ATTR_displayW				L"display"
#define FBL_ATTR_open					"open"
#define FBL_ATTR_close					"close"
#define FBL_ATTR_columnalign			"columnalign"
#define FBL_ATTR_frame					"frame"

#define FBL_ATTR_notation				"notation"
#define FBL_ATTR_subscriptshift			"subscriptshift"
#define FBL_ATTR_superscriptshift		"superscriptshift"
#define FBL_ATTR_accentunder			"accentunder"

#define FBL_ATTR_align					"align"
#define FBL_ATTR_rowalign				"rowalign"
#define FBL_ATTR_columnalign			"columnalign"
#define FBL_ATTR_groupalign	 			"groupalign"
#define FBL_ATTR_alignmentscope 		"alignmentscope"
#define FBL_ATTR_columnwidth			"columnwidth"
#define FBL_ATTR_rowspacing	 			"rowspacing"
#define FBL_ATTR_columnspacing			"columnspacing"
#define FBL_ATTR_rowlines				"rowlines"
#define FBL_ATTR_columnlines			"columnlines"
#define FBL_ATTR_frame					"frame"
#define FBL_ATTR_framespacing			"framespacing"
#define FBL_ATTR_equalrows				"equalrows"
#define FBL_ATTR_equalcolumns			"equalcolumns"
#define FBL_ATTR_side					"side"
#define FBL_ATTR_minlabelspacing		"minlabelspacing"

#define FBL_ATTR_actiontype				"actiontype"
#define FBL_ATTR_selection				"selection"

#define FBL_ATTR_mathvariant			"mathvariant"
#define FBL_ATTR_mathcolor				"mathcolor"
#define FBL_ATTR_color					"color"
#define FBL_ATTR_mathsize				"mathsize"
#define FBL_ATTR_fontsize				"fontsize"
#define FBL_ATTR_fontweight				"fontweight"
#define FBL_ATTR_fontstyle				"fontstyle"
#define FBL_ATTR_fontfamily				"fontfamily"
#define FBL_ATTR_mathbackground			"mathbackground"
#define FBL_ATTR_form					"form"
#define FBL_ATTR_fence					"fence"
#define FBL_ATTR_separator				"separator"
#define FBL_ATTR_separators				"separators"
#define FBL_ATTR_lspace					"lspace"
#define FBL_ATTR_rspace					"rspace"
#define FBL_ATTR_stretchy				"stretchy"
#define FBL_ATTR_symmetric				"symmetric"
#define FBL_ATTR_maxsize				"maxsize"
#define FBL_ATTR_minsize				"minsize"
#define FBL_ATTR_largeop				"largeop"
#define FBL_ATTR_movablelimits			"movablelimits"
#define FBL_ATTR_accent					"accent"
#define FBL_ATTR_lquote					"lquote"
#define FBL_ATTR_rquote					"rquote"
#define FBL_ATTR_width					"width"
#define FBL_ATTR_height					"height"
#define FBL_ATTR_depth					"depth"
#define FBL_ATTR_linebreak				"linebreak"
#define FBL_ATTR_linethickness			"linethickness"
#define FBL_ATTR_denomalign				"denomalign"
#define FBL_ATTR_numalign				"numalign"
#define FBL_ATTR_rowalign				"rowalign"
#define FBL_ATTR_columnalign			"columnalign"
#define FBL_ATTR_fontfamily				"fontfamily"
#define FBL_ATTR_index					"index"
#define FBL_ATTR_alt					"alt"
#define FBL_ATTR_occurrence				"occurrence"
#define FBL_ATTR_nargs					"nargs"
#define FBL_ATTR_bevelled				"bevelled"
#define FBL_ATTR_scope					"scope"
#define FBL_ATTR_order					"order"

#define FBL_ATTR_image_path				"image-path"
#define FBL_ATTR_rows					_T("rows")
#define FBL_ATTR_columns				_T("columns")

///////////////////////////////////////////////////////////////////////////////

#define FBL_VAL_prefix					"prefix"
#define FBL_VAL_postfix					"postfix"
#define FBL_VAL_infix					"infix"
#define FBL_VAL_veryverythinmathspace	"veryverythinmathspace"
#define FBL_VAL_verythinmathspace		"verythinmathspace"
#define FBL_VAL_thinmathspace			"thinmathspace"
#define FBL_VAL_mediummathspace			"mediummathspace"
#define FBL_VAL_thickmathspace			"thickmathspace"
#define FBL_VAL_verythickmathspace		"verythickmathspace"
#define FBL_VAL_veryverythickmathspace	"veryverythickmathspace"

//#define FBL_VAL_left					"left"
#define FBL_VAL_block					"block"
#define FBL_VAL_blockW					L"block"
#define FBL_VAL_inline					"inline"
#define FBL_VAL_inlineW					L"inline"
#define FBL_VAL_normal					"normal"
#define FBL_VAL_bold					"bold"
#define FBL_VAL_italic					"italic"
#define FBL_VAL_bold_italic				"bold-italic"
#define FBL_VAL_bold_script				"bold-script"
#define FBL_VAL_script					"script"
#define FBL_VAL_monospace				"monospace"
#define FBL_VAL_sans_serif				"sans-serif"
#define FBL_VAL_bold_sans_serif			"bold-sans-serif"
#define FBL_VAL_sans_serif_italic		"sans-serif-italic"
#define FBL_VAL_sans_serif_bold_italic	"sans-serif-bold-italic"
#define FBL_VAL_double_struck			"double-struck"
#define FBL_VAL_fraktur					"fraktur"
#define FBL_VAL_bold_fraktur			"bold-fraktur"
#define FBL_VAL_solid					"solid"
#define FBL_VAL_dashed					"dashed"
#define FBL_VAL_true					"true"
#define FBL_VAL_false					"false"

#define FBL_VAL_longdiv					"longdiv"
#define FBL_VAL_actuarial				"actuarial"
#define FBL_VAL_radical					"radical"
#define FBL_VAL_box						"box"
#define FBL_VAL_roundedbox				"roundedbox"
#define FBL_VAL_circle					"circle"
#define FBL_VAL_left					"left"
#define FBL_VAL_right					"right"
#define FBL_VAL_top						"top"
#define FBL_VAL_bottom					"bottom"
#define FBL_VAL_updiagonalstrike		"updiagonalstrike"
#define FBL_VAL_downdiagonalstrike		"downdiagonalstrike"
#define FBL_VAL_verticalstrike			"verticalstrike"
#define FBL_VAL_horizontalstrike		"horizontalstrike"
#define FBL_VAL_top_left				"top-left"
#define FBL_VAL_top_right				"top-right"
#define FBL_VAL_bottom_left				"bottom-left"
#define FBL_VAL_bottom_right			"bottom-right"
#define FBL_VAL_strike_through			"strike-through"
#define FBL_VAL_joint_status			"joint-status"

#define FBL_VAL_plus1					"+1"

#define FBL_VAL_small					"small"
#define FBL_VAL_big						"big"

#define FBL_ATTR_STRING_root				"display = 'block'"
#define FBL_ATTR_STRING_root_xmlns			"display = 'block' xmlns='http://www.w3.org/1998/Math/MathML'"
#define FBL_ATTR_STRING_root4html_block		"display = 'block' xmlns='http://www.w3.org/1998/Math/MathML'"
#define FBL_ATTR_STRING_root4html_inline	"display = 'inline' xmlns='http://www.w3.org/1998/Math/MathML'"

#define FBL_VAL_DEFAULT_mfenced_open		"("
#define FBL_VAL_DEFAULT_mfenced_close		")"
#define FBL_VAL_DEFAULT_mfenced_separator	","

///////////////////////////////////////////////////////////////////////////////

// token elements
#define FBL_TAG_cn						"cn"
#define FBL_TAG_ci						"ci"
#define FBL_TAG_csymbol					"csymbol"
// basic content elements 
#define FBL_TAG_apply					"apply"
#define FBL_TAG_reln					"reln"
#define FBL_TAG_fn						"fn"
#define FBL_TAG_interval				"interval"
#define FBL_TAG_inverse					"inverse"
#define FBL_TAG_sep						"sep"
#define FBL_TAG_condition				"condition"
#define FBL_TAG_declare					"declare"
#define FBL_TAG_lambda					"lambda"
#define FBL_TAG_compose					"compose"
#define FBL_TAG_ident					"ident"
#define FBL_TAG_domain					"domain"
#define FBL_TAG_codomain				"codomain"
#define FBL_TAG_image					"image"
#define FBL_TAG_domainofapplication		"domainofapplication"
#define FBL_TAG_piecewise				"piecewise"
#define FBL_TAG_piece					"piece"
#define FBL_TAG_otherwise				"otherwise"
// arithmetic, algebra and logic 
#define FBL_TAG_quotient				"quotient"
#define FBL_TAG_exp						"exp"
#define FBL_TAG_factorial				"factorial"
#define FBL_TAG_divide					"divide"
#define FBL_TAG_max						"max"
#define FBL_TAG_min						"min"
#define FBL_TAG_minus					"minus"
#define FBL_TAG_plus					"plus"
#define FBL_TAG_power					"power"
#define FBL_TAG_rem						"rem"
#define FBL_TAG_times					"times"
#define FBL_TAG_root					"root"
#define FBL_TAG_gcd						"gcd"
#define FBL_TAG_and						"and"
#define FBL_TAG_or						"or"
#define FBL_TAG_xor						"xor"
#define FBL_TAG_not						"not"
#define FBL_TAG_implies					"implies"
#define FBL_TAG_forall					"forall"
#define FBL_TAG_exists					"exists"
#define FBL_TAG_abs						"abs"
#define FBL_TAG_conjugate				"conjugate"
#define FBL_TAG_arg						"arg"
#define FBL_TAG_real					"real"
#define FBL_TAG_imaginary				"imaginary"
#define FBL_TAG_lcm						"lcm"
#define FBL_TAG_floor					"floor"
#define FBL_TAG_ceiling					"ceiling"
#define FBL_TAG_eq						"eq"
#define FBL_TAG_neq						"neq"
#define FBL_TAG_gt						"gt"
#define FBL_TAG_lt						"lt"
#define FBL_TAG_geq						"geq"
#define FBL_TAG_leq						"leq"
#define FBL_TAG_equivalent				"equivalent"
#define FBL_TAG_approx					"approx"
#define FBL_TAG_factorof				"factorof"
// calculus and vector calculus 
#define FBL_TAG_int						"int"
#define FBL_TAG_diff					"diff"
#define FBL_TAG_partialdiff				"partialdiff"
#define FBL_TAG_lowlimit				"lowlimit"
#define FBL_TAG_uplimit					"uplimit"
#define FBL_TAG_bvar					"bvar"
#define FBL_TAG_degree					"degree"
#define FBL_TAG_logbase					"logbase"
#define FBL_TAG_divergence				"divergence"
#define FBL_TAG_grad					"grad"
#define FBL_TAG_curl					"curl"
#define FBL_TAG_laplacian				"laplacian"
// theory of sets 
#define FBL_TAG_set						"set"
#define FBL_TAG_list					"list"
#define FBL_TAG_union					"union"
#define FBL_TAG_intersect				"intersect"
#define FBL_TAG_in						"in"
#define FBL_TAG_notin					"notin"
#define FBL_TAG_subset					"subset"
#define FBL_TAG_prsubset				"prsubset"
#define FBL_TAG_notsubset				"notsubset"
#define FBL_TAG_notprsubset				"notprsubset"
#define FBL_TAG_setdiff					"setdiff"
#define FBL_TAG_card					"card"
#define FBL_TAG_cartesianproduct		"cartesianproduct"
// sequences and series 
#define FBL_TAG_sum						"sum"
#define FBL_TAG_product					"product"
#define FBL_TAG_limit					"limit"
#define FBL_TAG_tendsto					"tendsto"
// elementary classical functions 
#define FBL_TAG_exp						"exp"
#define FBL_TAG_ln						"ln"
#define FBL_TAG_log						"log"
#define FBL_TAG_sin	 					"sin"		
#define FBL_TAG_cos	 					"cos"		
#define FBL_TAG_tan	 					"tan"		
#define FBL_TAG_sec	 					"sec"		
#define FBL_TAG_csc	 					"csc"		
#define FBL_TAG_cot	 					"cot"		
#define FBL_TAG_sinh					"sinh"		
#define FBL_TAG_cosh					"cosh"		
#define FBL_TAG_tanh					"tanh"		
#define FBL_TAG_sech					"sech"		
#define FBL_TAG_csch					"csch"		
#define FBL_TAG_coth					"coth"		
#define FBL_TAG_arcsin					"arcsin"	 
#define FBL_TAG_arccos					"arccos"	 
#define FBL_TAG_arctan					"arctan"	 
#define FBL_TAG_arccosh 				"arccosh"	
#define FBL_TAG_arccot					"arccot"	 
#define FBL_TAG_arccoth 				"arccoth"	
#define FBL_TAG_arccsc					"arccsc"	 
#define FBL_TAG_arccsch 				"arccsch"	
#define FBL_TAG_arcsec					"arcsec"	 
#define FBL_TAG_arcsech 				"arcsech"	
#define FBL_TAG_arcsinh 				"arcsinh"	
#define FBL_TAG_arctanh 				"arctanh"	
// statistics
#define FBL_TAG_mean					"mean"		
#define FBL_TAG_sdev					"sdev"		
#define FBL_TAG_variance				"variance"	
#define FBL_TAG_median					"median"	 
#define FBL_TAG_mode					"mode"		
#define FBL_TAG_moment					"moment"	 
#define FBL_TAG_momentabout				"momentabout"
// linear algebra 
#define FBL_TAG_vector	 				"vector"		
#define FBL_TAG_matrix	 				"matrix"		
#define FBL_TAG_matrixrow				"matrixrow"	
#define FBL_TAG_determinant				"determinant"	
#define FBL_TAG_transpose				"transpose"	
#define FBL_TAG_selector				"selector"	 
#define FBL_TAG_vectorproduct			"vectorproduct"
#define FBL_TAG_scalarproduct			"scalarproduct"
#define FBL_TAG_outerproduct			"outerproduct" 
// semantic mapping elements 
#define FBL_TAG_annotation 				"annotation"	
#define FBL_TAG_semantics				"semantics"	 
#define FBL_TAG_annotation_xml			"annotation-xml"
// constant and symbol elements 
#define FBL_TAG_integers				"integers"		
#define FBL_TAG_reals					"reals"		 
#define FBL_TAG_rationals				"rationals"	 
#define FBL_TAG_naturalnumbers			"naturalnumbers"
#define FBL_TAG_complexes				"complexes"	 
#define FBL_TAG_primes	 				"primes"		
#define FBL_TAG_exponentiale			"exponentiale"	
#define FBL_TAG_imaginaryi 				"imaginaryi"	
#define FBL_TAG_notanumber 				"notanumber"	
#define FBL_TAG_true					"true"			
#define FBL_TAG_false					"false"		 
#define FBL_TAG_emptyset				"emptyset"		
#define FBL_TAG_pi		 				"pi"			
#define FBL_TAG_eulergamma 				"eulergamma"	
#define FBL_TAG_infinity				"infinity"
// Hermitech's symbol for No Operation in Content MathML
#define FBL_TAG_nop						"nop"		

///////////////////////////////////////////////////////////////////////////////

#define FBL_ATTR_rowspan				"rowspan"
#define FBL_ATTR_columnspan				"columnspan"

#define FBL_ATTR_type					"type"		
#define FBL_ATTR_base					"base"
#define FBL_ATTR_closure				"closure"
#define FBL_ATTR_encoding				"encoding"
#define FBL_ATTR_definitionURL			"definitionURL"

#define FBL_VAL_MathML_Presentation		"MathML-Presentation"
#define FBL_VAL_MathML_Content			"MathML-Content"

#define FBL_VAL_open					"open"
#define FBL_VAL_closed					"closed"
#define FBL_VAL_open_closed				"open-closed"
#define FBL_VAL_closed_open				"closed-open"

#define FBL_VAL_matrix					"matrix"
#define FBL_VAL_vector					"vector"	 
#define FBL_VAL_real					"real"	 
#define FBL_VAL_integer					"integer"	 
#define FBL_VAL_rational				"rational"	 
#define FBL_VAL_complex_cartesian		"complex-cartesian"	 
#define FBL_VAL_complex_polar 			"complex-polar"	 
#define FBL_VAL_constant				"constant"	 

#define FBL_VAL_above					"above"
#define FBL_VAL_below					"below"

#define FBL_VAL_axis					"axis"

#define FBL_VAL_auto					"auto"
#define FBL_VAL_newline					"newline"
#define FBL_VAL_indentingnewline		"indentingnewline"
#define FBL_VAL_nobreak					"nobreak"
#define FBL_VAL_goodbreak				"goodbreak"
#define FBL_VAL_badbreak				"badbreak"

///////////////////////////////////////////////////////////////////////////////

enum TAG2MML
{
	TAG2MML_math = 0,
	TAG2MML_mrow,
	TAG2MML_malignmark,
	TAG2MML_maligngroup,
	TAG2MML_mlabeledtr,
	TAG2MML_mn,
	TAG2MML_mo,
	TAG2MML_mi,
	TAG2MML_mtext,
	TAG2MML_ms,
	TAG2MML_mspace,
	TAG2MML_mtable,
	TAG2MML_mtr,
	TAG2MML_mtd,
	TAG2MML_mfenced,
	TAG2MML_mfrac,
	TAG2MML_mstyle,
	TAG2MML_mglyph,
	TAG2MML_msqrt,
	TAG2MML_merror,
	TAG2MML_menclose,
	TAG2MML_mpadded,
	TAG2MML_mphantom,
	TAG2MML_maction,
	TAG2MML_munder,
	TAG2MML_mover,
	TAG2MML_munderover,
	TAG2MML_msup,
	TAG2MML_msub,
	TAG2MML_mmultiscripts,
	TAG2MML_cn,
	TAG2MML_ci,
	TAG2MML_csymbol,
	TAG2MML_apply,
	TAG2MML_reln,
	TAG2MML_fn,
	TAG2MML_interval,
	TAG2MML_inverse,
	TAG2MML_sep,
	TAG2MML_condition,
	TAG2MML_declare,
	TAG2MML_lambda,
	TAG2MML_compose,
	TAG2MML_ident,
	TAG2MML_domain,
	TAG2MML_codomain,
	TAG2MML_image,
	TAG2MML_domainofapplication,
	TAG2MML_piecewise,
	TAG2MML_piece,
	TAG2MML_otherwise,
	TAG2MML_quotient,
	TAG2MML_exp,
	TAG2MML_factorial,
	TAG2MML_divide,
	TAG2MML_max,
	TAG2MML_min,
	TAG2MML_minus,
	TAG2MML_plus,
	TAG2MML_power,
	TAG2MML_rem,
	TAG2MML_times,
	TAG2MML_root,
	TAG2MML_gcd,
	TAG2MML_and,
	TAG2MML_or,
	TAG2MML_xor,
	TAG2MML_not,
	TAG2MML_implies,
	TAG2MML_forall,
	TAG2MML_exists,
	TAG2MML_abs,
	TAG2MML_conjugate,
	TAG2MML_arg,
	TAG2MML_real,
	TAG2MML_imaginary,
	TAG2MML_lcm,
	TAG2MML_floor,
	TAG2MML_ceiling,
	TAG2MML_eq,
	TAG2MML_neq,
	TAG2MML_gt,
	TAG2MML_lt,
	TAG2MML_geq,
	TAG2MML_leq,
	TAG2MML_equivalent,
	TAG2MML_approx,
	TAG2MML_factorof,
	TAG2MML_int,
	TAG2MML_diff,
	TAG2MML_partialdiff,
	TAG2MML_lowlimit,
	TAG2MML_uplimit,
	TAG2MML_bvar,
	TAG2MML_degree,
	TAG2MML_logbase,
	TAG2MML_divergence,
	TAG2MML_grad,
	TAG2MML_curl,
	TAG2MML_laplacian,
	TAG2MML_set,
	TAG2MML_list,
	TAG2MML_union,
	TAG2MML_intersect,
	TAG2MML_in,
	TAG2MML_notin,
	TAG2MML_subset,
	TAG2MML_prsubset,
	TAG2MML_notsubset,
	TAG2MML_notprsubset,
	TAG2MML_setdiff,
	TAG2MML_card,
	TAG2MML_cartesianproduct,
	TAG2MML_sum,
	TAG2MML_product,
	TAG2MML_limit,
	TAG2MML_tendsto,
	TAG2MML_ln,
	TAG2MML_log,
	TAG2MML_sin,
	TAG2MML_cos,
	TAG2MML_tan,
	TAG2MML_sec,
	TAG2MML_csc,
	TAG2MML_cot,
	TAG2MML_sinh,
	TAG2MML_cosh,
	TAG2MML_tanh,
	TAG2MML_sech,
	TAG2MML_csch,
	TAG2MML_coth,
	TAG2MML_arcsin,
	TAG2MML_arccos,
	TAG2MML_arctan,
	TAG2MML_arccosh,
	TAG2MML_arccot,
	TAG2MML_arccoth,
	TAG2MML_arccsc,
	TAG2MML_arccsch,
	TAG2MML_arcsec,
	TAG2MML_arcsech,
	TAG2MML_arcsinh,
	TAG2MML_arctanh,
	TAG2MML_mean,
	TAG2MML_sdev,
	TAG2MML_variance,
	TAG2MML_median,
	TAG2MML_mode,
	TAG2MML_moment,
	TAG2MML_momentabout,
	TAG2MML_vector,
	TAG2MML_matrix,
	TAG2MML_matrixrow,
	TAG2MML_determinant,
	TAG2MML_transpose,
	TAG2MML_selector,
	TAG2MML_vectorproduct,
	TAG2MML_scalarproduct,
	TAG2MML_outerproduct,
	TAG2MML_annotation,
	TAG2MML_semantics,
	TAG2MML_annotation_xml,
	TAG2MML_integers,
	TAG2MML_reals,
	TAG2MML_rationals,
	TAG2MML_naturalnumbers,
	TAG2MML_complexes,
	TAG2MML_primes,
	TAG2MML_exponentiale,
	TAG2MML_imaginaryi,
	TAG2MML_notanumber,
	TAG2MML_true,
	TAG2MML_false,
	TAG2MML_emptyset,
	TAG2MML_pi,
	TAG2MML_eulergamma,
	TAG2MML_infinity,
	TAG2MML_nop,
	TAG2MML_LAST,
	TAG2MML_NONE
};

///////////////////////////////////////////////////////////////////////////////

inline
enum TAG2MML mml_getTag2MML( QString& name, enum TAG2MML inputId = TAG2MML_NONE )
{
	static QString szMMLTags[ TAG2MML_LAST ] =
	{
		FBL_TAG_math,
		FBL_TAG_mrow,
		FBL_TAG_malignmark,
		FBL_TAG_maligngroup,
		FBL_TAG_mlabeledtr,
		FBL_TAG_mn,
		FBL_TAG_mo,
		FBL_TAG_mi,
		FBL_TAG_mtext,
		FBL_TAG_ms,
		FBL_TAG_mspace,
		FBL_TAG_mtable,
		FBL_TAG_mtr,
		FBL_TAG_mtd,
		FBL_TAG_mfenced,
		FBL_TAG_mfrac,
		FBL_TAG_mstyle,
		FBL_TAG_mglyph,
		FBL_TAG_msqrt,
		FBL_TAG_merror,
		FBL_TAG_menclose,
		FBL_TAG_mpadded,
		FBL_TAG_mphantom,
		FBL_TAG_maction,
		FBL_TAG_munder,
		FBL_TAG_mover,
		FBL_TAG_munderover,
		FBL_TAG_msup,
		FBL_TAG_msub,
		FBL_TAG_mmultiscripts,
		FBL_TAG_cn,
		FBL_TAG_ci,
		FBL_TAG_csymbol,
		FBL_TAG_apply,
		FBL_TAG_reln,
		FBL_TAG_fn,
		FBL_TAG_interval,
		FBL_TAG_inverse,
		FBL_TAG_sep,
		FBL_TAG_condition,
		FBL_TAG_declare,
		FBL_TAG_lambda,
		FBL_TAG_compose,
		FBL_TAG_ident,
		FBL_TAG_domain,
		FBL_TAG_codomain,
		FBL_TAG_image,
		FBL_TAG_domainofapplication,
		FBL_TAG_piecewise,
		FBL_TAG_piece,
		FBL_TAG_otherwise,
		FBL_TAG_quotient,
		FBL_TAG_exp,
		FBL_TAG_factorial,
		FBL_TAG_divide,
		FBL_TAG_max,
		FBL_TAG_min,
		FBL_TAG_minus,
		FBL_TAG_plus,
		FBL_TAG_power,
		FBL_TAG_rem,
		FBL_TAG_times,
		FBL_TAG_root,
		FBL_TAG_gcd,
		FBL_TAG_and,
		FBL_TAG_or,
		FBL_TAG_xor,
		FBL_TAG_not,
		FBL_TAG_implies,
		FBL_TAG_forall,
		FBL_TAG_exists,
		FBL_TAG_abs,
		FBL_TAG_conjugate,
		FBL_TAG_arg,
		FBL_TAG_real,
		FBL_TAG_imaginary,
		FBL_TAG_lcm,
		FBL_TAG_floor,
		FBL_TAG_ceiling,
		FBL_TAG_eq,
		FBL_TAG_neq,
		FBL_TAG_gt,
		FBL_TAG_lt,
		FBL_TAG_geq,
		FBL_TAG_leq,
		FBL_TAG_equivalent,
		FBL_TAG_approx,
		FBL_TAG_factorof,
		FBL_TAG_int,
		FBL_TAG_diff,
		FBL_TAG_partialdiff,
		FBL_TAG_lowlimit,
		FBL_TAG_uplimit,
		FBL_TAG_bvar,
		FBL_TAG_degree,
		FBL_TAG_logbase,
		FBL_TAG_divergence,
		FBL_TAG_grad,
		FBL_TAG_curl,
		FBL_TAG_laplacian,
		FBL_TAG_set,
		FBL_TAG_list,
		FBL_TAG_union,
		FBL_TAG_intersect,
		FBL_TAG_in,
		FBL_TAG_notin,
		FBL_TAG_subset,
		FBL_TAG_prsubset,
		FBL_TAG_notsubset,
		FBL_TAG_notprsubset,
		FBL_TAG_setdiff,
		FBL_TAG_card,
		FBL_TAG_cartesianproduct,
		FBL_TAG_sum,
		FBL_TAG_product,
		FBL_TAG_limit,
		FBL_TAG_tendsto,
		FBL_TAG_ln,
		FBL_TAG_log,
		FBL_TAG_sin,
		FBL_TAG_cos,
		FBL_TAG_tan,
		FBL_TAG_sec,
		FBL_TAG_csc,
		FBL_TAG_cot,
		FBL_TAG_sinh,
		FBL_TAG_cosh,
		FBL_TAG_tanh,
		FBL_TAG_sech,
		FBL_TAG_csch,
		FBL_TAG_coth,
		FBL_TAG_arcsin,
		FBL_TAG_arccos,
		FBL_TAG_arctan,
		FBL_TAG_arccosh,
		FBL_TAG_arccot,
		FBL_TAG_arccoth,
		FBL_TAG_arccsc,
		FBL_TAG_arccsch,
		FBL_TAG_arcsec,
		FBL_TAG_arcsech,
		FBL_TAG_arcsinh,
		FBL_TAG_arctanh,
		FBL_TAG_mean,
		FBL_TAG_sdev,
		FBL_TAG_variance,
		FBL_TAG_median,
		FBL_TAG_mode,
		FBL_TAG_moment,
		FBL_TAG_momentabout,
		FBL_TAG_vector,
		FBL_TAG_matrix,
		FBL_TAG_matrixrow,
		FBL_TAG_determinant,
		FBL_TAG_transpose,
		FBL_TAG_selector,
		FBL_TAG_vectorproduct,
		FBL_TAG_scalarproduct,
		FBL_TAG_outerproduct,
		FBL_TAG_annotation,
		FBL_TAG_semantics,
		FBL_TAG_annotation_xml,
		FBL_TAG_integers,
		FBL_TAG_reals,
		FBL_TAG_rationals,
		FBL_TAG_naturalnumbers,
		FBL_TAG_complexes,
		FBL_TAG_primes,
		FBL_TAG_exponentiale,
		FBL_TAG_imaginaryi,
		FBL_TAG_notanumber,
		FBL_TAG_true,
		FBL_TAG_false,
		FBL_TAG_emptyset,
		FBL_TAG_pi,
		FBL_TAG_eulergamma,
		FBL_TAG_infinity,
		FBL_TAG_nop
	};
	static HKMapString2Long _tag2mml_map( szMMLTags, TAG2MML_LAST );

	if( inputId == TAG2MML_NONE )
	{
		long *p = _tag2mml_map.isFind( name );
		if( p && *p >= 0 && *p < TAG2MML_LAST )
			return (enum TAG2MML)*p;
	}
	else
	{
		name = (inputId >= 0 && inputId < TAG2MML_LAST ? QString( szMMLTags[ inputId ] ) : QString());
	}

	return TAG2MML_NONE;
}

inline
QString mml_getTagID2TagName( enum TAG2MML tag_id )
{
	QString ret;
	::mml_getTag2MML( ret, tag_id );
	return ret;
}

///////////////////////////////////////////////////////////////////////////////

#define CONTENT_MATHML_CN_TYPE_NONE			"<none>"
#define CONTENT_MATHML_CN_DEFAULT_BASE_STR	"10"
#define CONTENT_MATHML_CN_DEFAULT_BASE		10

#define CONTENT_MATHML_CN_TYPE_REAL			0
#define CONTENT_MATHML_CN_TYPE_INTEGER		1
#define CONTENT_MATHML_CN_TYPE_RATIONAL		2
#define CONTENT_MATHML_CN_TYPE_COMPLEX_C	3
#define CONTENT_MATHML_CN_TYPE_COMPLEX_P	4
#define CONTENT_MATHML_CN_TYPE_CONSTANT		5

///////////////////////////////////////////////////////////////////////////////

#define MML_OPERATOR_HOTKEY_ASSIGN		QChar(':')
#define MML_OPERATOR_BODY_ASSIGN		QChar(0x2254)
#define MML_OPERATOR_BODY_ASSIGN_ENTITY	"&Assign;"

inline
int mml_isAssignOpBody( QString str )
{
	return (str == MML_OPERATOR_BODY_ASSIGN || str == MML_OPERATOR_BODY_ASSIGN_ENTITY || str == ":=");
}

inline
int mml_isAssignOpHotkey( QString str )
{
	return str == MML_OPERATOR_HOTKEY_ASSIGN || ::mml_isAssignOpBody( str );
}

///////////////////////////////////////////////////////////////////////////////

#endif
