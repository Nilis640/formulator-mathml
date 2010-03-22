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

#include "../astdsr.h"

/////////////////////////////////////////
//	Expr		:	Node, Op, Node;
/////////////////////////////////////////
void CDsrAST_Expr::convertArgVar2Polynomial( CParseDsrSymbolTable& smbtable, long max_num_arg )
{
	if( args )
		args->convertArgVar2Polynomial( smbtable, max_num_arg );
}

int CDsrAST_Expr::placeDataType_assign( CParseDsrSymbolTable& smbtable, CDsrDataType_Node*& left_arg_fun_node )
{
	left_arg_fun_node = 0;

	if( args->getArgNumber() != 2 ||
		((CDsrDataType_Composition*)(args->getDataType()))->n_arg.ArrayDim() != 2 ||
		!args->expr[ 0 ] || 
		!args->expr[ 1 ] ||
		!args->expr[ 0 ]->getDataType() || 
		!args->expr[ 1 ]->getDataType() )
	{
		setDataType( smbtable.getTypeTable().makeDataTypeNode( DSRDATA_TYPE_ERROR ) );
		smbtable.AddLine( _T("'%1' : left operand must be l-value"), getLine(), ::ml_Op2String( fun_id ) );
		return -1;
	}

	CDsrAST_CompoundID *smb_cid = 0;
	CDsrAST_PrimaryID *smb_pid = 0;
	CParseDsrSymbol *smb_var = 0;
	if( args->expr[ 0 ]->getType() == DsrASTT_CompoundID )
	{
		smb_cid = ((CDsrAST_CompoundID*)(args->expr[ 0 ]));
		if( smb_cid->stmt.ArrayDim() )
			smb_pid = smb_cid->stmt[ smb_cid->stmt.ArrayDim() - 1 ];
		smb_var = smb_cid->getSymbol( smbtable );
	}
	else if( args->expr[ 0 ]->getType() == DsrASTT_PrimaryID )
	{
		smb_pid = ((CDsrAST_PrimaryID*)(args->expr[ 0 ]));
		smb_var = smb_pid->getSymbol( smbtable );
	}

	CDsrAST_CompoundID *smb_cid2 = 0;
	CDsrAST_PrimaryID *smb_pid2 = 0;
	CParseDsrSymbol *smb_var2 = 0;
	if( args->expr[ 1 ]->getType() == DsrASTT_CompoundID )
	{
		smb_cid2 = ((CDsrAST_CompoundID*)(args->expr[ 1 ]));
		if( smb_cid2->stmt.ArrayDim() )
			smb_pid2 = smb_cid2->stmt[ smb_cid2->stmt.ArrayDim() - 1 ];
		smb_var2 = smb_cid2->getSymbol( smbtable );
	}
	else if( args->expr[ 1 ]->getType() == DsrASTT_PrimaryID )
	{
		smb_pid2 = ((CDsrAST_PrimaryID*)(args->expr[ 1 ]));
		smb_var2 = smb_pid2->getSymbol( smbtable );
	}

	CDsrDataType_Node*& left_node = ((CDsrDataType_Composition*)(args->getDataType()))->n_arg[ 0 ];
	if( left_node &&
		op->getObjectContext( smbtable ) != -1 &&
		smb_pid && !smb_pid->isLocalSmbTable() && 
		smb_pid->id == op->getObjectContext( smbtable ) &&
		smb_var && smb_var->getDataType() &&
		(smb_var->getDataType()->getType() == DSRDATA_TYPE_FUNCTION) )
		// not ASSIGN, but RET
	{
		fun_id = (enum CDsrFunctionListID)OPERATOR_op_return;
		CDsrDataType_Function* ret_fun_type = (CDsrDataType_Function*)(smb_var->getDataType());
		if( !ret_fun_type->n_res || 
			ret_fun_type->n_res->getType() == DSRDATA_TYPE_UNKNOWN )
		{
			ret_fun_type->n_res = smbtable.cloneDataTypeNode( args->expr[ 1 ]->getDataType() );
			ret_fun_type->n_res->resetModifier(); // ignore strange modifiers
			ret_fun_type->n_res->setLValue();
			args->expr[ 0 ]->placeDataType( smbtable );
			left_node = args->expr[ 0 ]->getDataType();
		}
		left_arg_fun_node = left_node;	// store old type of args[ 0 ]
		left_node = smbtable.cloneDataTypeNode( ret_fun_type->n_res ); // forge a new type of arg[ 0 ], that is a function return node
	}
	else
	{
		if( (!args->getDataType()->isXValue() && 
			!args->expr[ 0 ]->getDataType()->isLValue()) )
		{
			setDataType( smbtable.getTypeTable().makeDataTypeNode( DSRDATA_TYPE_ERROR ) );
			smbtable.AddLine( _T("'%1' : left operand must be l-value"), getLine(), ::ml_Op2String( fun_id ) );
			return -1;
		}

		if( args->getDataType()->isXValue() )
		{
			if( smb_var && smb_var2 && smb_var2->param3 == OPERATOR_VAR_ask )
			{
				fun_id = (enum CDsrFunctionListID)OPERATOR_op_create_x;
				if( args->expr[ 0 ]->getDataType()->getType() != DSRDATA_TYPE_UNKNOWN )
				{
					setDataType( smbtable.getTypeTable().makeDataTypeNode( DSRDATA_TYPE_ERROR ) );
					smbtable.AddLine( _T("'%1' : left operand must be unknown variable to became independent variable"), getLine(), ::ml_Op2String( fun_id ) );
					return -1;
				}
			}
			else
			{
				switch( op->id )
				{
				case OPERATOR_op_assign:
					fun_id = OPERATOR_op_equation_e;
					break;
				case OPERATOR_op_rel_l:
					fun_id = OPERATOR_op_equation_l;
					break;
				case OPERATOR_op_rel_le:
					fun_id = OPERATOR_op_equation_le;
					break;
				case OPERATOR_op_rel_g:
					fun_id = OPERATOR_op_equation_g;
					break;
				case OPERATOR_op_rel_ge:
					fun_id = OPERATOR_op_equation_ge;
					break;
				}
				long i;
				CParseDsrSymbol *v_used;
				for( i = 0; i < (long) smbtable.getUsedXVar().ArrayDim(); i++ )
				{
					v_used = smbtable.getSymbol( smbtable.getUsedXVar()[ i ] );
					if( v_used && v_used->getDataType() )
					{
						v_used->getDataType()->setUsed();
						m_used_xvar.Add( smbtable.getUsedXVar()[ i ] );
					}
				}
				for( i = 0; i < (long) smbtable.getUsedDXVar().ArrayDim(); i++ )
				{
					v_used = smbtable.getSymbol( smbtable.getUsedDXVar()[ i ] );
					if( v_used && v_used->getDataType() )
					{
						v_used->getDataType()->setUsed();
						m_used_dxvar.Add( smbtable.getUsedDXVar()[ i ] );
					}
				}
				smbtable.FlushUsedXVar();
				smbtable.FlushUsedDXVar();
				if( smb_var && smb_var->param1 == FORMULA_prime )
					is_koshi = 1;

				if( fun_id != OPERATOR_op_equation_e )
				{
					setDataType( smbtable.getTypeTable().makeDataTypeNode( DSRDATA_TYPE_ERROR ) );
					smbtable.AddLine( _T("'%1' : illegal, inequalities are not supported"), getLine(), ::ml_Op2String( fun_id ) );
					return -1;
				}
			}
		}
    
		if( args->expr[ 0 ]->getDataType()->getType() == DSRDATA_TYPE_UNKNOWN && 
			smb_var && left_node )
		{
			CDsrDataType_Node *tmpn = smbtable.cloneDataTypeNode( args->expr[ 1 ]->getDataType() );
			if( !args->getDataType()->isXValue() )
			{
				tmpn->resetModifier(); // ignore strange modifiers
				tmpn->setLValue();
			}
			else if( fun_id != (enum CDsrFunctionListID)OPERATOR_op_create_x )
			{
				setDataType( smbtable.getTypeTable().makeDataTypeNode( DSRDATA_TYPE_ERROR ) );
				reportTypeError( smbtable );
				return -1;
			}
			smb_var->setDataType( tmpn );

			if( tmpn->getType() == DSRDATA_TYPE_POLYNOM )
				smb_var->param1 = FUNCTION_USER_POLYNOMIAL;
			else if( tmpn->getType() == DSRDATA_TYPE_RATIONAL_FUN )
				smb_var->param1 = FUNCTION_USER_RATIONAL_FUN;

			args->expr[ 0 ]->placeDataType( smbtable );
			left_node = args->expr[ 0 ]->getDataType();
		}
	}
	return 0;
}

