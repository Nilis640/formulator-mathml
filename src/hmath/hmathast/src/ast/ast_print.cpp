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

#include "HUtils/iustring.h"
#include "astdsr.h"

///////////////////////////////////////////////////////////////////////////////

void CDsrAST_Node::debugPrint( QString& dest, CParseDsrSymbolTable * /*smbtable*/, long call_level )
{
	print2blank( dest, call_level );
	dest += getCDsrASTNodeName( this ) + (getDataType() ? (QString( _T(" -> (") ) + CDsrDataTypeNames[ getDataType()->getType() ] + QString( _T(")") )) : QString( _T(" -> (reserved-none)") ) ) + QString( _T(" \n") );
	//if( before_char ) before_char->debugPrint( dest, smbtable, call_level + 1 );
	//if( after_char ) after_char->debugPrint( dest, smbtable, call_level + 1 );
}

void CDsrAST_StmtList::debugPrint( QString& dest, CParseDsrSymbolTable *smbtable, long call_level )
{
	print2blank( dest, call_level );
	dest += getCDsrASTNodeName( this ) + (getDataType() ? (QString( _T(" -> (") ) + CDsrDataTypeNames[ getDataType()->getType() ] + QString( _T(")") )) : QString( _T(" -> (reserved-none)") ) ) + QString( _T(" \n") );
	//if( before_char ) before_char->debugPrint( dest, smbtable, call_level + 1 );
	for( long i = 0; i < (long) stmt.ArrayDim(); i++ )
		if( stmt[ i ] ) stmt[ i ]->debugPrint( dest, smbtable, call_level + 1 );
	//if( after_char ) after_char->debugPrint( dest, smbtable, call_level + 1 );
}

void CDsrAST_Compound::debugPrint( QString& dest, CParseDsrSymbolTable *smbtable, long call_level )
{
	print2blank( dest, call_level );
	dest += getCDsrASTNodeName( this ) + (getDataType() ? (QString( _T(" -> (") ) + CDsrDataTypeNames[ getDataType()->getType() ] + QString( _T(")") )) : QString( _T(" -> (reserved-none)") ) ) + QString( _T(" \n") );
	//if( before_char ) before_char->debugPrint( dest, smbtable, call_level + 1 );
	if( k_begin ) k_begin->debugPrint( dest, smbtable, call_level + 1 );
	if( stmt ) stmt->debugPrint( dest, smbtable, call_level + 1 );
	if( k_end ) k_end->debugPrint( dest, smbtable, call_level + 1 );
	//if( after_char ) after_char->debugPrint( dest, smbtable, call_level + 1 );
}

void CDsrAST_ArgList::debugPrint( QString& dest, CParseDsrSymbolTable *smbtable, long call_level )
{
	print2blank( dest, call_level );
	dest += getCDsrASTNodeName( this ) + (getDataType() ? (QString( _T(" -> (") ) + CDsrDataTypeNames[ getDataType()->getType() ] + QString( _T(")") )) : QString( _T(" -> (reserved-none)") ) ) + QString( _T(" \n") );
	//if( before_char ) before_char->debugPrint( dest, smbtable, call_level + 1 );
	for( long i = 0; i < (long) expr.ArrayDim(); i++ )
		if( expr[ i ] ) expr[ i ]->debugPrint( dest, smbtable, call_level + 1 );
	if( sep ) sep->debugPrint( dest, smbtable, call_level + 1 );
	//if( after_char ) after_char->debugPrint( dest, smbtable, call_level + 1 );
}

void CDsrAST_NullStmt::debugPrint( QString& dest, CParseDsrSymbolTable *smbtable, long call_level )
{
	print2blank( dest, call_level );
	dest += getCDsrASTNodeName( this ) + (getDataType() ? (QString( _T(" -> (") ) + CDsrDataTypeNames[ getDataType()->getType() ] + QString( _T(")") )) : QString( _T(" -> (reserved-none)") ) ) + QString( _T(" \n") );
	//if( before_char ) before_char->debugPrint( dest, smbtable, call_level + 1 );
	if( sep ) sep->debugPrint( dest, smbtable, call_level + 1 );
	//if( after_char ) after_char->debugPrint( dest, smbtable, call_level + 1 );
}

