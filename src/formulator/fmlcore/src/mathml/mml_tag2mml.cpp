/*
 *  mml_tag2mml.cpp, v.1.0 07/11/2004
 *
 *  Copyright (C) 2004 Hermitech Laboratory
 *
 *  Hermitech Equation Builder (Formulator)
 *  Content Tree Nodes: Templates
 *
 *  HISTORY:
 *	 07/11/2004	The first draft
 *
 */ 

#include <vector>

#include "HUtils/iustring.h"
#include "HUtils/ihkflag.h"
#include "HUtils/ixml.h"
#include "HUtils/ihkmap.h"
#include "HUtils/ihkstring.h"

#include "mml_tag2mml.h"

///////////////////////////////////////////////////////////////////////////////

QString mml_getCM2PMTag_standalone( enum TAG2MML tag_id, AST_TXML_Tag *xml_node )
{
	if( !xml_node || tag_id < TAG2MML_math || tag_id >= TAG2MML_LAST )
		return _T("");
	return szCMTag2PMTag_standalone[ tag_id ];
}

QString mml_getCM2PMText_standalone( enum TAG2MML tag_id, AST_TXML_Tag *xml_node )
{
	QString ret = _T("");

	if( !xml_node || tag_id < TAG2MML_math || tag_id >= TAG2MML_LAST )
		return ret;
	
	AST_TXML_Attribute *_attr = 0;
	switch( tag_id )
	{
	case TAG2MML_tendsto:
		_attr = xml_node->getAttribute( FBL_ATTR_type );
		if( _attr )
		{
			if( !xml_strcmp( _attr->getStringValue(), FBL_VAL_above ) )
			{
				ret = _T("&searr;");
				break;
			}
			else if( !xml_strcmp( _attr->getStringValue(), FBL_VAL_below ) )
			{
				ret = _T("&nearr;");
				break;
			}
		}
		// reuse the next statement
	default:
		ret = szCMTag2PMText_standalone[ tag_id ];
	}

	return ret;
}

QString szCMTag2PMTag_standalone[ TAG2MML_LAST ] =
{
	_T(""), // FBL_TAG_math,
	_T(""), // FBL_TAG_mrow,
	_T(""), // FBL_TAG_malignmark,
	_T(""), // FBL_TAG_maligngroup,
	_T(""), // FBL_TAG_mlabeledtr,
	_T(""), // FBL_TAG_mn,
	_T(""), // FBL_TAG_mo,
	_T(""), // FBL_TAG_mi,
	_T(""), // FBL_TAG_mtext,
	_T(""), // FBL_TAG_ms,
	_T(""), // FBL_TAG_mspace,
	_T(""), // FBL_TAG_mtable,
	_T(""), // FBL_TAG_mtr,
	_T(""), // FBL_TAG_mtd,
	_T(""), // FBL_TAG_mfenced,
	_T(""), // FBL_TAG_mfrac,
	_T(""), // FBL_TAG_mstyle,
	_T(""), // FBL_TAG_mglyph,
	_T(""), // FBL_TAG_msqrt,
	_T(""), // FBL_TAG_merror,
	_T(""), // FBL_TAG_menclose,
	_T(""), // FBL_TAG_mpadded,
	_T(""), // FBL_TAG_mphantom,
	_T(""), // FBL_TAG_maction,
	_T(""), // FBL_TAG_munder,
	_T(""), // FBL_TAG_mover,
	_T(""), // FBL_TAG_munderover,
	_T(""), // FBL_TAG_msup,
	_T(""), // FBL_TAG_msub,
	_T(""), // FBL_TAG_mmultiscripts,
	_T(""), // FBL_TAG_cn,
	_T(""), // FBL_TAG_ci,
	_T(""), // FBL_TAG_csymbol,
	_T(""), // FBL_TAG_apply,
	_T(""), // FBL_TAG_reln,
	_T(""), // FBL_TAG_fn,
	_T(""), // FBL_TAG_interval,
	_T(""), // FBL_TAG_inverse,
	_T(""), // FBL_TAG_sep,
	_T(""), // FBL_TAG_condition,
	_T("mi"), // FBL_TAG_declare,
	_T("mo"), // FBL_TAG_lambda,
	_T("mo"), // FBL_TAG_compose,
	_T("mi"), // FBL_TAG_ident,
	_T("mi"), // FBL_TAG_domain,
	_T("mi"), // FBL_TAG_codomain,
	_T("mi"), // FBL_TAG_image,
	_T(""), // FBL_TAG_domainofapplication,
	_T(""), // FBL_TAG_piecewise,
	_T(""), // FBL_TAG_piece,
	_T(""), // FBL_TAG_otherwise,
	_T("mi"), // FBL_TAG_quotient,
	_T("mi"), // FBL_TAG_exp,
	_T("mo"), // FBL_TAG_factorial,
	_T("mi"), // FBL_TAG_divide,
	_T("mi"), // FBL_TAG_max,
	_T("mi"), // FBL_TAG_min,
	_T("mo"), // FBL_TAG_minus,
	_T("mo"), // FBL_TAG_plus,
	_T("mi"), // FBL_TAG_power,
	_T("mo"), // FBL_TAG_rem,
	_T("mo"), // FBL_TAG_times,
	_T("mi"), // FBL_TAG_root,
	_T("mi"), // FBL_TAG_gcd,
	_T("mo"), // FBL_TAG_and,
	_T("mo"), // FBL_TAG_or,
	_T("mo"), // FBL_TAG_xor,
	_T("mo"), // FBL_TAG_not,
	_T("mo"), // FBL_TAG_implies,
	_T("mo"), // FBL_TAG_forall,
	_T("mo"), // FBL_TAG_exists,
	_T("mi"), // FBL_TAG_abs,
	_T("mi"), // FBL_TAG_conjugate,
	_T("mi"), // FBL_TAG_arg,
	_T("mi"), // FBL_TAG_real,
	_T("mi"), // FBL_TAG_imaginary,
	_T("mi"), // FBL_TAG_lcm,
	_T("mi"), // FBL_TAG_floor,
	_T("mi"), // FBL_TAG_ceiling,
	_T("mo"), // FBL_TAG_eq,
	_T("mo"), // FBL_TAG_neq,
	_T("mo"), // FBL_TAG_gt,
	_T("mo"), // FBL_TAG_lt,
	_T("mo"), // FBL_TAG_geq,
	_T("mo"), // FBL_TAG_leq,
	_T("mo"), // FBL_TAG_equivalent,
	_T("mo"), // FBL_TAG_approx,
	_T("mo"), // FBL_TAG_factorof,
	_T("mo"), // FBL_TAG_int,
	_T("mi"), // FBL_TAG_diff,
	_T("mo"), // FBL_TAG_partialdiff,
	_T(""), // FBL_TAG_lowlimit,
	_T(""), // FBL_TAG_uplimit,
	_T(""), // FBL_TAG_bvar,
	_T(""), // FBL_TAG_degree,
	_T(""), // FBL_TAG_logbase
	_T("mi"), // FBL_TAG_divergence,
	_T("mo"), // FBL_TAG_grad,
	_T("mi"), // FBL_TAG_curl,
	_T(""), // FBL_TAG_laplacian,
	_T(""), // FBL_TAG_set,
	_T(""), // FBL_TAG_list,
	_T("mo"), // FBL_TAG_union,
	_T("mo"), // FBL_TAG_intersect,
	_T("mo"), // FBL_TAG_in,
	_T("mo"), // FBL_TAG_notin,
	_T("mo"), // FBL_TAG_subset,
	_T("mo"), // FBL_TAG_prsubset,
	_T("mo"), // FBL_TAG_notsubset,
	_T("mo"), // FBL_TAG_notprsubset,
	_T("mo"), // FBL_TAG_setdiff,
	_T("mi"), // FBL_TAG_card,
	_T("mo"), // FBL_TAG_cartesianproduct,
	_T("mo"), // FBL_TAG_sum,
	_T("mo"), // FBL_TAG_product,
	_T("mi"), // FBL_TAG_limit,
	_T("mo"), // FBL_TAG_tendsto,
	_T("mi"), // FBL_TAG_ln,
	_T("mi"), // FBL_TAG_log,
	_T("mi"), // FBL_TAG_sin,
	_T("mi"), // FBL_TAG_cos,
	_T("mi"), // FBL_TAG_tan,
	_T("mi"), // FBL_TAG_sec,
	_T("mi"), // FBL_TAG_csc,
	_T("mi"), // FBL_TAG_cot,
	_T("mi"), // FBL_TAG_sinh,
	_T("mi"), // FBL_TAG_cosh,
	_T("mi"), // FBL_TAG_tanh,
	_T("mi"), // FBL_TAG_sech,
	_T("mi"), // FBL_TAG_csch,
	_T("mi"), // FBL_TAG_coth,
	_T("mi"), // FBL_TAG_arcsin,
	_T("mi"), // FBL_TAG_arccos,
	_T("mi"), // FBL_TAG_arctan,
	_T("mi"), // FBL_TAG_arccosh,
	_T("mi"), // FBL_TAG_arccot,
	_T("mi"), // FBL_TAG_arccoth,
	_T("mi"), // FBL_TAG_arccsc,
	_T("mi"), // FBL_TAG_arccsch,
	_T("mi"), // FBL_TAG_arcsec,
	_T("mi"), // FBL_TAG_arcsech,
	_T("mi"), // FBL_TAG_arcsinh,
	_T("mi"), // FBL_TAG_arctanh,
	_T("mi"), // FBL_TAG_mean,
	_T("mi"), // FBL_TAG_sdev,
	_T(""), // FBL_TAG_variance,
	_T("mi"), // FBL_TAG_median,
	_T("mi"), // FBL_TAG_mode,
	_T("mi"), // FBL_TAG_moment,
	_T(""), // FBL_TAG_momentabout,
	_T(""), // FBL_TAG_vector,
	_T(""), // FBL_TAG_matrix,
	_T(""), // FBL_TAG_matrixrow,
	_T("mi"), // FBL_TAG_determinant,
	_T(""), // FBL_TAG_transpose,
	_T(""), // FBL_TAG_selector,
	_T("mo"), // FBL_TAG_vectorproduct,
	_T("mo"), // FBL_TAG_scalarproduct,
	_T("mo"), // FBL_TAG_outerproduct,
	_T(""), // FBL_TAG_annotation,
	_T(""), // FBL_TAG_semantics,
	_T(""), // FBL_TAG_annotation_xml,
	_T("mi"), // FBL_TAG_integers,
	_T("mi"), // FBL_TAG_reals,
	_T("mi"), // FBL_TAG_rationals,
	_T("mi"), // FBL_TAG_naturalnumbers,
	_T("mi"), // FBL_TAG_complexes,
	_T("mi"), // FBL_TAG_primes,
	_T("mi"), // FBL_TAG_exponentiale,
	_T("mi"), // FBL_TAG_imaginaryi,
	_T("mi"), // FBL_TAG_notanumber,
	_T("mi"), // FBL_TAG_true,
	_T("mi"), // FBL_TAG_false,
	_T("mo"), // FBL_TAG_emptyset,
	_T("mi"), // FBL_TAG_pi,
	_T("mi"), // FBL_TAG_eulergamma,
	_T("mi"), // FBL_TAG_infinity
	_T("mo")  // FBL_TAG_nop
};

