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

#include "../execop_call.h"
#include "../../vmevallink.h"

///////////////////////////////////////////////////////////////////////////////

__HMATHLNGDLL__
void _call_dsrop_NotEqual_natural( UniWord *arg )
{
	_dsrop_NotEqual_natural( *(arg - 2), *(arg - 2), *(arg - 1) );
}
__HMATHLNGDLL__
void _call_dsrop_NotEqual_integer( UniWord *arg )
{
	_dsrop_NotEqual_integer( *(arg - 2), *(arg - 2), *(arg - 1) );
}
__HMATHLNGDLL__
void _call_dsrop_NotEqual_rational( UniWord *arg )
{
	_dsrop_NotEqual_rational( *(arg - 2), *(arg - 2), *(arg - 1) );
}
__HMATHLNGDLL__
void _call_dsrop_NotEqual_real( UniWord *arg )
{
	_dsrop_NotEqual_real( *(arg - 2), *(arg - 2), *(arg - 1) );
}
__HMATHLNGDLL__
void _call_dsrop_NotEqual_complex( UniWord *arg )
{
	_dsrop_NotEqual_complex( *(arg - 2), *(arg - 2), *(arg - 1) );
}
__HMATHLNGDLL__
void _call_dsrop_NotEqual_bool( UniWord *arg )
{
	_dsrop_NotEqual_bool( *(arg - 2), *(arg - 2), *(arg - 1) );
}
__HMATHLNGDLL__
void _call_dsrop_NotEqual_string( UniWord *arg )
{
	_dsrop_NotEqual_string( *(arg - 2), *(arg - 2), *(arg - 1) );
}

__HMATHLNGDLL__
void _call_dsrop_PolynomialNotEqual_integer( UniWord *arg )
{
	*(arg - 2) = _dsrop_PolynomialNotEqual_integer( (arg - 2)->getPolynomial(), (arg - 1)->getPolynomial() );
}
__HMATHLNGDLL__
void _call_dsrop_PolynomialNotEqual_real( UniWord *arg )
{
	*(arg - 2) = _dsrop_PolynomialNotEqual_real( (arg - 2)->getPolynomial(), (arg - 1)->getPolynomial() );
}
__HMATHLNGDLL__
void _call_dsrop_PolynomialNotEqual_complex( UniWord *arg )
{
	*(arg - 2) = _dsrop_PolynomialNotEqual_complex( (arg - 2)->getPolynomial(), (arg - 1)->getPolynomial() );
}

__HMATHLNGDLL__
void _call_dsrop_RationalFunNotEqual_integer( UniWord *arg )
{
	*(arg - 2) = _dsrop_RationalFunNotEqual_integer( (arg - 2)->getRationalFun(), (arg - 1)->getRationalFun() );
}
__HMATHLNGDLL__
void _call_dsrop_RationalFunNotEqual_real( UniWord *arg )
{
	*(arg - 2) = _dsrop_RationalFunNotEqual_real( (arg - 2)->getRationalFun(), (arg - 1)->getRationalFun() );
}
__HMATHLNGDLL__
void _call_dsrop_RationalFunNotEqual_complex( UniWord *arg )
{
	*(arg - 2) = _dsrop_RationalFunNotEqual_complex( (arg - 2)->getRationalFun(), (arg - 1)->getRationalFun() );
}

__HMATHLNGDLL__
void _call_dsrop_VectorNotEqual_natural( UniWord *arg )
{
	*(arg - 2) = _dsrop_VectorNotEqual_natural( (arg - 2)->getVector(), (arg - 1)->getVector() );
}
__HMATHLNGDLL__
void _call_dsrop_VectorNotEqual_integer( UniWord *arg )
{
	*(arg - 2) = _dsrop_VectorNotEqual_integer( (arg - 2)->getVector(), (arg - 1)->getVector() );
}
__HMATHLNGDLL__
void _call_dsrop_VectorNotEqual_rational( UniWord *arg )
{
	*(arg - 2) = _dsrop_VectorNotEqual_rational( (arg - 2)->getVector(), (arg - 1)->getVector() );
}
__HMATHLNGDLL__
void _call_dsrop_VectorNotEqual_real( UniWord *arg )
{
	*(arg - 2) = _dsrop_VectorNotEqual_real( (arg - 2)->getVector(), (arg - 1)->getVector() );
}
__HMATHLNGDLL__
void _call_dsrop_VectorNotEqual_complex( UniWord *arg )
{
	*(arg - 2) = _dsrop_VectorNotEqual_complex( (arg - 2)->getVector(), (arg - 1)->getVector() );
}

__HMATHLNGDLL__
void _call_dsrop_SetNotEqual( UniWord *arg )
{
	*(arg - 2) = _dsrop_SetNotEqual_anytype( (arg - 2)->getSet(), (arg - 1)->getSet() );
}
///////////////////////////////////////////////////////////////////////////////

typedef void (*SUBCALL_NOTEQUAL)( UniWord *arg );
#define __Call_NotEqual_subcalls_LENGTH	19
static SUBCALL_NOTEQUAL __Call_NotEqual_subcalls[ __Call_NotEqual_subcalls_LENGTH ] = 
{
	_call_dsrop_NotEqual_natural,
	_call_dsrop_NotEqual_integer,
	_call_dsrop_NotEqual_rational,
	_call_dsrop_NotEqual_real,
	_call_dsrop_NotEqual_complex,
	_call_dsrop_NotEqual_bool,
	_call_dsrop_NotEqual_string,
	_call_dsrop_PolynomialNotEqual_integer,
	_call_dsrop_PolynomialNotEqual_real,
	_call_dsrop_PolynomialNotEqual_complex,
	_call_dsrop_RationalFunNotEqual_integer,
	_call_dsrop_RationalFunNotEqual_real,
	_call_dsrop_RationalFunNotEqual_complex,
	_call_dsrop_VectorNotEqual_natural,
	_call_dsrop_VectorNotEqual_integer,
	_call_dsrop_VectorNotEqual_rational,
	_call_dsrop_VectorNotEqual_real,
	_call_dsrop_VectorNotEqual_complex,
	_call_dsrop_SetNotEqual
};
long __Call_NotEqual( CDSR_VMEval& /*vm*/, MMD_Address& addr, UniWord *arg )
{
#if _DEBUG
	if( addr.param2 < 0 || 
		addr.param2 >= __Call_NotEqual_subcalls_LENGTH )
		throw _T("__Call_NotEqual : internal error, out of range");
#endif
	__Call_NotEqual_subcalls[ addr.param2 ]( arg );
	return 1 - addr.param3;
}

///////////////////////////////////////////////////////////////////////////////