void CDsrAST_If::debugPrint( QString& dest, CParseDsrSymbolTable *smbtable, long call_level )
{
	print2blank( dest, call_level );
	dest += getCDsrASTNodeName( this ) + (getDataType() ? (QString( _T(" -> (") ) + CDsrDataTypeNames[ getDataType()->getType() ] + QString( _T(")") )) : QString( _T(" -> (reserved-none)") ) ) + QString( _T(" \n") );
	//if( before_char ) before_char->debugPrint( dest, smbtable, call_level + 1 );
	if( k_if ) k_if->debugPrint( dest, smbtable, call_level + 1 );
	if( n_expr ) n_expr->debugPrint( dest, smbtable, call_level + 1 );
	if( k_then ) k_then->debugPrint( dest, smbtable, call_level + 1 );
	if( n_then ) n_then->debugPrint( dest, smbtable, call_level + 1 );
	if( k_else ) k_else->debugPrint( dest, smbtable, call_level + 1 );
	if( n_else ) n_else->debugPrint( dest, smbtable, call_level + 1 );
	//if( after_char ) after_char->debugPrint( dest, smbtable, call_level + 1 );
}

void CDsrAST_ConstInt::debugPrint( QString& dest, CParseDsrSymbolTable * /*smbtable*/, long call_level )
{
	print2blank( dest, call_level );
	dest += getCDsrASTNodeName( this ) + (getDataType() ? (QString( _T(" -> (") ) + CDsrDataTypeNames[ getDataType()->getType() ] + QString( _T(")") )) : QString( _T(" -> (reserved-none)") ) ) + QString( _T(" \n") );
	//if( before_char ) before_char->debugPrint( dest, smbtable, call_level + 1 );
	print2blank( dest, call_level + 1 );
	dest += aspect + QString( _T(" \n") );
	//if( after_char ) after_char->debugPrint( dest, smbtable, call_level + 1 );
}

void CDsrAST_ConstReal::debugPrint( QString& dest, CParseDsrSymbolTable * /*smbtable*/, long call_level )
{
	print2blank( dest, call_level );
	dest += getCDsrASTNodeName( this ) + (getDataType() ? (QString( _T(" -> (") ) + CDsrDataTypeNames[ getDataType()->getType() ] + QString( _T(")") )) : QString( _T(" -> (reserved-none)") ) ) + QString( _T(" \n") );
	//if( before_char ) before_char->debugPrint( dest, smbtable, call_level + 1 );
	print2blank( dest, call_level + 1 );
	dest += aspect + QString( _T(" \n") );
	//if( after_char ) after_char->debugPrint( dest, smbtable, call_level + 1 );
}

void CDsrAST_ConstString::debugPrint( QString& dest, CParseDsrSymbolTable * /*smbtable*/, long call_level )
{
	print2blank( dest, call_level );
	dest += getCDsrASTNodeName( this ) + (getDataType() ? (QString( _T(" -> (") ) + CDsrDataTypeNames[ getDataType()->getType() ] + QString( _T(")") )) : QString( _T(" -> (reserved-none)") ) ) + QString( _T(" \n") );
	//if( before_char ) before_char->debugPrint( dest, smbtable, call_level + 1 );
	print2blank( dest, call_level + 1 );
	dest += QString( _T("value='") ) + hstr_escape_blank(value) + QString( _T("'\n") );
	//if( after_char ) after_char->debugPrint( dest, smbtable, call_level + 1 );
}

void CDsrAST_ConstStringList::debugPrint( QString& dest, CParseDsrSymbolTable *smbtable, long call_level )
{
	print2blank( dest, call_level );
	dest += getCDsrASTNodeName( this ) + (getDataType() ? (QString( _T(" -> (") ) + CDsrDataTypeNames[ getDataType()->getType() ] + QString( _T(")") )) : QString( _T(" -> (reserved-none)") ) ) + QString( _T(" \n") );
	//if( before_char ) before_char->debugPrint( dest, smbtable, call_level + 1 );
	for( long i = 0; i < (long) stmt.ArrayDim(); i++ )
		if( stmt[ i ] ) stmt[ i ]->debugPrint( dest, smbtable, call_level + 1 );
	//if( after_char ) after_char->debugPrint( dest, smbtable, call_level + 1 );
}