QString szCMTag2PMText_standalone[ TAG2MML_LAST ] =
{
	_T(""), // FBL_TAG_math,
	_T(""), // FBL_TAG_mrow,
	_T(""), // FBL_TAG_malignmark,
	_T(""), // FBL_TAG_maligngroup,
	_T(""), // FBL_TAG_mlabeledtr,
	_T(""), // FBL_TAG_mn,
	_T(""), // FBL_TAG_mo,
	_T(""), // FBL_TAG_mi,
	_T(""), // FBL_TAG_mtext,
	_T(""), // FBL_TAG_ms,
	_T(""), // FBL_TAG_mspace,
	_T(""), // FBL_TAG_mtable,
	_T(""), // FBL_TAG_mtr,
	_T(""), // FBL_TAG_mtd,
	_T(""), // FBL_TAG_mfenced,
	_T(""), // FBL_TAG_mfrac,
	_T(""), // FBL_TAG_mstyle,
	_T(""), // FBL_TAG_mglyph,
	_T(""), // FBL_TAG_msqrt,
	_T(""), // FBL_TAG_merror,
	_T(""), // FBL_TAG_menclose,
	_T(""), // FBL_TAG_mpadded,
	_T(""), // FBL_TAG_mphantom,
	_T(""), // FBL_TAG_maction,
	_T(""), // FBL_TAG_munder,
	_T(""), // FBL_TAG_mover,
	_T(""), // FBL_TAG_munderover,
	_T(""), // FBL_TAG_msup,
	_T(""), // FBL_TAG_msub,
	_T(""), // FBL_TAG_mmultiscripts,
	_T(""), // FBL_TAG_cn,
	_T(""), // FBL_TAG_ci,
	_T(""), // FBL_TAG_csymbol,
	_T(""), // FBL_TAG_apply,
	_T(""), // FBL_TAG_reln,
	_T(""), // FBL_TAG_fn,
	_T(""), // FBL_TAG_interval,
	_T(""), // FBL_TAG_inverse,
	_T(""), // FBL_TAG_sep,
	_T(""), // FBL_TAG_condition,
	_T("declare:"), // FBL_TAG_declare,
	_T("&lambda;"), // FBL_TAG_lambda,
	_T("&compfn;"), // FBL_TAG_compose,
	_T("id"), // FBL_TAG_ident,
	_T("domain"), // FBL_TAG_domain,
	_T("codomain"), // FBL_TAG_codomain,
	_T("image"), // FBL_TAG_image,
	_T(""), // FBL_TAG_domainofapplication,
	_T(""), // FBL_TAG_piecewise,
	_T(""), // FBL_TAG_piece,
	_T(""), // FBL_TAG_otherwise,
	_T("quotient"), // FBL_TAG_quotient,
	_T("&ExponentialE;"), // FBL_TAG_exp,
	_T("!"), // FBL_TAG_factorial,
	_T("/"), // FBL_TAG_divide,
	_T("max"), // FBL_TAG_max,
	_T("min"), // FBL_TAG_min,
	_T("&minus;"), // FBL_TAG_minus,
	_T("+"), // FBL_TAG_plus,
	_T("pow"), // FBL_TAG_power,
	_T("mod"), // FBL_TAG_rem,
	_T("&times;"), // FBL_TAG_times, // "&sdot;"), // FBL_TAG_times, //		_T("&InvisibleTimes;"), // FBL_TAG_times,
	_T("root"), // FBL_TAG_root,
	_T("gcd"), // FBL_TAG_gcd,
	_T("&and;"), // FBL_TAG_and,
	_T("&or;"), // FBL_TAG_or,
	_T("xor"), // FBL_TAG_xor,
	_T("&not;"), // FBL_TAG_not,
	_T("&rArr;"), // FBL_TAG_implies,
	_T("&forall;"), // FBL_TAG_forall,
	_T("&exist;"), // FBL_TAG_exists,
	_T("abs"), // FBL_TAG_abs,
	_T("conjugate"), // FBL_TAG_conjugate,
	_T("arg"), // FBL_TAG_arg,
	_T("&real;"), // FBL_TAG_real,
	_T("&image;"), // FBL_TAG_imaginary,
	_T("lcm"), // FBL_TAG_lcm,
	_T("floor"), // FBL_TAG_floor,
	_T("ceiling"), // FBL_TAG_ceiling,
	_T("="), // FBL_TAG_eq,
	_T("&ne;"), // FBL_TAG_neq,
	_T("&gt;"), // FBL_TAG_gt,
	_T("&lt;"), // FBL_TAG_lt,
	_T("&ge;"), // FBL_TAG_geq,
	_T("&le;"), // FBL_TAG_leq,
	_T("&equiv;"), // FBL_TAG_equivalent,
	_T("&ap;"), // FBL_TAG_approx,
	_T("&verbar;"), // FBL_TAG_factorof,
	_T("&int;"), // FBL_TAG_int,
	_T("d"), // FBL_TAG_diff,
	_T("&PartialD;"), // FBL_TAG_partialdiff,
	_T(""), // FBL_TAG_lowlimit,
	_T(""), // FBL_TAG_uplimit,
	_T(""), // FBL_TAG_bvar,
	_T(""), // FBL_TAG_degree,
	_T(""),	// FBL_TAG_logbase
	_T("div"), // FBL_TAG_divergence,
	_T("&nabla;"), // FBL_TAG_grad,
	_T("curl"), // FBL_TAG_curl,
	_T(""), // FBL_TAG_laplacian,
	_T(""), // FBL_TAG_set,
	_T(""), // FBL_TAG_list,
	_T("&cup;"), // FBL_TAG_union,
	_T("&cap;"), // FBL_TAG_intersect,
	_T("&isin;"), // FBL_TAG_in,
	_T("&notin;"), // FBL_TAG_notin,
	_T("&subE;"), // FBL_TAG_subset,
	_T("&sub;"), // FBL_TAG_prsubset,
	_T("&NotSubsetEqual;"), // FBL_TAG_notsubset,
	_T("&vnsub;"), // FBL_TAG_notprsubset,
	_T("\\"), // FBL_TAG_setdiff,
	_T("card"), // FBL_TAG_card,
	_T("&times;"), // FBL_TAG_cartesianproduct,
	_T("&sum;"), // FBL_TAG_sum,
	_T("&prod;"), // FBL_TAG_product,
	_T("lim"), // FBL_TAG_limit,
	_T("&rarr;"), // FBL_TAG_tendsto,
	_T("ln"), // FBL_TAG_ln,
	_T("log"), // FBL_TAG_log,
	_T("sin"), // FBL_TAG_sin,
	_T("cos"), // FBL_TAG_cos,
	_T("tan"), // FBL_TAG_tan,
	_T("sec"), // FBL_TAG_sec,
	_T("csc"), // FBL_TAG_csc,
	_T("cot"), // FBL_TAG_cot,
	_T("sinh"), // FBL_TAG_sinh,
	_T("cosh"), // FBL_TAG_cosh,
	_T("tanh"), // FBL_TAG_tanh,
	_T("sech"), // FBL_TAG_sech,
	_T("csch"), // FBL_TAG_csch,
	_T("coth"), // FBL_TAG_coth,
	_T("arcsin"), // FBL_TAG_arcsin,
	_T("arccos"), // FBL_TAG_arccos,
	_T("arctan"), // FBL_TAG_arctan,
	_T("arccosh"), // FBL_TAG_arccosh,
	_T("arccot"), // FBL_TAG_arccot,
	_T("arccoth"), // FBL_TAG_arccoth,
	_T("arccsc"), // FBL_TAG_arccsc,
	_T("arccsch"), // FBL_TAG_arccsch,
	_T("arcsec"), // FBL_TAG_arcsec,
	_T("arcsech"), // FBL_TAG_arcsech,
	_T("arcsinh"), // FBL_TAG_arcsinh,
	_T("arctanh"), // FBL_TAG_arctanh,
	_T("mean"), // FBL_TAG_mean,
	_T("&sigma;"), // FBL_TAG_sdev,
	_T("variance"), // FBL_TAG_variance,
	_T("median"), // FBL_TAG_median,
	_T("mode"), // FBL_TAG_mode,
	_T("&mu;"), // FBL_TAG_moment,
	_T(""), // FBL_TAG_momentabout,
	_T(""), // FBL_TAG_vector,
	_T(""), // FBL_TAG_matrix,
	_T(""), // FBL_TAG_matrixrow,
	_T("det"), // FBL_TAG_determinant,
	_T(""), // FBL_TAG_transpose,
	_T(""), // FBL_TAG_selector,
	_T("&times;"), // FBL_TAG_vectorproduct,
	_T("&sdot;"), // FBL_TAG_scalarproduct,
	_T("&CircleTimes;"), // FBL_TAG_outerproduct,
	_T(""), // FBL_TAG_annotation,
	_T(""), // FBL_TAG_semantics,
	_T(""), // FBL_TAG_annotation_xml,
	_T("&integers;"), // FBL_TAG_integers,
	_T("&reals;"), // FBL_TAG_reals,
	_T("&rationals;"), // FBL_TAG_rationals,
	_T("&naturals;"), // FBL_TAG_naturalnumbers,
	_T("&complexes;"), // FBL_TAG_complexes,
	_T("&primes;"), // FBL_TAG_primes,
	_T("&ExponentialE;"), // FBL_TAG_exponentiale,
	_T("&ImaginaryI;"), // FBL_TAG_imaginaryi,
	_T("NaN"), // FBL_TAG_notanumber,
	_T("true"), // FBL_TAG_true,
	_T("false"), // FBL_TAG_false,
	_T("&emptyv;"), // FBL_TAG_emptyset,
	_T("&pi;"), // FBL_TAG_pi,
	_T("&gamma;"), // FBL_TAG_eulergamma,
	_T("&infin;"), // FBL_TAG_infinity
	_T("&#x25AE;") // FBL_TAG_nop
};

