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

#define __Call_BoolAnd_subcalls_LENGTH 1
long __Call_BoolAnd( CDSR_VMEval& /*vm*/, MMD_Address& addr, UniWord *arg )
{
#if _DEBUG
	if( addr.param2 < 0 || 
		addr.param2 >= __Call_BoolAnd_subcalls_LENGTH )
		throw _T("__Call_BoolAnd : internal error, out of range");
#endif
	_dsrop_BoolAnd_bool( *(arg - 2), *(arg - 2), *(arg - 1) );
	return 1 - addr.param3;
}

///////////////////////////////////////////////////////////////////////////////

#define __Call_BoolOr_subcalls_LENGTH 1
long __Call_BoolOr( CDSR_VMEval& /*vm*/, MMD_Address& addr, UniWord *arg )
{
#if _DEBUG
	if( addr.param2 < 0 || 
		addr.param2 >= __Call_BoolOr_subcalls_LENGTH )
		throw _T("__Call_BoolOr : internal error, out of range");
#endif
	_dsrop_BoolOr_bool( *(arg - 2), *(arg - 2), *(arg - 1) );
	return 1 - addr.param3;
}

///////////////////////////////////////////////////////////////////////////////

#define __Call_BoolNot_subcalls_LENGTH 1
long __Call_BoolNot( CDSR_VMEval& /*vm*/, MMD_Address& addr, UniWord *arg )
{
#if _DEBUG
	if( addr.param2 < 0 || 
		addr.param2 >= __Call_BoolNot_subcalls_LENGTH )
		throw _T("__Call_BoolNot : internal error, out of range");
#endif
	_dsrop_BoolNot_bool( *(arg - 1), *(arg - 1) );
	return 1 - addr.param3;
}

///////////////////////////////////////////////////////////////////////////////
