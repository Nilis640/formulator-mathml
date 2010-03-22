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
void CDsrAST_ArgList::convertArgVar2Polynomial( CParseDsrSymbolTable& smbtable, long max_num_arg )
{
	for( long i = 0; i < (long) expr.ArrayDim(); i++ )
	{
		if( !expr[ i ] ) continue;
		if( expr[ i ]->getType() == DsrASTT_CompoundID )
		{
			CDsrAST_CompoundID *cvar = (CDsrAST_CompoundID*)expr[ i ];
			if( cvar->stmt.ArrayDim() && 
				cvar->stmt[ cvar->stmt.ArrayDim() - 1 ] &&
				cvar->stmt[ cvar->stmt.ArrayDim() - 1 ]->getType() == DsrASTT_PrimaryID )
			{
				CDsrAST_PrimaryID *x = (CDsrAST_PrimaryID*)cvar->stmt[ cvar->stmt.ArrayDim() - 1 ];
				if( x && x->local_smbtable && 
					x->id != -1 && x->id < max_num_arg &&
					x->context != CompoundID_CONTEXT__TYPESPEC &&
					x->context != CompoundID_CONTEXT__FUNCTION )
				{
					cvar->placeDataType( smbtable );
					if( x->getDataType() && x->getDataType()->isScalar() )
					{
						enum CDsrFunctionListID res_x = FUNCTION_STD_NONE;
						switch( x->getDataType()->getType() )
						{
						case DSRDATA_TYPE_INTEGER:
							res_x = CONST_polynom_x_integer;
							break;
						case DSRDATA_TYPE_REAL:
							res_x = CONST_polynom_x_real;
							break;
						case DSRDATA_TYPE_COMPLEX:
							res_x = CONST_polynom_x_complex;
							break;
						default:
							break;
						}
						long *xsmb_id = smbtable.is_find( ::ml_Op2String( res_x ) );
						if( xsmb_id )
						{
							CParseDsrSymbol *xsmb_old = x->getSymbol( smbtable );
							CParseDsrSymbol *xsmb_new = smbtable.getSymbol( *xsmb_id );
							if( xsmb_old && xsmb_new )
							{
								xsmb_old->param1 = xsmb_new->param1;
								xsmb_old->param2 = xsmb_new->param2;
								xsmb_old->param3 = xsmb_new->param3;
								xsmb_old->data_type = xsmb_new->data_type;
								cvar->placeDataType( smbtable );
							}
						}
					}
				}
			}
		}
		else
			expr[ i ]->convertArgVar2Polynomial( smbtable, max_num_arg );
	}
}

void CDsrAST_ArgList::convertArgTypes( const __CDSRTypeConversionOpVector& opc, CParseDsrSymbolTable& smbtable )
{
	int ret = 0, res;
	for( long i = 0; i < (long) expr.ArrayDim() && i < (long) opc.ArrayDim(); i++ )
	{
		res = convertArgTypes( smbtable, i, opc[ i ] );
		ret = ret || res;
	}
	if( ret )
		placeDataType( smbtable );
}

long CDsrAST_ArgList::convertArgTypes( 
	CParseDsrSymbolTable& smbtable, long idx, enum CDsrFunctionListID opc )
{
	int ret = 0;
	CDsrAST_Expr *n_expr;
	if( expr[ idx ] && opc != FUNCTION_STD_NONE )
	{
		CDsrDataType_Node *tmp_node = expr[ idx ]->getDataType() ? 
			smbtable.getTypeTable().cloneDataTypeNode( expr[ idx ]->getDataType() ) :
			0;
		n_expr = new CDsrAST_Expr( new CDsrAST_Op( opc ), expr[ idx ] );
		n_expr->setLine( expr[ idx ]->getLine() );
		expr[ idx ] = n_expr;
		expr[ idx ]->placeDataType( smbtable );
		if( !expr[ idx ]->getDataType() &&
			expr[ idx ]->getDataType()->getType() == DSRDATA_TYPE_ERROR )
		{
			smbtable.AddLine( _T("internal error: wrong format of type record for the conversion operator" ), getLine() );
			setDataType( smbtable.getTypeTable().makeDataTypeNode( DSRDATA_TYPE_ERROR ) );
		}
		if( getDataType() && 
			getDataType()->getType() == DSRDATA_TYPE_COMPOSITION && 
			((long) ((CDsrDataType_Composition*)getDataType())->n_arg.ArrayDim()) > idx )
		{
			((CDsrDataType_Composition*)getDataType())->n_arg[ idx ] = expr[ idx ]->getDataType();
		}
		else
			ret = 1;
		if( tmp_node )
			expr[ idx ]->getDataType()->copyModifier( tmp_node );
	}
	return ret;
}

void CDsrAST_ArgList::placeDataType( CParseDsrSymbolTable& smbtable )
{
	long i;
	int err = 0;

	run_time_stack_overhead = 0;
	for( i = 0; i < (long) expr.ArrayDim(); i++ )
	{
		if( expr[ i ] )
		{
			expr[ i ]->placeDataType( smbtable );
			if( expr[ i ]->getDataType() &&
				expr[ i ]->getDataType()->getType() == DSRDATA_TYPE_ERROR )
				err = 1;
			run_time_stack_overhead += expr[ i ]->getRTStackOverhead();
		}
	}
	if( sep ) sep->placeDataType( smbtable );
	if( err )
		setDataType( smbtable.getTypeTable().makeDataTypeNode( DSRDATA_TYPE_ERROR ) );
	else
	{
		CDsrDataType_Composition *args = (CDsrDataType_Composition*)smbtable.getTypeTable().makeDataTypeNode( DSRDATA_TYPE_COMPOSITION );
		int is_XValue = 0, is_Returns = 0, is_Dynamic = 0, is_LinearEQ = 1;
		for( i = 0; i < (long) expr.ArrayDim(); i++ )
		{
			if( expr[ i ] && expr[ i ]->getDataType() )
			{
				args->Add( expr[ i ]->getDataType() );
				is_XValue  = is_XValue  || expr[ i ]->getDataType()->isXValue();
				is_Returns = is_Returns || expr[ i ]->getDataType()->isReturns();
				is_Dynamic = is_Dynamic || expr[ i ]->getDataType()->isDynamic();
				is_LinearEQ = is_LinearEQ && expr[ i ]->getDataType()->isLinearEQ();
			}
			else
				args->Add( smbtable.getTypeTable().makeDataTypeNode( DSRDATA_TYPE_NONE ) );
		}
		if( is_XValue )
			args->setXValue();
		if( is_Returns )
			args->setReturns();
		if( is_Dynamic )
			args->setDynamic();
		if( is_LinearEQ )
			args->setLinearEQ();
		setDataType( args );
	}
}

///////////////////////////////////////////////////////////////////////////////