///////////////////////////////////////////////////////////////////////////////

long anCM2Argc[ TAG2MML_LAST ] =
{
	-1, // FBL_TAG_math,
	-1, // FBL_TAG_mrow,
	-1, // FBL_TAG_malignmark
	-1, // FBL_TAG_maligngroup
	-1, // FBL_TAG_mlabeledtr
	-1, // FBL_TAG_mn,
	-1, // FBL_TAG_mo,
	-1, // FBL_TAG_mi,
	-1, // FBL_TAG_mtext,
	-1, // FBL_TAG_ms,
	-1, // FBL_TAG_mspace,
	-1, // FBL_TAG_mtable,
	-1, // FBL_TAG_mtr,
	-1, // FBL_TAG_mtd,
	-1, // FBL_TAG_mfenced,
	-1, // FBL_TAG_mfrac,
	-1, // FBL_TAG_mstyle,
	-1, // FBL_TAG_mglyph,
	-1, // FBL_TAG_msqrt,
	-1, // FBL_TAG_merror,
	-1, // FBL_TAG_menclose,
	-1, // FBL_TAG_mpadded,
	-1, // FBL_TAG_mphantom,
	-1, // FBL_TAG_maction,
	-1, // FBL_TAG_munder,
	-1, // FBL_TAG_mover,
	-1, // FBL_TAG_munderover,
	-1, // FBL_TAG_msup,
	-1, // FBL_TAG_msub,
	-1, // FBL_TAG_mmultiscripts,
	-1, // FBL_TAG_cn,
	-1, // FBL_TAG_ci,
	-1, // FBL_TAG_csymbol,
	-1, // FBL_TAG_apply,
	-1, // FBL_TAG_reln,
	-1, // FBL_TAG_fn,
	-1, // FBL_TAG_interval,
	1, // FBL_TAG_inverse,
	-1, // FBL_TAG_sep,
	-1, // FBL_TAG_condition,
	-1, // FBL_TAG_declare,
	-1, // FBL_TAG_lambda,
	-1, // FBL_TAG_compose,
	-1, // FBL_TAG_ident,
	-1, // FBL_TAG_domain,
	-1, // FBL_TAG_codomain,
	1, // FBL_TAG_image,
	-1, // FBL_TAG_domainofapplication,
	-1, // FBL_TAG_piecewise,
	-1, // FBL_TAG_piece,
	-1, // FBL_TAG_otherwise,
	2, // FBL_TAG_quotient,
	1, // FBL_TAG_exp,
	1, // FBL_TAG_factorial,
	2, // FBL_TAG_divide,
	-1, // FBL_TAG_max,
	-1, // FBL_TAG_min,
	-1, // FBL_TAG_minus,
	-1, // FBL_TAG_plus,
	2, // FBL_TAG_power,
	2, // FBL_TAG_rem,
	-1, // FBL_TAG_times,
	2, // FBL_TAG_root,
	-1, // FBL_TAG_gcd,
	-1, // FBL_TAG_and,
	-1, // FBL_TAG_or,
	-1, // FBL_TAG_xor,
	1, // FBL_TAG_not,
	2, // FBL_TAG_implies,
	-1, // FBL_TAG_forall,
	-1, // FBL_TAG_exists,
	1, // FBL_TAG_abs,
	1, // FBL_TAG_conjugate,
	1, // FBL_TAG_arg,
	1, // FBL_TAG_real,
	1, // FBL_TAG_imaginary,
	-1, // FBL_TAG_lcm,
	1, // FBL_TAG_floor,
	1, // FBL_TAG_ceiling,
	-1, // FBL_TAG_eq,
	2, // FBL_TAG_neq,
	-1, // FBL_TAG_gt,
	-1, // FBL_TAG_lt,
	-1, // FBL_TAG_geq,
	-1, // FBL_TAG_leq,
	-1, // FBL_TAG_equivalent,
	-1, // FBL_TAG_approx,
	2, // FBL_TAG_factorof,
	-1, // FBL_TAG_int,
	-1, // FBL_TAG_diff,
	-1, // FBL_TAG_partialdiff,
	-1, // FBL_TAG_lowlimit,
	-1, // FBL_TAG_uplimit,
	-1, // FBL_TAG_bvar,
	-1, // FBL_TAG_degree,
	-1, // FBL_TAG_logbase
	1, // FBL_TAG_divergence,
	1, // FBL_TAG_grad,
	1, // FBL_TAG_curl,
	1, // FBL_TAG_laplacian,
	-1, // FBL_TAG_set,
	-1, // FBL_TAG_list,
	-1, // FBL_TAG_union,
	-1, // FBL_TAG_intersect,
	2, // FBL_TAG_in,
	2, // FBL_TAG_notin,
	2, // FBL_TAG_subset,
	2, // FBL_TAG_prsubset,
	2, // FBL_TAG_notsubset,
	2, // FBL_TAG_notprsubset,
	2, // FBL_TAG_setdiff,
	1, // FBL_TAG_card,
	-1, // FBL_TAG_cartesianproduct,
	-1, // FBL_TAG_sum,
	-1, // FBL_TAG_product,
	-1, // FBL_TAG_limit,
	2, // FBL_TAG_tendsto,
	1, // FBL_TAG_ln,
	2, // FBL_TAG_log,
	1, // FBL_TAG_sin,
	1, // FBL_TAG_cos,
	1, // FBL_TAG_tan,
	1, // FBL_TAG_sec,
	1, // FBL_TAG_csc,
	1, // FBL_TAG_cot,
	1, // FBL_TAG_sinh,
	1, // FBL_TAG_cosh,
	1, // FBL_TAG_tanh,
	1, // FBL_TAG_sech,
	1, // FBL_TAG_csch,
	1, // FBL_TAG_coth,
	1, // FBL_TAG_arcsin,
	1, // FBL_TAG_arccos,
	1, // FBL_TAG_arctan,
	1, // FBL_TAG_arccosh,
	1, // FBL_TAG_arccot,
	1, // FBL_TAG_arccoth,
	1, // FBL_TAG_arccsc,
	1, // FBL_TAG_arccsch,
	1, // FBL_TAG_arcsec,
	1, // FBL_TAG_arcsech,
	1, // FBL_TAG_arcsinh,
	1, // FBL_TAG_arctanh,
	-1, // FBL_TAG_mean,
	-1, // FBL_TAG_sdev,
	-1, // FBL_TAG_variance,
	-1, // FBL_TAG_median,
	-1, // FBL_TAG_mode,
	-1, // FBL_TAG_moment,
	-1, // FBL_TAG_momentabout,
	-1, // FBL_TAG_vector,
	-1, // FBL_TAG_matrix,
	-1, // FBL_TAG_matrixrow,
	1, // FBL_TAG_determinant,
	1, // FBL_TAG_transpose,
	-1, // FBL_TAG_selector,
	2, // FBL_TAG_vectorproduct,
	2, // FBL_TAG_scalarproduct,
	2, // FBL_TAG_outerproduct,
	-1, // FBL_TAG_annotation,
	-1, // FBL_TAG_semantics,
	-1, // FBL_TAG_annotation_xml,
	-1, // FBL_TAG_integers,
	-1, // FBL_TAG_reals,
	-1, // FBL_TAG_rationals,
	-1, // FBL_TAG_naturalnumbers,
	-1, // FBL_TAG_complexes,
	-1, // FBL_TAG_primes,
	-1, // FBL_TAG_exponentiale,
	-1, // FBL_TAG_imaginaryi,
	-1, // FBL_TAG_notanumber,
	-1, // FBL_TAG_true,
	-1, // FBL_TAG_false,
	-1, // FBL_TAG_emptyset,
	-1, // FBL_TAG_pi,
	-1, // FBL_TAG_eulergamma,
	-1, // FBL_TAG_infinity
	-1 // FBL_TAG_nop
};

