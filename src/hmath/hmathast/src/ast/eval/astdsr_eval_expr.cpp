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

#include "../vmevallink.h"
#include "../astdsr.h"

/////////////////////////////////////////
//	ArgList		:	Expr*, Separator;
/////////////////////////////////////////
void CDsrAST_ArgList::getInstruction( CDSR_VMEval& vm, CDSR_VMEval_Line& dest )
{
	for( long i = 0; i < (long) expr.ArrayDim(); i++ )
		if( expr[ i ] ) expr[ i ]->getInstruction( vm, dest );
}

/////////////////////////////////////////
//	Expr		:	Node, Op, Node;
/////////////////////////////////////////
void CDsrAST_Expr::getInstruction( CDSR_VMEval& vm, CDSR_VMEval_Line& dest )
{
	// put instruction
	//////////////////
	UniWord uniop;
	uniop.v_addr.param1 = fun_id;
	uniop.v_addr.param2 = fun_id_param;
	uniop.v_addr.param3 = args ? args->getArgNumber() : 0;
	CDSR_VMEval_Cmd cmd( VMC_DSR_OP, uniop, getLine() );

	switch( fun_id )
	{
	//=== TO DO
	case OPERATOR_op_type_spec:
		throw _T("internal error: not yet realized");
		break;
	//=== NOP
	case OPERATOR_op_create_x:
		{
			CDSR_VMEval_Cmd item( VMC_DSR_NOP, getLine() );
			dest.Add( item );
		}
		break;
	case OPERATOR_op_return:
		{
			if( args && args->expr[ 1 ] ) args->expr[ 1 ]->getInstruction( vm, dest );
			CParseDsrSymbol *smb = vm.getSmbTable().getSymbol( op ? op->object_id : -1 );
			UniWord uw1 = _create_lparam( 0, /* <...> is the number of the assigned function result */
								0, smb ? smb->getArgNumber() : 0, smb ? smb->getResNumber() : 0 );
			CDSR_VMEval_Cmd item1( VMC_DSR_ASSIGN_FUN_RES, 
				uw1, 
				getLine() );
			dest.Add( item1 );
			UniWord uw2 = _create_lparam( 0, /* current function result number is always 0 for the in-line functions */
								  /* for parametric and implicit functions we must evaluate in type-procedure for the Op_return_x the number of the left part of the assignment and use it here as this parameter <...> */
								0, smb ? smb->getArgNumber() : 0, smb ? smb->getResNumber() : 0 );
			CDSR_VMEval_Cmd item2( VMC_DSR_RETURN, 
				uw2, 
				getLine() );
			dest.Add( item2 );
		}
		break;
	case OPERATOR_curly_brackets:
		if( uniop.v_addr.param3 == 1 && 
			args->expr[ 0 ]->getType() == DsrASTT_ArgList )
		{
			args->getInstruction( vm, dest );
			cmd.operand.v_addr.param3 = ((CDsrAST_ArgList*)(args->expr[ 0 ]))->getArgNumber();
			dest.Add( cmd );
		}
		break;
	case OPERATOR_round_brackets:
		if( args ) args->getInstruction( vm, dest );
		break;
	case OPERATOR_op_equation_e:
	case OPERATOR_op_equation_l:
	case OPERATOR_op_equation_le:
	case OPERATOR_op_equation_g:
	case OPERATOR_op_equation_ge:
		getInstruction_equation( vm, dest );
		break;
	default:
		if( args ) args->getInstruction( vm, dest );
		dest.Add( cmd );
	}
}

void CDsrAST_Expr::getInstruction_equation( CDSR_VMEval& vm, CDSR_VMEval_Line& /*dest*/ )
{
	CDSR_VMEval_Equation equ;

	equ.used_xvar.Copy( m_used_xvar );
	equ.used_dxvar.Copy( m_used_dxvar );
	equ.is_linear = ( getDataType() && getDataType()->isLinearEQ() ) ? 1 : 0;
	equ.is_koshi = is_koshi;
	equ.is_differential = equ.used_dxvar.ArrayDim() > 0;
	equ.is_algebraic = !equ.is_differential;

	equ.data_type = getDataType();

	switch( fun_id )
	{
	case OPERATOR_op_equation_e:
		equ.sign = CDSR_VMEval_Equation_e;
		break;
	case OPERATOR_op_equation_l:
		equ.sign = CDSR_VMEval_Equation_l;
		break;
	case OPERATOR_op_equation_le:
		equ.sign = CDSR_VMEval_Equation_le;
		break;
	case OPERATOR_op_equation_g:
		equ.sign = CDSR_VMEval_Equation_g;
		break;
	case OPERATOR_op_equation_ge:
		equ.sign = CDSR_VMEval_Equation_ge;
		break;
	default:
		break;
	}

	if( equ.is_koshi )
	{
		if( args && args->expr[ 0 ] )
			args->expr[ 0 ]->getInstruction( vm, equ.left );
		if( args && args->expr[ 1 ] )
			args->expr[ 1 ]->getInstruction( vm, equ.right );
	}
	else
	{
		if( args && args->expr[ 0 ] )
			args->expr[ 0 ]->getInstruction( vm, equ.left );
		if( args && args->expr[ 1 ] )
			args->expr[ 1 ]->getInstruction( vm, equ.right );
	}

	// diagnostics : check forbidden combinations
	if( equ.is_differential && !equ.is_koshi )
	{
		vm.getSmbTable().AddLine( _T("illegal differential equation, only Koshi form of DE is supported"), getLine() );
		vm.setError();
	}

	equ.is_linear = ( getDataType() && getDataType()->isLinearEQ() ) ? 1 : 0;
	equ.is_koshi = is_koshi;
	equ.is_differential = equ.used_dxvar.ArrayDim() > 0;
	equ.is_algebraic = !equ.is_differential;

	vm.m_equ_table.Add( equ );
}

