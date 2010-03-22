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

#include "HMathBS/inumber.h"
#include "HMathBS/iarray.h"
#include "HMathBS/ipolynomial.h"
#include "HMathBS/irationalf.h"
#include "HMathBS/ieigenval.h"
#include "HMathBS/islaecommon.h"
#include "execop_call.h"
#include "../vmevallink.h"

///////////////////////////////////////////////////////////////////////////////

void _dsrop_fun_abs_natural( UniWord *arg, CParseDsrSymbolTable& /*smbtable*/ )
{
	_dsrop_abs_natural( arg );
}

void _dsrop_fun_abs_integer( UniWord *arg, CParseDsrSymbolTable& /*smbtable*/ )
{
	_dsrop_abs_integer( arg );
}

void _dsrop_fun_abs_rational( UniWord *arg, CParseDsrSymbolTable& /*smbtable*/ )
{
	_dsrop_abs_rational( arg );
}

void _dsrop_fun_abs_real( UniWord *arg, CParseDsrSymbolTable& /*smbtable*/ )
{
	_dsrop_abs_real( arg );
}

void _dsrop_fun_abs_complex( UniWord *arg, CParseDsrSymbolTable& /*smbtable*/ )
{
	_dsrop_abs_complex( arg );
}

void _dsrop_fun_abs_vector_integer( UniWord *arg, CParseDsrSymbolTable& smbtable )
{
	_dsrop_abs_Vector_integer( arg, arg->getVector(), smbtable );
}

void _dsrop_fun_abs_vector_real( UniWord *arg, CParseDsrSymbolTable& smbtable )
{
	_dsrop_abs_Vector_real( arg, arg->getVector(), smbtable );
}

void _dsrop_fun_abs_vector_complex( UniWord *arg, CParseDsrSymbolTable& smbtable )
{
	_dsrop_abs_Vector_complex( arg, arg->getVector(), smbtable );
}

typedef void (*SUBCALL_ABS)( UniWord *arg, CParseDsrSymbolTable& smbtable );
#define __Call_abs_subcalls_LENGTH	8
static SUBCALL_ABS __Call_abs_subcalls[ __Call_abs_subcalls_LENGTH ] = 
{
	_dsrop_fun_abs_natural,
	_dsrop_fun_abs_integer,
	_dsrop_fun_abs_rational,
	_dsrop_fun_abs_real,
	_dsrop_fun_abs_complex,
	_dsrop_fun_abs_vector_integer,
	_dsrop_fun_abs_vector_real,
	_dsrop_fun_abs_vector_complex
};
long __Call_abs( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg )
{
#if _DEBUG
	if( addr.param2 < 0 || 
		addr.param2 >= __Call_abs_subcalls_LENGTH )
		throw _T("__Call_abs : internal error, out of range");
#endif
	__Call_abs_subcalls[ addr.param2 ]( arg - addr.param3, vm.getSmbTable() );
	return 1 - addr.param3;
}

///////////////////////////////////////////////////////////////////////////////

long __Call_arch( CDSR_VMEval& /*vm*/, MMD_Address& addr, UniWord *arg )
{
#if _DEBUG
	if( addr.param2 < 0 )
		throw _T("__Call_(fun) : internal error, out of range");
#endif
	if( addr.param2 == 0 )	// DSRDATA_TYPE_REAL
		*(arg - 1) = CDSRReal( arch( (arg - 1)->getReal() ) );
	else					// DSRDATA_TYPE_COMPLEX
		*(arg - 1) = CDSRComplex( arch( (arg - 1)->getComplex() ) );
	return 1 - addr.param3;
}

long __Call_arccos( CDSR_VMEval& /*vm*/, MMD_Address& addr, UniWord *arg )
{
#if _DEBUG
	if( addr.param2 < 0 )
		throw _T("__Call_(fun) : internal error, out of range");
#endif
	if( addr.param2 == 0 )	// DSRDATA_TYPE_REAL
		*(arg - 1) = CDSRReal( arccos( (arg - 1)->getReal() ) );
	else					// DSRDATA_TYPE_COMPLEX
		*(arg - 1) = CDSRComplex( arccos( (arg - 1)->getComplex() ) );
	return 1 - addr.param3;
}

long __Call_arccosec( CDSR_VMEval& /*vm*/, MMD_Address& addr, UniWord *arg )
{
#if _DEBUG
	if( addr.param2 < 0 )
		throw _T("__Call_(fun) : internal error, out of range");
#endif
	if( addr.param2 == 0 )	// DSRDATA_TYPE_REAL
		*(arg - 1) = CDSRReal( arccosec( (arg - 1)->getReal() ) );
	else					// DSRDATA_TYPE_COMPLEX
		*(arg - 1) = CDSRComplex( arccosec( (arg - 1)->getComplex() ) );
	return 1 - addr.param3;
}

