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
//	ConstInt	:	long;
/////////////////////////////////////////
void CDsrAST_ConstInt::placeDataType( CParseDsrSymbolTable& smbtable )
{
	// <0 => integer, else -> natural !!! this rule is used for specific cases (e.g., identity) and must be stable
	setDataType( smbtable.getTypeTable().makeDataTypeNode( value <= 0 ? DSRDATA_TYPE_INTEGER : DSRDATA_TYPE_NATURAL ) );
	run_time_stack_overhead = 1;
	getDataType()->setLinearEQ();
}

/////////////////////////////////////////
//	ConstReal	:	double;
/////////////////////////////////////////
void CDsrAST_ConstReal::placeDataType( CParseDsrSymbolTable& smbtable )
{
	setDataType( smbtable.getTypeTable().makeDataTypeNode( DSRDATA_TYPE_REAL ) );
	run_time_stack_overhead = 1;
	getDataType()->setLinearEQ();
}

/////////////////////////////////////////
//	ConstString	:	QString;
/////////////////////////////////////////
void CDsrAST_ConstString::placeDataType( CParseDsrSymbolTable& smbtable )
{
	setDataType( smbtable.getTypeTable().makeDataTypeNode( DSRDATA_TYPE_STRING ) );
	run_time_stack_overhead = 1;
	getDataType()->setLinearEQ();
}

/////////////////////////////////////////
//	CDsrAST_ConstStringList	:	CDsrAST_ConstString*;
/////////////////////////////////////////
void CDsrAST_ConstStringList::placeDataType( CParseDsrSymbolTable& smbtable )
{
	for( long i = 0; i < (long) stmt.ArrayDim(); i++ )
		if( stmt[ i ] ) stmt[ i ]->placeDataType( smbtable );
	setDataType( smbtable.getTypeTable().makeDataTypeNode( DSRDATA_TYPE_STRING ) );
	run_time_stack_overhead = 1;
	getDataType()->setLinearEQ();
}

/////////////////////////////////////////
//	IdxListItem	:	Separator, ArgList, Separator;
/////////////////////////////////////////
void CDsrAST_IdxListItem::placeDataType( CParseDsrSymbolTable& smbtable )
{
	if( lb ) lb->placeDataType( smbtable );
	if( expr ) expr->placeDataType( smbtable );
	if( rb ) rb->placeDataType( smbtable );

	if( expr )
	{
		setDataType( smbtable.cloneDataTypeNode( expr->getDataType() ) );
		if( expr->getDataType()->isXValue() )
			getDataType()->setXValue();
		if( expr->getDataType()->isLinearEQ() )
			getDataType()->setLinearEQ();
		if( expr->getDataType()->isReturns() )
			getDataType()->setReturns();
		if( expr->getDataType()->isDynamic() )
			getDataType()->setDynamic();
	}
}

/////////////////////////////////////////
//	CompoundID	:	PrimaryID*, Separator;
/////////////////////////////////////////
void CDsrAST_CompoundID::placeDataType( CParseDsrSymbolTable& smbtable )
{
	long i;
	int err = 0;

	for( i = 0; i < (long) stmt.ArrayDim(); i++ )
	{
		if( stmt[ i ] )
		{
			stmt[ i ]->placeDataType( smbtable );
			if( stmt[ i ]->getDataType() &&
				stmt[ i ]->getDataType()->getType() == DSRDATA_TYPE_ERROR )
				err = 1;
		}
	}

	if( err )
		setDataType( smbtable.getTypeTable().makeDataTypeNode( DSRDATA_TYPE_ERROR ) );
	else if( !stmt.ArrayDim() ||
			 !stmt[ stmt.ArrayDim() - 1 ] || 
			 !stmt[ stmt.ArrayDim() - 1 ]->getDataType() )	// var end-name is mandatory
	{
		setDataType( smbtable.getTypeTable().makeDataTypeNode( DSRDATA_TYPE_ERROR ) );
		smbtable.AddLine( _T("internal error: variable name is missing"), getLine() );
	}
	else
	{
		setDataType( stmt[ stmt.ArrayDim() - 1 ]->getDataType() );
		run_time_stack_overhead = stmt[ stmt.ArrayDim() - 1 ]->getRTStackOverhead();
		if( stmt[ stmt.ArrayDim() - 1 ]->getDataType()->isXValue() )
			getDataType()->setXValue();
		if( stmt[ stmt.ArrayDim() - 1 ]->getDataType()->isLinearEQ() )
			getDataType()->setLinearEQ();
	}
}

void CDsrAST_CompoundID::setAddrScheme4LValue( CParseDsrSymbolTable& smbtable )
{
	if( stmt.ArrayDim() && stmt[ stmt.ArrayDim() - 1 ] )
		stmt[ stmt.ArrayDim() - 1 ]->setAddrScheme4LValue( smbtable );
}

