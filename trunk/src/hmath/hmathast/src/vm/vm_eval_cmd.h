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

#if !defined( __PARSEDSR_VM_VM_EVAL_CMD_H__ )
#define __PARSEDSR_VM_VM_EVAL_CMD_H__

#include "../ast/astdsr.h"
#include "../data/data.h"
#include "vmidsr.h"

///////////////////////////////////////////////////////////////////////////////

enum CDSR_VMEval_Command
{
	VMC_DSR_NOP,

	VMC_DSR_SKIP_ON_FALSE,
	VMC_DSR_SKIP,
	VMC_DSR_PUSH_VARIABLE_ADDR,
	VMC_DSR_PUSH_VARIABLE_VALUE,
	VMC_DSR_PUSH_NATURAL,
	VMC_DSR_PUSH_INTEGER,
	VMC_DSR_PUSH_REAL,
	VMC_DSR_PUSH_STRING,
	VMC_DSR_OP,
	VMC_DSR_RETURN,	// param1 == the cardinal number of the result being assigned, param2 == function results number
	VMC_DSR_ASSIGN_FUN_RES,
	VMC_DSR_DEC_RUNSTACK_PTR,

	//VMC_DSR_SAVE_RUNSTACK_PTR,
	//VMC_DSR_LOAD_RUNSTACK_PTR,

	VMC_DSR_LAST
};

///////////////////////////////////////////////////////////////////////////////

struct __HMATHLNGDLL__ CDSR_VMEval_Cmd
{
	enum CDSR_VMEval_Command cmd;
	UniWord operand;
	long line;

	CDSR_VMEval_Cmd( void )
	{
		cmd = VMC_DSR_NOP;
		line = 0;
	}
	CDSR_VMEval_Cmd( enum CDSR_VMEval_Command _cmd, UniWord& _operand, long _line = 0 )
	{
		cmd = _cmd;
		operand = _operand;
		line = _line;
	}
	CDSR_VMEval_Cmd( enum CDSR_VMEval_Command _cmd, long _line = 0 )
	{
		cmd = _cmd;
		line = _line;
	}
	void setLParam( long d )
	{
		operand.v_addr.param1 = d;
	}
	long getLParam( void )
	{
		return operand.v_addr.param1;
	}

        int operator == ( CDSR_VMEval_Cmd& /*a*/ ) const
	{
		return 0;
	}
};

///////////////////////////////////////////////////////////////////////////////

class __HMATHLNGDLL__ CDSR_VMEval_Line : public MArray<CDSR_VMEval_Cmd>
{
public:
	CDSR_VMEval_Line( void ) : 
		MArray<CDSR_VMEval_Cmd>( 64, 0, 64 )
	{}
};

///////////////////////////////////////////////////////////////////////////////

__HMATHLNGDLL__
extern QString CDSR_VMEval_CommandName[ FUNCTION_STD_LAST ];
__HMATHLNGDLL__
QString vm_Cmd2String( long id );
__HMATHLNGDLL__
QString getTextView( CParseDsrSymbolTable& smbtable, CDSR_VMEval_Cmd& c );
__HMATHLNGDLL__
QString getTextView( CParseDsrSymbolTable& smbtable, CDSR_VMEval_Line& c );

///////////////////////////////////////////////////////////////////////////////

#endif