void CDsrAST_Expr::debugPrint( QString& dest, CParseDsrSymbolTable *smbtable, long call_level )
{
	print2blank( dest, call_level );
	dest += getCDsrASTNodeName( this );
	if( data_type )
	{
		dest += QString( _T(", data_type = ") ) + data_type->getTypeName();
		if( data_type->isXValue() )
			dest += QString( _T(", x-value") );
		if( data_type->isReturns() )
			dest += QString( _T(", returns") );
		if( data_type->isDynamic() )
			dest += QString( _T(", dynamic") );
		dest += QString( _T(", call_id = '") ) + ::ml_Op2String( fun_id ) + QString( _T("'") );
	}
	dest += QString( _T(" \n") );
	//if( before_char ) before_char->debugPrint( dest, smbtable, call_level + 1 );
	if( op ) op->debugPrint( dest, smbtable, call_level + 1 );
	if( args ) args->debugPrint( dest, smbtable, call_level + 1 );
	/*if( arg1 ) arg1->debugPrint( dest, smbtable, call_level + 1 );
	if( arg2 ) arg2->debugPrint( dest, smbtable, call_level + 1 );*/
	//if( after_char ) after_char->debugPrint( dest, smbtable, call_level + 1 );
}

void CDsrAST_Function::debugPrint( QString& dest, CParseDsrSymbolTable *smbtable, long call_level )
{
	QString buffer;
	print2blank( dest, call_level );
	dest += getCDsrASTNodeName( this ) + (getDataType() ? (QString( _T(" -> (") ) + getDataType()->getTypeName() + QString( _T(")") )) : QString( _T(" -> (reserved-none)") ) );
	buffer = QString("%1").arg( fun_id_param );
	dest += QString( _T(" -> ") ) + ::ml_Op2String( fun_id ) + QString( _T("(") ) + QString(buffer) + QString( _T(")") );
	dest += QString( _T(" \n") );
	//if( before_char ) before_char->debugPrint( dest, smbtable, call_level + 1 );
	if( name ) name->debugPrint( dest, smbtable, call_level + 1 );
	if( lb ) lb->debugPrint( dest, smbtable, call_level + 1 );
	if( args ) args->debugPrint( dest, smbtable, call_level + 1 );
	if( rb ) rb->debugPrint( dest, smbtable, call_level + 1 );
	//if( after_char ) after_char->debugPrint( dest, smbtable, call_level + 1 );
}

void CDsrAST_IdxListItem::debugPrint( QString& dest, CParseDsrSymbolTable *smbtable, long call_level )
{
	print2blank( dest, call_level );
	dest += getCDsrASTNodeName( this ) + (getDataType() ? (QString( _T(" -> (") ) + CDsrDataTypeNames[ getDataType()->getType() ] + QString( _T(")") )) : QString( _T(" -> (reserved-none)") ) ) + QString( _T(" \n") );
	//if( before_char ) before_char->debugPrint( dest, smbtable, call_level + 1 );
	if( lb ) lb->debugPrint( dest, smbtable, call_level + 1 );
	if( expr ) expr->debugPrint( dest, smbtable, call_level + 1 );
	if( rb ) rb->debugPrint( dest, smbtable, call_level + 1 );
	//if( after_char ) after_char->debugPrint( dest, smbtable, call_level + 1 );
}

void CDsrAST_CompoundID::debugPrint( QString& dest, CParseDsrSymbolTable *smbtable, long call_level )
{
	print2blank( dest, call_level );
	dest += getCDsrASTNodeName( this ) + (getDataType() ? (QString( _T(" -> (") ) + CDsrDataTypeNames[ getDataType()->getType() ] + QString( _T(")") )) : QString( _T(" -> (reserved-none)") ) ) + QString( _T(" \n") );
	//if( before_char ) before_char->debugPrint( dest, smbtable, call_level + 1 );
	for( long i = 0; i < (long) stmt.ArrayDim(); i++ )
		if( stmt[ i ] ) stmt[ i ]->debugPrint( dest, smbtable, call_level + 1 );
	if( sep ) sep->debugPrint( dest, smbtable, call_level + 1 );
	//if( after_char ) after_char->debugPrint( dest, smbtable, call_level + 1 );
}