/////////////////////////////////////////
//	PrimaryID	:	long, Special;
/////////////////////////////////////////
void CDsrAST_PrimaryID::placeDataType( CParseDsrSymbolTable& _smbtable )
{
	CParseDsrSymbolTable& smbtable = local_smbtable ? *local_smbtable : _smbtable;
	CParseDsrSymbol *smb = 0;
	if( id != -1 )
	{
		smb = getSymbol( smbtable );
		if( smb  )
		{
			if( !smb->getDataType() )
			{
				smb->setDataType( _smbtable.getTypeTable().makeDataTypeNode( DSRDATA_TYPE_UNKNOWN ) );
				if( smb->isVariable() )
					smb->getDataType()->setLValue();
				if( local_smbtable )
					smb->getDataType()->setLocal();
			}
			setDataType( smb->getDataType() );
			run_time_stack_overhead = 1;
			getDataType()->copyModifier( smb->getDataType() );
		}
		else
		{
			setDataType( _smbtable.getTypeTable().makeDataTypeNode( DSRDATA_TYPE_ERROR ) );
			_smbtable.AddLine( _T("internal error: symbol table record is missing"), getLine() );
		}
	}
	else if( spec )
	{
		spec->placeDataType( _smbtable );
		if( !spec->getDataType() || spec->getDataType()->getType() == DSRDATA_TYPE_ERROR )
			setDataType( _smbtable.getTypeTable().makeDataTypeNode( DSRDATA_TYPE_ERROR ) );
		else
		{
			setDataType( _smbtable.getTypeTable().cloneDataTypeNode( spec->getDataType() ) );
			run_time_stack_overhead = spec->getRTStackOverhead();
		}
	}
	if( getDataType() )
	{
		getDataType()->setLinearEQ();
		if( id != -1 && getDataType()->isXValue() )
			smbtable.AddUsedXVar( id );
	}
}

void CDsrAST_PrimaryID::setAddrScheme4LValue( CParseDsrSymbolTable& smbtable )
{
	CParseDsrSymbol* smb = getSymbol( smbtable );
	if( smb && smb->smb_record_type == SMBTABLE_TYPE_VARIABLE )
		is_push_by_address = 1;
	else if( spec )
		spec->setAddrScheme4LValue( smbtable );
}

/////////////////////////////////////////
//	Keyword		:	long;
/////////////////////////////////////////
void CDsrAST_Keyword::placeDataType( CParseDsrSymbolTable& smbtable )
{
	setDataType( smbtable.getTypeTable().makeDataTypeNode( DSRDATA_TYPE_NONE ) );
	if( getDataType() ) getDataType()->setLinearEQ();
}

/////////////////////////////////////////
//	Op			:	long;
/////////////////////////////////////////
void CDsrAST_Op::placeDataType( CParseDsrSymbolTable& smbtable )
{
	if( id != -1 )
	{
		QString opname = ::ml_Op2String( id );
		long *pid = smbtable.is_find( opname );
		if( pid )
		{
			CParseDsrSymbol *smb = smbtable.getSymbol( *pid );
			if( smb && smb->getDataType() )
			{
				setDataType( /*smbtable.cloneDataTypeNode( */smb->getDataType()/* ) */);//lev15032004
				return;
			}
		}
		smbtable.AddLine( _T("\'%1\' : unknown operator"), getLine(), opname );
	}
	setDataType( smbtable.getTypeTable().makeDataTypeNode( DSRDATA_TYPE_UNKNOWN ) );
	if( getDataType() ) getDataType()->setLinearEQ();
}

/////////////////////////////////////////
//	Separator	:	long;
/////////////////////////////////////////
void CDsrAST_Separator::placeDataType( CParseDsrSymbolTable& smbtable )
{
	setDataType( smbtable.getTypeTable().makeDataTypeNode( DSRDATA_TYPE_NONE ) );
	if( getDataType() ) getDataType()->setLinearEQ();
}

/////////////////////////////////////////
//	Remark		:	QString;
/////////////////////////////////////////
void CDsrAST_Remark::placeDataType( CParseDsrSymbolTable& smbtable )
{
	setDataType( smbtable.getTypeTable().makeDataTypeNode( DSRDATA_TYPE_NONE ) );
	if( getDataType() ) getDataType()->setLinearEQ();
}

/////////////////////////////////////////
//	Blank		:	QString;
/////////////////////////////////////////
void CDsrAST_Blank::placeDataType( CParseDsrSymbolTable& smbtable )
{
	setDataType( smbtable.getTypeTable().makeDataTypeNode( DSRDATA_TYPE_NONE ) );
	if( getDataType() ) getDataType()->setLinearEQ();
}

/////////////////////////////////////////
//	0d0a		:	;
/////////////////////////////////////////
void CDsrAST_0d0a::placeDataType( CParseDsrSymbolTable& smbtable )
{
	setDataType( smbtable.getTypeTable().makeDataTypeNode( DSRDATA_TYPE_NONE ) );
	if( getDataType() ) getDataType()->setLinearEQ();
}

/////////////////////////////////////////
//	unknown		:	QString;
/////////////////////////////////////////
void CDsrAST_unknown::placeDataType( CParseDsrSymbolTable& smbtable )
{
	setDataType( smbtable.getTypeTable().makeDataTypeNode( DSRDATA_TYPE_NONE ) );
	if( getDataType() ) getDataType()->setLinearEQ();
}

///////////////////////////////////////////////////////////////////////////////