long __Call_arcsec( CDSR_VMEval& /*vm*/, MMD_Address& addr, UniWord *arg )
{
#if _DEBUG
	if( addr.param2 < 0 )
		throw _T("__Call_(fun) : internal error, out of range");
#endif
	if( addr.param2 == 0 )	// DSRDATA_TYPE_REAL
		*(arg - 1) = CDSRReal( arcsec( (arg - 1)->getReal() ) );
	else					// DSRDATA_TYPE_COMPLEX
		*(arg - 1) = CDSRComplex( arcsec( (arg - 1)->getComplex() ) );
	return 1 - addr.param3;
}

long __Call_arcsin( CDSR_VMEval& /*vm*/, MMD_Address& addr, UniWord *arg )
{
#if _DEBUG
	if( addr.param2 < 0 )
		throw _T("__Call_(fun) : internal error, out of range");
#endif
	if( addr.param2 == 0 )	// DSRDATA_TYPE_REAL
		*(arg - 1) = CDSRReal( arcsin( (arg - 1)->getReal() ) );
	else					// DSRDATA_TYPE_COMPLEX
		*(arg - 1) = CDSRComplex( arcsin( (arg - 1)->getComplex() ) );
	return 1 - addr.param3;
}

long __Call_arctg( CDSR_VMEval& /*vm*/, MMD_Address& addr, UniWord *arg )
{
#if _DEBUG
	if( addr.param2 < 0 )
		throw _T("__Call_(fun) : internal error, out of range");
#endif
	if( addr.param2 == 0 )	// DSRDATA_TYPE_REAL
		*(arg - 1) = CDSRReal( arctg( (arg - 1)->getReal() ) );
	else					// DSRDATA_TYPE_COMPLEX
		*(arg - 1) = CDSRComplex( arctg( (arg - 1)->getComplex() ) );
	return 1 - addr.param3;
}

long __Call_arcth( CDSR_VMEval& /*vm*/, MMD_Address& addr, UniWord *arg )
{
#if _DEBUG
	if( addr.param2 < 0 )
		throw _T("__Call_(fun) : internal error, out of range");
#endif
	if( addr.param2 == 0 )	// DSRDATA_TYPE_REAL
		*(arg - 1) = CDSRReal( arcth( (arg - 1)->getReal() ) );
	else					// DSRDATA_TYPE_COMPLEX
		*(arg - 1) = CDSRComplex( arcth( (arg - 1)->getComplex() ) );
	return 1 - addr.param3;
}

long __Call_arsh( CDSR_VMEval& /*vm*/, MMD_Address& addr, UniWord *arg )
{
#if _DEBUG
	if( addr.param2 < 0 )
		throw _T("__Call_(fun) : internal error, out of range");
#endif
	if( addr.param2 == 0 )	// DSRDATA_TYPE_REAL
		*(arg - 1) = CDSRReal( arsh( (arg - 1)->getReal() ) );
	else					// DSRDATA_TYPE_COMPLEX
		*(arg - 1) = CDSRComplex( arsh( (arg - 1)->getComplex() ) );
	return 1 - addr.param3;
}

long __Call_ch( CDSR_VMEval& /*vm*/, MMD_Address& addr, UniWord *arg )
{
#if _DEBUG
	if( addr.param2 < 0 )
		throw _T("__Call_(fun) : internal error, out of range");
#endif
	if( addr.param2 == 0 )	// DSRDATA_TYPE_REAL
		*(arg - 1) = CDSRReal( ch( (arg - 1)->getReal() ) );
	else					// DSRDATA_TYPE_COMPLEX
		*(arg - 1) = CDSRComplex( ch( (arg - 1)->getComplex() ) );
	return 1 - addr.param3;
}

long __Call_cos( CDSR_VMEval& /*vm*/, MMD_Address& addr, UniWord *arg )
{
#if _DEBUG
	if( addr.param2 < 0 )
		throw _T("__Call_(fun) : internal error, out of range");
#endif
	if( addr.param2 == 0 )	// DSRDATA_TYPE_REAL
		*(arg - 1) = CDSRReal( cos( (arg - 1)->getReal() ) );
	else					// DSRDATA_TYPE_COMPLEX
		*(arg - 1) = CDSRComplex( cos( (arg - 1)->getComplex() ) );
	return 1 - addr.param3;
}

long __Call_cosec( CDSR_VMEval& /*vm*/, MMD_Address& addr, UniWord *arg )
{
#if _DEBUG
	if( addr.param2 < 0 )
		throw _T("__Call_(fun) : internal error, out of range");
#endif
	if( addr.param2 == 0 )	// DSRDATA_TYPE_REAL
		*(arg - 1) = CDSRReal( cosec( (arg - 1)->getReal() ) );
	else					// DSRDATA_TYPE_COMPLEX
		*(arg - 1) = CDSRComplex( cosec( (arg - 1)->getComplex() ) );
	return 1 - addr.param3;
}

long __Call_cosech( CDSR_VMEval& /*vm*/, MMD_Address& addr, UniWord *arg )
{
#if _DEBUG
	if( addr.param2 < 0 )
		throw _T("__Call_(fun) : internal error, out of range");
#endif
	if( addr.param2 == 0 )	// DSRDATA_TYPE_REAL
		*(arg - 1) = CDSRReal( cosech( (arg - 1)->getReal() ) );
	else					// DSRDATA_TYPE_COMPLEX
		*(arg - 1) = CDSRComplex( cosech( (arg - 1)->getComplex() ) );
	return 1 - addr.param3;
}