void CDsrAST_PrimaryID::debugPrint( QString& dest, CParseDsrSymbolTable *smbtable, long call_level )
{
	print2blank( dest, call_level );
	dest += getCDsrASTNodeName( this ) + (getDataType() ? (QString( _T(" -> (") ) + CDsrDataTypeNames[ getDataType()->getType() ] + QString( _T(")") )) : QString( _T(" -> (reserved-none)") ) ) + QString( _T(" \n") );
	//if( before_char ) before_char->debugPrint( dest, smbtable, call_level + 1 );
	if( spec ) spec->debugPrint( dest, smbtable, call_level + 1 );
	if( id != -1 )
	{
		print2blank( dest, call_level + 1 );
		if( smbtable )
		{
			CParseDsrSymbol *smb = getSymbol( *smbtable );
			if( smb )
			{
				smb->debugPrint( dest, smbtable );
				switch( context )
				{
				case CompoundID_CONTEXT__SINGLE:
					dest += QString( _T(", context = single") );
					break;
				case CompoundID_CONTEXT__TYPESPEC:
					dest += QString( _T(", context = typespec") );
					break;
				case CompoundID_CONTEXT__FUNCTION:
					dest += QString( _T(", context = function call") );
					break;
				}
				dest.append( _T('\n') );
			}
			else
				dest += QString("id=%1\n").arg(id);
		}
		else
			dest += QString("id=%1\n").arg(id);
	}
	//if( after_char ) after_char->debugPrint( dest, smbtable, call_level + 1 );
}

void CDsrAST_Special::debugPrint( QString& dest, CParseDsrSymbolTable *smbtable, long call_level )
{
	print2blank( dest, call_level );
	dest += getCDsrASTNodeName( this ) + (getDataType() ? (QString( _T(" -> (") ) + CDsrDataTypeNames[ getDataType()->getType() ] + QString( _T(")") )) : QString( _T(" -> (reserved-none)") ) ) + QString( _T(" \n") );
	//if( before_char ) before_char->debugPrint( dest, smbtable, call_level + 1 );
	if( name ) name->debugPrint( dest, smbtable, call_level + 1 );
	if( lb ) lb->debugPrint( dest, smbtable, call_level + 1 );
	if( args ) args->debugPrint( dest, smbtable, call_level + 1 );
	if( rb ) rb->debugPrint( dest, smbtable, call_level + 1 );
	//if( after_char ) after_char->debugPrint( dest, smbtable, call_level + 1 );
}

void CDsrAST_TypeSpec::debugPrint( QString& dest, CParseDsrSymbolTable *smbtable, long call_level )
{
	print2blank( dest, call_level );
	dest += getCDsrASTNodeName( this ) + (getDataType() ? (QString( _T(" -> (") ) + CDsrDataTypeNames[ getDataType()->getType() ] + QString( _T(")") )) : QString( _T(" -> (reserved-none)") ) ) + QString( _T(" \n") );
	//if( before_char ) before_char->debugPrint( dest, smbtable, call_level + 1 );
	if( op ) op->debugPrint( dest, smbtable, call_level + 1 );
	if( name ) name->debugPrint( dest, smbtable, call_level + 1 );
	if( lb ) lb->debugPrint( dest, smbtable, call_level + 1 );
	if( args ) args->debugPrint( dest, smbtable, call_level + 1 );
	if( rb ) rb->debugPrint( dest, smbtable, call_level + 1 );
	//if( after_char ) after_char->debugPrint( dest, smbtable, call_level + 1 );
}

void CDsrAST_ObjDef_Assign::debugPrint( QString& dest, CParseDsrSymbolTable *smbtable, long call_level )
{
	print2blank( dest, call_level );
	dest += getCDsrASTNodeName( this ) + (getDataType() ? (QString( _T(" -> (") ) + CDsrDataTypeNames[ getDataType()->getType() ] + QString( _T(")") )) : QString( _T(" -> (reserved-none)") ) ) + QString( _T(" \n") );
	//if( before_char ) before_char->debugPrint( dest, smbtable, call_level + 1 );
	if( head ) head->debugPrint( dest, smbtable, call_level + 1 );
	if( op ) op->debugPrint( dest, smbtable, call_level + 1 );
	if( expr ) expr->debugPrint( dest, smbtable, call_level + 1 );
	if( sep ) sep->debugPrint( dest, smbtable, call_level + 1 );
	//if( after_char ) after_char->debugPrint( dest, smbtable, call_level + 1 );
}

