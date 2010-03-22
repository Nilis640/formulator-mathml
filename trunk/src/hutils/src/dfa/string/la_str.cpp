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
#include "la_str.h"

long LStringLeapes[ LStringStates * LStringAlphabet ] = {
	//         "  \  D  A ...
	2, 0, 0, 0, 0,
	7, 3, 0, 0, 2,
	2, 2, 4, 0, 2,
	0, 0, 0, 2, 0,
	0, 0, 0, 6, 0,
	2, 0, 5, 0, 0,
	2, 0, 5, 0, 0
};

long LStringsEndStates[ LStringStates ] = {
	0, 0, 0, 0, 0, LString::String, LString::String
};

long LString::What( const QChar S )
//                ~~~~
{
	for(;;)
	{
		if( S == quat ) return 1;
		if( S == _T('\\') ) return 2;
		if( S == _T('\x0D') ) return 3;
		if( S == _T('\x0A') ) return 4;
		if( S.isPrint() ) return 5;
		return 0;
	}
}

void LString::Deeds( void )
//            ~~~~~
{
	switch( CurrentState )
	{
	case 1:
		read_string = _T("");
		break;
	case 2:
		if( PrevState == 2 ) read_string += character;
		else if( PrevState == 3 )
			switch( character.toAscii() )
			{
			case _T('n'):
				read_string += _T("\n");
				break;
			case _T('r'):
				read_string += _T("\r");
				break;
			case _T('t'):
				read_string += _T("\t");
				break;
			case _T('v'):
				read_string += _T("\v");
				break;
			case _T('b'):
				read_string += _T("\b");
				break;
			case _T('a'):
				read_string += _T("\a");
				break;
			case _T('\\'):
				read_string += _T("\\");
				break;
			default:
				read_string += character;
			}
		break;
	case 3:
		break;
	}
}

LString::LString( QChar _quat )
//       ~~~~~~~
: LAuto( LStringStates, LStringAlphabet, LStringLeapes, LStringsEndStates )
{
	quat = _quat;
}
