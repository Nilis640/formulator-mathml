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
void _call_dsrop_Div_natural( UniWord *arg )
{
	_dsrop_Div_natural( *(arg - 2), *(arg - 2), *(arg - 1) );
}
__HMATHLNGDLL__
void _call_dsrop_Div_integer( UniWord *arg )
{
	_dsrop_Div_integer( *(arg - 2), *(arg - 2), *(arg - 1) );
}
__HMATHLNGDLL__
void _call_dsrop_Div_rational( UniWord *arg )
{
	_dsrop_Div_rational( *(arg - 2), *(arg - 2), *(arg - 1) );
}
__HMATHLNGDLL__
void _call_dsrop_Div_real( UniWord *arg )
{
	_dsrop_Div_real( *(arg - 2), *(arg - 2), *(arg - 1) );
}
__HMATHLNGDLL__
void _call_dsrop_Div_complex( UniWord *arg )
{
	_dsrop_Div_complex( *(arg - 2), *(arg - 2), *(arg - 1) );
}

__HMATHLNGDLL__
void _call_dsrop_PolynomialDiv_integer( UniWord *arg )
{
	*(arg - 2) = _dsrop_PolynomialDiv_integer( (arg - 2)->getPolynomial(), *(arg - 1) );
}
__HMATHLNGDLL__
void _call_dsrop_PolynomialDiv_real( UniWord *arg )
{
	*(arg - 2) = _dsrop_PolynomialDiv_real( (arg - 2)->getPolynomial(), *(arg - 1) );
}
__HMATHLNGDLL__
void _call_dsrop_PolynomialDiv_complex( UniWord *arg )
{
	*(arg - 2) = _dsrop_PolynomialDiv_complex( (arg - 2)->getPolynomial(), *(arg - 1) );
}

__HMATHLNGDLL__
void _call_dsrop_PolynomialDivPolynomial_integer( UniWord *arg )
{
	*(arg - 2) = _dsrop_PolynomialDiv_integer( (arg - 2)->getPolynomial(), (arg - 1)->getPolynomial() );
}
__HMATHLNGDLL__
void _call_dsrop_PolynomialDivPolynomial_real( UniWord *arg )
{
	*(arg - 2) = _dsrop_PolynomialDiv_real( (arg - 2)->getPolynomial(), (arg - 1)->getPolynomial() );
}
__HMATHLNGDLL__
void _call_dsrop_PolynomialDivPolynomial_complex( UniWord *arg )
{
	*(arg - 2) = _dsrop_PolynomialDiv_complex( (arg - 2)->getPolynomial(), (arg - 1)->getPolynomial() );
}

__HMATHLNGDLL__
void _call_dsrop_RationalFunDiv_integer( UniWord *arg )
{
	*(arg - 2) = _dsrop_RationalFunDiv_integer( (arg - 2)->getRationalFun(), (arg - 1)->getRationalFun() );
}
__HMATHLNGDLL__
void _call_dsrop_RationalFunDiv_real( UniWord *arg )
{
	*(arg - 2) = _dsrop_RationalFunDiv_real( (arg - 2)->getRationalFun(), (arg - 1)->getRationalFun() );
}
__HMATHLNGDLL__
void _call_dsrop_RationalFunDiv_complex( UniWord *arg )
{
	*(arg - 2) = _dsrop_RationalFunDiv_complex( (arg - 2)->getRationalFun(), (arg - 1)->getRationalFun() );
}

__HMATHLNGDLL__
void _call_dsrop_VectorDiv_natural( UniWord *arg )
{
	*(arg - 2) = _dsrop_VectorDiv_natural( (arg - 2)->getVector(), *(arg - 1) );
}
__HMATHLNGDLL__
void _call_dsrop_VectorDiv_integer( UniWord *arg )
{
	*(arg - 2) = _dsrop_VectorDiv_integer( (arg - 2)->getVector(), *(arg - 1) );
}
__HMATHLNGDLL__
void _call_dsrop_VectorDiv_rational( UniWord *arg )
{
	*(arg - 2) = _dsrop_VectorDiv_rational( (arg - 2)->getVector(), *(arg - 1) );
}
__HMATHLNGDLL__
void _call_dsrop_VectorDiv_real( UniWord *arg )
{
	*(arg - 2) = _dsrop_VectorDiv_real( (arg - 2)->getVector(), *(arg - 1) );
}
__HMATHLNGDLL__
void _call_dsrop_VectorDiv_complex( UniWord *arg )
{
	*(arg - 2) = _dsrop_VectorDiv_complex( (arg - 2)->getVector(), *(arg - 1) );
}

///////////////////////////////////////////////////////////////////////////////

typedef void (*SUBCALL_DIV)( UniWord *arg );
#define __Call_Div_subcalls_LENGTH	19
static SUBCALL_DIV __Call_Div_subcalls[ __Call_Div_subcalls_LENGTH ] = 
{
	_call_dsrop_Div_natural,
	_call_dsrop_Div_integer,
	_call_dsrop_Div_rational,
	_call_dsrop_Div_real,
	_call_dsrop_Div_complex,
	_call_dsrop_PolynomialDiv_integer,
	_call_dsrop_PolynomialDiv_real,
	_call_dsrop_PolynomialDiv_complex,
	_call_dsrop_PolynomialDivPolynomial_integer,
	_call_dsrop_PolynomialDivPolynomial_real,
	_call_dsrop_PolynomialDivPolynomial_complex,
	_call_dsrop_RationalFunDiv_integer,
	_call_dsrop_RationalFunDiv_real,
	_call_dsrop_RationalFunDiv_complex,
	_call_dsrop_VectorDiv_natural,
	_call_dsrop_VectorDiv_integer,
	_call_dsrop_VectorDiv_rational,
	_call_dsrop_VectorDiv_real,
	_call_dsrop_VectorDiv_complex
};
long __Call_Div( CDSR_VMEval& /*vm*/, MMD_Address& addr, UniWord *arg )
{
#if _DEBUG
	if( addr.param2 < 0 || 
		addr.param2 >= __Call_Div_subcalls_LENGTH )
		throw _T("__Call_Div : internal error, out of range");
#endif
	__Call_Div_subcalls[ addr.param2 ]( arg );
	return 1 - addr.param3;
}

///////////////////////////////////////////////////////////////////////////////
