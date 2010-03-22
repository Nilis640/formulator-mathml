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
//	ObjDef_Assign	:	ObjHead, Op, Expr, Separator;
/////////////////////////////////////////
void CDsrAST_ObjDef_Assign::reportTypeError( CParseDsrSymbolTable& smbtable, CParseDsrSymbol *smb, CDsrDataType_Node *ret_type )
{
	QString txt = smb ? 
		QString("'%1' : illegal object definition, \'%1\' : illegal").arg( smb->name ).arg( ::ml_Op2String( op->id ) ) :
		QString("illegal object definition, \'%1\' : illegal").arg( ::ml_Op2String( op->id ) );
	if( ret_type )
		txt += QString(", left operand has type \'%1\'").arg( ret_type->getTypeName() );
	txt += QString(", right operand has type \'%1\'").arg( expr->getDataType()->getTypeName() );
	smbtable.AddLine( txt, getLine() );
}

void CDsrAST_ObjDef_Assign::placeDataType( CParseDsrSymbolTable& smbtable )
{
	if( head ) head->placeDataType( smbtable );
	if( op ) op->placeDataType( smbtable );
	if( sep ) sep->placeDataType( smbtable );

	CParseDsrSymbol *smb = 0;
	if( !op || !head || !head->name || !(smb = head->name->getSymbol( smbtable )) ||
		!head->getDataType() || head->getDataType()->getType() == DSRDATA_TYPE_ERROR ||
		!op || !op->getDataType() || op->getDataType()->getType() == DSRDATA_TYPE_ERROR ||
		!sep || !sep->getDataType() || sep->getDataType()->getType() == DSRDATA_TYPE_ERROR )
	{
		setDataType( smbtable.getTypeTable().makeDataTypeNode( DSRDATA_TYPE_ERROR ) );
		return;
	}

	if( expr )
	{
		// consider all parameters of polynomial and rational function 
		// having types "polynomial" and "rational function"
		if( ::isType_Var_and_Function( head->getDataType()->getType() ) )
		{
			expr->convertArgVar2Polynomial( smbtable, head->getDataType()->getArgumentsNumber() );
			CDsrDataType_Node *n = smb->getDataType();
			smb->setDataType( smbtable.getTypeTable().makeDataTypeNode( DSRDATA_TYPE_UNKNOWN ) );
			expr->placeDataType( smbtable );
			smb->setDataType( n );
		}
		else
			expr->placeDataType( smbtable );
	}

	if( !expr || !expr->getDataType() || expr->getDataType()->getType() == DSRDATA_TYPE_ERROR )
	{
		setDataType( smbtable.getTypeTable().makeDataTypeNode( DSRDATA_TYPE_ERROR ) );
		return;
	}
	else
	{
		if( head->getDataType()->getType() == DSRDATA_TYPE_FUNCTION )
		{
			if( expr->getDataType()->isXValue() )
			{
				smbtable.AddLine( _T("'%1' : illegal, function cannot depends on a unknown independent variable"), getLine(), smb->name );
				setDataType( smbtable.getTypeTable().makeDataTypeNode( DSRDATA_TYPE_ERROR ) );
				return;
			}
		}
		else
		{
			if( expr->getDataType()->isXValue() || expr->getDataType()->isDynamic() )
			{
				smbtable.AddLine( _T("'%1' : object must be a static variable"), getLine(), smb->name );
				setDataType( smbtable.getTypeTable().makeDataTypeNode( DSRDATA_TYPE_ERROR ) );
				return;
			}
		}
	}

	CDsrDataType_Node *ret_type = 0;
	CDsrDataType_Function *node = 0;
	if( head->keyword->getKeywordType() == DSRDATA_TYPE_FUNCTION )
	{
		if( head->getDataType()->getType() != DSRDATA_TYPE_FUNCTION ||
			smb->getDataType()->getType() != DSRDATA_TYPE_FUNCTION )
		{
			setDataType( smbtable.getTypeTable().makeDataTypeNode( DSRDATA_TYPE_ERROR ) );
			smbtable.AddLine( _T("internal error: keyword and object types mismatch"), getLine() );
			return;
		}
		node = (CDsrDataType_Function*)(head->getDataType());
		if( !node->n_res || node->n_res->getType() == DSRDATA_TYPE_UNKNOWN )
			node->n_res = smbtable.getTypeTable().cloneDataTypeNode( expr->getDataType() );
		((CDsrDataType_Function*)(smb->getDataType()))->n_res = smbtable.getTypeTable().cloneDataTypeNode( node->n_res );
		ret_type = node->n_res;
	}
	else
	{
		ret_type = head->getDataType();
		if( ::isType_Var_and_Function( head->getDataType()->getType() ) )
		{
			if( !head->getDataType()->getNestedType() ||
				head->getDataType()->getNestedType()->getType() == DSRDATA_TYPE_UNKNOWN )
			{
				switch( expr->getDataType()->getType() )
				{
				case DSRDATA_TYPE_NATURAL:
				case DSRDATA_TYPE_INTEGER:
					head->getDataType()->setNestedType( smbtable.getTypeTable().makeDataTypeNode(DSRDATA_TYPE_INTEGER) );
					smb->getDataType()->setNestedType( smbtable.getTypeTable().makeDataTypeNode(DSRDATA_TYPE_INTEGER) );
					break;
				case DSRDATA_TYPE_RATIONAL:
				case DSRDATA_TYPE_REAL:
					head->getDataType()->setNestedType( smbtable.getTypeTable().makeDataTypeNode(DSRDATA_TYPE_REAL) );
					smb->getDataType()->setNestedType( smbtable.getTypeTable().makeDataTypeNode(DSRDATA_TYPE_REAL) );
					break;
				case DSRDATA_TYPE_COMPLEX:
					head->getDataType()->setNestedType( smbtable.getTypeTable().makeDataTypeNode(DSRDATA_TYPE_COMPLEX) );
					smb->getDataType()->setNestedType( smbtable.getTypeTable().makeDataTypeNode(DSRDATA_TYPE_COMPLEX) );
					break;
				case DSRDATA_TYPE_POLYNOM:
				case DSRDATA_TYPE_RATIONAL_FUN:
					if( head->keyword->getKeywordType() == expr->getDataType()->getType() &&
						expr->getDataType()->getNestedType() )
					{
						head->getDataType()->setNestedType( smbtable.getTypeTable().makeDataTypeNode( expr->getDataType()->getNestedType()->getType() ) );
						smb->getDataType()->setNestedType( smbtable.getTypeTable().makeDataTypeNode( expr->getDataType()->getNestedType()->getType() ) );
					}
					break;
				default:
					break;
				}
			}
		}
	}

	if( head )
	{
		enum CDsrFunctionListID rest;
		if( !ret_type->compare( expr->getDataType() ) )
		{
			rest = ::getTypeConvOp( ret_type, 
									expr->getDataType() );
			if( rest == FUNCTION_STD_NONE )
			{
				setDataType( smbtable.getTypeTable().makeDataTypeNode( DSRDATA_TYPE_ERROR ) );
				reportTypeError( smbtable, smb, ret_type );
				return;
			}
			else
			{
				CDsrAST_Expr *n_expr;
				CDsrDataType_Node *tmp_node = expr->getDataType() ? 
					smbtable.getTypeTable().cloneDataTypeNode( expr->getDataType() ) :
					0;
				n_expr = new CDsrAST_Expr( new CDsrAST_Op( rest ), expr );
				n_expr->setLine( expr->getLine() );
				expr = n_expr;
				expr->placeDataType( smbtable );
				if( !expr->getDataType() &&
					expr->getDataType()->getType() == DSRDATA_TYPE_ERROR )
				{
					smbtable.AddLine( _T("internal error: wrong format of type record for the conversion operator"), getLine() );
					setDataType( smbtable.getTypeTable().makeDataTypeNode( DSRDATA_TYPE_ERROR ) );
				}
				if( expr->getDataType() && tmp_node )
					expr->getDataType()->copyModifier( tmp_node );
			}
		}

		//if( ::isType_Var_and_Function( head->getDataType()->getType() ) )
		if( head->keyword->getKeywordType() != DSRDATA_TYPE_FUNCTION )
		{
			CParseDsrSymbol *smb = op->getSymbol( smbtable );
			std::vector<__CDSRTypeConversionOpVector> opc;
			long cb = 0;
			CDsrDataType_Composition *args = (CDsrDataType_Composition*)smbtable.getTypeTable().makeDataTypeNode( DSRDATA_TYPE_COMPOSITION );
			args->Add( head->getDataType() );
			args->Add( expr->getDataType() );
			fun_id = (enum CDsrFunctionListID)(op->id);
			CompareOp2Args( fun_id, op->getDataType(), fun_id_param,
							args, opc, cb, smbtable, smb );
			
			//=== change addressing scheme for l-values in the left part of assignment
			if( fun_id == OPERATOR_op_assign )
				head->name->setAddrScheme4LValue( smbtable );
		}
	}

	setDataType( smbtable.getTypeTable().makeDataTypeNode( DSRDATA_TYPE_NONE ) );
	if( expr->getDataType()->isLinearEQ() )
		getDataType()->setLinearEQ();
}