int CDsrAST_Expr::placeDataType_type_spec( CParseDsrSymbolTable& smbtable, CParseDsrSymbol *smb )
{
	if( args->getArgNumber() != 2 ||
		!args->expr[ 0 ] ||
		!args->expr[ 1 ] ||
		!args->expr[ 0 ]->getDataType() ||
		!args->expr[ 1 ]->getDataType() ||
		args->expr[ 0 ]->getType() != DsrASTT_CompoundID )
	{
		setDataType( smbtable.getTypeTable().makeDataTypeNode( DSRDATA_TYPE_ERROR ) );
		reportTypeError( smbtable );
		return -1;
	}
	if( args->expr[ 0 ]->getDataType()->getType() == DSRDATA_TYPE_UNKNOWN )
	{
		CDsrDataType_Node *n = args->expr[ 0 ]->getDataType();
		args->expr[ 0 ]->setDataType( smbtable.cloneDataTypeNode( args->expr[ 1 ]->getDataType() ) );
		args->expr[ 0 ]->getDataType()->copyModifier( n );
		setDataType( smbtable.cloneDataTypeNode( args->expr[ 1 ]->getDataType() ) );
	}
	else
		setDataType( smbtable.cloneDataTypeNode( args->expr[ 0 ]->getDataType() ) );
	if( smb ) fun_id = (enum CDsrFunctionListID)smb->param1;

/*	else
	{
		setDataType( smbtable.getTypeTable().makeDataTypeNode( DSRDATA_TYPE_ERROR ) );
		CParseDsrSymbol *smb = ((CDsrAST_CompoundID*)(args->expr[ 0 ]))->getSymbol( smbtable );
		if( smb )
			AddLine( _T("'%1' : illegal type specification, type is already known"), getLine(), smb->name );
		else
			AddLine( _T("illegal type specification, type is already known"), getLine() );
		return -1;
	}*/
	return -1;
}

int CDsrAST_Expr::placeDataType_square_brackets( CParseDsrSymbolTable& smbtable )
{
	if( args->getArgNumber() != 2 ||
		!args->expr[ 1 ] ||
		!args->expr[ 1 ]->getDataType() ||
		args->expr[ 1 ]->getType() != DsrASTT_ArgList ||
		args->expr[ 1 ]->getDataType()->getType() != DSRDATA_TYPE_COMPOSITION )
	{
		setDataType( smbtable.getTypeTable().makeDataTypeNode( DSRDATA_TYPE_ERROR ) );
		smbtable.AddLine( _T("'[]' : illegal, operands inside square brackets have to be integers"), getLine() );
		return -1;
	}
	CDsrAST_ArgList* _idxs = (CDsrAST_ArgList*)(args->expr[ 1 ]);
	enum CDsrFunctionListID rest;
	if( _idxs->getArgNumber() == 0 )
	{
		setDataType( smbtable.getTypeTable().makeDataTypeNode( DSRDATA_TYPE_ERROR ) );
		smbtable.AddLine( _T("'[]' : illegal, non-empty list of operands inside square brackets is expected"), getLine() );
		return -1;
	}
	for( long i = 0; i < _idxs->getArgNumber(); i++ )
	{
		if( !_idxs->expr[ i ] || !_idxs->expr[ i ]->getDataType() )
		{
			setDataType( smbtable.getTypeTable().makeDataTypeNode( DSRDATA_TYPE_ERROR ) );
			smbtable.AddLine( _T("'[]' : illegal, operands inside square brackets have to be integers"), getLine() );
			return -1;
		}
		if( _idxs->expr[ i ]->getDataType()->getType() != DSRDATA_TYPE_INTEGER )
		{
			rest = ::getTypeConvOp( DSRDATA_TYPE_INTEGER, 
									_idxs->expr[ i ]->getDataType()->getType() );
			if( rest == FUNCTION_STD_NONE )
			{
				setDataType( smbtable.getTypeTable().makeDataTypeNode( DSRDATA_TYPE_ERROR ) );
				smbtable.AddLine( _T("'[]' : illegal, operands inside square brackets have to be integers"), getLine() );
				return -1;
			}
			else
				_idxs->convertArgTypes( smbtable, i, rest );
		}
	}

	return 0;
}

