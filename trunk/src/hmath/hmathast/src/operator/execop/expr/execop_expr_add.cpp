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
void _call_dsrop_Add_natural( UniWord *arg )
{
	_dsrop_Add_natural( *(arg - 2), *(arg - 2), *(arg - 1) );
}
__HMATHLNGDLL__
void _call_dsrop_Add_integer( UniWord *arg )
{
	_dsrop_Add_integer( *(arg - 2), *(arg - 2), *(arg - 1) );
}
__HMATHLNGDLL__
void _call_dsrop_Add_rational( UniWord *arg )
{
	_dsrop_Add_rational( *(arg - 2), *(arg - 2), *(arg - 1) );
}
__HMATHLNGDLL__
void _call_dsrop_Add_real( UniWord *arg )
{
	_dsrop_Add_real( *(arg - 2), *(arg - 2), *(arg - 1) );
}
__HMATHLNGDLL__
void _call_dsrop_Add_complex( UniWord *arg )
{
	_dsrop_Add_complex( *(arg - 2), *(arg - 2), *(arg - 1) );
}

__HMATHLNGDLL__
void _call_dsrop_PolynomialAdd_integer( UniWord *arg )
{
	*(arg - 2) = _dsrop_PolynomialAdd_integer( (arg - 2)->getPolynomial(), (arg - 1)->getPolynomial() );
}
__HMATHLNGDLL__
void _call_dsrop_PolynomialAdd_real( UniWord *arg )
{
	*(arg - 2) = _dsrop_PolynomialAdd_real( (arg - 2)->getPolynomial(), (arg - 1)->getPolynomial() );
}
__HMATHLNGDLL__
void _call_dsrop_PolynomialAdd_complex( UniWord *arg )
{
	*(arg - 2) = _dsrop_PolynomialAdd_complex( (arg - 2)->getPolynomial(), (arg - 1)->getPolynomial() );
}

__HMATHLNGDLL__
void _call_dsrop_RationalFunAdd_integer( UniWord *arg )
{
	*(arg - 2) = _dsrop_RationalFunAdd_integer( (arg - 2)->getRationalFun(), (arg - 1)->getRationalFun() );
}
__HMATHLNGDLL__
void _call_dsrop_RationalFunAdd_real( UniWord *arg )
{
	*(arg - 2) = _dsrop_RationalFunAdd_real( (arg - 2)->getRationalFun(), (arg - 1)->getRationalFun() );
}
__HMATHLNGDLL__
void _call_dsrop_RationalFunAdd_complex( UniWord *arg )
{
	*(arg - 2) = _dsrop_RationalFunAdd_complex( (arg - 2)->getRationalFun(), (arg - 1)->getRationalFun() );
}

__HMATHLNGDLL__
void _call_dsrop_VectorAdd_natural( UniWord *arg )
{
	*(arg - 2) = _dsrop_VectorAdd_natural( (arg - 2)->getVector(), (arg - 1)->getVector() );
}
__HMATHLNGDLL__
void _call_dsrop_VectorAdd_integer( UniWord *arg )
{
	*(arg - 2) = _dsrop_VectorAdd_integer( (arg - 2)->getVector(), (arg - 1)->getVector() );
}
__HMATHLNGDLL__
void _call_dsrop_VectorAdd_rational( UniWord *arg )
{
	*(arg - 2) = _dsrop_VectorAdd_rational( (arg - 2)->getVector(), (arg - 1)->getVector() );
}
__HMATHLNGDLL__
void _call_dsrop_VectorAdd_real( UniWord *arg )
{
	*(arg - 2) = _dsrop_VectorAdd_real( (arg - 2)->getVector(), (arg - 1)->getVector() );
}
__HMATHLNGDLL__
void _call_dsrop_VectorAdd_complex( UniWord *arg )
{
	*(arg - 2) = _dsrop_VectorAdd_complex( (arg - 2)->getVector(), (arg - 1)->getVector() );
}

__HMATHLNGDLL__
void _call_dsrop_Add_string( UniWord *arg )
{
	_dsrop_Add_string( *(arg - 2), *(arg - 2), *(arg - 1) );
}

__HMATHLNGDLL__
void _call_dsrop_Plus_natural( UniWord *arg )
{
	_dsrop_Plus_natural( *(arg - 1), *(arg - 1) );
}
__HMATHLNGDLL__
void _call_dsrop_Plus_integer( UniWord *arg )
{
	_dsrop_Plus_integer( *(arg - 1), *(arg - 1) );
}
__HMATHLNGDLL__
void _call_dsrop_Plus_rational( UniWord *arg )
{
	_dsrop_Plus_rational( *(arg - 1), *(arg - 1) );
}
__HMATHLNGDLL__
void _call_dsrop_Plus_real( UniWord *arg )
{
	_dsrop_Plus_real( *(arg - 1), *(arg - 1) );
}
__HMATHLNGDLL__
void _call_dsrop_Plus_complex( UniWord *arg )
{
	_dsrop_Plus_complex( *(arg - 1), *(arg - 1) );
}