/////////////////////////////////////////
//	ObjDef_Compound	:	ObjHead, Compound;
/////////////////////////////////////////
void CDsrAST_ObjDef_Compound::placeDataType( CParseDsrSymbolTable& smbtable )
{
	if( head ) head->placeDataType( smbtable );
	CParseDsrSymbol *smb = 0;
	if( !head || !head->keyword || !head->name || 
		!(smb = head->name->getSymbol( smbtable )) ||
		!head->getDataType() || head->getDataType()->getType() == DSRDATA_TYPE_ERROR )
	{
		setDataType( smbtable.getTypeTable().makeDataTypeNode( DSRDATA_TYPE_ERROR ) );
		return;
	}

	if( compound ) compound->placeDataType( smbtable );

	if( !compound || !compound->getDataType() || compound->getDataType()->getType() == DSRDATA_TYPE_ERROR )
	{
		setDataType( smbtable.getTypeTable().makeDataTypeNode( DSRDATA_TYPE_ERROR ) );
		return;
	}
	else if( !compound->getDataType()->isReturns() )
	{
		smbtable.AddLine( _T("'%1' : all control paths must return a value"), getLine(), smb->name );
		setDataType( smbtable.getTypeTable().makeDataTypeNode( DSRDATA_TYPE_ERROR ) );
		return;
	}

	if( head->getDataType()->getType() == DSRDATA_TYPE_FUNCTION )
	{
		if( compound->getDataType()->isXValue() )
		{
			smbtable.AddLine( _T("'%1' : illegal, function cannot depends on a unknown independent variable"), getLine(), smb->name );
			setDataType( smbtable.getTypeTable().makeDataTypeNode( DSRDATA_TYPE_ERROR ) );
			return;
		}
	}
	else
	{
		if( compound->getDataType()->isXValue() || 
			compound->getDataType()->isDynamic() )
		{
			smbtable.AddLine( _T("'%1' : object must be a static variable"), getLine(), smb->name );
			setDataType( smbtable.getTypeTable().makeDataTypeNode( DSRDATA_TYPE_ERROR ) );
			return;
		}
	}

	if( head->keyword->getKeywordType() == DSRDATA_TYPE_POLYNOM ||
		head->keyword->getKeywordType() == DSRDATA_TYPE_RATIONAL_FUN )
	{
		smbtable.AddLine( _T("'%1' : this definition style is illegal for polynomials and rational functions"), getLine(), smb->name );
		setDataType( smbtable.getTypeTable().makeDataTypeNode( DSRDATA_TYPE_ERROR ) );
	}
	else
		setDataType( smbtable.getTypeTable().makeDataTypeNode( DSRDATA_TYPE_NONE ) );

	if( getDataType() && compound->getDataType()->isLinearEQ() )
		getDataType()->setLinearEQ();
}