void CDsrAST_Expr::reportTypeError( CParseDsrSymbolTable& smbtable )
{
	QString str = ::ml_Op2String( fun_id );
	//if( str == _T("%") )
	//	str = _T("%%");
	QString txt = QString("\'%1\' : illegal").arg( str );
	if( args->expr[ 0 ] && !args->expr[ 1 ] && args->expr[ 0 ]->getDataType() )
		txt += QString( ", operand has type \'%1\'" ).arg( args->expr[ 0 ]->getDataType()->getTypeName() );
	else
	{
		if( args->expr[ 0 ] && args->expr[ 0 ]->getDataType() )
			txt += QString( ", left operand has type \'%1\'" ).arg( args->expr[ 0 ]->getDataType()->getTypeName() );
		if( args->expr[ 1 ] && args->expr[ 1 ]->getDataType() )
			txt += QString( ", right operand has type \'%1\'" ).arg( args->expr[ 1 ]->getDataType()->getTypeName() );
	}
	smbtable.AddLine( txt, getLine() );
}

void CDsrAST_Expr::placeDataType( CParseDsrSymbolTable& smbtable )
{
	if( op ) op->placeDataType( smbtable );
	if( args )
	{
		args->placeDataType( smbtable );
	}

	if( op   &&   op->getDataType() &&   op->getDataType()->getType() != DSRDATA_TYPE_ERROR &&
		args && args->getDataType() && args->getDataType()->getType() != DSRDATA_TYPE_ERROR && 
		args->getDataType()->getType() == DSRDATA_TYPE_COMPOSITION )
	{
		CParseDsrSymbol *smb = op->getSymbol( smbtable );
		std::vector<__CDSRTypeConversionOpVector> opc;
		long cb = 0;
		CDsrDataType_Node* ret_type = 0, *left_arg_fun_node = 0;

		if( !smb )
		{
			setDataType( smbtable.getTypeTable().makeDataTypeNode( DSRDATA_TYPE_ERROR ) );
			smbtable.AddLine( _T("internal error: operator entry is missing"), getLine() );
			return;
		}

		fun_id = (enum CDsrFunctionListID)(op->id);

		if( fun_id == OPERATOR_op_assign )
		{
			if( placeDataType_assign( smbtable, left_arg_fun_node ) == -1 )
				return;
		}
		else if( fun_id == OPERATOR_op_type_spec )
		{
			if( placeDataType_assign( smbtable, left_arg_fun_node ) == -1 )
				return;
			//fun_id = (enum CDsrFunctionListID)op->id;
			ret_type = args->expr[ 0 ]->getDataType();
		}

		CDsrDataType_Node *op_type = correctOpDataType2New( smbtable, fun_id, op );
		if( !op_type )
		{
			smbtable.AddLine( _T("\'%1\' : unknown operator"), getLine(), ::ml_Op2String( fun_id ) );
			setDataType( smbtable.getTypeTable().makeDataTypeNode( DSRDATA_TYPE_ERROR ) );
			return;
		}

		CDsrDataType_Composition* _argt = (CDsrDataType_Composition*)(args->getDataType());
		int is_Returns = 0;
		if( !ret_type )
			ret_type = CompareOp2Args( 
					fun_id,
					op_type, /*fun_id, */fun_id_param,
					_argt,
					opc, cb, smbtable, smb );

		// restore original type of args[ 0 ], if forgery was made for function ret type
		if( op->id == OPERATOR_op_assign && left_arg_fun_node )
		{
			((CDsrDataType_Composition*)(args->getDataType()))->n_arg[ 0 ] = left_arg_fun_node;
			// return control path is found
			is_Returns = 1;
		}

		if( ret_type->getType() == DSRDATA_TYPE_ERROR )
		{
			reportTypeError( smbtable );
		}
		else if( cb > 0 )
			args->convertArgTypes( opc[ fun_id_param ], smbtable );
		if( ret_type->getType() == DSRDATA_TYPE_INTERNAL_NESTED &&
			args && args->expr[ 0 ] && args->expr[ 0 ]->getDataType() && args->expr[ 0 ]->getDataType()->getNestedType() )
		{
			CDsrDataType_Node *tn = smbtable.cloneDataTypeNode( args->expr[ 0 ]->getDataType()->getNestedType() );
			tn->copyModifier( args->expr[ 0 ]->getDataType() );
			setDataType( tn );
		}
		else
			setDataType( smbtable.cloneDataTypeNode( ret_type ) );

		if( !getDataType() || getDataType()->getType() == DSRDATA_TYPE_ERROR )
			return;

		switch( getDataType()->getType() )
		{
		case DSRDATA_TYPE_RESERVED:
		case DSRDATA_TYPE_ERROR:
		case DSRDATA_TYPE_KEYWORD:
		case DSRDATA_TYPE_UNKNOWN:
		case DSRDATA_TYPE_NONE:
		case DSRDATA_TYPE_IGNORE:
		case DSRDATA_TYPE_INTERNAL_NESTED:
			run_time_stack_overhead = 0;
			break;
		case DSRDATA_TYPE_COMPOSITION:
			run_time_stack_overhead = ((CDsrDataType_Composition*)getDataType())->getChildNumber();
			break;
		default:
			run_time_stack_overhead = 1;
		}

		//=== change addressing scheme for l-values in the left part of assignment
		if( fun_id == OPERATOR_op_assign )
		{
			if( args && args->expr[ 0 ] )
				args->expr[ 0 ]->setAddrScheme4LValue( smbtable );
		}

		long i;
		//=== check whether equation and independent variables have the same type
		CParseDsrSymbol *v_used;
		if( args && args->expr[ 0 ] &&
			args->expr[ 0 ]->getDataType() )
		{
			for( i = 0; i < (long) m_used_xvar.ArrayDim(); i++ )
			{
				v_used = smbtable.getSymbol( m_used_xvar[ i ] );
				if( v_used && v_used->getDataType() &&
					!args->expr[ 0 ]->getDataType()->compare( v_used->getDataType() ) )
				{
					setDataType( smbtable.getTypeTable().makeDataTypeNode( DSRDATA_TYPE_ERROR ) );
					smbtable.AddLine( _T("illegal equation, types of the equation and unknown independent variable '%1' differ"), getLine(), v_used->name );
					break;
				}
			}
			if( i == (long) m_used_xvar.ArrayDim() )
			{
				for( i = 0; i < (long) m_used_dxvar.ArrayDim(); i++ )
				{
					v_used = smbtable.getSymbol( m_used_dxvar[ i ] );
					if( v_used && v_used->getDataType() &&
						!args->expr[ 0 ]->getDataType()->compare( v_used->getDataType() ) )
					{
						setDataType( smbtable.getTypeTable().makeDataTypeNode( DSRDATA_TYPE_ERROR ) );
						smbtable.AddLine( _T("illegal equation, types of the equation and unknown independent variable '%1' differ"), getLine(), v_used->name );
						break;
					}
				}
			}
		}
		//===

		if( args->getDataType()->isXValue() )
			getDataType()->setXValue();
		if( args->getDataType()->isDynamic() )
			getDataType()->setDynamic();
		if( is_Returns )
			getDataType()->setReturns();
		if( args->getDataType()->isLinearEQ() )
		{
			if( args->getDataType()->isXValue() )
			{
				int op_f = ::ml_getOpLinearFeature( fun_id/*op->id*/ );
				switch( op_f )
				{
				case OP_IS_LINEAR_FEATURE__ignore:
					getDataType()->setLinearEQ();
					break;
				case OP_IS_LINEAR_FEATURE__one_c:
					for( i = 0; i < args->getArgNumber(); i++ )
					{
						if( args->expr[ i ] && 
							args->expr[ i ]->getDataType() &&
							!args->expr[ i ]->getDataType()->isXValue() )
						{
							getDataType()->setLinearEQ();
							break;
						}
					}
					break;
				case OP_IS_LINEAR_FEATURE__left_c:
					if( args->expr[ 0 ] && 
						args->expr[ 0 ]->getDataType() &&
						!args->expr[ 0 ]->getDataType()->isXValue() )
						getDataType()->setLinearEQ();
					break;
				case OP_IS_LINEAR_FEATURE__right_c:
					if( args->expr[ 1 ] && 
						args->expr[ 1 ]->getDataType() &&
						!args->expr[ 1 ]->getDataType()->isXValue() )
						getDataType()->setLinearEQ();
					break;
				}
			}
			else
				getDataType()->setLinearEQ();
		}
	}
	else
		setDataType( smbtable.getTypeTable().makeDataTypeNode( DSRDATA_TYPE_ERROR ) );
}

