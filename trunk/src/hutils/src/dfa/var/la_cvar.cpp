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
#include "la_cvar.h"

long LCVarLeapes[ LCVarStates * LCVarAlphabet ] =
{
//  l  d  
	2, 0, 
	2, 2 
};

long LCVarsEndStates[ LCVarStates ] =
{
	0, 1
};

long LCVar::What( const QChar S )
//          ~~~~
{
	for(;;)
	{
		if( S.isLetter() || ( S == _T('_') ) )
			return 1;
		if( S.isDigit() )
			return 2;
		return 0;
	}
}

void LCVar::Deeds( void )
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
	}
}

LCVar::LCVar( void )
//    ~~~~
: LAuto( LCVarStates, LCVarAlphabet, LCVarLeapes, LCVarsEndStates )
{
	identifier = _T("");
}