/////////////////////////////////////////
//	ObjHead			:	Keyword, PrimaryID, ObjArg;
/////////////////////////////////////////
void CDsrAST_ObjHead::placeDataType( CParseDsrSymbolTable& smbtable )
{
	if( keyword ) keyword->placeDataType( smbtable );
	if( name ) name->placeDataType( smbtable );
	if( args ) args->placeDataType( smbtable );

	CDsrDataType_Composition *n_arg;
	CDsrDataType_Node *n_res, *nested_node;
	CParseDsrSymbol *smb = name ? name->getSymbol( smbtable ) : 0;
	if( !keyword || !name || !name->getDataType() || !smb ||
		!(keyword->getKeywordType() == DSRDATA_TYPE_POLYNOM ||
		  keyword->getKeywordType() == DSRDATA_TYPE_RATIONAL_FUN ||
		  keyword->getKeywordType() == DSRDATA_TYPE_ABS_GROUP ||
		  keyword->getKeywordType() == DSRDATA_TYPE_FUNCTION) )
	{
		setDataType( smbtable.getTypeTable().makeDataTypeNode( DSRDATA_TYPE_ERROR ) );
		smbtable.AddLine( _T("internal error: unknown object name"), name->getLine() );
		return;
	}
	if( name->getDataType()->getType() != DSRDATA_TYPE_UNKNOWN )
	{
		setDataType( smbtable.getTypeTable().makeDataTypeNode( DSRDATA_TYPE_ERROR ) );
		smbtable.AddLine( _T("'%1' : redefinition"), name->getLine(), smb->name );
		return;
	}
	if( name->getDataType()->getType() == DSRDATA_TYPE_ERROR ||
		(args && (!args->getDataType() || args->getDataType()->getType() == DSRDATA_TYPE_ERROR)) )
	{
		setDataType( smbtable.getTypeTable().makeDataTypeNode( DSRDATA_TYPE_ERROR ) );
		return;
	}
	if( args )
	{
		if( !args->getDataType() || args->getDataType()->getType() == DSRDATA_TYPE_ERROR )
		{
			setDataType( smbtable.getTypeTable().makeDataTypeNode( DSRDATA_TYPE_ERROR ) );
			return;
		}
		if( keyword->getKeywordType() == DSRDATA_TYPE_FUNCTION )
		{
			name->setDataType( smbtable.getTypeTable().cloneDataTypeNode( args->getDataType() ) );
		}
		else
		{
			if( args->args && args->args->getArgNumber() )
			{
				enum CDsrDataTypes res = ::convert2MaxScalarType( smbtable, args->args );
				if( res == DSRDATA_TYPE_ERROR )
				{
					setDataType( smbtable.getTypeTable().makeDataTypeNode( DSRDATA_TYPE_ERROR ) );
					smbtable.AddLine( _T("'%1' : invalid definition, wrong parameter types"), getLine(), smb->name );
					return;
				}
				if( keyword->getKeywordType() == DSRDATA_TYPE_ABS_GROUP )
				{
					if( res != DSRDATA_TYPE_GROUP_STRING )
					{
						setDataType( smbtable.getTypeTable().makeDataTypeNode( DSRDATA_TYPE_ERROR ) );
						smbtable.AddLine( _T("'%1' : invalid definition, wrong parameter types"), getLine(), smb->name );
						return;
					}
					name->setDataType( smbtable.getTypeTable().makeDataTypeNode( keyword->getKeywordType() ) );
				}
				else
				{
					if( res != DSRDATA_TYPE_INTEGER && 
						res != DSRDATA_TYPE_REAL &&
						res != DSRDATA_TYPE_COMPLEX )
					{
						setDataType( smbtable.getTypeTable().makeDataTypeNode( DSRDATA_TYPE_ERROR ) );
						smbtable.AddLine( _T("'%1' : type is not allowed in the object definition"), getLine(), ::getTypeName( res ) );
						return;
					}
					else if( args->args->getArgNumber() > 1 )
					{
						setDataType( smbtable.getTypeTable().makeDataTypeNode( DSRDATA_TYPE_ERROR ) );
						smbtable.AddLine( _T("'%1' : illegal definition, polynomial and rational functions can have 0 or 1 argument"), getLine(), smb->name );
						return;
					}
					nested_node = smbtable.getTypeTable().makeDataTypeNode( res );
					if( keyword->getKeywordType() == DSRDATA_TYPE_POLYNOM )
					{
						CDsrDataType_Polynomial *n = (CDsrDataType_Polynomial*)smbtable.getTypeTable().makeDataTypeNode( DSRDATA_TYPE_POLYNOM );
						n->n_arg = smbtable.getTypeTable().makeDataTypeNode( res );
						name->setDataType( n );
					}
					else //DSRDATA_TYPE_RATIONAL_FUN
					{
						CDsrDataType_RationalFun *n = (CDsrDataType_RationalFun*)smbtable.getTypeTable().makeDataTypeNode( DSRDATA_TYPE_RATIONAL_FUN );
						n->n_arg = smbtable.getTypeTable().makeDataTypeNode( res );
						name->setDataType( n );
					}
				}
			}
			else
			{
				nested_node = smbtable.getTypeTable().makeDataTypeNode( DSRDATA_TYPE_UNKNOWN );
				name->setDataType( smbtable.getTypeTable().makeDataTypeNode( keyword->getKeywordType() ) );
				name->getDataType()->setNestedType( nested_node );
			}

		}
	}
	else
	{
		n_arg = (CDsrDataType_Composition*)smbtable.getTypeTable().makeDataTypeNode( DSRDATA_TYPE_COMPOSITION );
		n_res = smbtable.getTypeTable().makeDataTypeNode( DSRDATA_TYPE_UNKNOWN );
		n_res->setLValue();
		switch( keyword->getKeywordType() )
		{
		case DSRDATA_TYPE_POLYNOM:
		case DSRDATA_TYPE_RATIONAL_FUN:
		case DSRDATA_TYPE_ABS_GROUP:
			name->setDataType( smbtable.getTypeTable().makeDataTypeNode( keyword->getKeywordType() ) );
			break;
		case DSRDATA_TYPE_FUNCTION:
			name->setDataType( smbtable.getTypeTable().makeDataTypeNode( new CDsrDataType_Function( n_arg, n_res ) ) );
			break;
		default:
			break;
		}
	}

	switch( keyword->getKeywordType() )
	{
	case DSRDATA_TYPE_POLYNOM:
		name->getDataType()->setLValue();
		//smb->param1 = name->isLocalSmbTable() ? FUNCTION_USER_POLYNOMIAL_LOCAL : FUNCTION_USER_POLYNOMIAL;
		smb->param1 = FUNCTION_USER_POLYNOMIAL;
		break;
	case DSRDATA_TYPE_RATIONAL_FUN:
		name->getDataType()->setLValue();
		//smb->param1 = name->isLocalSmbTable() ? FUNCTION_USER_RATIONAL_FUN_LOCAL : FUNCTION_USER_RATIONAL_FUN;
		smb->param1 = FUNCTION_USER_RATIONAL_FUN;
		break;
	case DSRDATA_TYPE_ABS_GROUP:
		name->getDataType()->setLValue();
		break;
	case DSRDATA_TYPE_FUNCTION:
		name->getDataType()->clearLValue();
		smb->param1 = FUNCTION_USER;
		if( !smb->param2 )
			smb->param2 = smbtable.getNewFunNo();
		break;
	default:
		break;
	}
	smb->setDataType( smbtable.getTypeTable().cloneDataTypeNode( name->getDataType() ) );

	setDataType( name->getDataType() );
	getDataType()->copyModifier( name->getDataType() );
}

