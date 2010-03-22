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

#include "vmeval.h"
#include "../operator/execop/execop.h"

#include <time.h>
#include <cstdlib>
#include <cstdio>

///////////////////////////////////////////////////////////////////////////////

void __Call_execCommand_NOP( CDSR_VMEval& vm, CDSR_VMEval_Cmd& /*call*/ )
{
	vm.m_IP++;
}

void __Call_execCommand_SKIP_ON_FALSE( CDSR_VMEval& vm, CDSR_VMEval_Cmd& call )
{
#if _DEBUG
	if( vm.m_run_stack_ptr < 1 )
	{
		vm.getSmbTable().AddLine( _T("internal problem while running: run srack pointer is out of range"), call.line );
		vm.setError();
		return;
	}
#endif
	if( !vm.m_run_stack[ vm.m_run_stack_ptr - 1 ].getInteger() )
		vm.m_IP += call.operand.v_addr.param1;
	else
		vm.m_IP++;
	vm.m_run_stack_ptr--;
}

void __Call_execCommand_SKIP( CDSR_VMEval& vm, CDSR_VMEval_Cmd& call )
{
	vm.m_IP += call.operand.v_addr.param1;
}

void __Call_execCommand_PUSH_VARIABLE_ADDR( CDSR_VMEval& vm, CDSR_VMEval_Cmd& call )
{
	vm.m_run_stack[ vm.m_run_stack_ptr++ ] = call.operand.getAddress();
	vm.m_IP++;
}

void __Call_execCommand_PUSH_VARIABLE_VALUE( CDSR_VMEval& vm, CDSR_VMEval_Cmd& call )
{
	long var_no = call.operand.v_addr.param2;
	switch( call.operand.v_addr.param1 )
	{
	case VARIABLE_USER:
#if _DEBUG
		if( var_no <= 0 || var_no >= (long) vm.m_var_table.ArrayDim() )
		{
			vm.getSmbTable().AddLine( _T("internal problem while running: gloval variable index is out of range"), call.line );
			vm.setError();
			return;
		}
#endif
		vm.m_run_stack[ vm.m_run_stack_ptr++ ] = vm.m_var_table[ var_no ];
		break;
	case LOCAL_VARIABLE_USER:
#if _DEBUG
		if( var_no < 1 || vm.m_BP + var_no - 1 >= vm.m_SP )
		{
			vm.getSmbTable().AddLine( _T("internal problem while running: local variable index is out of range"), call.line );
			vm.setError();
			return;
		}
#endif
		vm.m_run_stack[ vm.m_run_stack_ptr++ ] = vm.m_stack[ vm.m_BP + var_no - 1 ];
		break;
	}
	vm.m_IP++;
}

void __Call_execCommand_PUSH_NATURAL( CDSR_VMEval& vm, CDSR_VMEval_Cmd& call )
{
	vm.m_run_stack[ vm.m_run_stack_ptr++ ] = call.operand.v_natural;
	vm.m_IP++;
}

void __Call_execCommand_PUSH_INTEGER( CDSR_VMEval& vm, CDSR_VMEval_Cmd& call )
{
	vm.m_run_stack[ vm.m_run_stack_ptr++ ] = call.operand.v_integer;
	vm.m_IP++;
}

void __Call_execCommand_PUSH_REAL( CDSR_VMEval& vm, CDSR_VMEval_Cmd& call )
{
	vm.m_run_stack[ vm.m_run_stack_ptr++ ] = call.operand.getReal();
	vm.m_IP++;
}

void __Call_execCommand_PUSH_STRING( CDSR_VMEval& vm, CDSR_VMEval_Cmd& call )
{
	vm.m_run_stack[ vm.m_run_stack_ptr++ ] = call.operand.getString();
	vm.m_IP++;
}

void __Call_execCommand_OP( CDSR_VMEval& vm, CDSR_VMEval_Cmd& call )
{
	vm.m_run_stack_ptr += ::__ml_OperationCall( vm, call.operand.v_addr, &(vm.m_run_stack[ vm.m_run_stack_ptr - 1 ]) + 1 );
	if( call.operand.v_addr.param1 != FUNCTION_USER )
		vm.m_IP++;
#if _DEBUG
	if( vm.m_run_stack_ptr < 0 || vm.m_run_stack_ptr > (long) vm.m_run_stack.ArrayDim() )
	{
		vm.getSmbTable().AddLine( _T("internal problem while running: run stack pointer is out of range"), call.line );
		vm.setError();
		return;
	}
#endif
}

