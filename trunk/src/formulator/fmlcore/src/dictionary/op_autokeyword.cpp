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

#include "HUtils/idfas.h"
#include "../settings/options.h"
#include "../button/toolbarset.h"
#include "op_autokeyword.h"
#include "op_iddefs.h"

///////////////////////////////////////////////////////////////////////////////

#define autodetectCMKeywordPrefix_Length	168

static QString autodetectCMKeywordPrefix_Text[ autodetectCMKeywordPrefix_Length ] =
{
	_T("ab"),
	_T("abs"),
	_T("id"),
	_T("do"),
	_T("dom"),
	_T("doma"),
	_T("domai"),
	_T("domain"),
	_T("co"),
	_T("cod"),
	_T("codo"),
	_T("codom"),
	_T("codoma"),
	_T("codomai"),
	_T("codomain"),
	_T("im"),
	_T("ima"),
	_T("imag"),
	_T("image"),
	_T("ma"),
	_T("max"),
	_T("mi"),
	_T("min"),
	_T("mo"),
	_T("mod"),
	_T("gc"),
	_T("gcd"),
	_T("ar"),
	_T("arg"),
	_T("lc"),
	_T("lcm"),
	_T("di"),
	_T("div"),
	_T("cu"),
	_T("cur"),
	_T("curl"),
	_T("li"),
	_T("lim"),
	_T("me"),
	_T("med"),
	_T("medi"),
	_T("media"),
	_T("median"),
	_T("mo"),
	_T("mod"),
	_T("mode"),
	_T("de"),
	_T("det"),
	_T("Na"),
	_T("NaN"),
	_T("tr"),
	_T("tru"),
	_T("true"),
	_T("fa"),
	_T("fal"),
	_T("fals"),
	_T("false"),
	_T("di"),
	_T("div"),
	_T("gr"),
	_T("gra"),
	_T("grad"),
	_T("me"),
	_T("mea"),
	_T("mean"),
	_T("ex"),
	_T("exp"),
	_T("ln"),     
	_T("lo"),    
	_T("log"),    
	_T("si"),    
	_T("sin"),    
	_T("co"),    
	_T("cos"),    
	_T("ta"),    
	_T("tan"),    
	_T("se"),    
	_T("sec"),    
	_T("cs"),    
	_T("csc"),    
	_T("co"),    
	_T("cot"),    
	_T("si"),   
	_T("sin"),   
	_T("sinh"),   
	_T("co"),   
	_T("cos"),   
	_T("cosh"),   
	_T("ta"),   
	_T("tan"),   
	_T("tanh"),   
	_T("se"),   
	_T("sec"),   
	_T("sech"),   
	_T("cs"),   
	_T("csc"),   
	_T("csch"),   
	_T("co"),   
	_T("cot"),   
	_T("coth"),   
	_T("ar"), 
	_T("arc"), 
	_T("arcs"), 
	_T("arcsi"), 
	_T("arcsin"), 
	_T("ar"), 
	_T("arc"), 
	_T("arcc"), 
	_T("arcco"), 
	_T("arccos"), 
	_T("ar"), 
	_T("arc"), 
	_T("arct"), 
	_T("arcta"), 
	_T("arctan"), 
	_T("ar"),
	_T("arc"),
	_T("arcc"),
	_T("arcco"),
	_T("arccos"),
	_T("arccosh"),
	_T("ar"), 
	_T("arc"), 
	_T("arcc"), 
	_T("arcco"), 
	_T("arccot"), 
	_T("ar"),
	_T("arc"),
	_T("arcc"),
	_T("arcco"),
	_T("arccot"),
	_T("arccoth"),
	_T("ar"), 
	_T("arc"), 
	_T("arcc"), 
	_T("arccs"), 
	_T("arccsc"), 
	_T("ar"),
	_T("arc"),
	_T("arcc"),
	_T("arccs"),
	_T("arccsc"),
	_T("arccsch"),
	_T("ar"), 
	_T("arc"), 
	_T("arcs"), 
	_T("arcse"), 
	_T("arcsec"), 
	_T("ar"),
	_T("arc"),
	_T("arcs"),
	_T("arcse"),
	_T("arcsec"),
	_T("arcsech"),
	_T("ar"),
	_T("arc"),
	_T("arcs"),
	_T("arcsi"),
	_T("arcsin"),
	_T("arcsinh"),
	_T("ar"),
	_T("arc"),
	_T("arct"),
	_T("arcta"),
	_T("arctan"),
	_T("arctanh"),
	_T("p"),
	_T("pi")
};


