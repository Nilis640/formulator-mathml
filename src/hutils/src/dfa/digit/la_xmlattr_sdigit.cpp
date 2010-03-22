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
#include "la_xmlattr_sdigit.h"

long LXMLSizeSDigitLeapes[ LXMLSizeSDigitStates * LXMLSizeSDigitAlphabet ] =
{
//  d  .  +  -  _
	3, 4, 2, 2, 0,
	3, 4, 0, 0, 0,
	3, 5, 0, 0, 3,
	5, 0, 0, 0, 0,
	5, 0, 0, 0, 5
};

long LXMLSizeSDigitsEndStates[ LXMLSizeSDigitStates ] =
{
	0, 0, LXMLSizeSDigit::Integer, 0, LXMLSizeSDigit::Float
};

long LXMLSizeSDigit::What( const QChar S )
//               ~~~~
{
	for(;;)
	{
		if( S.isDigit() )
			return 1;
		if( S == QChar('.') )
			return 2;
		if( S == QChar('+') )
			return 3;
		if( S == QChar('-') )
			return 4;
		if( S == QChar('_') )
			return 5;
		return 0;
	}
}

void LXMLSizeSDigit::Deeds( void )
//           ~~~~~
{
	switch( CurrentState )
	{
	case 1:
		asign = 1;
		pre_int = mnt_int = 0;
		is_big = 0;
		break;
	case 2:
		if( character == _T('-') )
			asign = -1;
		break;
	case 3:
		if( character != _T('_') )
		{
			mnt_int = 10 * mnt_int + character.digitValue();
			if( mnt_int < pre_int )
				is_big = 1;
			else
				pre_int = mnt_int;
		}
		break;
	case 4:
		mnt_float = mnt_int;
		power = 0;
		break;
	case 5:
		if( GetPreviousState() == 3 )
		{
			mnt_float = mnt_int;
			power = 0;
		}
		else
		{
			if( character != _T('_') )
			{
				decimal++;
				mnt_float = 10 * mnt_float + character.digitValue();
				power--;
			}
		}
		break;
	}
}

LXMLSizeSDigit::LXMLSizeSDigit( void )
//      ~~~~~~
: LAuto( LXMLSizeSDigitStates, LXMLSizeSDigitAlphabet, LXMLSizeSDigitLeapes, LXMLSizeSDigitsEndStates )
{
	asign = 1;
	is_big = 0;
	decimal = 0;
}