long mml_getCM2Argc( enum TAG2MML tag_id )
{
	if( tag_id < TAG2MML_math || tag_id >= TAG2MML_LAST )
		return -1;

	return anCM2Argc[ tag_id ];
}

long anCM2Fenced[ TAG2MML_LAST ] =
{
	0, // FBL_TAG_math,
	0, // FBL_TAG_mrow,
	0, // FBL_TAG_malignmark,
	0, // FBL_TAG_maligngroup,
	0, // FBL_TAG_mlabeledtr,
	0, // FBL_TAG_mn,
	0, // FBL_TAG_mo,
	0, // FBL_TAG_mi,
	0, // FBL_TAG_mtext,
	0, // FBL_TAG_ms,
	0, // FBL_TAG_mspace,
	0, // FBL_TAG_mtable,
	0, // FBL_TAG_mtr,
	0, // FBL_TAG_mtd,
	0, // FBL_TAG_mfenced,
	0, // FBL_TAG_mfrac,
	0, // FBL_TAG_mstyle,
	0, // FBL_TAG_mglyph,
	0, // FBL_TAG_msqrt,
	0, // FBL_TAG_merror,
	0, // FBL_TAG_menclose,
	0, // FBL_TAG_mpadded,
	0, // FBL_TAG_mphantom,
	0, // FBL_TAG_maction,
	0, // FBL_TAG_munder,
	0, // FBL_TAG_mover,
	0, // FBL_TAG_munderover,
	0, // FBL_TAG_msup,
	0, // FBL_TAG_msub,
	0, // FBL_TAG_mmultiscripts,
	0, // FBL_TAG_cn,
	0, // FBL_TAG_ci,
	0, // FBL_TAG_csymbol,
	0, // FBL_TAG_apply,
	0, // FBL_TAG_reln,
	0, // FBL_TAG_fn,
	0, // FBL_TAG_interval,
	0, // FBL_TAG_inverse,
	0, // FBL_TAG_sep,
	0, // FBL_TAG_condition,
	0, // FBL_TAG_declare,
	0, // FBL_TAG_lambda,
	0, // FBL_TAG_compose,
	1, // FBL_TAG_ident,
	1, // FBL_TAG_domain,
	1, // FBL_TAG_codomain,
	1, // FBL_TAG_image,
	0, // FBL_TAG_domainofapplication,
	0, // FBL_TAG_piecewise,
	0, // FBL_TAG_piece,
	0, // FBL_TAG_otherwise,
	0, // FBL_TAG_quotient,
	0, // FBL_TAG_exp,
	0, // FBL_TAG_factorial,
	0, // FBL_TAG_divide,
	1, // FBL_TAG_max,
	1, // FBL_TAG_min,
	0, // FBL_TAG_minus,
	0, // FBL_TAG_plus,
	0, // FBL_TAG_power,
	0, // FBL_TAG_rem,
	0, // FBL_TAG_times,
	0, // FBL_TAG_root,
	1, // FBL_TAG_gcd,
	0, // FBL_TAG_and,
	0, // FBL_TAG_or,
	1, // FBL_TAG_xor,
	0, // FBL_TAG_not,
	0, // FBL_TAG_implies,
	0, // FBL_TAG_forall,
	0, // FBL_TAG_exists,
	0, // FBL_TAG_abs,
	0, // FBL_TAG_conjugate,
	0, // FBL_TAG_arg,
	0, // FBL_TAG_real,
	0, // FBL_TAG_imaginary,
	1, // FBL_TAG_lcm,
	0, // FBL_TAG_floor,
	0, // FBL_TAG_ceiling,
	0, // FBL_TAG_eq,
	0, // FBL_TAG_neq,
	0, // FBL_TAG_gt,
	0, // FBL_TAG_lt,
	0, // FBL_TAG_geq,
	0, // FBL_TAG_leq,
	0, // FBL_TAG_equivalent,
	0, // FBL_TAG_approx,
	0, // FBL_TAG_factorof,
	0, // FBL_TAG_int,
	0, // FBL_TAG_diff,
	0, // FBL_TAG_partialdiff,
	0, // FBL_TAG_lowlimit,
	0, // FBL_TAG_uplimit,
	0, // FBL_TAG_bvar,
	0, // FBL_TAG_degree,
	0, // FBL_TAG_logbase,
	0, // FBL_TAG_divergence,
	0, // FBL_TAG_grad,
	0, // FBL_TAG_curl,
	0, // FBL_TAG_laplacian,
	0, // FBL_TAG_set,
	0, // FBL_TAG_list,
	0, // FBL_TAG_union,
	0, // FBL_TAG_intersect,
	0, // FBL_TAG_in,
	0, // FBL_TAG_notin,
	0, // FBL_TAG_subset,
	0, // FBL_TAG_prsubset,
	0, // FBL_TAG_notsubset,
	0, // FBL_TAG_notprsubset,
	0, // FBL_TAG_setdiff,
	0, // FBL_TAG_card,
	0, // FBL_TAG_cartesianproduct,
	0, // FBL_TAG_sum,
	0, // FBL_TAG_product,
	0, // FBL_TAG_limit,
	0, // FBL_TAG_tendsto,
	0, // FBL_TAG_ln,
	0, // FBL_TAG_log,
	0, // FBL_TAG_sin,
	0, // FBL_TAG_cos,
	0, // FBL_TAG_tan,
	0, // FBL_TAG_sec,
	0, // FBL_TAG_csc,
	0, // FBL_TAG_cot,
	0, // FBL_TAG_sinh,
	0, // FBL_TAG_cosh,
	0, // FBL_TAG_tanh,
	0, // FBL_TAG_sech,
	0, // FBL_TAG_csch,
	0, // FBL_TAG_coth,
	0, // FBL_TAG_arcsin,
	0, // FBL_TAG_arccos,
	0, // FBL_TAG_arctan,
	0, // FBL_TAG_arccosh,
	0, // FBL_TAG_arccot,
	0, // FBL_TAG_arccoth,
	0, // FBL_TAG_arccsc,
	0, // FBL_TAG_arccsch,
	0, // FBL_TAG_arcsec,
	0, // FBL_TAG_arcsech,
	0, // FBL_TAG_arcsinh,
	0, // FBL_TAG_arctanh,
	0, // FBL_TAG_mean,
	1, // FBL_TAG_sdev,
	1, // FBL_TAG_variance,
	1, // FBL_TAG_median,
	1, // FBL_TAG_mode,
	0, // FBL_TAG_moment,
	0, // FBL_TAG_momentabout,
	0, // FBL_TAG_vector,
	0, // FBL_TAG_matrix,
	0, // FBL_TAG_matrixrow,
	0, // FBL_TAG_determinant,
	0, // FBL_TAG_transpose,
	0, // FBL_TAG_selector,
	0, // FBL_TAG_vectorproduct,
	0, // FBL_TAG_scalarproduct,
	0, // FBL_TAG_outerproduct,
	0, // FBL_TAG_annotation,
	0, // FBL_TAG_semantics,
	0, // FBL_TAG_annotation_xml,
	0, // FBL_TAG_integers,
	0, // FBL_TAG_reals,
	0, // FBL_TAG_rationals,
	0, // FBL_TAG_naturalnumbers,
	0, // FBL_TAG_complexes,
	0, // FBL_TAG_primes,
	0, // FBL_TAG_exponentiale,
	0, // FBL_TAG_imaginaryi,
	0, // FBL_TAG_notanumber,
	0, // FBL_TAG_true,
	0, // FBL_TAG_false,
	0, // FBL_TAG_emptyset,
	0, // FBL_TAG_pi,
	0, // FBL_TAG_eulergamma,
	0, // FBL_TAG_infinity
	0 // FBL_TAG_nop
};  

