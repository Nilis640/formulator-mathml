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

#include "HUtils/iustring.h"
#include "la_dsr2var.h"

const static QString kw_begin				= _T("begin");
const static QString kw_end					= _T("end");
const static QString kw_if					= _T("if");
const static QString kw_then				= _T("then");
const static QString kw_else				= _T("else");
const static QString kw_function			= _T("function");
const static QString kw_group				= _T("group");
const static QString kw_polynomial			= _T("polynomial");
const static QString kw_rational_function	= _T("rational");

long LDsr2VarLeapes[ LDsr2VarStates * LDsr2VarAlphabet ] =
{
//	l  d  $
//  ~~~~~~~
	2, 0, 0, 
	2, 2, 3,
	4, 0, 0,
	4, 4, 0
};

long LDsr2VarsEndStates[ LDsr2VarStates ] =
{
	0, LDsr2Var::Identifier, 0, LDsr2Var::Identifier
};

long LDsr2Var::What( const QChar S )
//             ~~~~
{
	for(;;)
	{
		if( S.isLetter() || (S == _T('_')) )
			return 1;
		if( S.isDigit() )
			return 2;
		if( S == _T('$') )
			return 3;
		return 0;
	}
}

void LDsr2Var::Deeds( void )
//             ~~~~~
{
	switch( CurrentState )
	{
	case 1 :
		identifier = _T("");
		break;
	case 2 :
		identifier += character;
		break;
	case 3 :
		modifier = _T("");
		break;
	case 4 :
		modifier += character;
		break;
	}
}

int LDsr2Var::Analisys( const QStringRef& aText )
//            ~~~~~~~~
{
	int ret = LAuto::Analisys( aText );
	if( ret )
	{
		if( identifier == kw_begin )
			return LDsr2Var::KW_begin;
		else if( identifier == kw_end )
			return LDsr2Var::KW_end;
		else if( identifier == kw_if )
			return LDsr2Var::KW_if;
		else if( identifier == kw_then )
			return LDsr2Var::KW_then;
		else if( identifier == kw_else )
			return LDsr2Var::KW_else;
		else if( identifier == kw_function )
			return LDsr2Var::KW_function;
		else if( identifier == kw_group )
			return LDsr2Var::KW_group;
		else if( identifier == kw_polynomial )
			return LDsr2Var::KW_polynomial;
		else if( identifier == kw_rational_function )
			return LDsr2Var::KW_rational_function;
	}
	return ret;
}

LDsr2Var::LDsr2Var( void )
//        ~~~~~~~~
	: LAuto( LDsr2VarStates, LDsr2VarAlphabet, LDsr2VarLeapes, LDsr2VarsEndStates )
	, keywords(32, 0, 32)
{
	identifier = _T("");
	modifier = _T("");

	keywords[ LDsr2Var::KW_begin ] = kw_begin;
	keywords[ LDsr2Var::KW_end ] = kw_end;
	keywords[ LDsr2Var::KW_if ] = kw_if;
	keywords[ LDsr2Var::KW_then ] = kw_then;
	keywords[ LDsr2Var::KW_else ] = kw_else;
	keywords[ LDsr2Var::KW_function ] = kw_function;
	keywords[ LDsr2Var::KW_group ] = kw_group;
	keywords[ LDsr2Var::KW_polynomial ] = kw_polynomial;
	keywords[ LDsr2Var::KW_rational_function ] = kw_rational_function;
}