#define autodetectCMKeyword_Length	75

static QString autodetectCMKeyword_Text[ autodetectCMKeyword_Length ] =
{
//	_T("{"),
	_T("^"),
	_T("\'"),
	_T("\""),
	_T(" -"),
	_T(" +"),
	_T("+-"),
	_T(" +-"),
	_T("/"),
	_T("-"),
	_T("+"),
	_T("*"),
	QString(QChar(0x2217)), // 0x2217
	_T("&"),
	_T("|"),
	_T(" !"),
	_T("="),
	_T("!="),
	_T(">"),
	_T("<"),
	_T(">="),
	_T("<="),
	_T("abs"),
	_T("id"),
	_T("domain"),
	_T("codomain"),
	_T("image"),
	_T("max"),
	_T("min"),
	_T("mod"),
	_T("gcd"),
	_T("arg"),
	_T("lcm"),
	_T("div"),
	_T("curl"),
	_T("lim"),
	_T("median"),
	_T("mode"),
	_T("det"),
	_T("NaN"),
	_T("true"),
	_T("false"),
	_T("div"),
	_T("grad"),
	_T("mean"),
	_T("exp"),
	_T("ln"),     
	_T("log"),    
	_T("sin"),    
	_T("cos"),    
	_T("tan"),    
	_T("sec"),    
	_T("csc"),    
	_T("cot"),    
	_T("sinh"),   
	_T("cosh"),   
	_T("tanh"),   
	_T("sech"),   
	_T("csch"),   
	_T("coth"),   
	_T("arcsin"), 
	_T("arccos"), 
	_T("arctan"), 
	_T("arccosh"),
	_T("arccot"), 
	_T("arccoth"),
	_T("arccsc"), 
	_T("arccsch"),
	_T("arcsec"), 
	_T("arcsech"),
	_T("arcsinh"),
	_T("arctanh"),
	HF_BTNID_NOP_VALUE,
	HF_BTNID_COLONEQ_VALUE,
	MML_OPERATOR_HOTKEY_ASSIGN,
	_T("pi")
};

static QString autodetectCMKeyword_ButtonID[ autodetectCMKeyword_Length ] =
{
//	_T("@piecewise1_ask_1"),// {
	_T("@cm_power2"),		// ^
	_T("@cm_diff1_degree1"),// '
	_T("@cm_diff3_degree2"),// "
	_T("@cm_minus1"),		//  -
	_T("@cm_plus1"),		//  +
	_T("@cm_plus_minus2"),	// +=
	_T("@cm_plus_minus1"),	//  +=
	_T("@cm_divide2"),		// /
	_T("@cm_minus2"),		// -
	_T("@cm_plus2"),		// +
	_T("@cm_times2"),		// *
	_T("@cm_times2"),		// 0x2217
	_T("@and2"),			// &
	_T("@or2"),				// |
	_T("@cm_factorial1"),	// !
	_T("@cm_eq2"),			// =
	_T("@cm_neq2"),			// !=
	_T("@cm_gt2"),			// >
	_T("@cm_lt2"),			// <
	_T("@cm_geq2"),			// >=
	_T("@cm_leq2"),			// <=
	_T("@cm_abs1"),
	_T("@ident"),
	_T("@domain_apply"),
	_T("@codomain_apply"),
    _T("@image"),
    _T("@cm_max2"),
    _T("@cm_min2"),
    _T("@cm_rem2"),
    _T("@cm_gcd2"),
    _T("@cm_arg1"),
    _T("@cm_lcm2"),
    _T("@cm_divergence1"),
    _T("@cm_curl1"),
    _T("@cm_limit1"),
    _T("@cm_median2"),
    _T("@cm_mode2"),
    _T("@determinant_apply"),
	_T("@notanumber_cm"),
	_T("@true"),
	_T("@false"),
	_T("@cm_divergence1"),
	_T("@cm_grad1"),
	_T("@cm_mean2"),
	_T("@exp_apply"),
	_T("@ln_apply"),     
	_T("@log_apply"),    
	_T("@sin_apply"),    
	_T("@cos_apply"),    
	_T("@tan_apply"),    
	_T("@sec_apply"),    
	_T("@csc_apply"),    
	_T("@cot_apply"),    
	_T("@sinh_apply"),   
	_T("@cosh_apply"),   
	_T("@tanh_apply"),   
	_T("@sech_apply"),   
	_T("@csch_apply"),   
	_T("@coth_apply"),   
	_T("@arcsin_apply"), 
	_T("@arccos_apply"), 
	_T("@arctan_apply"), 
	_T("@arccosh_apply"),
	_T("@arccot_apply"), 
	_T("@arccoth_apply"),
	_T("@arccsc_apply"), 
	_T("@arccsch_apply"),
	_T("@arcsec_apply"), 
	_T("@arcsech_apply"),
	_T("@arcsinh_apply"),
	_T("@arctanh_apply"),
	_T("@cm_nop2"),
	HF_BTNID_CMASSIGN2_ID,
	HF_BTNID_CMASSIGN2_ID,
	_T("@pi_cm")
};