int mml_getCM2isOp( enum TAG2MML tag_id )
{
	if( tag_id < TAG2MML_math || tag_id >= TAG2MML_LAST )
		return 0;

	return !anCM2Fenced[ tag_id ];
}

///////////////////////////////////////////////////////////////////////////////
// Returns 0 for associative or having no such property at all, -1 for not assoc, but left grouping; +1 for no assoc, but right grouping
///////////////////////////////////////////////////////////////////////////////

short mml_getAssoc( enum TAG2MML tag_id, long /*argc*/, QString opBody )
{
	switch( tag_id )
	{
	case TAG2MML_minus:
	case TAG2MML_divide:
	case TAG2MML_vectorproduct:
	case TAG2MML_setdiff:
	case TAG2MML_not:
		return -1;
	case TAG2MML_power:
		return 1;
	case TAG2MML_csymbol:
		if( opBody.length() )
		{
			if( ::mml_isAssignOpBody( opBody ) )
				return 1;
		}
		break;
	default:
		break;
	}
	return 0;
}

bool mml_shouldWrapByPrecAssoc( long parent_precedence, long self_precedence, short parent_assoc, short /*self_assoc*/, short position )
{
	if( parent_precedence < self_precedence )
		return true;
	else if( parent_precedence > self_precedence )
		return false;
	else
	{
		if( parent_assoc == 0 )
			return false;
		else if( parent_assoc < 0 ) // left grouping
		{
			return (position < 0) ? false : true;
		}
		else // right grouping
		{
			return (position > 0) ? false : true;
		}
	}
}

