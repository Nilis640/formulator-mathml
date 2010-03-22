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
void _call_dsrop_Less_natural( UniWord *arg )
{
	_dsrop_Less_natural( *(arg - 2), *(arg - 2), *(arg - 1) );
}

__HMATHLNGDLL__
void _call_dsrop_Less_integer( UniWord *arg )
{
	_dsrop_Less_integer( *(arg - 2), *(arg - 2), *(arg - 1) );
}

__HMATHLNGDLL__
void _call_dsrop_Less_rational( UniWord *arg )
{
	_dsrop_Less_rational( *(arg - 2), *(arg - 2), *(arg - 1) );
}

__HMATHLNGDLL__
void _call_dsrop_Less_real( UniWord *arg )
{
	_dsrop_Less_real( *(arg - 2), *(arg - 2), *(arg - 1) );
}

///////////////////////////////////////////////////////////////////////////////

typedef void (*SUBCALL_LESS)( UniWord *arg );
#define __Call_Less_subcalls_LENGTH	4
static SUBCALL_LESS __Call_Less_subcalls[ __Call_Less_subcalls_LENGTH ] = 
{
	_call_dsrop_Less_natural,
	_call_dsrop_Less_integer,
	_call_dsrop_Less_rational,
	_call_dsrop_Less_real
};
long __Call_Less( CDSR_VMEval& /*vm*/, MMD_Address& addr, UniWord *arg )
{
#if _DEBUG
	if( addr.param2 < 0 || 
		addr.param2 >= __Call_Less_subcalls_LENGTH )
		throw _T("__Call_Less : internal error, out of range");
#endif
	__Call_Less_subcalls[ addr.param2 ]( arg );
	return 1 - addr.param3;
}

///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////

__HMATHLNGDLL__
void _call_dsrop_LessEq_natural( UniWord *arg )
{
	_dsrop_LessEq_natural( *(arg - 2), *(arg - 2), *(arg - 1) );
}

__HMATHLNGDLL__
void _call_dsrop_LessEq_integer( UniWord *arg )
{
	_dsrop_LessEq_integer( *(arg - 2), *(arg - 2), *(arg - 1) );
}

__HMATHLNGDLL__
void _call_dsrop_LessEq_rational( UniWord *arg )
{
	_dsrop_LessEq_rational( *(arg - 2), *(arg - 2), *(arg - 1) );
}

__HMATHLNGDLL__
void _call_dsrop_LessEq_real( UniWord *arg )
{
	_dsrop_LessEq_real( *(arg - 2), *(arg - 2), *(arg - 1) );
}

///////////////////////////////////////////////////////////////////////////////

typedef void (*SUBCALL_LESSEQ)( UniWord *arg );
#define __Call_LessEq_subcalls_LENGTH	4
static SUBCALL_LESSEQ __Call_LessEq_subcalls[ __Call_LessEq_subcalls_LENGTH ] = 
{
	_call_dsrop_LessEq_natural,
	_call_dsrop_LessEq_integer,
	_call_dsrop_LessEq_rational,
	_call_dsrop_LessEq_real
};
long __Call_LessEq( CDSR_VMEval& /*vm*/, MMD_Address& addr, UniWord *arg )
{
#if _DEBUG
	if( addr.param2 < 0 || 
		addr.param2 >= __Call_LessEq_subcalls_LENGTH )
		throw _T("__Call_LessEq : internal error, out of range");
#endif
	__Call_LessEq_subcalls[ addr.param2 ]( arg );
	return 1 - addr.param3;
}

///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////

__HMATHLNGDLL__
void _call_dsrop_Greater_natural( UniWord *arg )
{
	_dsrop_Greater_natural( *(arg - 2), *(arg - 2), *(arg - 1) );
}

__HMATHLNGDLL__
void _call_dsrop_Greater_integer( UniWord *arg )
{
	_dsrop_Greater_integer( *(arg - 2), *(arg - 2), *(arg - 1) );
}

__HMATHLNGDLL__
void _call_dsrop_Greater_rational( UniWord *arg )
{
	_dsrop_Greater_rational( *(arg - 2), *(arg - 2), *(arg - 1) );
}

__HMATHLNGDLL__
void _call_dsrop_Greater_real( UniWord *arg )
{
	_dsrop_Greater_real( *(arg - 2), *(arg - 2), *(arg - 1) );
}

///////////////////////////////////////////////////////////////////////////////

typedef void (*SUBCALL_GREATER)( UniWord *arg );
#define __Call_Greater_subcalls_LENGTH	4
static SUBCALL_GREATER __Call_Greater_subcalls[ __Call_Greater_subcalls_LENGTH ] = 
{
	_call_dsrop_Greater_natural,
	_call_dsrop_Greater_integer,
	_call_dsrop_Greater_rational,
	_call_dsrop_Greater_real
};
long __Call_Greater( CDSR_VMEval& /*vm*/, MMD_Address& addr, UniWord *arg )
{
#if _DEBUG
	if( addr.param2 < 0 || 
		addr.param2 >= __Call_Greater_subcalls_LENGTH )
		throw _T("__Call_Greater : internal error, out of range");
#endif
	__Call_Greater_subcalls[ addr.param2 ]( arg );
	return 1 - addr.param3;
}

///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////

__HMATHLNGDLL__
void _call_dsrop_GreaterEq_natural( UniWord *arg )
{
	_dsrop_GreaterEq_natural( *(arg - 2), *(arg - 2), *(arg - 1) );
}

__HMATHLNGDLL__
void _call_dsrop_GreaterEq_integer( UniWord *arg )
{
	_dsrop_GreaterEq_integer( *(arg - 2), *(arg - 2), *(arg - 1) );
}

__HMATHLNGDLL__
void _call_dsrop_GreaterEq_rational( UniWord *arg )
{
	_dsrop_GreaterEq_rational( *(arg - 2), *(arg - 2), *(arg - 1) );
}

__HMATHLNGDLL__
void _call_dsrop_GreaterEq_real( UniWord *arg )
{
	_dsrop_GreaterEq_real( *(arg - 2), *(arg - 2), *(arg - 1) );
}

///////////////////////////////////////////////////////////////////////////////

typedef void (*SUBCALL_GREATEREQ)( UniWord *arg );
#define __Call_GreaterEq_subcalls_LENGTH	4
static SUBCALL_GREATEREQ __Call_GreaterEq_subcalls[ __Call_GreaterEq_subcalls_LENGTH ] = 
{
	_call_dsrop_GreaterEq_natural,
	_call_dsrop_GreaterEq_integer,
	_call_dsrop_GreaterEq_rational,
	_call_dsrop_GreaterEq_real
};
long __Call_GreaterEq( CDSR_VMEval& /*vm*/, MMD_Address& addr, UniWord *arg )
{
#if _DEBUG
	if( addr.param2 < 0 || 
		addr.param2 >= __Call_GreaterEq_subcalls_LENGTH )
		throw _T("__Call_GreaterEq : internal error, out of range");
#endif
	__Call_GreaterEq_subcalls[ addr.param2 ]( arg );
	return 1 - addr.param3;
}

///////////////////////////////////////////////////////////////////////////////