static enum TAG2MML autodetectCMKeyword_Tag[ autodetectCMKeyword_Length ] =
{
//	TAG2MML_NONE,
    TAG2MML_power,
    TAG2MML_NONE,
    TAG2MML_NONE,
    TAG2MML_NONE,
    TAG2MML_NONE,
    TAG2MML_NONE,
    TAG2MML_NONE,
    TAG2MML_divide,
    TAG2MML_minus,
    TAG2MML_plus,
    TAG2MML_times,
    TAG2MML_times,
    TAG2MML_and,
    TAG2MML_or,
    TAG2MML_not,
    TAG2MML_eq,
    TAG2MML_neq,
    TAG2MML_gt,
    TAG2MML_lt,
    TAG2MML_geq,
    TAG2MML_leq,
	TAG2MML_abs,
	TAG2MML_ident,
	TAG2MML_domain,
	TAG2MML_codomain,
    TAG2MML_image,
    TAG2MML_max,
    TAG2MML_min,
    TAG2MML_rem,
    TAG2MML_gcd,
    TAG2MML_arg,
    TAG2MML_lcm,
    TAG2MML_divide,
    TAG2MML_curl,
    TAG2MML_limit,
    TAG2MML_median,
    TAG2MML_mode,
    TAG2MML_determinant,
	TAG2MML_notanumber,
	TAG2MML_true,
	TAG2MML_false,
	TAG2MML_divide,
	TAG2MML_grad,
	TAG2MML_mean,
	TAG2MML_exp,
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
	TAG2MML_nop,
	TAG2MML_csymbol,
	TAG2MML_csymbol,
    TAG2MML_pi
};

////////////////////////////////////////////////////////////////////////////////

#define autodetectCMOnChar_Length	18

static QChar autodetectCMOnChar_Text[ autodetectCMOnChar_Length ] =
{
	//_T('('),
	_T('['),
	_T('{'),
	_T('!'),
	_T('/'),
	_T('-'),
	_T('+'),
	_T('*'),
	0x2217,
	_T('^'),
	_T('&'),
	_T('|'),
	_T('='),
	_T('>'),
	_T('<'),
	_T('\''),
	_T('"'),
	HF_BTNID_COLONEQ_VALUE,
	MML_OPERATOR_HOTKEY_ASSIGN
};

static QString autodetectCMOnChar_Tag[ autodetectCMOnChar_Length ] =
{
	//_T("@interval_o"),
	_T("@selector_apply2"),
	_T("@piecewise1_ask_1"),
	_T("@cm_factorial1"),
	_T("@cm_divide2"),
	_T("@cm_minus2"),
	_T("@cm_plus2"),
	_T("@cm_times2"),
	_T("@cm_times2"),
	_T("@cm_power2"),
	_T("@and2"),
	_T("@or2"),
	_T("@cm_eq2"),
	_T("@cm_gt2"),
	_T("@cm_lt2"),
	_T("@cm_diff1_degree1"),
	_T("@cm_diff3_degree2"),
	HF_BTNID_CMASSIGN2_ID,
	HF_BTNID_CMASSIGN2_ID
};

////////////////////////////////////////////////////////////////////////////////

#define autodetectCMButton_Length	40