long mml_getPrecedence( enum TAG2MML tag_id, long argc, QString opBody )
{
	if( argc < 1 ) return mmlPrecedence_Atom;

	long ret = mmlPrecedence_apply;
	switch( tag_id )
	{
	case TAG2MML_implies:
		ret = mmlPrecedence_implies;
		break;
	case TAG2MML_tendsto:
		ret = mmlPrecedence_tendsto;
		break;

	case TAG2MML_not:
	case TAG2MML_factorial:
		ret = mmlPrecedence_unary;
		break;
	case TAG2MML_minus:
	case TAG2MML_plus:
		if( argc == 1 )
			ret = mmlPrecedence_add; //mmlPrecedence_mult;//mmlPrecedence_unary;
		else
			ret = mmlPrecedence_add;
		break;

	case TAG2MML_times:
	//case TAG2MML_divide:
	case TAG2MML_rem:
	case TAG2MML_cartesianproduct:
	case TAG2MML_vectorproduct:
	case TAG2MML_scalarproduct:
	case TAG2MML_outerproduct:
		ret = mmlPrecedence_mult;
		break;

	case TAG2MML_union:
	case TAG2MML_intersect:
	case TAG2MML_setdiff:
		ret = mmlPrecedence_intersect;
		break;

	case TAG2MML_in:
	case TAG2MML_notin:
	case TAG2MML_subset:
	case TAG2MML_prsubset:
	case TAG2MML_notsubset:
	case TAG2MML_notprsubset:
		ret = mmlPrecedence_in;
		break;

	case TAG2MML_gt:
	case TAG2MML_lt:
		ret = mmlPrecedence_gt;
		break;

	case TAG2MML_geq:
	case TAG2MML_leq:
		ret = mmlPrecedence_geq;
		break;

	case TAG2MML_eq:
	case TAG2MML_neq:
	case TAG2MML_equivalent:
	case TAG2MML_approx:
		ret = mmlPrecedence_eq;
		break;

	case TAG2MML_and:
	case TAG2MML_or:
	case TAG2MML_xor:
		ret = mmlPrecedence_and;
		break;

	case TAG2MML_factorof:
		ret = mmlPrecedence_factorof;
		break;

	case TAG2MML_compose:
		ret = mmlPrecedence_compose;
		break;

	case TAG2MML_forall:
	case TAG2MML_exists:
		ret = mmlPrecedence_Last;
		break;

	case TAG2MML_nop:
		ret = mmlPrecedence_and;//2do mmlPrecedence_unary;
		break;

	case TAG2MML_csymbol:
		if( opBody.length() )
		{
			if( ::mml_isAssignOpBody( opBody ) )
				ret = mmlPrecedence_assign;
		}
		break;
	default:
		break;
	}

	return ret;
}

///////////////////////////////////////////////////////////////////////////////

