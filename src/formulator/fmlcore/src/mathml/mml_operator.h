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

#if !defined( __FORMULATOR_DICTIONARY_MML_OPERATOR_H__ )
#define __FORMULATOR_DICTIONARY_MML_OPERATOR_H__

///////////////////////////////////////////////////////////////////////////////

#include "../fmlcore_global.h"
#include "HUtils/ihkstring.h"

///////////////////////////////////////////////////////////////////////////////

enum mmlPrecedence
{
	mmlPrecedence_Atom = 0,

	mmlPrecedence_apply,
	mmlPrecedence_implies,
	mmlPrecedence_tendsto,
	mmlPrecedence_unary,	// minus plus factorial
	mmlPrecedence_mult,	 	// divide times <set-vector>product rem
	mmlPrecedence_add,		// mmlPrecedence_union
	mmlPrecedence_intersect,// mmlPrecedence_setdiff
	mmlPrecedence_in,		// notin notprsubset
	mmlPrecedence_gt,
	mmlPrecedence_geq = mmlPrecedence_gt,
	mmlPrecedence_eq,
	mmlPrecedence_and,
	mmlPrecedence_factorof,
	mmlPrecedence_compose,
	mmlPrecedence_assign,

	mmlPrecedence_Last,
};

///////////////////////////////////////////////////////////////////////////////

// Math operator reserved spacing
#define HMathOpAttr_SPACE_veryverythinmathspace		0.0555556
#define HMathOpAttr_SPACE_verythinmathspace			0.1111110
#define HMathOpAttr_SPACE_thinmathspace				0.1666670
#define HMathOpAttr_SPACE_mediummathspace			0.2222220
#define HMathOpAttr_SPACE_thickmathspace			0.2777780
#define HMathOpAttr_SPACE_verythickmathspace		0.3333330
#define HMathOpAttr_SPACE_veryverythickmathspace	0.3888890

// Math operator forms
#define HMathOpAttr_FORM_INFIX						0x00000000
#define HMathOpAttr_FORM_POSTFIX					0x00000001
#define HMathOpAttr_FORM_PREFIX						0x00000010

// Math operator minimal size
#define HMathOpAttr_MINSIZE_NULL					0

///////////////////////////////////////////////////////////////////////////////

struct __HERMITECH_FORMULIB__ HMathOpAttr
{
	//=== presentation
	QString mmlname;				//
	quint8 form;				// prefix, postfix, infix
	quint8 fence;			// type: is it a fence?
	quint8 separator;		// type: is it a separator?
	quint8 accent;			// type: is it a accent?
	quint8 largeop;			// type: is it a large operator? (ignored in HF 1.0)
	quint8 movablelimits;	// are limits movable? (ignored in HF 1.0)
	quint8 stretchy;		// cab be stretched?
	double lspace;			// left spacing (x/18 "em" units)
	double rspace;			// right spacing (x/18 "em" units)
	QString dsrname;		//
	long minsize;			// minimal size of an operator or -1 if it is undefined (ignored in HF 1.0)
	//=== content
	enum mmlPrecedence precedence;	// operator precedence

	HMathOpAttr(const QString& _mmlname = _T(""), 
		quint8 _form = HMathOpAttr_FORM_INFIX,
		quint8 _fence = 0,
		quint8 _separator = 0,
		quint8 _accent = 0,
		quint8 _largeop = 0,
		quint8 _movablelimits = 0,
		quint8 _stretchy = 0,
		double _lspace = 0.0,
		double _rspace = 0.0,
		const QString& _dsrname = _T(""),
		long _minsize = 1,
		enum mmlPrecedence _precedence = mmlPrecedence_Atom)
	{
		mmlname = _mmlname;
		form = _form;
		fence = _fence;
		separator = _separator;
		accent = _accent;
		largeop = _largeop;
		movablelimits = _movablelimits;
		stretchy = _stretchy;
		lspace = _lspace;
		rspace = _rspace;
		dsrname = _dsrname;
		minsize = _minsize;
		precedence = _precedence;
	}
};

#define HMathOpAttr_OPERATOR_NUMBER	555

///////////////////////////////////////////////////////////////////////////////

__HERMITECH_FORMULIB__
extern HMathOpAttr HMathOpAttr_OperatorDictionary[ HMathOpAttr_OPERATOR_NUMBER ];

///////////////////////////////////////////////////////////////////////////////

#endif