void CDsrAST_Expr::setAddrScheme4LValue( CParseDsrSymbolTable& smbtable )
{
	if( fun_id == OPERATOR_square_brackets )
	{
		fun_id = OPERATOR_square_brackets_byaddr;
		if( args && args->expr[ 0 ] )
			args->expr[ 0 ]->setAddrScheme4LValue( smbtable );
	}
}

///////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////
//	Function	:	CompoundID, Separator, ArgList, Separator;
/////////////////////////////////////////
void CDsrAST_Function::convertArgVar2Polynomial( CParseDsrSymbolTable& smbtable, long max_num_arg )
{
	if( args )
		args->convertArgVar2Polynomial( smbtable, max_num_arg );
}

void CDsrAST_Function::reportTypeError( CParseDsrSymbolTable& smbtable, CParseDsrSymbol *smb )
{
	QString txt = smb ? QString("\'%1\' : illegal").arg( smb->name ) : QString("illegal");
	if( !args || args->getArgNumber() == 0 )
		txt += QString("), function doesn't take 0 arguments");
	else
	{
		if( args->getArgNumber() == 1 )
			txt += QString("), function doesn't take 1 argument of type ");
		else if( args->getArgNumber() > 1 )
			txt += QString("), function doesn't take %1 arguments of types ").arg( args->getArgNumber() );

		if( args->expr[ 0 ] && args->expr[ 0 ]->getDataType() )
			txt += QString("\'%1\'").arg( args->expr[ 0 ]->getDataType()->getTypeName() );
		for( long i = 1; i < args->getArgNumber(); i++ )
		{
			if( args->expr[ i ] && args->expr[ i ]->getDataType() )
				txt += QString("), \'%1\'").arg( args->expr[ i ]->getDataType()->getTypeName() );
		}
	}
	smbtable.AddLine( txt, getLine() );
}

void CDsrAST_Function::placeDataType( CParseDsrSymbolTable& smbtable )
{
	if( name ) name->placeDataType( smbtable );
	if( lb ) lb->placeDataType( smbtable );
	if( args ) args->placeDataType( smbtable );
	if( rb ) rb->placeDataType( smbtable );

	CDsrDataType_Composition *arg_type = 0;
	if( name && name->getDataType() && name->getDataType()->getType() != DSRDATA_TYPE_ERROR )
	{
		if( name->getDataType()->getType() == DSRDATA_TYPE_UNKNOWN )
		{
			CParseDsrSymbol *smb = name->getSymbol( smbtable );
			if( smb )
				smbtable.AddLine( _T("unknown function \'%1\'"), name->getLine(), smb->name );
			else
				smbtable.AddLine( _T("unknown function"), name->getLine() );
			setDataType( smbtable.getTypeTable().makeDataTypeNode( DSRDATA_TYPE_ERROR ) );
			return;
		}

		if( !args )
			arg_type = (CDsrDataType_Composition*)smbtable.getTypeTable().makeDataTypeNode( DSRDATA_TYPE_COMPOSITION );
		else if( args->getDataType() && args->getDataType()->getType() == DSRDATA_TYPE_COMPOSITION )
			arg_type = (CDsrDataType_Composition*)(args->getDataType());
		else
		{
			setDataType( smbtable.getTypeTable().makeDataTypeNode( DSRDATA_TYPE_ERROR ) );
			return;
		}
		placeDataType_args( smbtable, arg_type );
	}
	else
		setDataType( smbtable.getTypeTable().makeDataTypeNode( DSRDATA_TYPE_ERROR ) );
}

