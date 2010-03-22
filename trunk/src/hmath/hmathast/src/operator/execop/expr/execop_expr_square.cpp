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

long __Call_Square( CDSR_VMEval& /*vm*/, MMD_Address& addr, UniWord *arg )
{
	if( (arg - 2)->getVector()->isValidIdx( (arg - 1)->getInteger() ) )
		*(arg - 2) = (arg - 2)->getVector()->get( (arg - 1)->getInteger() );
	else
		throw "vector error: out of range";
	return 1 - addr.param3;
}

///////////////////////////////////////////////////////////////////////////////

long __Call_SquareLValue( CDSR_VMEval& vm, MMD_Address& addr, UniWord *arg )
{
	switch( (arg - 2)->v_addr.param1 )
	{
	case VARIABLE_USER:
#if _DEBUG
		if( (arg - 2)->v_addr.param2 < vm.getVariable_Begin() || 
			(arg - 2)->v_addr.param2 >= vm.getVariable_End() )
			throw _T("__Call_SquareLValue : internal error, variable number is out of range in assignment");
#endif
		(arg - 2)->v_addr.param1 = VARIABLE_USER_INDIRECT;
		(arg - 2)->v_addr.param3 = (arg - 1)->getInteger();
		if( !vm.m_var_table[ (arg - 2)->v_addr.param2 ].getVector()->isValidIdx( (arg - 2)->v_addr.param3 ) )
			throw "vector error: out of range";
		break;

	case LOCAL_VARIABLE_USER:
#if _DEBUG
		if( (arg - 2)->v_addr.param2 < 1 || 
			vm.m_BP + (arg - 2)->v_addr.param2 - 1 >= vm.m_SP )
			throw _T("__Call_SquareLValue : internal error, local variable number is out of range in assignment");
#endif
		(arg - 2)->v_addr.param1 = LOCAL_VARIABLE_USER_INDIRECT;
		(arg - 2)->v_addr.param3 = (arg - 1)->getInteger();
		if( !vm.m_stack[ vm.m_BP + (arg - 2)->v_addr.param2 - 1 ].getVector()->isValidIdx( (arg - 2)->v_addr.param3 ) )
			throw "vector error: out of range";
		break;

	case VARIABLE_USER_INDIRECT:
		{
#if _DEBUG
		if( (arg - 2)->v_addr.param2 < vm.getVariable_Begin() || 
			(arg - 2)->v_addr.param2 >= vm.getVariable_End() )
			throw _T("__Call_SquareLValue : internal error, variable number is out of range in assignment");
#endif
		UniWord& vect = vm.m_var_table[ (arg - 2)->v_addr.param2 ];
#if _DEBUG
		if( !vect.getVector() ||
			(arg - 2)->v_addr.param3 < 1 || 
			(arg - 2)->v_addr.param3 > vect.getVector()->getRows() )
			throw _T("__Call_SquareLValue : internal error, global vector variable index is invalid");
#endif
		if( (arg - 1)->getInteger() < 1 || 
			(arg - 1)->getInteger() > vect.getVector()->getColumns() )
			throw "vector error: out of range";
		(arg - 2)->v_addr.param3 = ((arg - 2)->v_addr.param3 - 1) * vect.getVector()->getColumns() + (arg - 1)->getInteger();
		if( (arg - 2)->v_addr.param3 > (long) vect.getVector()->getCount() )
			throw "vector error: out of range";
		}
		break;

	case LOCAL_VARIABLE_USER_INDIRECT:
		{
#if _DEBUG
		if( (arg - 2)->v_addr.param2 < 1 || 
			vm.m_BP + (arg - 2)->v_addr.param2 - 1 >= vm.m_SP )
			throw _T("__Call_SquareLValue : internal error, local variable number is out of range in assignment");
#endif
		UniWord& vect = vm.m_stack[ vm.m_BP + (arg - 2)->v_addr.param2 - 1 ];
#if _DEBUG
		if( !vect.getVector() ||
			(arg - 2)->v_addr.param3 < 1 || 
			(arg - 2)->v_addr.param3 > vect.getVector()->getRows() )
			throw _T("__Call_SquareLValue : internal error, local vector variable index is invalid");
#endif
		if( (arg - 1)->getInteger() < 1 || 
			(arg - 1)->getInteger() > vect.getVector()->getColumns() )
			throw "vector error: out of range";
		(arg - 2)->v_addr.param3 = ((arg - 2)->v_addr.param3 - 1) * vect.getVector()->getColumns() + (arg - 1)->getInteger();
		if( (arg - 2)->v_addr.param3 > (long) vect.getVector()->getCount() )
			throw "vector error: out of range";
		}
		break;

#if _DEBUG
	default:
		throw _T("__Call_SquareLValue : internal error, unknown addressing scheme");
#endif
	}

	return 1 - addr.param3;
}

///////////////////////////////////////////////////////////////////////////////
