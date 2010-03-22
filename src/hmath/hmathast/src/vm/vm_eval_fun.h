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

#if !defined( __PARSEDSR_VM_VM_EVAL_FUN_H__ )
#define __PARSEDSR_VM_VM_EVAL_FUN_H__

#include "vm_eval_cmd.h"

///////////////////////////////////////////////////////////////////////////////

class __HMATHLNGDLL__ CDSR_VMEval_Function
{
public:
	QString name;
	long id;
	long n_arg, n_res;
	UniWordVector args, res;
	CDsrDataType_Composition *m_arg_data;
	CDsrDataType_Node *m_res_data;
	CDSR_VMEval_Line body;

	CDSR_VMEval_Function( void )
	{
		name = _T("");
		n_arg = 0;
		n_res = 0;
		m_arg_data = 0;
		m_res_data = 0;
	}

        int operator == ( CDSR_VMEval_Function& /*a*/ ) const
	{
		return 0;
	}
};

class __HMATHLNGDLL__ CDSR_VMEval_FT : public MArray<CDSR_VMEval_Function>
{
public:
	CDSR_VMEval_FT( void ) :
		MArray<CDSR_VMEval_Function>( 64, 0, 64 )
	{}
};

///////////////////////////////////////////////////////////////////////////////

__HMATHLNGDLL__
QString getTextView( CParseDsrSymbolTable& smbtable, CDSR_VMEval_Function& c );
__HMATHLNGDLL__
QString getTextView( CParseDsrSymbolTable& smbtable, CDSR_VMEval_FT& c );

///////////////////////////////////////////////////////////////////////////////

#endif