long anCM2ApplyForm[ TAG2MML_LAST ] =
{
	__APPLY_RENDER_FORM_FENCED_FUN, // FBL_TAG_math,
	__APPLY_RENDER_FORM_FENCED_FUN, // FBL_TAG_mrow,
	__APPLY_RENDER_FORM_FENCED_FUN, // FBL_TAG_malignmark,
	__APPLY_RENDER_FORM_FENCED_FUN, // FBL_TAG_maligngroup,
	__APPLY_RENDER_FORM_FENCED_FUN, // FBL_TAG_mlabeledtr,
	__APPLY_RENDER_FORM_FENCED_FUN, // FBL_TAG_mn,
	__APPLY_RENDER_FORM_FENCED_FUN, // FBL_TAG_mo,
	__APPLY_RENDER_FORM_FENCED_FUN, // FBL_TAG_mi,
	__APPLY_RENDER_FORM_FENCED_FUN, // FBL_TAG_mtext,
	__APPLY_RENDER_FORM_FENCED_FUN, // FBL_TAG_ms,
	__APPLY_RENDER_FORM_FENCED_FUN, // FBL_TAG_mspace,
	__APPLY_RENDER_FORM_FENCED_FUN, // FBL_TAG_mtable,
	__APPLY_RENDER_FORM_FENCED_FUN, // FBL_TAG_mtr,
	__APPLY_RENDER_FORM_FENCED_FUN, // FBL_TAG_mtd,
	__APPLY_RENDER_FORM_FENCED_FUN, // FBL_TAG_mfenced,
	__APPLY_RENDER_FORM_FENCED_FUN, // FBL_TAG_mfrac,
	__APPLY_RENDER_FORM_FENCED_FUN, // FBL_TAG_mstyle,
	__APPLY_RENDER_FORM_FENCED_FUN, // FBL_TAG_mglyph,
	__APPLY_RENDER_FORM_FENCED_FUN, // FBL_TAG_msqrt,
	__APPLY_RENDER_FORM_FENCED_FUN, // FBL_TAG_merror,
	__APPLY_RENDER_FORM_FENCED_FUN, // FBL_TAG_menclose,
	__APPLY_RENDER_FORM_FENCED_FUN, // FBL_TAG_mpadded,
	__APPLY_RENDER_FORM_FENCED_FUN, // FBL_TAG_mphantom,
	__APPLY_RENDER_FORM_FENCED_FUN, // FBL_TAG_maction,
	__APPLY_RENDER_FORM_FENCED_FUN, // FBL_TAG_munder,
	__APPLY_RENDER_FORM_FENCED_FUN, // FBL_TAG_mover,
	__APPLY_RENDER_FORM_FENCED_FUN, // FBL_TAG_munderover,
	__APPLY_RENDER_FORM_FENCED_FUN, // FBL_TAG_msup,
	__APPLY_RENDER_FORM_FENCED_FUN, // FBL_TAG_msub,
	__APPLY_RENDER_FORM_FENCED_FUN, // FBL_TAG_mmultiscripts,
	__APPLY_RENDER_FORM_FENCED_FUN, // FBL_TAG_cn,
	__APPLY_RENDER_FORM_FENCED_FUN, // FBL_TAG_ci,
	__APPLY_RENDER_FORM_FENCED_FUN, // FBL_TAG_csymbol,
	__APPLY_RENDER_FORM_FENCED_FUN, // FBL_TAG_apply,
	__APPLY_RENDER_FORM_FENCED_FUN, // FBL_TAG_reln,
	__APPLY_RENDER_FORM_FENCED_FUN, // FBL_TAG_fn,
	__APPLY_RENDER_FORM_FENCED_FUN, // FBL_TAG_interval,
	__APPLY_RENDER_SPECIAL, // FBL_TAG_inverse,
	__APPLY_RENDER_FORM_FENCED_FUN, // FBL_TAG_sep,
	__APPLY_RENDER_FORM_FENCED_FUN, // FBL_TAG_condition,
	__APPLY_RENDER_FORM_FENCED_FUN, // FBL_TAG_declare,
	__APPLY_RENDER_FORM_FENCED_FUN, // FBL_TAG_lambda,
	__APPLY_RENDER_FORM_OP, // FBL_TAG_compose,
	__APPLY_RENDER_FORM_FENCED_FUN, // FBL_TAG_ident,
	__APPLY_RENDER_FORM_FENCED_FUN, // FBL_TAG_domain,
	__APPLY_RENDER_FORM_FENCED_FUN, // FBL_TAG_codomain,
	__APPLY_RENDER_FORM_FENCED_FUN, // FBL_TAG_image,
	__APPLY_RENDER_FORM_FENCED_FUN, // FBL_TAG_domainofapplication,
	__APPLY_RENDER_FORM_FENCED_FUN, // FBL_TAG_piecewise,
	__APPLY_RENDER_FORM_FENCED_FUN, // FBL_TAG_piece,
	__APPLY_RENDER_FORM_FENCED_FUN, // FBL_TAG_otherwise,
	__APPLY_RENDER_SPECIAL, // FBL_TAG_quotient,
	__APPLY_RENDER_SPECIAL, // FBL_TAG_exp,
	__APPLY_RENDER_FORM_OP, // FBL_TAG_factorial,
	__APPLY_RENDER_SPECIAL, // FBL_TAG_divide,
	__APPLY_RENDER_SPECIAL, // FBL_TAG_max,
	__APPLY_RENDER_SPECIAL, // FBL_TAG_min,
	__APPLY_RENDER_FORM_OP, // FBL_TAG_minus,
	__APPLY_RENDER_FORM_OP, // FBL_TAG_plus,
	__APPLY_RENDER_SPECIAL, // FBL_TAG_power,
	__APPLY_RENDER_FORM_OP, // FBL_TAG_rem,
	__APPLY_RENDER_FORM_OP, // FBL_TAG_times,
	__APPLY_RENDER_SPECIAL, // FBL_TAG_root,
	__APPLY_RENDER_FORM_FENCED_FUN, // FBL_TAG_gcd,
	__APPLY_RENDER_FORM_OP, // FBL_TAG_and,
	__APPLY_RENDER_FORM_OP, // FBL_TAG_or,
	__APPLY_RENDER_FORM_OP, // FBL_TAG_xor,
	__APPLY_RENDER_FORM_OP, // FBL_TAG_not,
	__APPLY_RENDER_FORM_OP, // FBL_TAG_implies,
	__APPLY_RENDER_SPECIAL, // FBL_TAG_forall,
	__APPLY_RENDER_SPECIAL, // FBL_TAG_exists,
	__APPLY_RENDER_SPECIAL, // FBL_TAG_abs,
	__APPLY_RENDER_SPECIAL, // FBL_TAG_conjugate,
	__APPLY_RENDER_FORM_NONFENCED_FUN, // FBL_TAG_arg,
	__APPLY_RENDER_FORM_NONFENCED_FUN, // FBL_TAG_real,
	__APPLY_RENDER_FORM_NONFENCED_FUN, // FBL_TAG_imaginary,
	__APPLY_RENDER_FORM_FENCED_FUN, // FBL_TAG_lcm,
	__APPLY_RENDER_SPECIAL, // FBL_TAG_floor,
	__APPLY_RENDER_SPECIAL, // FBL_TAG_ceiling,
	__APPLY_RENDER_FORM_OP, // FBL_TAG_eq,
	__APPLY_RENDER_FORM_OP, // FBL_TAG_neq,
	__APPLY_RENDER_FORM_OP, // FBL_TAG_gt,
	__APPLY_RENDER_FORM_OP, // FBL_TAG_lt,
	__APPLY_RENDER_FORM_OP, // FBL_TAG_geq,
	__APPLY_RENDER_FORM_OP, // FBL_TAG_leq,
	__APPLY_RENDER_FORM_OP, // FBL_TAG_equivalent,
	__APPLY_RENDER_FORM_OP, // FBL_TAG_approx,
	__APPLY_RENDER_FORM_OP, // FBL_TAG_factorof,
	__APPLY_RENDER_SPECIAL, // FBL_TAG_int,
	__APPLY_RENDER_SPECIAL, // FBL_TAG_diff,
	__APPLY_RENDER_SPECIAL, // FBL_TAG_partialdiff,
	__APPLY_RENDER_FORM_FENCED_FUN, // FBL_TAG_lowlimit,
	__APPLY_RENDER_FORM_FENCED_FUN, // FBL_TAG_uplimit,
	__APPLY_RENDER_FORM_FENCED_FUN, // FBL_TAG_bvar,
	__APPLY_RENDER_FORM_FENCED_FUN, // FBL_TAG_degree,
	__APPLY_RENDER_FORM_FENCED_FUN, // FBL_TAG_logbase
	__APPLY_RENDER_SPECIAL, // FBL_TAG_divergence,
	__APPLY_RENDER_SPECIAL, // FBL_TAG_grad,
	__APPLY_RENDER_SPECIAL, // FBL_TAG_curl,
	__APPLY_RENDER_SPECIAL, // FBL_TAG_laplacian,
	__APPLY_RENDER_FORM_FENCED_FUN, // FBL_TAG_set,
	__APPLY_RENDER_FORM_FENCED_FUN, // FBL_TAG_list,
	__APPLY_RENDER_FORM_OP, // FBL_TAG_union,
	__APPLY_RENDER_FORM_OP, // FBL_TAG_intersect,
	__APPLY_RENDER_FORM_OP, // FBL_TAG_in,
	__APPLY_RENDER_FORM_OP, // FBL_TAG_notin,
	__APPLY_RENDER_FORM_OP, // FBL_TAG_subset,
	__APPLY_RENDER_FORM_OP, // FBL_TAG_prsubset,
	__APPLY_RENDER_FORM_OP, // FBL_TAG_notsubset,
	__APPLY_RENDER_FORM_OP, // FBL_TAG_notprsubset,
	__APPLY_RENDER_FORM_OP, // FBL_TAG_setdiff,
	__APPLY_RENDER_SPECIAL, // FBL_TAG_card,
	__APPLY_RENDER_FORM_OP, // FBL_TAG_cartesianproduct,
	__APPLY_RENDER_SPECIAL, // FBL_TAG_sum,
	__APPLY_RENDER_SPECIAL, // FBL_TAG_product,
	__APPLY_RENDER_SPECIAL, // FBL_TAG_limit,
	__APPLY_RENDER_FORM_OP, // FBL_TAG_tendsto,
	__APPLY_RENDER_FORM_NONFENCED_FUN, // FBL_TAG_ln,
	__APPLY_RENDER_SPECIAL, // FBL_TAG_log,
	__APPLY_RENDER_FORM_NONFENCED_FUN, // FBL_TAG_sin,
	__APPLY_RENDER_FORM_NONFENCED_FUN, // FBL_TAG_cos,
	__APPLY_RENDER_FORM_NONFENCED_FUN, // FBL_TAG_tan,
	__APPLY_RENDER_FORM_NONFENCED_FUN, // FBL_TAG_sec,
	__APPLY_RENDER_FORM_NONFENCED_FUN, // FBL_TAG_csc,
	__APPLY_RENDER_FORM_NONFENCED_FUN, // FBL_TAG_cot,
	__APPLY_RENDER_FORM_NONFENCED_FUN, // FBL_TAG_sinh,
	__APPLY_RENDER_FORM_NONFENCED_FUN, // FBL_TAG_cosh,
	__APPLY_RENDER_FORM_NONFENCED_FUN, // FBL_TAG_tanh,
	__APPLY_RENDER_FORM_NONFENCED_FUN, // FBL_TAG_sech,
	__APPLY_RENDER_FORM_NONFENCED_FUN, // FBL_TAG_csch,
	__APPLY_RENDER_FORM_NONFENCED_FUN, // FBL_TAG_coth,
	__APPLY_RENDER_FORM_NONFENCED_FUN, // FBL_TAG_arcsin,
	__APPLY_RENDER_FORM_NONFENCED_FUN, // FBL_TAG_arccos,
	__APPLY_RENDER_FORM_NONFENCED_FUN, // FBL_TAG_arctan,
	__APPLY_RENDER_FORM_NONFENCED_FUN, // FBL_TAG_arccosh,
	__APPLY_RENDER_FORM_NONFENCED_FUN, // FBL_TAG_arccot,
	__APPLY_RENDER_FORM_NONFENCED_FUN, // FBL_TAG_arccoth,
	__APPLY_RENDER_FORM_NONFENCED_FUN, // FBL_TAG_arccsc,
	__APPLY_RENDER_FORM_NONFENCED_FUN, // FBL_TAG_arccsch,
	__APPLY_RENDER_FORM_NONFENCED_FUN, // FBL_TAG_arcsec,
	__APPLY_RENDER_FORM_NONFENCED_FUN, // FBL_TAG_arcsech,
	__APPLY_RENDER_FORM_NONFENCED_FUN, // FBL_TAG_arcsinh,
	__APPLY_RENDER_FORM_NONFENCED_FUN, // FBL_TAG_arctanh,
	__APPLY_RENDER_SPECIAL, // FBL_TAG_mean,
	__APPLY_RENDER_FORM_FENCED_FUN, // FBL_TAG_sdev,
	__APPLY_RENDER_FORM_FENCED_FUN, // FBL_TAG_variance,
	__APPLY_RENDER_FORM_FENCED_FUN, // FBL_TAG_median,
	__APPLY_RENDER_FORM_FENCED_FUN, // FBL_TAG_mode,
	__APPLY_RENDER_SPECIAL, // FBL_TAG_moment,
	__APPLY_RENDER_FORM_FENCED_FUN, // FBL_TAG_momentabout,
	__APPLY_RENDER_FORM_FENCED_FUN, // FBL_TAG_vector,
	__APPLY_RENDER_FORM_FENCED_FUN, // FBL_TAG_matrix,
	__APPLY_RENDER_FORM_FENCED_FUN, // FBL_TAG_matrixrow,
	__APPLY_RENDER_FORM_NONFENCED_FUN, // FBL_TAG_determinant,
	__APPLY_RENDER_SPECIAL, // FBL_TAG_transpose,
	__APPLY_RENDER_SPECIAL, // FBL_TAG_selector,
	__APPLY_RENDER_FORM_OP, // FBL_TAG_vectorproduct,
	__APPLY_RENDER_FORM_OP, // FBL_TAG_scalarproduct,
	__APPLY_RENDER_FORM_OP, // FBL_TAG_outerproduct,
	__APPLY_RENDER_FORM_FENCED_FUN, // FBL_TAG_annotation,
	__APPLY_RENDER_FORM_FENCED_FUN, // FBL_TAG_semantics,
	__APPLY_RENDER_FORM_FENCED_FUN, // FBL_TAG_annotation_xml,
	__APPLY_RENDER_FORM_FENCED_FUN, // FBL_TAG_integers,
	__APPLY_RENDER_FORM_FENCED_FUN, // FBL_TAG_reals,
	__APPLY_RENDER_FORM_FENCED_FUN, // FBL_TAG_rationals,
	__APPLY_RENDER_FORM_FENCED_FUN, // FBL_TAG_naturalnumbers,
	__APPLY_RENDER_FORM_FENCED_FUN, // FBL_TAG_complexes,
	__APPLY_RENDER_FORM_FENCED_FUN, // FBL_TAG_primes,
	__APPLY_RENDER_FORM_FENCED_FUN, // FBL_TAG_exponentiale,
	__APPLY_RENDER_FORM_FENCED_FUN, // FBL_TAG_imaginaryi,
	__APPLY_RENDER_FORM_FENCED_FUN, // FBL_TAG_notanumber,
	__APPLY_RENDER_FORM_FENCED_FUN, // FBL_TAG_true,
	__APPLY_RENDER_FORM_FENCED_FUN, // FBL_TAG_false,
	__APPLY_RENDER_FORM_FENCED_FUN, // FBL_TAG_emptyset,
	__APPLY_RENDER_FORM_FENCED_FUN, // FBL_TAG_pi,
	__APPLY_RENDER_FORM_FENCED_FUN, // FBL_TAG_eulergamma,
	__APPLY_RENDER_FORM_FENCED_FUN,	// FBL_TAG_infinity
	__APPLY_RENDER_FORM_OP // FBL_TAG_nop
};  