__HMATHLNGDLL__
void _call_dsrop_PolynomialPlus_integer( UniWord *arg )
{
	*(arg - 1) = _dsrop_PolynomialPlus_integer( (arg - 1)->getPolynomial() );
}
__HMATHLNGDLL__
void _call_dsrop_PolynomialPlus_real( UniWord *arg )
{
	*(arg - 1) = _dsrop_PolynomialPlus_real( (arg - 1)->getPolynomial() );
}
__HMATHLNGDLL__
void _call_dsrop_PolynomialPlus_complex( UniWord *arg )
{
	*(arg - 1) = _dsrop_PolynomialPlus_complex( (arg - 1)->getPolynomial() );
}

__HMATHLNGDLL__
void _call_dsrop_RationalFunPlus_integer( UniWord *arg )
{
	*(arg - 1) = _dsrop_RationalFunPlus_integer( (arg - 1)->getRationalFun() );
}
__HMATHLNGDLL__
void _call_dsrop_RationalFunPlus_real( UniWord *arg )
{
	*(arg - 1) = _dsrop_RationalFunPlus_real( (arg - 1)->getRationalFun() );
}
__HMATHLNGDLL__
void _call_dsrop_RationalFunPlus_complex( UniWord *arg )
{
	*(arg - 1) = _dsrop_RationalFunPlus_complex( (arg - 1)->getRationalFun() );
}

__HMATHLNGDLL__
void _call_dsrop_VectorPlus_natural( UniWord *arg )
{
	*(arg - 1) = _dsrop_VectorPlus_natural( (arg - 1)->getVector() );
}
__HMATHLNGDLL__
void _call_dsrop_VectorPlus_integer( UniWord *arg )
{
	*(arg - 1) = _dsrop_VectorPlus_integer( (arg - 1)->getVector() );
}
__HMATHLNGDLL__
void _call_dsrop_VectorPlus_rational( UniWord *arg )
{
	*(arg - 1) = _dsrop_VectorPlus_rational( (arg - 1)->getVector() );
}
__HMATHLNGDLL__
void _call_dsrop_VectorPlus_real( UniWord *arg )
{
	*(arg - 1) = _dsrop_VectorPlus_real( (arg - 1)->getVector() );
}
__HMATHLNGDLL__
void _call_dsrop_VectorPlus_complex( UniWord *arg )
{
	*(arg - 1) = _dsrop_VectorPlus_complex( (arg - 1)->getVector() );
}

///////////////////////////////////////////////////////////////////////////////

typedef void (*SUBCALL_ADD)( UniWord *arg );
#define __Call_Add_subcalls_LENGTH	33
static SUBCALL_ADD __Call_Add_subcalls[ __Call_Add_subcalls_LENGTH ] = 
{
	_call_dsrop_Add_natural,
	_call_dsrop_Add_integer,
	_call_dsrop_Add_rational,
	_call_dsrop_Add_real,
	_call_dsrop_Add_complex,
	_call_dsrop_PolynomialAdd_integer,
	_call_dsrop_PolynomialAdd_real,
	_call_dsrop_PolynomialAdd_complex,
	_call_dsrop_RationalFunAdd_integer,
	_call_dsrop_RationalFunAdd_real,
	_call_dsrop_RationalFunAdd_complex,
	_call_dsrop_VectorAdd_natural,
	_call_dsrop_VectorAdd_integer,
	_call_dsrop_VectorAdd_rational,
	_call_dsrop_VectorAdd_real,
	_call_dsrop_VectorAdd_complex,
	_call_dsrop_Add_string,
	_call_dsrop_Plus_natural,
	_call_dsrop_Plus_integer,
	_call_dsrop_Plus_rational,
	_call_dsrop_Plus_real,
	_call_dsrop_Plus_complex,
	_call_dsrop_PolynomialPlus_integer,
	_call_dsrop_PolynomialPlus_real,
	_call_dsrop_PolynomialPlus_complex,
	_call_dsrop_RationalFunPlus_integer,
	_call_dsrop_RationalFunPlus_real,
	_call_dsrop_RationalFunPlus_complex,
	_call_dsrop_VectorPlus_natural,
	_call_dsrop_VectorPlus_integer,
	_call_dsrop_VectorPlus_rational,
	_call_dsrop_VectorPlus_real,
	_call_dsrop_VectorPlus_complex
};
long __Call_Add( CDSR_VMEval& /*vm*/, MMD_Address& addr, UniWord *arg )
{
#if _DEBUG
	if( addr.param2 < 0 || 
		addr.param2 >= __Call_Add_subcalls_LENGTH )
		throw _T("__Call_Add : internal error, out of range");
#endif
	__Call_Add_subcalls[ addr.param2 ]( arg );
	return 1 - addr.param3;
}

///////////////////////////////////////////////////////////////////////////////