long __Call_ctg( CDSR_VMEval& /*vm*/, MMD_Address& addr, UniWord *arg )
{
#if _DEBUG
	if( addr.param2 < 0 )
		throw _T("__Call_(fun) : internal error, out of range");
#endif
	if( addr.param2 == 0 )	// DSRDATA_TYPE_REAL
		*(arg - 1) = CDSRReal( ctg( (arg - 1)->getReal() ) );
	else					// DSRDATA_TYPE_COMPLEX
		*(arg - 1) = CDSRComplex( ctg( (arg - 1)->getComplex() ) );
	return 1 - addr.param3;
}

long __Call_cth( CDSR_VMEval& /*vm*/, MMD_Address& addr, UniWord *arg )
{
#if _DEBUG
	if( addr.param2 < 0 )
		throw _T("__Call_(fun) : internal error, out of range");
#endif
	if( addr.param2 == 0 )	// DSRDATA_TYPE_REAL
		*(arg - 1) = CDSRReal( cth( (arg - 1)->getReal() ) );
	else					// DSRDATA_TYPE_COMPLEX
		*(arg - 1) = CDSRComplex( cth( (arg - 1)->getComplex() ) );
	return 1 - addr.param3;
}

long __Call_sec( CDSR_VMEval& /*vm*/, MMD_Address& addr, UniWord *arg )
{
#if _DEBUG
	if( addr.param2 < 0 )
		throw _T("__Call_(fun) : internal error, out of range");
#endif
	if( addr.param2 == 0 )	// DSRDATA_TYPE_REAL
		*(arg - 1) = CDSRReal( sec( (arg - 1)->getReal() ) );
	else					// DSRDATA_TYPE_COMPLEX
		*(arg - 1) = CDSRComplex( sec( (arg - 1)->getComplex() ) );
	return 1 - addr.param3;
}

long __Call_sech( CDSR_VMEval& /*vm*/, MMD_Address& addr, UniWord *arg )
{
#if _DEBUG
	if( addr.param2 < 0 )
		throw _T("__Call_(fun) : internal error, out of range");
#endif
	if( addr.param2 == 0 )	// DSRDATA_TYPE_REAL
		*(arg - 1) = CDSRReal( sech( (arg - 1)->getReal() ) );
	else					// DSRDATA_TYPE_COMPLEX
		*(arg - 1) = CDSRComplex( sech( (arg - 1)->getComplex() ) );
	return 1 - addr.param3;
}

long __Call_sin( CDSR_VMEval& /*vm*/, MMD_Address& addr, UniWord *arg )
{
#if _DEBUG
	if( addr.param2 < 0 )
		throw _T("__Call_(fun) : internal error, out of range");
#endif
	if( addr.param2 == 0 )	// DSRDATA_TYPE_REAL
		*(arg - 1) = CDSRReal( sin( (arg - 1)->getReal() ) );
	else					// DSRDATA_TYPE_COMPLEX
		*(arg - 1) = CDSRComplex( sin( (arg - 1)->getComplex() ) );
	return 1 - addr.param3;
}

long __Call_sh( CDSR_VMEval& /*vm*/, MMD_Address& addr, UniWord *arg )
{
#if _DEBUG
	if( addr.param2 < 0 )
		throw _T("__Call_(fun) : internal error, out of range");
#endif
	if( addr.param2 == 0 )	// DSRDATA_TYPE_REAL
		*(arg - 1) = CDSRReal( sh( (arg - 1)->getReal() ) );
	else					// DSRDATA_TYPE_COMPLEX
		*(arg - 1) = CDSRComplex( sh( (arg - 1)->getComplex() ) );
	return 1 - addr.param3;
}

long __Call_sqrt( CDSR_VMEval& /*vm*/, MMD_Address& addr, UniWord *arg )
{
#if _DEBUG
	if( addr.param2 < 0 )
		throw _T("__Call_(fun) : internal error, out of range");
#endif
	if( addr.param2 == 0 )	// DSRDATA_TYPE_REAL
		*(arg - 1) = CDSRReal( sqrt( (arg - 1)->getReal() ) );
	else					// DSRDATA_TYPE_COMPLEX
		*(arg - 1) = CDSRComplex( sqrt( (arg - 1)->getComplex() ) );
	return 1 - addr.param3;
}

long __Call_tg( CDSR_VMEval& /*vm*/, MMD_Address& addr, UniWord *arg )
{
#if _DEBUG
	if( addr.param2 < 0 )
		throw _T("__Call_(fun) : internal error, out of range");
#endif
	if( addr.param2 == 0 )	// DSRDATA_TYPE_REAL
		*(arg - 1) = CDSRReal( tg( (arg - 1)->getReal() ) );
	else					// DSRDATA_TYPE_COMPLEX
		*(arg - 1) = CDSRComplex( tg( (arg - 1)->getComplex() ) );
	return 1 - addr.param3;
}