static QString autodetectCMButton_Text[ autodetectCMButton_Length ] =
{
	_T("lambda"),
	_T("@exponentiale"),
	_T("@divide"),
	_T("@minus"),
	_T("@plus"),
	_T("@times"),
	_T("&"),
	_T("|"),
	_T("!"),
	_T("@darrow_r"),
	_T("@forall"),
	_T("@exist"),
	_T("real"),
	_T("imag"),
	_T("!="),
	_T(">="),
	_T("<="),
	_T("@identical"),
	_T("@almost_equal"),
	_T("@gradient"),
	_T("@smul"),
	_T("@in"),
	_T("@not_in"),
	_T("@set_in_eq"),
	_T("@set_in"),
	_T("@set_not_in"),
	_T("@sum"),
	_T("@product"),
	_T("@integers"),
	_T("@reals"),
	_T("@rationals"),
	_T("@naturals"),
	_T("@complexes"),
	_T("@exponentiale"),
	_T("@imaginaryi"),
	_T("@set_empty"),
	_T("pi"),
	_T("gamma"),
	_T("@infinity"),
	HF_BTNID_ASSIGN_ID
};

static enum TAG2MML autodetectCMButton_Tag[ autodetectCMButton_Length ] =
{
    TAG2MML_lambda,
    TAG2MML_exp,
    TAG2MML_divide,
    TAG2MML_minus,
    TAG2MML_plus,
    TAG2MML_times,
    TAG2MML_and,
    TAG2MML_or,
    TAG2MML_not,
    TAG2MML_implies,
    TAG2MML_forall,
    TAG2MML_exists,
    TAG2MML_real,
    TAG2MML_imaginary,
    TAG2MML_neq,
    TAG2MML_geq,
    TAG2MML_leq,
    TAG2MML_equivalent,
    TAG2MML_approx,
    TAG2MML_grad,
    TAG2MML_intersect,
    TAG2MML_in,
    TAG2MML_notin,
    TAG2MML_subset,
    TAG2MML_prsubset,
    TAG2MML_notprsubset,
    TAG2MML_sum,
    TAG2MML_product,
    TAG2MML_integers,
    TAG2MML_reals,
    TAG2MML_rationals,
    TAG2MML_naturalnumbers,
    TAG2MML_complexes,
    TAG2MML_exponentiale,
    TAG2MML_imaginaryi,
    TAG2MML_emptyset,
    TAG2MML_pi,
    TAG2MML_eulergamma,
    TAG2MML_infinity,
    TAG2MML_csymbol
};

////////////////////////////////////////////////////////////////////////////////

int detectContentTag( QString& name, QString& body, QString& /*sattr*/, QString& parsedBody, long parsedStyle )
{
	if( body.length() == 0 ) return -1;

	LSDigit ldigit;
	int res = ldigit.Analisys( body.midRef(0) );
	if( res > 0 )
	{
		name = FBL_TAG_cn;
		return 0;
	}

	switch( parsedStyle )
	{
	case FSTYLE_TEXT_BUTTON:
		{
			//std::string parsedBodyStd = parsedBody.toStdString();
			//long _toolbar_no = -1, _button_no = -1;
			//long len = sizeof(long) / sizeof(char);
			//std::copy( parsedBodyStd.begin(), parsedBodyStd.begin() + len, (char*) &_toolbar_no );
			//std::copy( parsedBodyStd.begin() + len, parsedBodyStd.begin() + len + len, (char*)&_button_no );
			//FXmlBtn *ptrFXmlBtn = ::getCurrentToolBarSet()->getButtonCalc( _toolbar_no, _button_no );
			FXmlBtn *ptrFXmlBtn = ::getCurrentToolBarSet()->getButtonCalc( parsedBody );
			if( !ptrFXmlBtn ) return -1;
			for( long i = 0; i < autodetectCMButton_Length; i++ )
			{
				if( ptrFXmlBtn->getID() == autodetectCMButton_Text[ i ] && autodetectCMButton_Tag[ i ] != TAG2MML_NONE )
				{
					enum TAG2MML dtagid = autodetectCMButton_Tag[ i ];
					name = ::mml_getTagID2TagName( dtagid );
					if( dtagid != TAG2MML_csymbol || ptrFXmlBtn->getID() != HF_BTNID_ASSIGN_ID )
						body = _T("");
					return 0;
				}
			}
			return -1;
		}
		break;

	case FSTYLE_FIXED:
	case FSTYLE_VECTOR:
	case FSTYLE_NUMBER:
	case FSTYLE_KEYWORD:
	case FSTYLE_REMARK:
	case FSTYLE_FUNCTION:
	case FSTYLE_OP:
	case FSTYLE_TEXT:
	case FSTYLE_MTEXTRA:
	case FSTYLE_USER1:
	case FSTYLE_USER2:
		{
			long i;
			for( i = 0; i < autodetectCMKeyword_Length; i++ )
			{
				if( parsedBody == autodetectCMKeyword_Text[ i ] && autodetectCMKeyword_Tag[ i ] != TAG2MML_NONE )
				{
					enum TAG2MML dtagid = autodetectCMKeyword_Tag[ i ];
					name = ::mml_getTagID2TagName( dtagid );
					if( !(dtagid == TAG2MML_csymbol && 
						  (parsedBody == QString(HF_BTNID_COLONEQ_VALUE) || parsedBody == MML_OPERATOR_HOTKEY_ASSIGN)) )
						body = _T("");
					return 0;
				}
			}
	    
			int iRet = 0;
			for( i = 0; i < parsedBody.length(); i++ )
				if( !parsedBody.at(i).isSpace() ) break;
			
			if( i < parsedBody.length() )
				name = FBL_TAG_ci;
			else
				iRet = -1;

			return iRet;
		}
	default:
		break;
	}

	return -1;
}

