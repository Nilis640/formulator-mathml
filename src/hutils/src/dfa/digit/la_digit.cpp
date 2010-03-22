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
#include "la_digit.h"

long LDigitLeapes[ LDigitStates * LDigitAlphabet ] =
{
//  d  e  .  +  -  _
	2, 0, 3, 0, 0, 0,
	2, 5, 4, 0, 0, 2,
	4, 0, 0, 0, 0, 0,
	4, 5, 0, 0, 0, 4,
	8, 0, 0, 6, 7, 0,
	8, 0, 0, 0, 0, 0,
	8, 0, 0, 0, 0, 0,
	8, 0, 0, 0, 0, 8
};

long LDigitsEndStates[ LDigitStates ] =
{
	0, LDigit::Integer, 0, LDigit::Float,
	0, 0, 0, LDigit::FloatIEEE
};

long LDigit::What( const QChar S )
//           ~~~~
{
	for(;;)
	{
		if( S.isDigit() )
			return 1;
		if( ( S == _T('E') ) || ( S == _T('e') ) )
			return 2;
		if( S == _T('.') )
			return 3;
		if( S == _T('+') )
			return 4;
		if( S == _T('-') )
			return 5;
		if( S == _T('_') )
			return 6;
		return 0;
	}
}

void LDigit::Deeds( void )
//           ~~~~~
{
	switch( CurrentState )
	{
	case 1:
		pre_int = mnt_int = 0;
		is_big = 0;
		break;
	case 2:
		if( character != _T('_') )
		{
			mnt_int = 10 * mnt_int + character.digitValue();
			if(mnt_int < pre_int)
				is_big = 1;
			else
				pre_int = mnt_int;
		}
		break;
	case 3 :
		mnt_float = mnt_int;
		power = 0;
		break;
	case 4 :
		if( GetPreviousState() == 2 )
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
	case 5 :
		if( GetPreviousState() == 2 )
		{
			mnt_float = mnt_int;
			power = 0;
		}
		powerE = 0;
		sign = 1;
		break;
		//  case 6 :
		//    break;
	case 7 :
		sign = -1;
		break;
	case 8 :
		if( character != _T('_') )
			powerE = 10 * powerE + character.digitValue();
		break;
	}
}

LDigit::LDigit( void )
//      ~~~~~~
: LAuto( LDigitStates, LDigitAlphabet, LDigitLeapes, LDigitsEndStates )
{
	is_big = 0;
	decimal = 0;
}