long __Call_th( CDSR_VMEval& /*vm*/, MMD_Address& addr, UniWord *arg )
{
#if _DEBUG
	if( addr.param2 < 0 )
		throw _T("__Call_(fun) : internal error, out of range");
#endif
	if( addr.param2 == 0 )	// DSRDATA_TYPE_REAL
		*(arg - 1) = CDSRReal( th( (arg - 1)->getReal() ) );
	else					// DSRDATA_TYPE_COMPLEX
		*(arg - 1) = CDSRComplex( th( (arg - 1)->getComplex() ) );
	return 1 - addr.param3;
}

long __Call_exp( CDSR_VMEval& /*vm*/, MMD_Address& addr, UniWord *arg )
{
#if _DEBUG
	if( addr.param2 < 0 )
		throw _T("__Call_(fun) : internal error, out of range");
#endif
	if( addr.param2 == 0 )	// DSRDATA_TYPE_REAL
		*(arg - 1) = CDSRReal( exp( (arg - 1)->getReal() ) );
	else					// DSRDATA_TYPE_COMPLEX
		*(arg - 1) = CDSRComplex( exp( (arg - 1)->getComplex() ) );
	return 1 - addr.param3;
}

long __Call_int( CDSR_VMEval& /*vm*/, MMD_Address& addr, UniWord *arg )
{
#if _DEBUG
	if( addr.param2 < 0 )
		throw _T("__Call_(fun) : internal error, out of range");
#endif
	if( addr.param2 == 0 )	// DSRDATA_TYPE_REAL
		*(arg - 1) = CDSRInteger( (long)((arg - 1)->getReal()) );
	else					// DSRDATA_TYPE_COMPLEX
		*(arg - 1) = CDSRInteger( (long)real((arg - 1)->getComplex()) );
	return 1 - addr.param3;
}

long __Call_lg( CDSR_VMEval& /*vm*/, MMD_Address& addr, UniWord *arg )
{
#if _DEBUG
	if( addr.param2 < 0 )
		throw _T("__Call_(fun) : internal error, out of range");
#endif
	if( addr.param2 == 0 )	// DSRDATA_TYPE_REAL
		*(arg - 1) = CDSRReal( lg( (arg - 1)->getReal() ) );
	else					// DSRDATA_TYPE_COMPLEX
		*(arg - 1) = CDSRComplex( lg( (arg - 1)->getComplex() ) );
	return 1 - addr.param3;
}

long __Call_ln( CDSR_VMEval& /*vm*/, MMD_Address& addr, UniWord *arg )
{
#if _DEBUG
	if( addr.param2 < 0 )
		throw _T("__Call_(fun) : internal error, out of range");
#endif
	if( addr.param2 == 0 )	// DSRDATA_TYPE_REAL
		*(arg - 1) = CDSRReal( ln( (arg - 1)->getReal() ) );
	else					// DSRDATA_TYPE_COMPLEX
		*(arg - 1) = CDSRComplex( ln( (arg - 1)->getComplex() ) );
	return 1 - addr.param3;
}

long __Call_log( CDSR_VMEval& /*vm*/, MMD_Address& addr, UniWord *arg )
{
#if _DEBUG
	if( addr.param2 < 0 )
		throw _T("__Call_(fun) : internal error, out of range");
#endif
	if( addr.param2 == 0 )	// DSRDATA_TYPE_REAL
		*(arg - 2) = CDSRReal( log( (arg - 2)->getReal() ) / log( (arg - 1)->getReal() ) );
	else					// DSRDATA_TYPE_COMPLEX
		*(arg - 2) = CDSRComplex( log( (arg - 2)->getComplex() ) / log( (arg - 1)->getComplex() ) );
	return 1 - addr.param3;
}

long __Call_pow( CDSR_VMEval& /*vm*/, MMD_Address& addr, UniWord *arg )
{
#if _DEBUG
	if( addr.param2 < 0 )
		throw _T("__Call_(fun) : internal error, out of range");
#endif
	switch( addr.param2 )
	{
	case 0:	// DSRDATA_TYPE_REAL,    DSRDATA_TYPE_REAL    -> DSRDATA_TYPE_REAL
		*(arg - 2) = CDSRReal( pow( (arg - 2)->getReal(), (arg - 1)->getReal() ) );
		break;
	case 1:	// DSRDATA_TYPE_COMPLEX, DSRDATA_TYPE_COMPLEX -> DSRDATA_TYPE_COMPLEX
		*(arg - 2) = CDSRComplex( pow( (arg - 2)->getComplex(), (arg - 1)->getComplex() ) );
		break;
	case 2:	// DSRDATA_TYPE_COMPLEX, DSRDATA_TYPE_REAL    -> DSRDATA_TYPE_COMPLEX
		*(arg - 2) = CDSRComplex( pow( (arg - 2)->getComplex(), (arg - 1)->getReal() ) );
		break;
	}
	return 1 - addr.param3;
}