/////////////////////////////////////////
//	ObjArg			:	Separator, ArgList, Separator, TypeSpec;
/////////////////////////////////////////
void CDsrAST_ObjArg::placeDataType_args( CParseDsrSymbolTable& smbtable, CDsrDataType_Node *type_spec )
{
	long i;
	if( getKeywordType() == DSRDATA_TYPE_ABS_GROUP && type_spec )
	{
		setDataType( smbtable.getTypeTable().makeDataTypeNode( DSRDATA_TYPE_ERROR ) );
		smbtable.AddLine( _T("invalid type definition of the abstract group"), args->getLine() );
	}

	if( args )
	{
		int err = 0, dup_err = 0;
		std::vector<CDsrAST_PrimaryID*> pids;
		std::vector<CDsrDataType_Node*> pid_types;
		CDsrDataType_Node *nested_type;
		if( args->getArgNumber() )
		{
			MArray<long> ids( args->getArgNumber() );
			CDsrAST_CompoundID *cid;
			CDsrAST_PrimaryID *pid;
			for( i = 0; i < args->getArgNumber(); i++ )
			{
				nested_type = 0;
				if( !args->expr[ i ] ) { err = 1; break; } //=== error

				if( args->expr[ i ]->getType() == DsrASTT_Expr )
				{ // whether it is parameter type definition
					args->expr[ i ]->placeDataType( smbtable );
					CDsrAST_Expr *args_expr = (CDsrAST_Expr*)(args->expr[ i ]);
					CDsrAST_ArgList *args_n = 0;
					if( (long) getKeywordType() == (long) DSRDATA_TYPE_ABS_GROUP ||
						!(nested_type = args_expr->getDataType())  ||
						(long) args_expr->getType() == (long) DSRDATA_TYPE_ERROR ||
						args_expr->fun_id != OPERATOR_op_type_spec ||
						(args_n = args_expr->args) == 0 ||
						!args_n->getArgNumber() )
					{ err = 1; break; } //=== error

					if( type_spec && (long) getKeywordType() != (long) DSRDATA_TYPE_FUNCTION 
						&& !nested_type->compare( type_spec ) )
					{
						smbtable.AddLine( _T("illegal definition of arguments, type of parameter(s) and return type are different"), args->getLine() );
						args->setDataType( smbtable.getTypeTable().makeDataTypeNode( DSRDATA_TYPE_ERROR ) );
						return;
					}
					cid = (CDsrAST_CompoundID*)(args_n->expr[ 0 ]);
				}
				else if( args->expr[ i ]->getType() == DsrASTT_CompoundID )
					cid = (CDsrAST_CompoundID*)(args->expr[ i ]);
				else { err = 1; break; } //=== error

				if( !cid->stmt.ArrayDim() || 
					!(pid = cid->stmt[ cid->stmt.ArrayDim() - 1 ]) )
				{
					err = 1;
					break;
				}
				if( pid->id == -1 )
				{
					err = 1;
					break;
				}
				if( ids.Find( pid->id ) != INT_MAX )
				{
					CParseDsrSymbol *smb = pid->getSymbol( smbtable );
					if( !smb )
						err = 1;
					else
					{
						smbtable.AddLine( _T("\'%1\' : redefinition, illegal definition of arguments"), args->getLine(), smb->name );
						dup_err = 1;
					}
					break;
				}
				ids.Add( pid->id );
				pids.push_back( pid );
				pid_types.push_back( nested_type );
			}
		}
		if( err )
		{
			smbtable.AddLine( _T("illegal definition of arguments, list of local parameters is expected"), args->getLine() );
			args->setDataType( smbtable.getTypeTable().makeDataTypeNode( DSRDATA_TYPE_ERROR ) );
		}
		else if( dup_err )
			args->setDataType( smbtable.getTypeTable().makeDataTypeNode( DSRDATA_TYPE_ERROR ) );
		else
		{
			args->placeDataType( smbtable );
			if( args->getDataType()->getType() != DSRDATA_TYPE_COMPOSITION )
			{
				args->setDataType( smbtable.getTypeTable().makeDataTypeNode( DSRDATA_TYPE_ERROR ) );
				return;
			}
			CParseDsrSymbol *smb;
			for( i = 0; i < (long) pids.size(); i++ )
			{
				if( pids[ i ]->getDataType() && 
					pids[ i ]->getDataType()->getType() == DSRDATA_TYPE_UNKNOWN &&
					(smb = pids[ i ]->getSymbol( smbtable ) ) != 0 && 
					smb->getDataType() )
				{
					// calculating default type
					CDsrDataType_Node *default_type;
					if( getKeywordType() == DSRDATA_TYPE_ABS_GROUP )
						default_type = smbtable.getTypeTable().makeDataTypeNode( DSRDATA_TYPE_GROUP_STRING );
					else if( type_spec && getKeywordType() != DSRDATA_TYPE_FUNCTION )
						default_type = smbtable.getTypeTable().cloneDataTypeNode( type_spec );
					else if( pid_types[ i ] )
						default_type = smbtable.getTypeTable().cloneDataTypeNode( pid_types[ i ] );
					else
						default_type = smbtable.makeDefaultDataTypeNode();
					default_type->resetModifier();
					default_type->copyModifier( smb->getDataType() );
					smb->setDataType( default_type );
				}
			}
			// recompute arguments list types and clear all l-value attributes
			// in the arguments list
			args->placeDataType( smbtable );
			CDsrDataType_Composition* args_type = (CDsrDataType_Composition*)(args->getDataType());
			for( i = 0; i < (long) args_type->n_arg.ArrayDim(); i++ )
			{
				args_type->n_arg[ i ] = smbtable.cloneDataTypeNode( args_type->n_arg[ i ] );
				args_type->n_arg[ i ]->clearLValue();
			}
		}
	}
}