///////////////////////////////////////////////////////////////////////////////

class HAutodetectCMKeyword
{
protected:
	HKMap<QString, QString> m_text2buttonID;
	HKMap<QString, enum TAG2MML> m_text2tagID;
	LKeyword m_keywordList;

public:
	HAutodetectCMKeyword();
	void getCMKeyword2ButtonID( QString keyword, QString& btnID );
	enum TAG2MML getCMKeyword2TagID( QString keyword, QString& opBody );
	int isCMKeywordPrefix( QString keyword );
	const HKMap<QString, QString>& getText2ButtonIDMap();
};

HAutodetectCMKeyword::HAutodetectCMKeyword() : 
	m_keywordList( autodetectCMKeyword_Text, autodetectCMKeyword_Length, 1 /* build prefixes for keywords */ )
{
	for( long i = 0; i < autodetectCMKeyword_Length; i++ )
	{
		if( autodetectCMKeyword_ButtonID[ i ].length() > 0 )
		{
			m_text2buttonID[ autodetectCMKeyword_Text[ i ] ] = autodetectCMKeyword_ButtonID[ i ];
			m_text2tagID[ autodetectCMKeyword_Text[ i ] ] = autodetectCMKeyword_Tag[ i ];
		}
	}
}

void HAutodetectCMKeyword::getCMKeyword2ButtonID( QString keyword, QString& btnID )
{
	QString *id = m_text2buttonID.is_find( keyword );
	btnID = id ? *id : _T("");
}

enum TAG2MML HAutodetectCMKeyword::getCMKeyword2TagID( QString keyword, QString& opBody )
{
	enum TAG2MML *id = m_text2tagID.is_find( keyword );
	opBody = "";
	if( !id )
	{
		return TAG2MML_NONE;
	}
	else if( *id == TAG2MML_csymbol )
	{
		if( ::mml_isAssignOpHotkey( keyword ) )
			opBody = HF_BTNID_COLONEQ_ENTITY;
	}
	return *id;
}

int HAutodetectCMKeyword::isCMKeywordPrefix( QString keyword )
{
	m_keywordList.Analisys( keyword.midRef(0) );
	if( m_keywordList.GetRead() != keyword.length() )
	{
		m_keywordList.Reset();
		return 0;
	}
	long sz = m_keywordList.getKeywordList().size();
	m_keywordList.Reset();
	return sz > 0;
}

const HKMap<QString, QString>& HAutodetectCMKeyword::getText2ButtonIDMap()
{
	return m_text2buttonID;
}

///////////////////////////////////////////////////////////////////////////////

static HAutodetectCMKeyword text2buttonID;

long getCMKeyword2ButtonID( QString keyword, QString& btnID )
{
	text2buttonID.getCMKeyword2ButtonID( keyword, btnID );
	return btnID.length();
}

enum TAG2MML getCMKeyword2TagID( QString keyword, QString& opBody )
{
	return text2buttonID.getCMKeyword2TagID( keyword, opBody );
}

int isCMKeywordPrefix( QString keyword )
{
	return text2buttonID.isCMKeywordPrefix( keyword );
}

const HKMap<QString, QString>& getText2ButtonIDMap()
{
	return text2buttonID.getText2ButtonIDMap();
}

///////////////////////////////////////////////////////////////////////////////