long __Call_root( CDSR_VMEval& /*vm*/, MMD_Address& addr, UniWord *arg )
{
#if _DEBUG
	if( addr.param2 < 0 )
		throw _T("__Call_(fun) : internal error, out of range");
#endif
	switch( addr.param2 )
	{
	case 0:	// DSRDATA_TYPE_REAL,    DSRDATA_TYPE_REAL    -> DSRDATA_TYPE_REAL
		*(arg - 2) = CDSRReal( pow( (arg - 2)->getReal(), 1.0 / (arg - 1)->getReal() ) );
		break;
	case 1:	// DSRDATA_TYPE_COMPLEX, DSRDATA_TYPE_COMPLEX -> DSRDATA_TYPE_COMPLEX
		*(arg - 2) = CDSRComplex( pow( (arg - 2)->getComplex(), 1.0 / (arg - 1)->getComplex() ) );
		break;
	case 2:	// DSRDATA_TYPE_COMPLEX, DSRDATA_TYPE_REAL    -> DSRDATA_TYPE_COMPLEX
		*(arg - 2) = CDSRComplex( pow( (arg - 2)->getComplex(), 1.0 / (arg - 1)->getReal() ) );
		break;
	}
	return 1 - addr.param3;
}

long __Call_atan2( CDSR_VMEval& /*vm*/, MMD_Address& addr, UniWord *arg )
{
#if _DEBUG
	if( addr.param2 < 0 )
		throw _T("__Call_(fun) : internal error, out of range");
#endif
	*(arg - 2) = CDSRReal( atan2( (arg - 2)->getReal(), (arg - 1)->getReal() ) );
	return 1 - addr.param3;
}

long __Call_fmod( CDSR_VMEval& /*vm*/, MMD_Address& addr, UniWord *arg )
{
#if _DEBUG
	if( addr.param2 < 0 )
		throw _T("__Call_(fun) : internal error, out of range");
#endif
	*(arg - 2) = CDSRReal( fmod( (arg - 2)->getReal(), (arg - 1)->getReal() ) );
	return 1 - addr.param3;
}

long __Call_polar( CDSR_VMEval& /*vm*/, MMD_Address& addr, UniWord *arg )
{
#if _DEBUG
	if( addr.param2 < 0 )
		throw _T("__Call_(fun) : internal error, out of range");
#endif
	*(arg - 2) = CDSRComplex( std::polar( (arg - 2)->getReal(), (arg - 1)->getReal() ) );
	return 1 - addr.param3;
}

long __Call_ceil( CDSR_VMEval& /*vm*/, MMD_Address& addr, UniWord *arg )
{
#if _DEBUG
	if( addr.param2 < 0 )
		throw _T("__Call_(fun) : internal error, out of range");
#endif
	*(arg - 1) = CDSRReal( ceil( (arg - 1)->getReal() ) );
	return 1 - addr.param3;
}

long __Call_floor( CDSR_VMEval& /*vm*/, MMD_Address& addr, UniWord *arg )
{
#if _DEBUG
	if( addr.param2 < 0 )
		throw _T("__Call_(fun) : internal error, out of range");
#endif
	*(arg - 1) = CDSRReal( floor( (arg - 1)->getReal() ) );
	return 1 - addr.param3;
}

long __Call_imag( CDSR_VMEval& /*vm*/, MMD_Address& addr, UniWord *arg )
{
#if _DEBUG
	if( addr.param2 < 0 )
		throw _T("__Call_(fun) : internal error, out of range");
#endif
	*(arg - 1) = CDSRReal( imag( (arg - 1)->getComplex() ) );
	return 1 - addr.param3;
}

long __Call_pow10( CDSR_VMEval& /*vm*/, MMD_Address& addr, UniWord *arg )
{
#if _DEBUG
	if( addr.param2 < 0 )
		throw _T("__Call_(fun) : internal error, out of range");
#endif
	*(arg - 1) = CDSRReal( pow10( (arg - 1)->getReal() ) );
	return 1 - addr.param3;
}

long __Call_real( CDSR_VMEval& /*vm*/, MMD_Address& addr, UniWord *arg )
{
#if _DEBUG
	if( addr.param2 < 0 )
		throw _T("__Call_(fun) : internal error, out of range");
#endif
	*(arg - 1) = CDSRReal( real( (arg - 1)->getComplex() ) );
	return 1 - addr.param3;
}

long __Call_random( CDSR_VMEval& vm, MMD_Address& addr, UniWord * /*arg*/ )
{
#if _DEBUG
	if( addr.param2 < 0 )
		throw _T("__Call_(fun) : internal error, out of range");
#endif
	vm.m_run_stack[ vm.m_run_stack_ptr ] = CDSRInteger( rand() );
	return 1 - addr.param3;
}

long __Call_identity( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg )
{
#if _DEBUG
	if( addr.param2 < 0 )
		throw _T("__Call_(fun) : internal error, out of range");
#endif
	*(arg - 1) = _dsrop_ConstructVector_identity( vm.getSmbTable(), (arg - 1)->getNatural() );
	return 1 - addr.param3;
}