void __Call_execCommand_RETURN( CDSR_VMEval& vm, CDSR_VMEval_Cmd& call )
{
#if _DEBUG
	if( vm.m_context != VMEval_CODE_SEGMENT_ID__FUNCTION )
	{
		vm.getSmbTable().AddLine( _T("internal problem while running: invalid context in return"), call.line );
		vm.setError();
		return;
	}
	if( vm.m_context_no < 1 || vm.m_context_no >= (long) vm.m_fun_table.ArrayDim() )
	{
		vm.getSmbTable().AddLine( _T("internal problem while running: invalid called function number in return"), call.line );
		vm.setError();
		return;
	}
#endif
	//=== decrease SP
	vm.m_SP -= call.operand.v_addr.param3 + call.operand.v_addr.param4;
	//=== restore run stack pointer
	vm.m_run_stack_ptr = vm.m_stack[ --vm.m_SP ].v_addr.param1;
	//=== move <call.operand.v_addr.param4> function results to the top of the run stack
	for( long i = 0; i < call.operand.v_addr.param4; i++ )
		vm.m_run_stack[ vm.m_run_stack_ptr++ ] = vm.m_stack[ vm.m_BP + call.operand.v_addr.param3 + i ];
	//=== restore BP
	vm.m_BP = vm.m_stack[ --vm.m_SP ].v_addr.param1;
	//=== restore IP
	MMD_Address addr = vm.m_stack[ --vm.m_SP ].v_addr;
	vm.m_context_no = addr.param2;
	switch( addr.param1 )
	{
	case VMEval_CODE_SEGMENT_ID__MAIN:
		vm.m_IP_base_ptr = &vm.m_cmd_table;
		break;
	case VMEval_CODE_SEGMENT_ID__FUNCTION:
		vm.m_IP_base_ptr = &vm.m_fun_table[ vm.m_context_no ].body;
		break;
	case VMEval_CODE_SEGMENT_ID__EQUATION_L:
		vm.m_IP_base_ptr = &vm.m_equ_table[ vm.m_context_no ].left;
		break;
	case VMEval_CODE_SEGMENT_ID__EQUATION_R:
		vm.m_IP_base_ptr = &vm.m_equ_table[ vm.m_context_no ].right;
		break;
	case VMEval_CODE_SEGMENT_ID__RESERVED:
		vm.m_IP_base_ptr = &vm.m_reserved4call;
		break;
#if _DEBUG
	default:
		vm.getSmbTable().AddLine( _T("internal problem while running: unknown code segment identifier"), call.line );
		vm.setError();
		return;
#endif
	}
	vm.m_IP = addr.param3;
	vm.m_context = addr.param1;
#if _DEBUG
	if( vm.m_IP < 0 || vm.m_IP > (long) vm.m_IP_base_ptr->ArrayDim() ) // it could be == vm.m_IP_base.ArrayDim() if CALL is the last operation
	{
		vm.getSmbTable().AddLine( _T("internal problem while running: invalid return offset in the code segment"), call.line );
		vm.setError();
		return;
	}
#endif

}

void __Call_execCommand_ASSIGN_FUN_RES( CDSR_VMEval& vm, CDSR_VMEval_Cmd& call )
{
	vm.m_stack[ vm.m_BP + call.operand.v_addr.param3 + call.operand.v_addr.param1 ] = vm.m_run_stack[ vm.m_run_stack_ptr - 1 ];
	vm.m_IP++;
}

void __Call_execCommand_DEC_RUNSTACK_PTR( CDSR_VMEval& vm, CDSR_VMEval_Cmd& call )
{
	vm.m_run_stack_ptr -= call.operand.v_addr.param3;
	vm.m_IP++;
}

/*
void __Call_execCommand_SAVE_RUNSTACK_PTR( CDSR_VMEval& vm, CDSR_VMEval_Cmd& call )
{
	vm.getSmbTable().AddLine( _T("internal problem while running: <SAVE RUNSTACK PTR> command is forbidden"), call.line );
	vm.setError();
	return;

	vm.m_stack[ vm.m_SP++ ] = _create_lparam( vm.m_run_stack_ptr );
	vm.m_IP++;
}

void __Call_execCommand_LOAD_RUNSTACK_PTR( CDSR_VMEval& vm, CDSR_VMEval_Cmd& call )
{
	vm.getSmbTable().AddLine( _T("internal problem while running: <LOAD RUNSTACK PTR> command is forbidden"), call.line );
	vm.setError();
	return;

	vm.m_run_stack_ptr = vm.m_stack[ --vm.m_SP ].v_addr.param1;
	vm.m_IP++;
}
*/
///////////////////////////////////////////////////////////////////////////////