void CDsrAST_Function::placeDataType_args( CParseDsrSymbolTable& smbtable, CDsrDataType_Composition *arg_type )
{
	CParseDsrSymbol *smb = name->getSymbol( smbtable );
	std::vector<__CDSRTypeConversionOpVector> opc;
	long cb;
	CDsrDataType_Function* fun_type;
	CDsrDataType_Node* ret_type, *nested_node, *n_res;

	if( !smb )
	{
		setDataType( smbtable.getTypeTable().makeDataTypeNode( DSRDATA_TYPE_ERROR ) );
		smbtable.AddLine(  _T("internal error: function operator entry is missing"), getLine() );
		return;
	}

	fun_id = (enum CDsrFunctionListID)(smb->param1);

	if( ::isType_Var_and_Function( smb->getDataType()->getType() ) )
	{
		nested_node = smb->getDataType()->getNestedType();
		CDsrDataType_Composition *n_arg = (CDsrDataType_Composition*)smbtable.getTypeTable().makeDataTypeNode( new CDsrDataType_Composition() );
		if( nested_node )
		{
			n_arg->Add( smbtable.getTypeTable().cloneDataTypeNode( nested_node ) );
			if( smb->getDataType()->getType() == DSRDATA_TYPE_RATIONAL_FUN &&
				nested_node->getType() == DSRDATA_TYPE_INTEGER )
			{
				// rational function (int) -> int/int = rational number
				n_res = smbtable.getTypeTable().makeDataTypeNode( DSRDATA_TYPE_RATIONAL );
			}
			else
				n_res = smbtable.getTypeTable().cloneDataTypeNode( nested_node );
		}
		else
		{
			n_arg->Add( smbtable.getTypeTable().makeDataTypeNode( DSRDATA_TYPE_UNKNOWN ) );
			n_res = smbtable.getTypeTable().makeDataTypeNode( DSRDATA_TYPE_UNKNOWN );
		}
		//n_res->setLValue();
		fun_type = (CDsrDataType_Function*)smbtable.getTypeTable().makeDataTypeNode( new CDsrDataType_Function( n_arg, n_res ) );
		ret_type = CompareOp2Args( 	fun_id,
									fun_type, /*fun_id, */fun_id_param,
									arg_type, opc, cb, smbtable, smb );
	}
	else
	{
		ret_type = CompareOp2Args( 	fun_id/*-1*/,
									name->getDataType(), /*fun_id, */fun_id_param,
									arg_type, opc, cb, smbtable, smb );
	}

	if( ret_type->getType() == DSRDATA_TYPE_ERROR )
	{
		reportTypeError( smbtable, smb );
	}
	else
	{
		if( args && cb > 0 )
			args->convertArgTypes( opc[ fun_id_param ], smbtable );
		if( fun_id == FUNCTION_std_identity )
		{// ignoring type conversions
			if( args->expr[ 0 ]->getType() != DsrASTT_ConstInt )
			{
				setDataType( smbtable.getTypeTable().makeDataTypeNode( DSRDATA_TYPE_ERROR ) );
				smbtable.AddLine(  _T("'%1' : the first parameter of the function should be the constant of the type 'natural number'"), getLine(), smb->name );
				return;
			}
			else if( ret_type->getType() == DSRDATA_TYPE_VECTOR )
			{
				long v = ((CDsrAST_ConstInt*)(args->expr[ 0 ]))->value;
				((CDsrDataType_Vector*)ret_type)->setRows( v );
				((CDsrDataType_Vector*)ret_type)->setColumns( v );
			}
		}
	}

	if( ret_type->getType() == DSRDATA_TYPE_INTERNAL_NESTED &&
		args && args->expr[ 0 ] && args->expr[ 0 ]->getDataType() && args->expr[ 0 ]->getDataType()->getNestedType() )
	{
		CDsrDataType_Node *tn = smbtable.cloneDataTypeNode( args->expr[ 0 ]->getDataType()->getNestedType() );
		tn->copyModifier( args->expr[ 0 ]->getDataType() );
		setDataType( tn );
	}
	else
		setDataType( smbtable.cloneDataTypeNode( ret_type ) );

	if( !getDataType() || getDataType()->getType() == DSRDATA_TYPE_ERROR )
		return;

	switch( getDataType()->getType() )
	{
	case DSRDATA_TYPE_RESERVED:
	case DSRDATA_TYPE_ERROR:
	case DSRDATA_TYPE_KEYWORD:
	case DSRDATA_TYPE_UNKNOWN:
	case DSRDATA_TYPE_NONE:
	case DSRDATA_TYPE_IGNORE:
	case DSRDATA_TYPE_INTERNAL_NESTED:
		run_time_stack_overhead = 0;
		break;
	case DSRDATA_TYPE_COMPOSITION:
		run_time_stack_overhead = ((CDsrDataType_Composition*)getDataType())->getChildNumber();
		break;
	default:
		run_time_stack_overhead = 1;
	}

	if( fun_id == FUNCTION_USER || 
		fun_id == FUNCTION_USER_POLYNOMIAL || 
		fun_id == FUNCTION_USER_RATIONAL_FUN )
	{
		fun_id_param = smb->param2;
	}

	if( args && args->getDataType() && getDataType() )
	{
		if( args->getDataType()->isXValue() )
			getDataType()->setXValue();
		if( args->getDataType()->isReturns() )
			getDataType()->setReturns();
		if( args->getDataType()->isDynamic() )
			getDataType()->setDynamic();
		if( args->getDataType()->isLinearEQ() )
		{
			if( args->getDataType()->isXValue() )
			{
				int op_f = ::ml_getOpLinearFeature( fun_id );
				long i;
				switch( op_f )
				{
				case OP_IS_LINEAR_FEATURE__ignore:
					getDataType()->setLinearEQ();
					break;
				case OP_IS_LINEAR_FEATURE__one_c:
					for( i = 0; i < args->getArgNumber(); i++ )
					{
						if( args->expr[ i ] && 
							args->expr[ i ]->getDataType() &&
							!args->expr[ i ]->getDataType()->isXValue() )
						{
							getDataType()->setLinearEQ();
							break;
						}
					}
					break;
				case OP_IS_LINEAR_FEATURE__left_c:
					if( args->expr[ 0 ] && 
						args->expr[ 0 ]->getDataType() &&
						!args->expr[ 0 ]->getDataType()->isXValue() )
						getDataType()->setLinearEQ();
					break;
				case OP_IS_LINEAR_FEATURE__right_c:
					if( args->expr[ 1 ] && 
						args->expr[ 1 ]->getDataType() &&
						!args->expr[ 1 ]->getDataType()->isXValue() )
						getDataType()->setLinearEQ();
					break;
				}
			}
			else
				getDataType()->setLinearEQ();
		}
	}
}