void CDsrAST_ObjDef_Compound::debugPrint( QString& dest, CParseDsrSymbolTable *smbtable, long call_level )
{
	print2blank( dest, call_level );
	dest += getCDsrASTNodeName( this ) + (getDataType() ? (QString( _T(" -> (") ) + CDsrDataTypeNames[ getDataType()->getType() ] + QString( _T(")") )) : QString( _T(" -> (reserved-none)") ) ) + QString( _T(" \n") );
	//if( before_char ) before_char->debugPrint( dest, smbtable, call_level + 1 );
	if( head ) head->debugPrint( dest, smbtable, call_level + 1 );
	if( compound ) compound->debugPrint( dest, smbtable, call_level + 1 );
	//if( after_char ) after_char->debugPrint( dest, smbtable, call_level + 1 );
}

void CDsrAST_ObjHead::debugPrint( QString& dest, CParseDsrSymbolTable *smbtable, long call_level )
{
	print2blank( dest, call_level );
	dest += getCDsrASTNodeName( this ) + (getDataType() ? (QString( _T(" -> (") ) + CDsrDataTypeNames[ getDataType()->getType() ] + QString( _T(")") )) : QString( _T(" -> (reserved-none)") ) ) + QString( _T(" \n") );
	//if( before_char ) before_char->debugPrint( dest, smbtable, call_level + 1 );
	if( keyword ) keyword->debugPrint( dest, smbtable, call_level + 1 );
	if( name ) name->debugPrint( dest, smbtable, call_level + 1 );
	if( args ) args->debugPrint( dest, smbtable, call_level + 1 );
	//if( after_char ) after_char->debugPrint( dest, smbtable, call_level + 1 );
}
	
void CDsrAST_ObjArg::debugPrint( QString& dest, CParseDsrSymbolTable *smbtable, long call_level )
{
	print2blank( dest, call_level );
	dest += getCDsrASTNodeName( this ) + (getDataType() ? (QString( _T(" -> (") ) + CDsrDataTypeNames[ getDataType()->getType() ] + QString( _T(")") )) : QString( _T(" -> (reserved-none)") ) ) + QString( _T(" \n") );
	//if( before_char ) before_char->debugPrint( dest, smbtable, call_level + 1 );
	if( lb ) lb->debugPrint( dest, smbtable, call_level + 1 );
	if( args ) args->debugPrint( dest, smbtable, call_level + 1 );
	if( rb ) rb->debugPrint( dest, smbtable, call_level + 1 );
	if( typespec ) typespec->debugPrint( dest, smbtable, call_level + 1 );
	//if( after_char ) after_char->debugPrint( dest, smbtable, call_level + 1 );
}

void CDsrAST_Keyword::debugPrint( QString& dest, CParseDsrSymbolTable *smbtable, long call_level )
{
	print2blank( dest, call_level );
	dest += getCDsrASTNodeName( this ) + (getDataType() ? (QString( _T(" -> (") ) + CDsrDataTypeNames[ getDataType()->getType() ] + QString( _T(")") )) : QString( _T(" -> (reserved-none)") ) ) + QString( _T(" \n") );
	//if( before_char ) before_char->debugPrint( dest, smbtable, call_level + 1 );
	print2blank( dest, call_level + 1 );
	CParseDsrSymbol *smb = 0;
	if( smbtable ) smb = smbtable->getSymbol( id );
	if( smbtable && smb )
	{
		smb->debugPrint( dest, smbtable );
		dest.append( _T('\n') );
	}
	else
		dest += QString(_T("id=%1\n")).arg(id);
	//if( after_char ) after_char->debugPrint( dest, smbtable, call_level + 1 );
}

void CDsrAST_Op::debugPrint( QString& dest, CParseDsrSymbolTable * /*smbtable*/, long call_level )
{
	print2blank( dest, call_level );
	dest += getCDsrASTNodeName( this ) + (getDataType() ? (QString( _T(" -> (") ) + CDsrDataTypeNames[ getDataType()->getType() ] + QString( _T(")") )) : QString( _T(" -> (reserved-none)") ) ) + QString( _T(" \n") );
	//if( before_char ) before_char->debugPrint( dest, smbtable, call_level + 1 );
	print2blank( dest, call_level + 1 );
	dest += QString( _T("id='") ) + ml_Op2String( id ) + QString( _T("'\n") );
	//if( after_char ) after_char->debugPrint( dest, smbtable, call_level + 1 );
}
	