long __Call_primes( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg )
{
#if _DEBUG
	if( addr.param2 < 0 )
		throw _T("__Call_(fun) : internal error, out of range");
#endif

#ifndef _NO_HMATHBS
	CDSRArray<CDSRNatural> res;
	if( addr.param2 == 0 )	// 1 arg
	{
		CDSRNatural nV( (arg - 1)->getNatural() );
		::EratosfenACM( nV, res );
		*(arg - 1) = vm.getSmbTable().makeUniWord_Vector( res );
	}
	else					// 2 arg
	{
		CDSRNatural nV1( (arg - 2)->getNatural() );
		CDSRNatural nV2( (arg - 1)->getNatural() );
		::EratosfenRange( nV1, 
						nV2, 
						res );
		*(arg - 2) = vm.getSmbTable().makeUniWord_Vector( res );
	}
#endif
	return 1 - addr.param3;
}

long __Call_isprime( CDSR_VMEval& /*vm*/, MMD_Address& addr, UniWord *arg )
{
#if _DEBUG
	if( addr.param2 < 0 )
		throw _T("__Call_(fun) : internal error, out of range");
#endif

#ifndef _NO_HMATHBS
	CDSRNatural nV( (arg - 1)->getNatural() );
	*(arg - 1) = CDSRInteger( ::IsNaturalSimple( nV ) );
#endif
	return 1 - addr.param3;
}

long __Call_gcd( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg )
{
#if _DEBUG
	if( addr.param2 < 0 )
		throw _T("__Call_(fun) : internal error, out of range");
#endif

#ifndef _NO_HMATHBS
	CDSRArray<CDSRNatural> m;
	vm.getSmbTable().convert2DSRNaturalArray( (arg - 1)->getVector(), m );
	*(arg - 1) = CDSRNatural( ::EuqlidArray( m ) );
#endif
	return 1 - addr.param3;
}

long __Call_lcm( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg )
{
#if _DEBUG
	if( addr.param2 < 0 )
		throw _T("__Call_(fun) : internal error, out of range");
#endif

#ifndef _NO_HMATHBS
	CDSRArray<CDSRNatural> m;
	vm.getSmbTable().convert2DSRNaturalArray( (arg - 1)->getVector(), m );
	*(arg - 1) = CDSRNatural( ::GetLeastCommonMultipleArray( m ) );
#endif
	return 1 - addr.param3;
}

long __Call_polyroots( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg )
{
#if _DEBUG
	if( addr.param2 < 0 )
		throw _T("__Call_(fun) : internal error, out of range");
#endif

#ifndef _NO_HMATHBS
	long i, j;
	if( addr.param2 == 0 )
	{
		CDSRArray<CDSRReal> pPolinom;
		CDSRArray<CDSRReal>* ptrPolinom = &pPolinom;
		CDSRArray<ROOT_INFO> pRootsArray;
		CDSRArray<ROOT_INFO>* ptrRootsArray = &pRootsArray;
		vm.getSmbTable().convert2DSRRealArray( (arg - 1)->getPolynomial(), pPolinom );
		_rootsOfPolinom( ptrPolinom, ptrRootsArray );
		CDSRArray<CDSRComplex> res;
		for( i = 0; i < (long) pRootsArray.size(); i++ )
			for( j = 0; j < pRootsArray[ i ].multiplicity; j++ )
				res.push_back( pRootsArray[ i ].getComplexValue() );
		*(arg - 1) = vm.getSmbTable().makeUniWord_Vector( res );
	}
	else
	{
		CDSRArray<CDSRReal> pPolinom;
		CDSRArray<CDSRReal>* ptrPolinom = &pPolinom;
		CDSRArray<ROOT_INFO> pRootsArray;
		CDSRArray<ROOT_INFO>* ptrRootsArray = &pRootsArray;
		vm.getSmbTable().convert2DSRRealArray( (arg - 2)->getPolynomial(), pPolinom );
		CDSRReal rlPrec = (arg - 1)->getReal();
		_rootsOfPolinom( ptrPolinom, ptrRootsArray, rlPrec );
		CDSRArray<CDSRComplex> res;
		for( i = 0; i < (long) pRootsArray.size(); i++ )
			for( j = 0; j < pRootsArray[ i ].multiplicity; j++ )
				res.push_back( pRootsArray[ i ].getComplexValue() );
		*(arg - 2) = vm.getSmbTable().makeUniWord_Vector( res );
	}
#endif
	return 1 - addr.param3;
}

long __Call_polyder( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg )
{
#if _DEBUG
	if( addr.param2 < 0 )
		throw _T("__Call_(fun) : internal error, out of range");
#endif

#ifndef _NO_HMATHBS
	CDSRArray<CDSRReal> pPolinom, pDerivative;
	vm.getSmbTable().convert2DSRRealArray( (arg - 2)->getPolynomial(), pPolinom );
	unsigned long nDerivative = (arg - 1)->getNatural();
	_derivativePolinom( &pPolinom, nDerivative, &pDerivative );
	*(arg - 2) = vm.getSmbTable().makeUniWord_Polynomial( pDerivative );
#endif
	return 1 - addr.param3;
}