///////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////
//	Special		:	long, Separator, ArgList, Separator;
/////////////////////////////////////////
void CDsrAST_Special::convertArgVar2Polynomial( CParseDsrSymbolTable& smbtable, long max_num_arg )
{
	if( args )
		args->convertArgVar2Polynomial( smbtable, max_num_arg );
}

void CDsrAST_Special::reportTypeError( CParseDsrSymbolTable& smbtable, CParseDsrSymbol *smb )
{
	QString txt = smb ? QString("\'%1\' : illegal").arg( smb->name ) : QString("illegal");
	if( !args || args->getArgNumber() == 0 )
		txt += QString("), function doesn't take 0 arguments");
	else
	{
		if( args->getArgNumber() == 1 )
			txt += QString("), special operator doesn't take 1 argument of type ");
		else if( args->getArgNumber() > 1 )
			txt += QString("), special operator doesn't take %1 arguments of types ").arg( args->getArgNumber() );

		if( args->expr[ 0 ] && args->expr[ 0 ]->getDataType() )
			txt += QString("\'%1\'").arg( args->expr[ 0 ]->getDataType()->getTypeName() );
		for( long i = 1; i < args->getArgNumber(); i++ )
		{
			if( args->expr[ i ] && args->expr[ i ]->getDataType() )
				txt += QString("), \'%1\'").arg( args->expr[ i ]->getDataType()->getTypeName() );
		}
	}
	smbtable.AddLine( txt, getLine() );
}

void CDsrAST_Special::passArgList2Out( CParseDsrSymbolTable& smbtable )
{
	if( !args || !args->getDataType() )
		setDataType( smbtable.getTypeTable().makeDataTypeNode( DSRDATA_TYPE_NONE ) );
	else if( args->getDataType()->getType() == DSRDATA_TYPE_COMPOSITION )
	{
		switch( args->getArgNumber() )
		{
		case 0:
			setDataType( smbtable.getTypeTable().makeDataTypeNode( DSRDATA_TYPE_NONE ) );
			break;
		case 1:
			if( args->expr[ 0 ] && args->expr[ 0 ]->getDataType() )
				setDataType( smbtable.getTypeTable().cloneDataTypeNode( args->expr[ 0 ]->getDataType() ) );
			else
				setDataType( smbtable.getTypeTable().cloneDataTypeNode( args->getDataType() ) );
			// vasya : check 1 arg after ignoring and more args ignoring after the given decomposing ArgList
			// vasya : examples 'sin(@frame(3.1415))', 'pow(@frame(2),@frame(3))', @frame(3.1415) + 1.0
			break;
		default:
			setDataType( smbtable.getTypeTable().cloneDataTypeNode( args->getDataType() ) );
		}
	}
	else
		setDataType( smbtable.getTypeTable().cloneDataTypeNode( args->getDataType() ) );
}

int CDsrAST_Special::placeDataType_prime( CParseDsrSymbolTable& smbtable, CDsrDataType_Composition *arg_type, CParseDsrSymbol *smb )
{
	if( args->getArgNumber() < 1 || arg_type->n_arg.ArrayDim() < 1 ||
		!args->expr[ 0 ] || !args->expr[ 0 ]->getDataType() ||
		(args->getArgNumber() > 1 && args->expr[ 1 ] && args->expr[ 1 ]->getType() != DsrASTT_ConstInt) )
	{
		reportTypeError( smbtable, smb );
		return -1;
	}

	CDsrAST_CompoundID *smb_cid = 0;
	CDsrAST_PrimaryID *smb_pid = 0;
	CParseDsrSymbol *smb_var = 0;
	if( args->expr[ 0 ]->getType() == DsrASTT_CompoundID )
	{
		smb_cid = ((CDsrAST_CompoundID*)(args->expr[ 0 ]));
		if( smb_cid->stmt.ArrayDim() )
			smb_pid = smb_cid->stmt[ smb_cid->stmt.ArrayDim() - 1 ];
		smb_var = smb_cid->getSymbol( smbtable );
	}
	else if( args->expr[ 0 ]->getType() == DsrASTT_PrimaryID )
	{
		smb_pid = ((CDsrAST_PrimaryID*)(args->expr[ 0 ]));
		smb_var = smb_pid->getSymbol( smbtable );
	}

	if( !smb_var || smb_pid->id == -1 ||
		!smb_pid->getDataType()->isLValue() || !args->expr[ 0 ]->getDataType()->isXValue() )
	{
		smbtable.AddLine( _T("'%1' : the first argument must be unknown independent variable"), getLine(), smb->name );
		setDataType( smbtable.getTypeTable().makeDataTypeNode( DSRDATA_TYPE_ERROR ) );
		return -1;
	}

	smbtable.AddUsedDXVar( smb_pid->id );

	return 0;
}

