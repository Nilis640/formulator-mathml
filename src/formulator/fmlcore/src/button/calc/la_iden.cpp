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

#include "la_iden.h"

const static QString kw_abs		= _T("abs");
const static QString kw_av		= _T("average");
const static QString kw_min		= _T("min");
const static QString kw_max		= _T("max");

long LIdenLeapes[ LIdenStates * LIdenAlphabet ] = {
	//	l  d
	//  ~~~~
	2, 0,
	2, 2
};

long LIdensEndStates[ LIdenStates ] = {
	0, LIden::Identifier
};

long LIden::What( const QChar S )
//              ~~~~
{
	for(;;)
	{
		if( S.isLetter() || (S == QChar('_')) )	return 1;
		if( S.isDigit() )						return 2;
		return 0;
	}
}

void LIden::Deeds()
//          ~~~~~
{
	switch( CurrentState )
	{
	case 1 :
		identifier = _T("");
		break;
	case 2 :
		identifier += character;
		break;
	default:
		break;
	}
}

int LIden::Analisys( const QStringRef& aText )
//         ~~~~~~~~
{
	int ret = LAuto::Analisys( aText );
	if( ret )
	{
		if( identifier == kw_abs )
			return LIden::KW_Abs;
		else if( identifier == kw_av )
			return LIden::KW_Av;
		else if( identifier == kw_min )
			return LIden::KW_Min;
		else if( identifier == kw_max )
			return LIden::KW_Max;
	}
	return ret;
}