void CDsrAST_ObjArg::placeDataType( CParseDsrSymbolTable& smbtable )
{
	if( lb ) lb->placeDataType( smbtable );
	if( rb ) rb->placeDataType( smbtable );

	if( typespec )
	{
		// order of the next 2 stmts is critical,
		// because typespec info is used for args typing
		//
		typespec->placeDataType( smbtable );
		placeDataType_args( smbtable, typespec->getDataType() );
	}
	else
		placeDataType_args( smbtable, 0 );

	if( (typespec && typespec->getDataType() && typespec->getDataType()->getType() == DSRDATA_TYPE_ERROR) ||
		(args && args->getDataType() && args->getDataType()->getType() != DSRDATA_TYPE_COMPOSITION) )
		setDataType( smbtable.getTypeTable().makeDataTypeNode( DSRDATA_TYPE_ERROR ) );
	else
	{
		CDsrDataType_Composition *n_arg = 0;
		CDsrDataType_Node *n_res;
		int err = 0;

		if( args )
		{
			if( args->getDataType() && args->getDataType()->getType() == DSRDATA_TYPE_COMPOSITION )
				n_arg = (CDsrDataType_Composition*)smbtable.getTypeTable().cloneDataTypeNode( args->getDataType() );
			else
				err = 1;
		}
		else
			n_arg = (CDsrDataType_Composition*)smbtable.getTypeTable().makeDataTypeNode( DSRDATA_TYPE_COMPOSITION );
		
		if( typespec )
		{
			if( typespec->getDataType() )
			{
				n_res = smbtable.getTypeTable().cloneDataTypeNode( typespec->getDataType() );
				n_res->setLValue();
			}
			else
				err = 1;
		}
		else
		{
			n_res = smbtable.getTypeTable().makeDataTypeNode( DSRDATA_TYPE_UNKNOWN );
			n_res->setLValue();
		}

		if( err )
			setDataType( smbtable.getTypeTable().makeDataTypeNode( DSRDATA_TYPE_ERROR ) );
		else
			setDataType( smbtable.getTypeTable().makeDataTypeNode( new CDsrDataType_Function( n_arg, n_res ) ) );
	}
}

