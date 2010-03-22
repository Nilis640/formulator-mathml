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

///////////////////////////////////////////////////////////////////////////////

CDSR_VMEval::CDSR_VMEval( CParseDsrSymbolTable*& _ptr_symbol_table, long init_mode ) :
	VM_Dsr( _ptr_symbol_table, init_mode ), 
	m_var_id_table( 10240, 0, 10240 ), 
	m_IP_base_ptr( &m_cmd_table ),
	m_var_table( 10240 ), 
	m_run_stack( 10240 ), m_stack( 10240 )
{
	if( !_ptr_symbol_table )
		throw _T("internal error: null symbol table");
	resetError();

	m_context = VMEval_CODE_SEGMENT_ID__MAIN;
	m_context_no = 0;
	m_IP = 0;
	m_run_stack_ptr = 0;
	m_SP = 0;
	m_BP = 0;
}

void CDSR_VMEval::getInstruction( CDsrAST_Node* root )
{
	resetError();
	InitVarTable();
	InitFunTable();
	root->getInstruction( *this, m_cmd_table );
	if( !isError() )
	{
		if( m_equ_table.Summarize( getSmbTable() ) == -1 )
			setError();
		InitSLAU_AB();
	}
}

void CDSR_VMEval::InitVarTable( void )
{
	long sz = getSmbTable().getVarMaxNo() + 1;
	CParseDsrSymbol *smb;
	m_var_table.resize( sz );

	m_var_id_table.resize( sz );
	m_var_id_table[ 0 ] = -1;

	for( long i = 0; i < getSmbTable().getSymbolNumber(); i++ )
	{
		if( !(smb = getSmbTable().getSymbol( i )) ) continue;
		if( smb->smb_record_type == SMBTABLE_TYPE_VARIABLE )
		{
			if( smb->param2 && smb->getDataType() && 
				smb->param2 > 0 && smb->param2 < (long) m_var_table.ArrayDim() )
			{
				m_var_table[ smb->param2 ] = smb->getDataType()->getType();
				m_var_id_table[ smb->param2 ] = i;
			}
			else
			{
				getSmbTable().AddLine( _T("internal problem, badly allocated variable '%1'"), 0, smb->name );
				setError();
			}
		}
	}
	ml_InitConstSmbTable_Value( getSmbTable(), m_var_table );
}

void CDSR_VMEval::InitFunTable( void )
{
	long sz = getSmbTable().getFunMaxNo() + 1;
	m_fun_table.resize( sz );
	CParseDsrSymbol *smb;
	for( long i = 0; i < getSmbTable().getSymbolNumber(); i++ )
	{
		if( !(smb = getSmbTable().getSymbol( i )) ) continue;
		if( smb->smb_record_type == SMBTABLE_TYPE_FUNCTION &&
			smb->param1 == FUNCTION_USER &&
			smb->param2 > 0 && smb->param2 < (long) m_fun_table.ArrayDim() &&
			smb->getDataType() )
		{
			CDSR_VMEval_Function& funr = m_fun_table[ smb->param2 ];
			funr.id = i;
			funr.name = smb->name;
			funr.n_arg = smb->getArgNumber();
			if( funr.n_arg ) funr.args.resize( funr.n_arg );
			funr.n_res = smb->getResNumber();
			if( funr.n_res ) funr.res.resize( funr.n_res );
			if( smb->getDataType()->getType() == DSRDATA_TYPE_FUNCTION )
			{
				CDsrDataType_Node *n_arg = ((CDsrDataType_Function*)(smb->getDataType()))->n_arg;
				if( n_arg->getType() == DSRDATA_TYPE_COMPOSITION )
					funr.m_arg_data = (CDsrDataType_Composition*)n_arg;
				else
				{
					funr.m_arg_data = (CDsrDataType_Composition*)getSmbTable().getTypeTable().makeDataTypeNode( new CDsrDataType_Composition() );
					funr.m_arg_data->Add( getSmbTable().getTypeTable().makeDataTypeNode( DSRDATA_TYPE_NONE ) );
				}
				funr.m_res_data = ((CDsrDataType_Function*)(smb->getDataType()))->n_res;
			}
			else
			{
				funr.m_arg_data = (CDsrDataType_Composition*)getSmbTable().getTypeTable().makeDataTypeNode( new CDsrDataType_Composition() );
				funr.m_arg_data->Add( getSmbTable().getTypeTable().makeDataTypeNode( DSRDATA_TYPE_NONE ) );
				funr.m_res_data = getSmbTable().getTypeTable().makeDataTypeNode( DSRDATA_TYPE_NONE );
			}
		}
	}
}

