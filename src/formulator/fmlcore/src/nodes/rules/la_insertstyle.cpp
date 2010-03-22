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

#include "la_insertstyle.h"

long LFInsertStyleLeapes[ LFInsertStyleStates * LFInsertStyleAlphabet ] = {
	//     l  d  .
	2, 3, 4,
	2, 0, 0,
	0, 3, 0,
	0, 0, 4
};

long LFInsertStylesEndStates[ LFInsertStyleStates ] = {
	0, 'a', '1', '.'
};

long LFInsertStyle::What( const QChar S )
//                ~~~~
{
	for(;;)
	{
		if( S.isLetter() || (S == QChar('_')) )
			return 1;
		if( S.isDigit() )
			return 2;
		if( S == _T('\0') )
			return 0;
		return 3;
	}
}

void LFInsertStyle::Deeds( void )
//            ~~~~~
{
	switch( CurrentState )
	{
	case 1 :
		text = _T("");
		break;
	case 2 :
		text += character;
		break;
	case 3 :
		text += character;
		break;
	case 4 :
		text += character;
		break;
	}
}

LFInsertStyle::LFInsertStyle( void )
//       ~~~~~~~
: LAuto( LFInsertStyleStates, LFInsertStyleAlphabet, LFInsertStyleLeapes, LFInsertStylesEndStates )
{
	text = _T("");
}


QString& LFInsertStyle::getText( void )
{
	return text;
}