CDsrDataType_Node* CDsrAST_Special::placeDataType_create_table( 
	CDsrDataType_Node* /*op_type*/, 
	//enum CDsrFunctionListID& fun_id,
	long& fun_id_param,
	CDsrDataType_Composition* /*args_type*/,
	std::vector<__CDSRTypeConversionOpVector>& __opc,
	long& cb,
	CParseDsrSymbolTable& smbtable,
	CParseDsrSymbol *smb )
{
	//if( smb ) fun_id = (enum CDsrFunctionListID)smb->param1;
	fun_id_param = 0;
	__opc.resize( 1 );
	__CDSRTypeConversionOpVector& opc = __opc[ fun_id_param ];
	cb = 0;

	CDsrDataType_Node *ret = 0;
	enum CDsrFunctionListID rest;
	enum CDsrDataTypes maxt = DSRDATA_TYPE_RESERVED;

	opc.Flush();

	int err = (args && args->getArgNumber() > 2) ? 0 : 1;
	int is_scalar = 0, is_complex = 0;
	MArray<long> dim( 2, 0, 2 );

	long i;
	for( i = 0; !err && i < args->getArgNumber(); i++ )
	{
		if( !args->expr[ i ] || !args->expr[ i ]->getDataType() )
		{
			err = 1;
			break;
		}
		switch( i )
		{
		case 0:
		case 1:
			if( args->expr[ i ]->getType() != DsrASTT_ConstInt )
			{
				err = 1;
				continue;
			}
			else
				dim[ i ] = ((CDsrAST_ConstInt*)(args->expr[ i ]))->value;
			if( !args->expr[ i ]->getDataType()->isScalar() )
			{
				err = 1;
				continue;
			}
			if( args->expr[ i ]->getDataType()->getType() != DSRDATA_TYPE_INTEGER )
			{
				rest = ::getTypeConvOp( DSRDATA_TYPE_INTEGER, 
										args->expr[ i ]->getDataType()->getType() );
				if( rest == FUNCTION_STD_NONE )
				{
					err = 1;
					continue;
				}
				cb++;
			}
			else
				rest = FUNCTION_STD_NONE;
			opc.Add( rest );
			break;
		default:
			if( args->expr[ i ]->getDataType()->isScalar() )
			{
				is_scalar = 1;
				if( maxt < args->expr[ i ]->getDataType()->getType() )
				{
					ret = args->expr[ i ]->getDataType();
					maxt = ret->getType();
				}
			}
			else
				is_complex = 1;
			opc.Add( rest = FUNCTION_STD_NONE );
		}
	}
	if( err || dim.ArrayDim() != 2 ||
		is_scalar + is_complex != 1 || 
		args->getArgNumber() != opc.ArrayDim() )
	{
		//reportTypeError( smbtable, smb );
		smbtable.AddLine( _T("'%1' : illegal, the 1st and the 2nd argument must be integer constants"), getLine(), smb->name );
		return smbtable.getTypeTable().makeDataTypeNode( DSRDATA_TYPE_ERROR );
	}
	else if( dim[ 0 ] * dim[ 1 ] != args->getArgNumber() - 2 )
	{
		smbtable.AddLine( QString("'%1' : %2 arguments are expected, %3 arguments are present")
							.arg(smb->name).arg(dim[ 0 ] * dim[ 1 ] + 2).arg(args->getArgNumber()),
							getLine() );
		return smbtable.getTypeTable().makeDataTypeNode( DSRDATA_TYPE_ERROR );
	}
	if( is_scalar )
	{
		fun_id_param = maxt;
		for( i = 2; i < args->getArgNumber(); i++ )
		{
			if( args->expr[ i ]->getDataType()->getType() != maxt )
			{
				rest = ::getTypeConvOp( maxt, 
										args->expr[ i ]->getDataType()->getType() );
				if( rest == FUNCTION_STD_NONE )
				{
					err = 1;
					break;
				}
				cb++;
				opc[ i ] = rest;
			}
		}
	}
	else
	{
		ret = args->expr[ 2 ]->getDataType();
		fun_id_param = ret->getType();
		for( i = 3; i < args->getArgNumber(); i++ )
		{
			if( !ret->compare( args->expr[ i ]->getDataType() ) )
			{
				err = 1;
				break;
			}
		}
	}

	if( err )
	{
		reportTypeError( smbtable, smb );
		return smbtable.getTypeTable().makeDataTypeNode( DSRDATA_TYPE_ERROR );
	}

	CDsrDataType_Vector *retv = new CDsrDataType_Vector( ret );
	retv->setRows( dim[ 0 ] );
	retv->setColumns( dim[ 1 ] );
	return smbtable.getTypeTable().makeDataTypeNode( retv );
}

void CDsrAST_Special::placeDataType( CParseDsrSymbolTable& smbtable )
{
	if( name ) name->placeDataType( smbtable );
	if( lb ) lb->placeDataType( smbtable );
	if( args ) args->placeDataType( smbtable );
	if( rb ) rb->placeDataType( smbtable );

	CDsrDataType_Composition *arg_type = 0;
	if( name && name->getDataType() && name->getDataType()->getType() != DSRDATA_TYPE_ERROR )
	{
		CParseDsrSymbol *smb = name->getSymbol( smbtable );
		if( name->getDataType()->getType() == DSRDATA_TYPE_UNKNOWN )
		{
			if( smb )
				smbtable.AddLine( _T("unknown special operator \'%1\'"), name->getLine(), smb->name );
			else
				smbtable.AddLine( _T("unknown special operator"), name->getLine() );
			setDataType( smbtable.getTypeTable().makeDataTypeNode( DSRDATA_TYPE_ERROR ) );
			return;
		}

		if(	smb )
		{
			if( smb->smb_record_type == SMBTABLE_TYPE_VARIABLE )
			{
				if( args )
				{
					smbtable.AddLine( _T("\'%1\' : special operator doesn't take any arguments"), name->getLine(), smb->name );
					setDataType( smbtable.getTypeTable().makeDataTypeNode( DSRDATA_TYPE_ERROR ) );
					return;
				}
				setDataType( smbtable.getTypeTable().cloneDataTypeNode( smb->getDataType() ) );
				return;
			}
			else if( smb->smb_record_type != SMBTABLE_TYPE_FUNCTION &&
					 smb->smb_record_type != SMBTABLE_TYPE_FORMULA )
			{
				setDataType( smbtable.getTypeTable().makeDataTypeNode( DSRDATA_TYPE_NONE ) );
				return;
			}
		}

		if( name->getDataType()->getType() == DSRDATA_TYPE_IGNORE )
			passArgList2Out( smbtable );
		else
		{
			if( !args )
			{
				arg_type = (CDsrDataType_Composition*)smbtable.getTypeTable().makeDataTypeNode( DSRDATA_TYPE_COMPOSITION );
				placeDataType_args( smbtable, arg_type );
			}
			else if( args->getDataType() && args->getDataType()->getType() == DSRDATA_TYPE_COMPOSITION )
			{
				arg_type = (CDsrDataType_Composition*)(args->getDataType());
				placeDataType_args( smbtable, arg_type );
			}
			else
				setDataType( smbtable.getTypeTable().makeDataTypeNode( DSRDATA_TYPE_ERROR ) );
		}
	}
	else
	{
		setDataType( smbtable.getTypeTable().makeDataTypeNode( DSRDATA_TYPE_ERROR ) );
		smbtable.AddLine( _T("internal error: special operator name is missing"), getLine() );
	}
}

