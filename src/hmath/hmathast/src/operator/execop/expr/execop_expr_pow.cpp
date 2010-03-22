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
void _call_dsrop_IntPow_natural( UniWord *arg )
{
	_dsrop_IntPow_natural( *(arg - 2), *(arg - 2), *(arg - 1) );
}
__HMATHLNGDLL__
void _call_dsrop_IntPow_integer( UniWord *arg )
{
	_dsrop_IntPow_integer( *(arg - 2), *(arg - 2), *(arg - 1) );
}
__HMATHLNGDLL__
void _call_dsrop_IntPow_rational( UniWord *arg )
{
	_dsrop_IntPow_rational( *(arg - 2), *(arg - 2), *(arg - 1) );
}
__HMATHLNGDLL__
void _call_dsrop_IntPow_real( UniWord *arg )
{
	_dsrop_IntPow_real( *(arg - 2), *(arg - 2), *(arg - 1) );
}
__HMATHLNGDLL__
void _call_dsrop_IntPow_complex( UniWord *arg )
{
	_dsrop_IntPow_complex( *(arg - 2), *(arg - 2), *(arg - 1) );
}

__HMATHLNGDLL__
void _call_dsrop_IntPow_group_string( UniWord *arg )
{
	_dsrop_IntPow_group_string( *(arg - 2), *(arg - 2), *(arg - 1) );
}

__HMATHLNGDLL__
void _call_dsrop_Pow_natural_real_real( UniWord *arg )
{
	*(arg - 2) = CDSRReal( 
		pow( CDSRReal( (arg - 2)->getNatural() ), 
			 (arg - 1)->getReal() ) );
}
__HMATHLNGDLL__
void _call_dsrop_Pow_integer_real_real( UniWord *arg )
{
	*(arg - 2) = CDSRReal( 
		pow( CDSRReal( (arg - 2)->getInteger() ), 
			 (arg - 1)->getReal() ) );
}
__HMATHLNGDLL__
void _call_dsrop_Pow_rational_real_real( UniWord *arg )
{
	*(arg - 2) = CDSRReal( 
		pow( CDSRReal( (arg - 2)->getRational().c ) / CDSRReal( (arg - 2)->getRational().z ), 
			 (arg - 1)->getReal() ) );
}
__HMATHLNGDLL__
void _call_dsrop_Pow_real_real_real( UniWord *arg )
{
	*(arg - 2) = CDSRReal( pow( (arg - 2)->getReal(), (arg - 1)->getReal() ) );
}
__HMATHLNGDLL__
void _call_dsrop_Pow_complex_real_complex( UniWord *arg )
{
	*(arg - 2) = CDSRComplex( pow( (arg - 2)->getComplex(), (arg - 1)->getReal() ) );
}
__HMATHLNGDLL__
void _call_dsrop_Pow_complex_complex_complex( UniWord *arg )
{
	*(arg - 2) = CDSRComplex( pow( (arg - 2)->getComplex(), (arg - 1)->getComplex() ) );
}


__HMATHLNGDLL__
void _call_dsrop_PolynomialPow_integer( UniWord *arg )
{
	*(arg - 2) = _dsrop_PolynomialPow_integer( (arg - 2)->getPolynomial(), *(arg - 1) );
}
__HMATHLNGDLL__
void _call_dsrop_PolynomialPow_real( UniWord *arg )
{
	*(arg - 2) = _dsrop_PolynomialPow_real( (arg - 2)->getPolynomial(), *(arg - 1) );
}
__HMATHLNGDLL__
void _call_dsrop_PolynomialPow_complex( UniWord *arg )
{
	*(arg - 2) = _dsrop_PolynomialPow_complex( (arg - 2)->getPolynomial(), *(arg - 1) );
}

__HMATHLNGDLL__
void _call_dsrop_RationalFunPow_integer( UniWord *arg )
{
	*(arg - 2) = _dsrop_RationalFunPow_integer( (arg - 2)->getRationalFun(), *(arg - 1) );
}
__HMATHLNGDLL__
void _call_dsrop_RationalFunPow_real( UniWord *arg )
{
	*(arg - 2) = _dsrop_RationalFunPow_real( (arg - 2)->getRationalFun(), *(arg - 1) );
}
__HMATHLNGDLL__
void _call_dsrop_RationalFunPow_complex( UniWord *arg )
{
	*(arg - 2) = _dsrop_RationalFunPow_complex( (arg - 2)->getRationalFun(), *(arg - 1) );
}

__HMATHLNGDLL__
void _call_dsrop_VectorPow_natural( UniWord *arg )
{
	*(arg - 2) = _dsrop_VectorPow_natural( (arg - 2)->getVector(), *(arg - 1) );
}
__HMATHLNGDLL__
void _call_dsrop_VectorPow_integer( UniWord *arg )
{
	*(arg - 2) = _dsrop_VectorPow_integer( (arg - 2)->getVector(), *(arg - 1) );
}
__HMATHLNGDLL__
void _call_dsrop_VectorPow_rational( UniWord *arg )
{
	*(arg - 2) = _dsrop_VectorPow_rational( (arg - 2)->getVector(), *(arg - 1) );
}
__HMATHLNGDLL__
void _call_dsrop_VectorPow_real( UniWord *arg )
{
	*(arg - 2) = _dsrop_VectorPow_real( (arg - 2)->getVector(), *(arg - 1) );
}
__HMATHLNGDLL__
void _call_dsrop_VectorPow_complex( UniWord *arg )
{
	*(arg - 2) = _dsrop_VectorPow_complex( (arg - 2)->getVector(), *(arg - 1) );
}

///////////////////////////////////////////////////////////////////////////////

typedef void (*SUBCALL_POW)( UniWord *arg );
#define __Call_Pow_subcalls_LENGTH	23
static SUBCALL_POW __Call_Pow_subcalls[ __Call_Pow_subcalls_LENGTH ] = 
{
	 _call_dsrop_IntPow_natural,
	 _call_dsrop_IntPow_integer,
	 _call_dsrop_IntPow_rational,
	 _call_dsrop_IntPow_real,
	 _call_dsrop_IntPow_complex,
	 _call_dsrop_IntPow_group_string,
	 _call_dsrop_Pow_natural_real_real,
	 _call_dsrop_Pow_integer_real_real,
	 _call_dsrop_Pow_rational_real_real,
	 _call_dsrop_Pow_real_real_real,
	 _call_dsrop_Pow_complex_real_complex,
	 _call_dsrop_Pow_complex_complex_complex,
	 _call_dsrop_PolynomialPow_integer,
	 _call_dsrop_PolynomialPow_real,
	 _call_dsrop_PolynomialPow_complex,
	 _call_dsrop_RationalFunPow_integer,
	 _call_dsrop_RationalFunPow_real,
	 _call_dsrop_RationalFunPow_complex,
	 _call_dsrop_VectorPow_natural,
	 _call_dsrop_VectorPow_integer,
	 _call_dsrop_VectorPow_rational,
	 _call_dsrop_VectorPow_real,
	 _call_dsrop_VectorPow_complex
};
long __Call_Pow( CDSR_VMEval& /*vm*/, MMD_Address& addr, UniWord *arg )
{
#if _DEBUG
	if( addr.param2 < 0 || 
		addr.param2 >= __Call_Pow_subcalls_LENGTH )
		throw _T("__Call_Pow : internal error, out of range");
#endif
	__Call_Pow_subcalls[ addr.param2 ]( arg );
	return 1 - addr.param3;
}

///////////////////////////////////////////////////////////////////////////////
