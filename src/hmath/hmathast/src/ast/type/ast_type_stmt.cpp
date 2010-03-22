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
// 	Node		:	;
/////////////////////////////////////////
void CDsrAST_Node::placeDataType( CParseDsrSymbolTable& smbtable )
{
	setDataType( smbtable.getTypeTable().makeDataTypeNode( DSRDATA_TYPE_NONE ) );
	getDataType()->setLinearEQ();
}

void CDsrAST_Node::convertArgVar2Polynomial( CParseDsrSymbolTable& /*smbtable*/, long /*max_num_arg*/ )
{
}

void CDsrAST_Node::setAddrScheme4LValue( CParseDsrSymbolTable& /*smbtable*/ )
{
}

/////////////////////////////////////////
//	StmtList	:	Node*;
/////////////////////////////////////////
void CDsrAST_StmtList::placeDataType( CParseDsrSymbolTable& smbtable, bool ignoreSemanticError )
{
	long i;
	int err = 0;
	int is_XValue = 0, is_Dynamic = 0, is_Returns = 0, is_LinearEQ = 1;
	for( i = 0; i < (long) stmt.ArrayDim(); i++ )
	{
		if( stmt[ i ] )
		{
			smbtable.FlushUsedXVar();
			smbtable.FlushUsedDXVar();

			stmt[ i ]->placeDataType( smbtable );
			if( !stmt[ i ]->getDataType() ||
				stmt[ i ]->getDataType()->getType() == DSRDATA_TYPE_ERROR )
				err = 1;
			else
			{
				is_XValue  = is_XValue  || stmt[ i ]->getDataType()->isXValue();
				is_Dynamic = is_Dynamic || stmt[ i ]->getDataType()->isDynamic();
				is_Returns = is_Returns || stmt[ i ]->getDataType()->isReturns();
				is_LinearEQ = is_LinearEQ && stmt[ i ]->getDataType()->isLinearEQ();
			}
		}
	}
	if( err && ignoreSemanticError )
	{
		for( i = 0; i < (long) stmt.ArrayDim(); )
		{
			if( stmt[ i ] && (!stmt[ i ]->getDataType() || stmt[ i ]->getDataType()->getType() == DSRDATA_TYPE_ERROR) )
			{
				delete stmt[ i ];
				stmt[ i ] = 0;
				stmt.erase( stmt.begin() + i );
			}
			else
				i++;
		}
		if( stmt.ArrayDim() > 0 )
			err = 0;
	}
	if( err )
		setDataType( smbtable.getTypeTable().makeDataTypeNode( DSRDATA_TYPE_ERROR ) );
	else
	{
		for( i = stmt.ArrayDim() - 1; i >= 0; i-- )
		{
			if( stmt[ i ] )
			{
				setDataType( smbtable.cloneDataTypeNode( stmt[ i ]->getDataType() ) );
				if( is_XValue )
					getDataType()->setXValue();
				if( is_Returns )
					getDataType()->setReturns();
				if( is_Dynamic )
					getDataType()->setDynamic();
				if( is_LinearEQ )
					getDataType()->setLinearEQ();
				break;
			}
		}
	}
}

void CDsrAST_StmtList::convertArgVar2Polynomial( CParseDsrSymbolTable& smbtable, long max_num_arg )
{
	for( long i = 0; i < (long) stmt.ArrayDim(); i++ )
		if( stmt[ i ] )
			stmt[ i ]->convertArgVar2Polynomial( smbtable, max_num_arg );
}

/////////////////////////////////////////
//	Compound	:	Keyword, StmtList, Keyword;
/////////////////////////////////////////
void CDsrAST_Compound::placeDataType( CParseDsrSymbolTable& smbtable )
{
	if( k_begin ) k_begin->placeDataType( smbtable );
	if( stmt ) stmt->placeDataType( smbtable );
	if( k_end ) k_end->placeDataType( smbtable );

	if( stmt )
	{
		setDataType( smbtable.cloneDataTypeNode( stmt->getDataType() ) );
		if( stmt->getDataType()->isXValue() )
			getDataType()->setXValue();
		if( stmt->getDataType()->isReturns() )
			getDataType()->setReturns();
		if( stmt->getDataType()->isDynamic() )
			getDataType()->setDynamic();
		if( stmt->getDataType()->isLinearEQ() )
			getDataType()->setLinearEQ();
	}
}

void CDsrAST_Compound::convertArgVar2Polynomial( CParseDsrSymbolTable& smbtable, long max_num_arg )
{
	if( stmt )
		stmt->convertArgVar2Polynomial( smbtable, max_num_arg );
}