/////////////////////////////////////////
//	Function	:	CompoundID, Separator, ArgList, Separator;
/////////////////////////////////////////
void CDsrAST_Function::getInstruction( CDSR_VMEval& vm, CDSR_VMEval_Line& dest )
{
	// put instruction
	//////////////////

	if( args )
		args->getInstruction( vm, dest );

	CParseDsrSymbol *fun_smb = name ? name->getSymbol( vm.getSmbTable() ) : 0;
	UniWord uniop;
	uniop.v_addr.param1 = fun_id;
	uniop.v_addr.param2 = fun_id_param;
	if( fun_id == FUNCTION_USER )
	{
		uniop.v_addr.param3 = fun_smb ? fun_smb->getArgNumber() : 0;
		uniop.v_addr.param4 = fun_smb ? fun_smb->getResNumber() : 0;
	}
	else
	{
		//=== can't use just fun_smb->getArgNumber(), 
		//=== because symbol type is a COMPOSITION of FUNCTIONS, 
		//=== but not a FUNCTION (NB: sure, we can use <fun_id_param> value to 
		//=== find out what variant of our COMPOSITION is actually called, but 
		//=== why complicate if we can use attributes of the actual parameters list)
		uniop.v_addr.param3 = args ? args->getArgNumber() : 0; 
		//=== don't care about returns for standart functions,
		//=== because they only are needed for RETURN code and nowhere else
		uniop.v_addr.param4 = 0;
	}
	CDSR_VMEval_Cmd cmd( VMC_DSR_OP, uniop, getLine() );
	dest.Add( cmd );
}

/////////////////////////////////////////
//	CompoundID	:	PrimaryID*, Separator;
/////////////////////////////////////////
void CDsrAST_CompoundID::getInstruction( CDSR_VMEval& vm, CDSR_VMEval_Line& dest )
{
	if( stmt.ArrayDim() )
	{
		if( stmt[ stmt.ArrayDim() - 1 ] ) 
			stmt[ stmt.ArrayDim() - 1 ]->getInstruction( vm, dest );
	}
}

/////////////////////////////////////////
//	PrimaryID	:	long, Special;
/////////////////////////////////////////
void CDsrAST_PrimaryID::getInstruction( CDSR_VMEval& vm, CDSR_VMEval_Line& dest )
{
	// put instruction
	//////////////////

	CParseDsrSymbol* smb = getSymbol( vm.getSmbTable() );
	if( !smb )
	{
		vm.getSmbTable().AddLine( _T("internal problem, unknown symbol record"), getLine() );
		vm.setError();
	}

	if( smb->smb_record_type == SMBTABLE_TYPE_VARIABLE )
	{
		UniWord uniop;
		//uniop.v_addr.param1 = isLocalSmbTable() ? LOCAL_VARIABLE_USER : VARIABLE_USER;
		if( smb->param1 == FUNCTION_USER_POLYNOMIAL || 
			smb->param1 == FUNCTION_USER_RATIONAL_FUN )
			uniop.v_addr.param1 = VARIABLE_USER;
		else
			uniop.v_addr.param1 = smb->param1;
		uniop.v_addr.param2 = smb->param2;
		uniop.v_addr.param3 = smb->param3;
		CDSR_VMEval_Cmd cmd( is_push_by_address ? VMC_DSR_PUSH_VARIABLE_ADDR : VMC_DSR_PUSH_VARIABLE_VALUE, 
							 uniop, getLine() );
		dest.Add( cmd );
	}
	else if( spec )
		spec->getInstruction( vm, dest );
}