/////////////////////////////////////////
//	TypeSpec	:	Op, PrimaryID, Separator, ArgList, Separator;
/////////////////////////////////////////
void CDsrAST_TypeSpec::placeDataType( CParseDsrSymbolTable& smbtable )
{
	if( op ) op->placeDataType( smbtable );
	if( name ) name->placeDataType( smbtable );
	if( lb ) lb->placeDataType( smbtable );
	if( args )
	{
		args->placeDataType( smbtable );
	}
	if( rb ) rb->placeDataType( smbtable );

	if( !op || !op->getDataType() || op->getDataType()->getType() == DSRDATA_TYPE_ERROR ||
		!name || !name->getDataType() || name->getDataType()->getType() == DSRDATA_TYPE_ERROR ||
		(args && args->getDataType() && args->getDataType()->getType() == DSRDATA_TYPE_ERROR) )
	{
		setDataType( smbtable.getTypeTable().makeDataTypeNode( DSRDATA_TYPE_ERROR ) );
		return;
	}
	CParseDsrSymbol *smb = name->getSymbol( smbtable );
	if( !smb )
	{
		setDataType( smbtable.getTypeTable().makeDataTypeNode( DSRDATA_TYPE_ERROR ) );
		smbtable.AddLine( _T("unknown type name"), getLine() );
		return;
	}
	CDsrDataType_Node *node;
	if( !smb->getDataType() || 
		smb->getDataType()->getType() != DSRDATA_TYPE_ABS_SET ||
		!(node = smb->getDataType()->getNestedType()) )
	{
		setDataType( smbtable.getTypeTable().makeDataTypeNode( DSRDATA_TYPE_ERROR ) );
		smbtable.AddLine( _T("'%1' : unknown type name"), getLine(), smb->name );
		return;
	}
	if( args && args->getDataType() )
	{
		long i;
		CDsrAST_ArgList*& _idxs = args;
		MArray<long> dim( 2, 0, 2 );
		dim.Add( i = -1 );
		dim.Add( i = -1 );
		if( _idxs->getArgNumber() == 0 || 
			args->getDataType()->isDynamic() )
		{
			setDataType( smbtable.getTypeTable().makeDataTypeNode( DSRDATA_TYPE_ERROR ) );
			smbtable.AddLine( _T("illegal vector type definition, non-empty list of integer constants is expected"), getLine() );
			return;
		}
		else if( _idxs->getArgNumber() > 2 )
		{
			setDataType( smbtable.getTypeTable().makeDataTypeNode( DSRDATA_TYPE_ERROR ) );
			smbtable.AddLine( _T("illegal vector type definition, dimensions list is too big"), getLine() );
			return;
		}
		for( i = 0; i < _idxs->getArgNumber(); i++ )
		{
			if( !_idxs->expr[ i ] || 
				!_idxs->expr[ i ]->getDataType() ||
				_idxs->expr[ i ]->getType() != DsrASTT_ConstInt )
			{
				setDataType( smbtable.getTypeTable().makeDataTypeNode( DSRDATA_TYPE_ERROR ) );
				smbtable.AddLine( _T("illegal vector type definition, non-empty list of integer constants is expected"), getLine() );
				return;
			}
			dim[ i ] = ((CDsrAST_ConstInt*)(_idxs->expr[ i ]))->value;
		}
		CDsrDataType_Vector *retv = new CDsrDataType_Vector( node );
		retv->setRows( dim[ 0 ] );
		retv->setColumns( dim[ 1 ] );
		setDataType( smbtable.getTypeTable().makeDataTypeNode( retv ) );
	}
	else
		setDataType( smbtable.getTypeTable().cloneDataTypeNode( node ) );
}

///////////////////////////////////////////////////////////////////////////////