///////////////////////////////////////////////////////////////////////////////


CDSR_VMEval::~CDSR_VMEval()
{
}


int CDSR_VMEval::isError( void )
{
	return is_error;
}


void CDSR_VMEval::setError( void )
{
	is_error = 1;
}


void CDSR_VMEval::resetError( void )
{
	is_error = 0;
}


void CDSR_VMEval::push2RunStack( UniWord& x )
{
	m_run_stack[ m_run_stack_ptr++ ] = x;
}


void CDSR_VMEval::Equation_AddPrimeIndex( long idx )
{
	m_equ_table.Equation_AddPrimeIndex( idx );
}

///////////////////////////////////////////////////////////////////////////////


long CDSR_VMEval::getEquation_Number( void )
{
	return (long) m_equ_table.ArrayDim();
}


int CDSR_VMEval::getEquation_isLinear( void )
{
	return m_equ_table.is_linear;
}


int CDSR_VMEval::getEquation_isAlgebraic( void )
{
	return m_equ_table.is_algebraic;
}


int CDSR_VMEval::getEquation_isDifferential( void )
{
	return m_equ_table.is_differential;
}


int CDSR_VMEval::getEquation_isKoshi( void )
{
	return m_equ_table.is_koshi;
}


enum CDsrDataTypes CDSR_VMEval::getEquation_Type( void )
{
	return m_equ_table.data_type ? m_equ_table.data_type->getType() : DSRDATA_TYPE_NONE;
}


long CDSR_VMEval::getEquation_UnknownNumber( void )
{
	return m_equ_table.m_unknown_cell;
}


int CDSR_VMEval::getEquation_getLeftKoshiVariable( long eq_no, long& var_id, long& var_idx )
{
	return m_equ_table.getKoshiEquationLeft( eq_no, var_id, var_idx );
}


UniWordVector& CDSR_VMEval::getEquation_LinearA( void )
{
	return m_equ_table.A;
}


UniWordVector& CDSR_VMEval::getEquation_LinearB( void )
{
	return m_equ_table.B;
}


int CDSR_VMEval::getEquation_getTimeVar( UniWord& value )
{
	return getVariable_getValue( ::ml_Op2String( CONST_TIME ), value );
}


int CDSR_VMEval::getEquation_setTimeVar( UniWord& value )
{
	return getVariable_setValue( ::ml_Op2String( CONST_TIME ), value );
}

///////////////////////////////////////////////////////////////////////////////


long CDSR_VMEval::getFunction_Number( void )
{
	return (long) m_fun_table.ArrayDim();
}


int CDSR_VMEval::getFunction_Begin( void )
{
	return 1;
}


int CDSR_VMEval::getFunction_End( void )
{
	return (int) m_fun_table.ArrayDim();
}

///////////////////////////////////////////////////////////////////////////////


int CDSR_VMEval::getVariable_Number( void )
{
	return (int) m_var_table.ArrayDim() - 1;
}


int CDSR_VMEval::getVariable_Begin( void )
{
	return 1;
}


int CDSR_VMEval::getVariable_End( void )
{
	return (int) m_var_table.ArrayDim();
}


long CDSR_VMEval::getVariable_getSmbID( long var_no )
{
	if( var_no > 0 && var_no < (long) m_var_table.ArrayDim() )
		return m_var_id_table[ var_no ];
	return -1;
}


QString CDSR_VMEval::getVariable_Name( long no )
{
	long pid = getVariable_getSmbID( no );
	if( pid == -1 ) return _T("");
	CParseDsrSymbol *smb = getSmbTable().getSymbol( pid );
	return smb ? smb->name : _T("");
}


int CDSR_VMEval::getVariable_getValue( QString varname, UniWord& value )
{
	return getVariable_getValue( getVariable_getNoByName( varname ), value );
}


int CDSR_VMEval::getVariable_setValue( QString varname, UniWord& value )
{
	return getVariable_setValue( getVariable_getNoByName( varname ), value );
}

///////////////////////////////////////////////////////////////////////////////