typedef void (*SUBCALL_EXEC_COMMAND)( CDSR_VMEval& vm, CDSR_VMEval_Cmd& call );
#define __Call_execCommand_subcalls_LENGTH	14
static SUBCALL_EXEC_COMMAND __Call_execCommand_subcalls[ __Call_execCommand_subcalls_LENGTH ] = 
{
	__Call_execCommand_NOP,
	__Call_execCommand_SKIP_ON_FALSE,
	__Call_execCommand_SKIP,
	__Call_execCommand_PUSH_VARIABLE_ADDR,
	__Call_execCommand_PUSH_VARIABLE_VALUE,
	__Call_execCommand_PUSH_NATURAL,
	__Call_execCommand_PUSH_INTEGER,
	__Call_execCommand_PUSH_REAL,
	__Call_execCommand_PUSH_STRING,
	__Call_execCommand_OP,
	__Call_execCommand_RETURN,
	__Call_execCommand_ASSIGN_FUN_RES,
	__Call_execCommand_DEC_RUNSTACK_PTR
	//__Call_execCommand_SAVE_RUNSTACK_PTR,
	//__Call_execCommand_LOAD_RUNSTACK_PTR
};
int CDSR_VMEval::execCommand( CDSR_VMEval_Cmd& call )
{
#if _DEBUG
	if( call.cmd < VMC_DSR_NOP || call.cmd >= VMC_DSR_LAST )
		throw _T("CDSR_VMEval::execCommand : internal error, unknown command");
#endif
	try
	{
		__Call_execCommand_subcalls[ call.cmd ]( *this, call );
	}
	catch( QString errMsg )
	{
		getSmbTable().AddLine( errMsg, call.line );
		setError();
		return -1;
	}
	//catch( CDSRMathErr math_err )
	//{
	//	getSmbTable().AddLine( getMathErrorMessage( math_err ), call.line );
	//	setError();
	//	return -1;
	//}
	return 0;
}

int CDSR_VMEval::execCommand( void )
{
	//=== init reserved context
	m_context = VMEval_CODE_SEGMENT_ID__MAIN;
	m_context_no = 0;
	initExecution( m_cmd_table );
	//=== execution
	while( !isError() && m_IP >= 0 && m_IP < (long) m_IP_base_ptr->ArrayDim() )
		execCommand( (*m_IP_base_ptr)[ m_IP ] );
	// return
	return isError() ? -1 : 0;
}

int CDSR_VMEval::execFunction( long fun_no, UniWordVector& res, const UniWordVector& args )
{
	//=== check whether function number is valid
	if( fun_no < 1 || fun_no >= (long) m_fun_table.ArrayDim() )
		return -1;
	if( getFunction_getArgNumber( fun_no ) != args.ArrayDim() )
		return -1;
	//=== init reserved context
	m_context = VMEval_CODE_SEGMENT_ID__RESERVED;
	m_context_no = 0;
	initExecution( m_reserved4call );
	//=== prepare parent command line
	m_reserved4call.Flush();
	UniWord uw( _create_lparam( FUNCTION_USER, fun_no, getFunction_getArgNumber( fun_no ), getFunction_getResNumber( fun_no ) ) );
	CDSR_VMEval_Cmd item( 
			VMC_DSR_OP,
			uw,
			0 );
	m_reserved4call.Add( item );
	//=== prepare run-time stack
	long i;
	for( i = 0; i < getFunction_getArgNumber( fun_no ); i++ )
		m_run_stack[ m_run_stack_ptr++ ] = args[ i ];
	//=== execution
	while( !isError() && m_IP >= 0 && m_IP < (long) m_IP_base_ptr->ArrayDim() )
		execCommand( (*m_IP_base_ptr)[ m_IP ] );
	//=== flush reserved command line
	m_reserved4call.Flush();
	//=== prepare vector of results
	res.resize( getFunction_getResNumber( fun_no ) );
	m_run_stack_ptr -= getFunction_getResNumber( fun_no );
#if _DEBUG
	if( m_run_stack_ptr < 0 )
	{
		getSmbTable().AddLine( _T("internal problem: run stack pointer is out of range in CDSR_VMEval::execFunction") );
		setError();
		return -1;
	}
#endif
	for( i = 0; i < getFunction_getResNumber( fun_no ); i++ )
		res[ i ] = m_run_stack[ m_run_stack_ptr + i ];
	// return
	return isError() ? -1 : 0;
}