/////////////////////////////////////////
//	NullStmt	:	Separator;
/////////////////////////////////////////
void CDsrAST_NullStmt::placeDataType( CParseDsrSymbolTable& smbtable )
{
	if( sep ) sep->placeDataType( smbtable );
	setDataType( smbtable.getTypeTable().makeDataTypeNode( DSRDATA_TYPE_NONE ) );
	getDataType()->setLinearEQ();
}

/////////////////////////////////////////
//	If			:	Keyword, Expr, Keyword, Node, Keyword, Node;
/////////////////////////////////////////
void CDsrAST_If::placeDataType( CParseDsrSymbolTable& smbtable )
{
	int is_XValue = 0, is_Returns = 0, is_Returns1 = 0, is_Returns2 = 0, is_Dynamic = 0;
	int is_L = 1, is_L1 = 1, is_L2 = 1;

	if( k_if ) k_if->placeDataType( smbtable );
	if( n_expr )
	{
		n_expr->placeDataType( smbtable );
		is_XValue  = is_XValue || n_expr->getDataType()->isXValue();
		is_Dynamic = is_Dynamic || n_expr->getDataType()->isDynamic();
		is_Returns = is_Returns || n_expr->getDataType()->isReturns();
		is_L = is_L && n_expr->getDataType()->isLinearEQ();
	}

	if( k_then ) k_then->placeDataType( smbtable );
	if( n_then )
	{
		n_then->placeDataType( smbtable );
		is_XValue   = is_XValue   || n_then->getDataType()->isXValue();
		is_Dynamic  = is_Dynamic  || n_then->getDataType()->isDynamic();
		is_Returns1 = is_Returns1 || n_then->getDataType()->isReturns();
		is_L1       = is_L1       && n_then->getDataType()->isLinearEQ();
	}

	if( k_else ) k_else->placeDataType( smbtable );
	if( n_else )
	{
		n_else->placeDataType( smbtable );
		is_XValue   = is_XValue   || n_else->getDataType()->isXValue();
		is_Dynamic  = is_Dynamic  || n_else->getDataType()->isDynamic();
		is_Returns2 = is_Returns2 || n_else->getDataType()->isReturns();
		is_L2       = is_L2       && n_else->getDataType()->isLinearEQ();
	}

	if( (n_expr && n_expr->getDataType() && n_expr->getDataType()->getType() == DSRDATA_TYPE_ERROR) ||
		(n_then && n_then->getDataType() && n_then->getDataType()->getType() == DSRDATA_TYPE_ERROR) ||
		(n_else && n_else->getDataType() && n_else->getDataType()->getType() == DSRDATA_TYPE_ERROR) )
		setDataType( smbtable.getTypeTable().makeDataTypeNode( DSRDATA_TYPE_ERROR ) );
	else if( !n_expr || !n_expr->getDataType() || n_expr->getDataType()->getType() != DSRDATA_TYPE_BOOL )
	{
		setDataType( smbtable.getTypeTable().makeDataTypeNode( DSRDATA_TYPE_ERROR ) );
		smbtable.AddLine( _T("\'if\' : condition has to be of type \'bool\'"), k_if ? k_if->getLine() : getLine() );
	}
	else
	{
		if( is_XValue )
		{
			smbtable.AddLine( _T("\'if\' : statement cannot depends on a unknown independent variable"), k_if ? k_if->getLine() : getLine() );
			setDataType( smbtable.getTypeTable().makeDataTypeNode( DSRDATA_TYPE_ERROR ) );
			return;
		}
		setDataType( smbtable.getTypeTable().makeDataTypeNode( DSRDATA_TYPE_NONE ) );
		if( is_Dynamic )
			getDataType()->setDynamic();
		if( is_Returns || (is_Returns1 && is_Returns2) )
			getDataType()->setReturns();
		if( is_L && is_L1 && is_L2 )
			getDataType()->setLinearEQ();
	}
}

void CDsrAST_If::convertArgVar2Polynomial( CParseDsrSymbolTable& smbtable, long max_num_arg )
{
	if( n_expr )
		n_expr->convertArgVar2Polynomial( smbtable, max_num_arg );
	if( n_then )
		n_then->convertArgVar2Polynomial( smbtable, max_num_arg );
	if( n_else )
		n_else->convertArgVar2Polynomial( smbtable, max_num_arg );
}

///////////////////////////////////////////////////////////////////////////////
