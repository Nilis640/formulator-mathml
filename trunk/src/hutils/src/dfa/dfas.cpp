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

#include <math.h>
#include "dfas.h"

int _hatol( const QStringRef& str, long& value )
{
	int err_level = ELH_NO_ERROR;

	LSDigit ldigit;
	int ret = ldigit.Analisys( str );
	switch( ret )
	{
	case LSDigit::Integer:
		value = ldigit.asign * ldigit.mnt_int;
		break;
	case LSDigit::Float:
		ldigit.mnt_float *= pow( 10.0f, (int)(ldigit.power) );
		value = long(ldigit.asign * ldigit.mnt_float);
		err_level = ELH_SOFT_ERROR;
		break;
	case LSDigit::FloatIEEE:
		ldigit.mnt_float *= pow( 10.0f, (int)(ldigit.power + ldigit.sign * ldigit.powerE) );
		value = long(ldigit.asign * ldigit.mnt_float);
		err_level = ELH_SOFT_ERROR;
		break;
	default:
		err_level = ELH_CRITICAL_ERROR;
	}
	return err_level;
}

int _hatof( const QStringRef& str, double& value )
{
	int err_level = ELH_NO_ERROR;

	LSDigit ldigit;
	int ret = ldigit.Analisys( str );
	switch( ret )
	{
	case LSDigit::Integer:
		value = ldigit.asign * double( ldigit.mnt_int );
		break;
	case LSDigit::Float:
		ldigit.mnt_float *= pow( 10.0f, (int)( ldigit.power ) );
		value = ldigit.asign * ldigit.mnt_float;
		break;
	case LSDigit::FloatIEEE:
		ldigit.mnt_float *= pow( 10.0f, (int)( ldigit.power + ldigit.sign * ldigit.powerE ) );
		value = ldigit.asign * ldigit.mnt_float;
		break;
	default:
		err_level = ELH_CRITICAL_ERROR;
	}
	return err_level;
}

int _hatol_base( const QStringRef& str, const QString& base, long& value )
{
	short int iBase = base.toInt();
	if( iBase < 2 || iBase > 36 || iBase == 10 )
		return ::_hatol( str, value );

	int err_level = ELH_NO_ERROR;

	LSDigitBase ldigit( iBase );
	int ret = ldigit.Analisys( str );
	switch( ret )
	{
	case LSDigitBase::Integer:
		value = ldigit.asign * ldigit.mnt_int;
		break;
	case LSDigitBase::Float:
		ldigit.mnt_float *= pow( (double) iBase, (int)(ldigit.power) );
		value = long(ldigit.asign * ldigit.mnt_float);
		err_level = ELH_SOFT_ERROR;
		break;
	case LSDigitBase::FloatIEEE:
		ldigit.mnt_float *= pow( (double) iBase, (int)(ldigit.power + ldigit.sign * ldigit.powerE) );
		value = long(ldigit.asign * ldigit.mnt_float);
		err_level = ELH_SOFT_ERROR;
		break;
	default:
		err_level = ELH_CRITICAL_ERROR;
	}
	return err_level;
}

int _hatof_base( const QStringRef& str, const QString& base, double& value )
{
	short int iBase = base.toInt();
	if( iBase < 2 || iBase > 36 || iBase == 10 )
		return ::_hatof( str, value );

	int err_level = ELH_NO_ERROR;

	LSDigitBase ldigit( iBase );
	int ret = ldigit.Analisys( str );
	switch( ret )
	{
	case LSDigitBase::Integer:
		value = ldigit.asign * double( ldigit.mnt_int );
		break;
	case LSDigitBase::Float:
		ldigit.mnt_float *= pow( (double) iBase, (int)( ldigit.power ) );
		value = ldigit.asign * ldigit.mnt_float;
		break;
	case LSDigitBase::FloatIEEE:
		ldigit.mnt_float *= pow( (double) iBase, (int)( ldigit.power + ldigit.sign * ldigit.powerE ) );
		value = ldigit.asign * ldigit.mnt_float;
		break;
	default:
		err_level = ELH_CRITICAL_ERROR;
	}
	return err_level;
}