/////////////////////////////////////////
//	Special		:	long, Separator, ArgList, Separator;
/////////////////////////////////////////
void CDsrAST_Special::getInstruction( CDSR_VMEval& vm, CDSR_VMEval_Line& dest )
{
	// put instruction
	//////////////////

	switch( fun_id )
	{
	// error : ignoring operand
	case FORMULA_box_lt:
	case FORMULA_box_rt:
	case FORMULA_box_lb:
	case FORMULA_box_rb:
	case FORMULA_box:
	case FORMULA_empty_frame:
	case FORMULA_plusminus:
	case FORMULA_circle_plus:
	case FORMULA_circle_times:
	case FORMULA_center_dot:
	case FORMULA_bullet:
	case FORMULA_lang:
	case FORMULA_rang:
	case FORMULA_identical:
	case FORMULA_almost_equal:
	case FORMULA_proportional:
	case FORMULA_therefore:
	case FORMULA_exist:
	case FORMULA_forall:
	case FORMULA_subsup:
	case FORMULA_over:
	case FORMULA_under:
	case FORMULA_underover:
		//vm.getSmbTable().AddLine( _T("'%1' : internal problem, code is unknown for the special operator"), getLine(), ::ml_Op2String( fun_id ) );
		//vm.setError();
		break;
	case FORMULA_prime:
		if( args && args->getArgNumber() > 1 && args->expr[ 1 ] && args->expr[ 1 ]->getType() == DsrASTT_ConstInt )
			vm.Equation_AddPrimeIndex( ((CDsrAST_ConstInt*)(args->expr[ 1 ]))->value );
		else
			vm.Equation_AddPrimeIndex( 0 );
		break;
	default:
		{
			if( args )
				args->getInstruction( vm, dest );
            
			UniWord uniop;
			uniop.v_addr.param1 = fun_id;
			uniop.v_addr.param2 = fun_id_param;
			uniop.v_addr.param3 = args ? args->getArgNumber() : 0;
			CDSR_VMEval_Cmd cmd( VMC_DSR_OP, uniop, getLine() );
			dest.Add( cmd );
		}
	}
}

/////////////////////////////////////////
//	Op			:	long;
/////////////////////////////////////////
void CDsrAST_Op::getInstruction( CDSR_VMEval& vm, CDSR_VMEval_Line& /*dest*/ )
{
	if( id != -1 )
		vm.getSmbTable().AddLine( _T("'%1' : internal problem, code is unknown for the operator symbol"), getLine(), ::ml_Op2String( id ) );
	else
		vm.getSmbTable().AddLine( _T("internal problem, code is unknown for the operator symbol"), getLine() );
	vm.setError();
}

/////////////////////////////////////////
//	ConstInt	:	long;
/////////////////////////////////////////
void CDsrAST_ConstInt::getInstruction( CDSR_VMEval& /*vm*/, CDSR_VMEval_Line& dest )
{
	// put instruction
	//////////////////
	if( getDataType() && getDataType()->getType() == DSRDATA_TYPE_NATURAL )
	{
		CDSRNatural val( value );
		UniWord uw( val );
		CDSR_VMEval_Cmd item( VMC_DSR_PUSH_NATURAL, uw, getLine() );
		dest.Add( item );
	}
	else
	{
		CDSRInteger val( value );
		UniWord uw( val );
		CDSR_VMEval_Cmd item( VMC_DSR_PUSH_INTEGER, uw, getLine() );
		dest.Add( item );
	}
}

/////////////////////////////////////////
//	ConstReal	:	double;
/////////////////////////////////////////
void CDsrAST_ConstReal::getInstruction( CDSR_VMEval& /*vm*/, CDSR_VMEval_Line& dest )
{
	// put instruction
	//////////////////
	CDSRReal val( value );
	UniWord uw( val );
	CDSR_VMEval_Cmd item( VMC_DSR_PUSH_REAL, uw, getLine() );
	dest.Add( item );
}

/////////////////////////////////////////
//	ConstString	:	QString;
/////////////////////////////////////////
void CDsrAST_ConstString::getInstruction( CDSR_VMEval& vm, CDSR_VMEval_Line& dest )
{
	// put instruction
	//////////////////
	UniWord uw( vm.getSmbTable().makeUniWord( new MMD_String( vm.getSmbTable(), value ) ) );
	CDSR_VMEval_Cmd item( 
			VMC_DSR_PUSH_STRING, 
			uw, 
			getLine() );
	dest.Add( item );
}

/////////////////////////////////////////
//	CDsrAST_ConstStringList	:	CDsrAST_ConstString*;
/////////////////////////////////////////
void CDsrAST_ConstStringList::getInstruction( CDSR_VMEval& vm, CDSR_VMEval_Line& dest )
{
	QString value = _T("");
	for( long i = 0; i < (long) stmt.ArrayDim(); i++ )
		if( stmt[ i ] ) value += stmt[ i ]->value;
	// put instruction
	//////////////////
	UniWord uw( vm.getSmbTable().makeUniWord( new MMD_String( vm.getSmbTable(), value ) ) );
	CDSR_VMEval_Cmd item( 
			VMC_DSR_PUSH_STRING, 
			uw, 
			getLine() );
	dest.Add( item );
}

///////////////////////////////////////////////////////////////////////////////