void CDsrAST_Special::placeDataType_args( CParseDsrSymbolTable& smbtable, CDsrDataType_Composition *arg_type )
{
	CParseDsrSymbol *smb = name->getSymbol( smbtable );
	std::vector<__CDSRTypeConversionOpVector> opc;
	long cb;
	CDsrDataType_Node* ret_type = 0;

	if( !smb )
	{
		setDataType( smbtable.getTypeTable().makeDataTypeNode( DSRDATA_TYPE_ERROR ) );
		smbtable.AddLine( _T("internal error: special operator entry is missing"), getLine() );
		return;
	}

	fun_id = (enum CDsrFunctionListID)(smb->param1);

	if( fun_id == FORMULA_table )
	{
		ret_type = placeDataType_create_table( 
						name->getDataType(), /*fun_id, */fun_id_param,
						arg_type, opc, cb, smbtable, smb );
		if( ret_type->getType() == DSRDATA_TYPE_ERROR )
		{
			if( !smb || smb->param1 != FORMULA_table )
				reportTypeError( smbtable, smb );
		}
		else if( args && cb > 0 )
			args->convertArgTypes( opc[ 0 ], smbtable );
	}
	else
	{
		if( fun_id == FORMULA_prime )
		{
			if( placeDataType_prime( smbtable, arg_type, smb ) == -1 )
				return;
		}
		ret_type = CompareOp2Args( 
						fun_id,
						name->getDataType(), /*fun_id, */fun_id_param,
						arg_type, opc, cb, smbtable, smb );
		if( ret_type->getType() == DSRDATA_TYPE_ERROR )
		{
			if( !smb || smb->param1 != FORMULA_table )
				reportTypeError( smbtable, smb );
		}
		else if( args && cb > 0 )
			args->convertArgTypes( opc[ fun_id_param ], smbtable );
	}

	if( ret_type->getType() == DSRDATA_TYPE_INTERNAL_NESTED &&
		args && args->expr[ 0 ] && args->expr[ 0 ]->getDataType() && args->expr[ 0 ]->getDataType()->getNestedType() )
	{
		CDsrDataType_Node *tn = smbtable.cloneDataTypeNode( args->expr[ 0 ]->getDataType()->getNestedType() );
		tn->copyModifier( args->expr[ 0 ]->getDataType() );
		setDataType( tn );
	}
	else
		setDataType( smbtable.cloneDataTypeNode( ret_type ) );

	if( !getDataType() || getDataType()->getType() == DSRDATA_TYPE_ERROR )
		return;

	switch( getDataType()->getType() )
	{
	case DSRDATA_TYPE_RESERVED:
	case DSRDATA_TYPE_ERROR:
	case DSRDATA_TYPE_KEYWORD:
	case DSRDATA_TYPE_UNKNOWN:
	case DSRDATA_TYPE_NONE:
	case DSRDATA_TYPE_IGNORE:
	case DSRDATA_TYPE_INTERNAL_NESTED:
		run_time_stack_overhead = 0;
		break;
	case DSRDATA_TYPE_COMPOSITION:
		run_time_stack_overhead = ((CDsrDataType_Composition*)getDataType())->getChildNumber();
		break;
	default:
		run_time_stack_overhead = 1;
	}

	if( fun_id == FORMULA_prime && getDataType()->getType() == DSRDATA_TYPE_VECTOR )
	{
		// check whether index is valid for unknown vector
		CDsrDataType_Vector *tp = (CDsrDataType_Vector*)getDataType();
		if( args && args->getArgNumber() > 1 && args->expr[ 1 ] && args->expr[ 1 ]->getType() == DsrASTT_ConstInt )
		{
			long idx = ((CDsrAST_ConstInt*)(args->expr[ 1 ]))->value;
			if( idx < 1 || idx >= tp->getRows() * tp->getColumns() ||
				(tp->getRows() != 1 && tp->getColumns() != 1 && idx >= tp->getRows() ) )
			{
				smbtable.AddLine( _T("'%1' : index is out of range"), getLine(), smb->name );
				setDataType( smbtable.getTypeTable().makeDataTypeNode( DSRDATA_TYPE_ERROR ) );
				return;
			}
		}
	}

	if( fun_id == FUNCTION_USER || 
		fun_id == FUNCTION_USER_POLYNOMIAL || 
		fun_id == FUNCTION_USER_RATIONAL_FUN )
		fun_id_param = smb->param2;

	if( args && args->getDataType() && getDataType() )
	{
		if( args->getDataType()->isXValue() )
			getDataType()->setXValue();
		if( args->getDataType()->isReturns() )
			getDataType()->setReturns();
		if( args->getDataType()->isDynamic() )
			getDataType()->setDynamic();
		if( args->getDataType()->isLinearEQ() )
		{
			if( args->getDataType()->isXValue() )
			{
				int op_f = ::ml_getOpLinearFeature( fun_id );
				long i;
				switch( op_f )
				{
				case OP_IS_LINEAR_FEATURE__ignore:
					getDataType()->setLinearEQ();
					break;
				case OP_IS_LINEAR_FEATURE__one_c:
					for( i = 0; i < args->getArgNumber(); i++ )
					{
						if( args->expr[ i ] && 
							args->expr[ i ]->getDataType() &&
							!args->expr[ i ]->getDataType()->isXValue() )
						{
							getDataType()->setLinearEQ();
							break;
						}
					}
					break;
				case OP_IS_LINEAR_FEATURE__left_c:
					if( args->expr[ 0 ] && 
						args->expr[ 0 ]->getDataType() &&
						!args->expr[ 0 ]->getDataType()->isXValue() )
						getDataType()->setLinearEQ();
					break;
				case OP_IS_LINEAR_FEATURE__right_c:
					if( args->expr[ 1 ] && 
						args->expr[ 1 ]->getDataType() &&
						!args->expr[ 1 ]->getDataType()->isXValue() )
						getDataType()->setLinearEQ();
					break;
				}
			}
			else
				getDataType()->setLinearEQ();
		}
	}
}

void CDsrAST_Special::setAddrScheme4LValue( CParseDsrSymbolTable& smbtable )
{
	if( fun_id == OPERATOR_square_brackets )
	{
		fun_id = OPERATOR_square_brackets_byaddr;
		if( args && args->expr[ 0 ] )
			args->expr[ 0 ]->setAddrScheme4LValue( smbtable );
	}
}

///////////////////////////////////////////////////////////////////////////