int CDSR_VMEval::execFunction( QString fun_name, UniWordVector& res, const UniWordVector& args )
{
	return execFunction( getFunction_getNoByName( fun_name ), res, args );
}

int CDSR_VMEval::execEquationLeft( long equ_no, UniWord *retvalue )
{
	//=== check whether function number is valid
	if( equ_no < 0 || equ_no >= (long) m_equ_table.ArrayDim() )
		return -1;
	//=== init reserved context
	m_context = VMEval_CODE_SEGMENT_ID__EQUATION_L;
	m_context_no = equ_no;
	initExecution( m_equ_table[ equ_no ].left );
	//=== execution
	while( !isError() && m_IP >= 0 && m_IP < (long) m_IP_base_ptr->ArrayDim() )
		execCommand( (*m_IP_base_ptr)[ m_IP ] );
#if _DEBUG
	if( m_run_stack_ptr != 1 )
	{
		getSmbTable().AddLine( _T("internal problem: run stack pointer is out of range in CDSR_VMEval::execEquationLeft") );
		setError();
		return -1;
	}
#endif
	if( retvalue )
		*retvalue = m_run_stack[ m_run_stack_ptr - 1 ];
	m_run_stack_ptr--;
	// return
	return isError() ? -1 : 0;
}

int CDSR_VMEval::execEquationRight( long equ_no, UniWord *retvalue )
{
	//=== check whether function number is valid
	if( equ_no < 0 || equ_no >= (long) m_equ_table.ArrayDim() )
		return -1;
	//=== init reserved context
	m_context = VMEval_CODE_SEGMENT_ID__EQUATION_R;
	m_context_no = equ_no;
	initExecution( m_equ_table[ equ_no ].right );
	//=== execution
	while( !isError() && m_IP >= 0 && m_IP < (long) m_IP_base_ptr->ArrayDim() )
		execCommand( (*m_IP_base_ptr)[ m_IP ] );
#if _DEBUG
	if( m_run_stack_ptr != 1 )
	{
		getSmbTable().AddLine( _T("internal problem: run stack pointer is out of range in CDSR_VMEval::execEquationRight") );
		setError();
		return -1;
	}
#endif
	if( retvalue )
		*retvalue = m_run_stack[ m_run_stack_ptr - 1 ];
	m_run_stack_ptr--;
	// return
	return isError() ? -1 : 0;
}

///////////////////////////////////////////////////////////////////////////////
/*
QString CDSR_VMEval::getMathErrorMessage( CDSRMathErr& math_err )
{
	QString mess = _T("");
	switch( math_err.errClass )
	{
	case EMC_FLOAT:
		mess = _T("float number : ");
		break;
	case EMC_INTEGER:
		mess = _T("integer number : ");
		break;
	case EMC_NATURAL:
		mess = _T("natural number : ");
		break;
	case EMC_REAL:
		mess = _T("real number : ");
		break;
	case EMC_RATIONAL:
		mess = _T("rational number : ");
		break;
	case EMC_PROBABILITY:
		mess = _T("probability : ");
		break;
	case EMC_POLYNOMIAL:
		mess = _T("polynomial : ");
		break;
	case EMC_MATRIX:
		mess = _T("matrix : ");
		break;
	}

	switch( math_err.errType )
	{
	case EMT_OWERFLOW:
		mess += _T("overflow");
		break;
	case EMT_DIV_BY_ZERRO:
		mess += _T("division by zero");
		break;
	case EMT_LOSE_PREC:
		mess += _T("operation looses precision");
		break;
	case EMT_RANGE_OUT:
		mess += _T("out of range");
		break;
	case EMT_POW2BIG:
		mess += _T("power value is too big");
		break;
	case EMT_SQUARE_MATRIX_EXPECTED:
		mess += _T("should be square matrix (the same number of rows as columns)");
		break;
	case EMT_OP_POW_NEGATIVE:
		mess += _T("power value cannot be negative");
		break;
	case EMT_OP_POW_NOT_POSITIVE:
		mess += _T("power value must be positive");
		break;
	default:
		mess += _T("unknown error");
	}

	return mess;
}
*/
int CDSR_VMEval::initExecution( CDSR_VMEval_Line& initline )
{
	m_IP_base_ptr = &initline;
	m_IP = 0;
	m_run_stack.Flush();
	m_run_stack_ptr = 0;
	m_stack.Flush();
	m_SP = 0;
	m_BP = 0;

	srand( (unsigned)time( NULL ) );

	return 0;
}

///////////////////////////////////////////////////////////////////////////////