long __Call_polyint( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg )
{
#if _DEBUG
	if( addr.param2 < 0 )
		throw _T("__Call_(fun) : internal error, out of range");
#endif

#ifndef _NO_HMATHBS
	CDSRArray<CDSRReal> pPolinom, pIntegral;
	CDSRArray<CDSRReal>* ptrPolinom = &pPolinom;
	CDSRArray<CDSRReal>* ptrIntegral = &pIntegral;
	vm.getSmbTable().convert2DSRRealArray( (arg - 3)->getPolynomial(), pPolinom );
	CDSRReal rlBegX = (arg - 2)->getReal();
	CDSRReal rlBegY = (arg - 1)->getReal();
	_integral( ptrPolinom, ptrIntegral, rlBegX, rlBegY );
	*(arg - 3) = vm.getSmbTable().makeUniWord_Polynomial( pIntegral );
#endif
	return 1 - addr.param3;
}

long __Call_polygcd( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg )
{
#if _DEBUG
	if( addr.param2 < 0 )
		throw _T("__Call_(fun) : internal error, out of range");
#endif

#ifndef _NO_HMATHBS
	CDSRArray<CDSRReal> p1, p2, pres;
	vm.getSmbTable().convert2DSRRealArray( (arg - 2)->getPolynomial(), p1 );
	vm.getSmbTable().convert2DSRRealArray( (arg - 1)->getPolynomial(), p2 );
	_greatCommonDevider( &p1, &p2, &pres );
	*(arg - 2) = vm.getSmbTable().makeUniWord_Polynomial( pres );
#endif
	return 1 - addr.param3;
}

long __Call_polylcm( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg )
{
#if _DEBUG
	if( addr.param2 < 0 )
		throw _T("__Call_(fun) : internal error, out of range");
#endif

#ifndef _NO_HMATHBS
	CDSRArray<CDSRReal> p1, p2, pres;
	CDSRArray<CDSRReal>* ptr_p1 = &p1;
	CDSRArray<CDSRReal>* ptr_p2 = &p2;
	CDSRArray<CDSRReal>* ptr_pres = &pres;
	vm.getSmbTable().convert2DSRRealArray( (arg - 2)->getPolynomial(), p1 );
	vm.getSmbTable().convert2DSRRealArray( (arg - 1)->getPolynomial(), p2 );
	_leastCommonMultiple( ptr_p1, ptr_p2, ptr_pres );
	*(arg - 2) = vm.getSmbTable().makeUniWord_Polynomial( pres );
#endif
	return 1 - addr.param3;
}

long __Call_lsolve( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg )
{
#if _DEBUG
	if( addr.param2 < 0 )
		throw _T("__Call_(fun) : internal error, out of range");
#endif

#ifndef _NO_HMATHBS
	QString _method = _T("");
	if( addr.param2 == 1 )
		_method = *((arg - 1)->getString());
	CDSRMMatrix<CDSRReal> pA;
	CDSRMVector<CDSRReal> pB;
	CDSRMVector<CDSRReal> pX;
	vm.getSmbTable().convert2DSRRealMMatrix( (arg - addr.param3)->getVector(), pA );
	vm.getSmbTable().convert2DSRRealMVector( (arg - addr.param3 + 1)->getVector(), pB );
	::_LSolve( &pA, &pB, &pX, _method.toStdString() );
	*(arg - addr.param3) = vm.getSmbTable().makeUniWord_Vector( pX );
#endif
	return 1 - addr.param3;
}

long __Call_ratfpart( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg )
{
#if _DEBUG
	if( addr.param2 < 0 )
		throw _T("__Call_(fun) : internal error, out of range");
#endif

#ifndef _NO_HMATHBS
	MMD_Polynomial *hi = (arg - 1)->getRationalFun()->hi;
	MMD_Polynomial *lo = (arg - 1)->getRationalFun()->lo;
	CDSRArray<CDSRReal> pNum, pDen;
	vm.getSmbTable().convert2DSRRealArray( hi, pNum );
	vm.getSmbTable().convert2DSRRealArray( lo, pDen );
    
	CDSRArray<CDSRArray<CDSRReal>*> partNums;
	CDSRArray<CDSRArray<CDSRReal>*> partDenoms;
	CDSRArray<unsigned long> MultiplicityArray;
    
	::_partFractDecompos( &pNum, &pDen, partNums, partDenoms, MultiplicityArray );

#if _DEBUG
	if( partNums.size() != partDenoms.size() || 
		partNums.size() != MultiplicityArray.size() )
		throw _T("__Call_ratfpart : internal error, _partFractDecompos does wrong output");
#endif

	std::vector<QString> rs;
	QString result = _T("");
	long i, j;
	for( i = 0; i < (long) MultiplicityArray.size(); i++ )
	{
		for( j = 0; j < (long) MultiplicityArray[ i ]; j++ )
		{
			MMD_Polynomial *hiRes, *loRes;
			hiRes = vm.getSmbTable().makeUniWord_Polynomial( *(partNums[ i ]) );
			loRes = vm.getSmbTable().makeUniWord_Polynomial( *(partDenoms[ i ]) );
			MMD_RationalFun *r = vm.getSmbTable().makeUniWord( new MMD_RationalFun( vm.getSmbTable(), hiRes, loRes ) );
			result += r->getFormulatorView();
			rs.push_back( result );
		}
	}

	if( rs.size() ) result = rs[ 0 ];
	for( i = 1; i < (long) rs.size(); i++ )
		result += " + " + rs[ i ];
	*(arg - 1) = vm.getSmbTable().makeUniWord( new MMD_String( vm.getSmbTable(), result ) );
#endif
	return 1 - addr.param3;
}