void CDsrAST_Separator::debugPrint( QString& dest, CParseDsrSymbolTable * /*smbtable*/, long call_level )
{
	print2blank( dest, call_level );
	dest += getCDsrASTNodeName( this ) + (getDataType() ? (QString( _T(" -> (") ) + CDsrDataTypeNames[ getDataType()->getType() ] + QString( _T(")") )) : QString( _T(" -> (reserved-none)") ) ) + QString( _T(" \n") );
	//if( before_char ) before_char->debugPrint( dest, smbtable, call_level + 1 );
	print2blank( dest, call_level + 1 );
	dest += QString("separator=%1\n").arg(id);
	//if( after_char ) after_char->debugPrint( dest, smbtable, call_level + 1 );
}
	
void CDsrAST_Remark::debugPrint( QString& dest, CParseDsrSymbolTable * /*smbtable*/, long call_level )
{
	print2blank( dest, call_level );
	dest += getCDsrASTNodeName( this ) + (getDataType() ? (QString( _T(" -> (") ) + CDsrDataTypeNames[ getDataType()->getType() ] + QString( _T(")") )) : QString( _T(" -> (reserved-none)") ) ) + QString( _T(" \n") );
	//if( before_char ) before_char->debugPrint( dest, smbtable, call_level + 1 );
	print2blank( dest, call_level + 1 );
	dest += QString( _T("value='") ) + hstr_escape_blank(value) + QString( _T("'\n") );
	//if( after_char ) after_char->debugPrint( dest, smbtable, call_level + 1 );
}
	
void CDsrAST_Blank::debugPrint( QString& dest, CParseDsrSymbolTable * /*smbtable*/, long call_level )
{
	print2blank( dest, call_level );
	dest += getCDsrASTNodeName( this ) + (getDataType() ? (QString( _T(" -> (") ) + CDsrDataTypeNames[ getDataType()->getType() ] + QString( _T(")") )) : QString( _T(" -> (reserved-none)") ) ) + QString( _T(" \n") );
	//if( before_char ) before_char->debugPrint( dest, smbtable, call_level + 1 );
	print2blank( dest, call_level + 1 );
	dest += QString( _T("value='") ) + hstr_escape_blank(value) + QString( _T("'\n") );
	//if( after_char ) after_char->debugPrint( dest, smbtable, call_level + 1 );
}

void CDsrAST_0d0a::debugPrint( QString& dest, CParseDsrSymbolTable * /*smbtable*/, long call_level )
{
	print2blank( dest, call_level );
	dest += getCDsrASTNodeName( this ) + (getDataType() ? (QString( _T(" -> (") ) + CDsrDataTypeNames[ getDataType()->getType() ] + QString( _T(")") )) : QString( _T(" -> (reserved-none)") ) ) + QString( _T(" \n") );
	//if( before_char ) before_char->debugPrint( dest, smbtable, call_level + 1 );
	print2blank( dest, call_level + 1 );
	dest += QString( _T("value='") ) + hstr_escape_blank(value) + QString( _T("'\n") );
	//if( after_char ) after_char->debugPrint( dest, smbtable, call_level + 1 );
}
	
void CDsrAST_unknown::debugPrint( QString& dest, CParseDsrSymbolTable * /*smbtable*/, long call_level )
{
	print2blank( dest, call_level );
	dest += getCDsrASTNodeName( this ) + (getDataType() ? (QString( _T(" -> (") ) + CDsrDataTypeNames[ getDataType()->getType() ] + QString( _T(")") )) : QString( _T(" -> (reserved-none)") ) ) + QString( _T(" \n") );
	//if( before_char ) before_char->debugPrint( dest, smbtable, call_level + 1 );
	print2blank( dest, call_level + 1 );
	dest += QString( _T("value='") ) + hstr_escape_blank(value) + QString( _T("'\n") );
	//if( after_char ) after_char->debugPrint( dest, smbtable, call_level + 1 );
}