long mml_getApplyForm( enum TAG2MML tag_id )
{
	if( tag_id < TAG2MML_math || tag_id >= TAG2MML_LAST )
		return __APPLY_RENDER_SPECIAL;

	return anCM2ApplyForm[ tag_id ];
}

///////////////////////////////////////////////////////////////////////////////

void mml_get_cn_attr( AST_TXML_Tag *node, QString& base, long& typeidx )
{
	static QString cn_types[] = 
	{
		QString( _T("real") ),
		QString( _T("integer") ),
		QString( _T("rational") ),
		QString( _T("complex-cartesian") ),
		QString( _T("complex-polar") ),
		QString( _T("constant") )
	};
	static long sizearr = sizeof(cn_types) / sizeof(cn_types[0]);
	static HKMap2Long<QString> cn2type(cn_types, sizearr);

	base = CONTENT_MATHML_CN_DEFAULT_BASE_STR;
	typeidx = 0;

	AST_TXML_Attribute *base_attr = node->getAttribute( FBL_ATTR_base );
	if( base_attr )
	{
		QString bs = base_attr->getStringValue();
		long _b = bs.length();
		if( _b >= 2 && _b <= 36 )
			base = bs;
	}

	AST_TXML_Attribute *type_attr = node->getAttribute( FBL_ATTR_type );
	if( type_attr )
	{
		long *p = cn2type.isFind(type_attr->getStringValue());
		if( p ) typeidx = *p;
	}
}

int mml_get_CM2PM_simple( enum TAG2MML tag_id, AST_TXML_Tag *xml_node, QString& newtag, QString& newtext, std::vector<QString>& /*attr*/ )
{
	newtag  = ::mml_getCM2PMTag_standalone( tag_id, xml_node );
	newtext = ::mml_getCM2PMText_standalone( tag_id, xml_node );
	if( newtag.length() && newtext.length() )
		return 1;
	return 0;
}

void mml_UsePreference_ContentTimes( long contentTimes )
{
	switch( contentTimes )
	{
	case TRANSLATE_CONTENT_MATHML2_SDOT:
		szCMTag2PMText_standalone[ TAG2MML_times ] = _T("&sdot;");
		break;
	case TRANSLATE_CONTENT_MATHML2_INVISIBLETIMES:
		szCMTag2PMText_standalone[ TAG2MML_times ] = _T("&InvisibleTimes;");
		break;
	default:
		szCMTag2PMText_standalone[ TAG2MML_times ] = _T("&times;");
	}
}

int mml_GetPreference_ContentTimes()
{
	if( szCMTag2PMText_standalone[ TAG2MML_times ] == _T("&sdot;") )
		return TRANSLATE_CONTENT_MATHML2_SDOT;
	else if( szCMTag2PMText_standalone[ TAG2MML_times ] == _T("&InvisibleTimes;") )
		return TRANSLATE_CONTENT_MATHML2_INVISIBLETIMES;
	return TRANSLATE_CONTENT_MATHML2_TIMES;
}

///////////////////////////////////////////////////////////////////////////////