long __Call_ratfder( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg )
{
#if _DEBUG
	if( addr.param2 < 0 )
		throw _T("__Call_(fun) : internal error, out of range");
#endif

#ifndef _NO_HMATHBS
	MMD_Polynomial *hi = (arg - 1)->getRationalFun()->hi;
	MMD_Polynomial *lo = (arg - 1)->getRationalFun()->lo;
	CDSRArray<CDSRReal> pNum, pDen;
	vm.getSmbTable().convert2DSRRealArray( hi, pNum );
	vm.getSmbTable().convert2DSRRealArray( lo, pDen );
	CDSRArray<CDSRReal> pNumRes, pDenRes;

	_rationalFuncDeriv( &pNum, &pDen, &pNumRes, &pDenRes );

	MMD_Polynomial *hiRes, *loRes;
	hiRes = vm.getSmbTable().makeUniWord_Polynomial( pNumRes );
	loRes = vm.getSmbTable().makeUniWord_Polynomial( pDenRes );
	*(arg - 1) = vm.getSmbTable().makeUniWord( new MMD_RationalFun( vm.getSmbTable(), hiRes, loRes ) );
#endif
	return 1 - addr.param3;
}

#define __Call_det_subcalls_LENGTH	3
static SUBCALL_ABS __Call_det_subcalls[ __Call_det_subcalls_LENGTH ] = 
{
	_dsrop_fun_abs_vector_integer,
	_dsrop_fun_abs_vector_real,
	_dsrop_fun_abs_vector_complex
};
long __Call_det( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg )
{
#if _DEBUG
	if( addr.param2 < 0 || 
		addr.param2 >= __Call_det_subcalls_LENGTH )
		throw _T("__Call_det : internal error, out of range");
#endif
	__Call_det_subcalls[ addr.param2 ]( arg - addr.param3, vm.getSmbTable() );
	return 1 - addr.param3;
}

long __Call_inverse( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg )
{
#if _DEBUG
	if( addr.param2 < 0 )
		throw _T("__Call_(fun) : internal error, out of range");
#endif

	if( addr.param2 == 0 ) // DSRDATA_TYPE_REAL
		_dsrop_VectorInverse_real( arg - addr.param3, (arg - addr.param3)->getVector(), vm.getSmbTable() );
	else // DSRDATA_TYPE_COMPLEX
		_dsrop_VectorInverse_complex( arg - addr.param3, (arg - addr.param3)->getVector(), vm.getSmbTable() );

	return 1 - addr.param3;
}

//!!!!!!!!!!!!!!!!!!!!!! TO DO !!!!!!!!!!!!!!!!!!!!!!
long __Call_eigenvals( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg )
{
#if _DEBUG
	if( addr.param2 < 0 )
		throw _T("__Call_(fun) : internal error, out of range");
#endif

#ifndef _NO_HMATHBS
	CDSRMMatrix<CDSRReal> a;
	CDSRArray<CDSRComplex> res;
	CDSRArray<ROOT_INFO> Lyambda;
	CDSRReal rlPrec;
	long i, j;

	switch( addr.param2 )
	{
	case 0: // Vec -> Vec
		vm.getSmbTable().convert2DSRRealMMatrix( (arg - 1)->getVector(), a );
		_eigenValDanilev( a, Lyambda );
		for( i = 0; i < (long) Lyambda.size(); i++ )
			for( j = 0; j < Lyambda[ i ].multiplicity; j++ )
				res.push_back( Lyambda[ i ].getComplexValue() );
		*(arg - 1) = vm.getSmbTable().makeUniWord_Vector( res );
		break;
	case 1: // Vec, Real -> Vec
		vm.getSmbTable().convert2DSRRealMMatrix( (arg - 2)->getVector(), a );
		rlPrec = (arg - 1)->getReal();
		_eigenValDanilev( a, Lyambda, rlPrec );
		for( i = 0; i < (long) Lyambda.size(); i++ )
			for( j = 0; j < Lyambda[ i ].multiplicity; j++ )
				res.push_back( Lyambda[ i ].getComplexValue() );
		*(arg - 2) = vm.getSmbTable().makeUniWord_Vector( res );
		break;
	}
#endif
	return 1 - addr.param3;
}

///////////////////////////////////////////////////////////////////////////////
