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

#include "astdsr.h"
#include "../vm/vmidsr.h"

QString DsrAST_Types_Name[ DsrASTT_Last ] =
{
	_T("None"),

	_T("StmtList"),
	_T("Compound"),
	_T("ArgList"),
	_T("NullStmt"),

	_T("If"),

	_T("ConstInt"),
	_T("ConstReal"),
	_T("ConstString"),
	_T("ConstStringList"),

	_T("Expr"),
	_T("Function"),
	_T("IdxListItem"),
	_T("CompoundID"),
	_T("PrimaryID"),
	_T("Special"),

	_T("TypeSpec"),

	_T("ObjDef_Assign"),
	_T("ObjDef_Compound"),
	_T("ObjHead"),
	_T("ObjArg"),

	_T("Keyword"),
	_T("Op"),
	_T("Separator"),

	_T("Remark"),
	_T("Blank"),
	_T("0d0a"),
	_T("unknown")
};

QString getCDsrASTNodeName( CDsrAST_Node *node )
{
	enum DsrAST_Types _type = node->getType();
	if( _type > DsrASTT_None && _type < DsrASTT_Last )
		return DsrAST_Types_Name[ _type ];
	return DsrAST_Types_Name[ DsrASTT_None ];
}

/////////////////////////////////////////
// 	Node		:	;
/////////////////////////////////////////
void CDsrAST_Node::getInstruction( VM_Dsr& vm )
{
	getBeforeInstruction( vm );	// heading

	vm.Nop( getLine(), getLevel() );

	getAfterInstruction( vm );	// tailing
}

void CDsrAST_Node::getBeforeInstruction( VM_Dsr& vm )
{
	if( before_char ) before_char->getInstruction( vm );
}

void CDsrAST_Node::getAfterInstruction( VM_Dsr& vm )
{
	if( after_char ) after_char->getInstruction( vm );
}

/////////////////////////////////////////
//	StmtList	:	Node*;
/////////////////////////////////////////

CDsrAST_StmtList::CDsrAST_StmtList( void ) : 
	stmt(MIArray<CDsrAST_Node>::LeaveItems, 8, 0, 8)
{
	type = DsrASTT_StmtList;
}

CDsrAST_StmtList::~CDsrAST_StmtList( void )
{
	for( long i = 0; i < (long) stmt.ArrayDim(); i++ )
	{
		CDsrAST_Node *ptr = stmt[ i ];
		if( ptr ) delete ptr;
	}
}

void CDsrAST_StmtList::Join( CDsrAST_StmtList *listNode )
{
	for( long i = 0; i < (long) listNode->stmt.ArrayDim(); i++ )
	{
		if( listNode->stmt[ i ] )
			stmt.Add( listNode->stmt[ i ] );
	}
	listNode->stmt.clear();
}

void CDsrAST_StmtList::setObjectContextThroughTree( long o_id )
{
	for( long i = 0; i < (long) stmt.ArrayDim(); i++ )
		if( stmt[ i ] ) stmt[ i ]->setObjectContextThroughTree( o_id );
}

void CDsrAST_StmtList::setLevelThroughTree( long lv )
{
	setLevel( lv );
	for( long i = 0; i < (long) stmt.ArrayDim(); i++ )
		if( stmt[ i ] ) stmt[ i ]->setLevelThroughTree( lv );
}

void CDsrAST_StmtList::getInstruction( VM_Dsr& vm )
{
	getBeforeInstruction( vm );	// heading

	for( long i = 0; i < (long) stmt.ArrayDim(); i++ )
		if( stmt[ i ] ) stmt[ i ]->getInstruction( vm );

	getAfterInstruction( vm );	// tailing
}

/////////////////////////////////////////
//	Compound	:	Keyword, StmtList, Keyword;
/////////////////////////////////////////
void CDsrAST_Compound::getInstruction( VM_Dsr& vm )
{
	getBeforeInstruction( vm );	// heading

	if( k_begin ) k_begin->getInstruction( vm );
	if( stmt ) stmt->getInstruction( vm );
	if( k_end ) k_end->getInstruction( vm );

	getAfterInstruction( vm );	// tailing
}

/////////////////////////////////////////
//	ArgList		:	Expr*, Separator;
/////////////////////////////////////////
CDsrAST_ArgList::~CDsrAST_ArgList( void )
{
	if( is_recursive_delete )
	{
		for( long i = 0; i < (long) expr.ArrayDim(); i++ )
		{
			CDsrAST_Node *ptr = expr[ i ];
			if( ptr )
			{
				delete ptr;
				ptr = 0;
			}
		}
	}
	if( sep ) delete sep;
}

void CDsrAST_ArgList::setObjectContextThroughTree( long o_id )
{
	for( long i = 0; i < (long) expr.ArrayDim(); i++ )
		if( expr[ i ] ) expr[ i ]->setObjectContextThroughTree( o_id );
}

void CDsrAST_ArgList::setLevelThroughTree( long lv )
{
	setLevel( lv );
	if( sep ) sep->setLevel( lv );
	for( long i = 0; i < (long) expr.ArrayDim(); i++ )
		if( expr[ i ] ) expr[ i ]->setLevelThroughTree( lv );
}

void CDsrAST_ArgList::getInstruction( VM_Dsr& vm )
{
	getBeforeInstruction( vm );	// heading

	for( long i = 0; i < (long) expr.ArrayDim() - 1; i++ )
	{
		if( expr[ i ] ) expr[ i ]->getInstruction( vm );
		if( getSeparator() ) getSeparator()->getInstruction( vm );
	}
	if( expr[ expr.ArrayDim() - 1 ] ) 
		expr[ expr.ArrayDim() - 1 ]->getInstruction( vm );

	getAfterInstruction( vm );	// tailing
}

void CDsrAST_ArgList::Add( MIArray<CDsrAST_Node>& new_expr )
{
	for( long i = 0; i < (long) new_expr.ArrayDim(); i++ )
		Add( new_expr[ i ] );
}


/////////////////////////////////////////
//	NullStmt	:	Separator;
/////////////////////////////////////////
void CDsrAST_NullStmt::getInstruction( VM_Dsr& vm )
{
	getBeforeInstruction( vm );	// heading

	if( sep ) sep->getInstruction( vm );

	getAfterInstruction( vm );	// tailing
}

/////////////////////////////////////////
//	If			:	Keyword, Expr, Keyword, Node, Keyword, Node;
/////////////////////////////////////////
void CDsrAST_If::getInstruction( VM_Dsr& vm )
{
	getBeforeInstruction( vm );	// heading

	if( k_if ) k_if->getInstruction( vm );
	if( n_expr ) n_expr->getInstruction( vm );
	if( k_then ) k_then->getInstruction( vm );
	if( n_then ) n_then->getInstruction( vm );
	if( k_else ) k_else->getInstruction( vm );
	if( n_else ) n_else->getInstruction( vm );

	getAfterInstruction( vm );	// tailing
}

/////////////////////////////////////////
//	ConstInt	:	long;
/////////////////////////////////////////
void CDsrAST_ConstInt::getInstruction( VM_Dsr& vm )
{
	getBeforeInstruction( vm );	// heading

	vm.ConstInt( value, getLine(), getLevel() );

	getAfterInstruction( vm );	// tailing
}

/////////////////////////////////////////
//	ConstReal	:	double;
/////////////////////////////////////////
void CDsrAST_ConstReal::getInstruction( VM_Dsr& vm )
{
	getBeforeInstruction( vm );	// heading

	vm.ConstReal( value, aspect, getLine(), getLevel() );

	getAfterInstruction( vm );	// tailing
}

/////////////////////////////////////////
//	ConstString	:	QString;
/////////////////////////////////////////
void CDsrAST_ConstString::getInstruction( VM_Dsr& vm )
{
	getBeforeInstruction( vm );	// heading

	vm.ConstString( value, getLine(), getLevel() );

	getAfterInstruction( vm );	// tailing
}

/////////////////////////////////////////
//	CDsrAST_ConstStringList	:	CDsrAST_ConstString*;
/////////////////////////////////////////
CDsrAST_ConstStringList::~CDsrAST_ConstStringList( void )
{
	for( int i = 0; i < (long) stmt.ArrayDim(); i++ )
	{
		CDsrAST_ConstString *ptr = stmt[ i ];
		if( ptr ) delete ptr;
	}
}

void CDsrAST_ConstStringList::setLevelThroughTree( long lv )
{
	setLevel( lv );
	for( long i = 0; i < (long) stmt.ArrayDim(); i++ )
		if( stmt[ i ] ) stmt[ i ]->setLevelThroughTree( lv );
}

void CDsrAST_ConstStringList::getInstruction( VM_Dsr& vm )
{
	getBeforeInstruction( vm );	// heading

	for( long i = 0; i < (long) stmt.ArrayDim(); i++ )
		if( stmt[ i ] ) stmt[ i ]->getInstruction( vm );

	getAfterInstruction( vm );	// tailing
}

/////////////////////////////////////////
//	Expr		:	Node, Op, Node;
/////////////////////////////////////////
void CDsrAST_Expr::getInstruction( VM_Dsr& vm )
{
	getBeforeInstruction( vm );	// heading

	switch( op->id )
	{
	case OPERATOR_op_type_spec:
		if( args ) args->getInstruction( vm );
		break;
	case OPERATOR_round_brackets:
//		vm.LeftRoundBracket( getLine(), getLevel() );
		if( args ) args->getInstruction( vm );
//		vm.RightRoundBracket( getLine(), getLevel() );
		break;
	case OPERATOR_square_brackets:
		if( args )
		{
			if( args->expr[ 0 ] && args->expr[ 1 ] && op )
			{
				args->expr[ 0 ]->getInstruction( vm );
				//vm.LeftSquareBracket( args->getLine(), args->getLevel() );
				args->expr[ 1 ]->getInstruction( vm );
				//vm.RightSquareBracket( args->getLine(), args->getLevel() );
			}
		}
//		if( args ) args->getInstruction( vm );
/*
		if( args )
		{
			if( args->expr[ 0 ] ) args->expr[ 0 ]->getInstruction( vm );
			if( args->expr[ 1 ] )
			{
				if( args->expr[ 1 ]->getType() != DsrASTT_ArgList ||
					args->expr[ 1 ]->getDataType()->getType() != DSRDATA_TYPE_COMPOSITION )
					args->expr[ 1 ]->getInstruction( vm );
				else
				{
					CDsrAST_ArgList *_args = (CDsrAST_ArgList*)args->expr[ 1 ];
					for( long i = 0; i < _args->getArgNumber(); i++ )
						if( _args->expr[ i ] )
						{
							vm.LeftSquareBracket( _args->expr[ i ]->getLine(), _args->expr[ i ]->getLevel() );
							_args->expr[ i ]->getInstruction( vm );
							vm.RightSquareBracket( _args->expr[ i ]->getLine(), _args->expr[ i ]->getLevel() );
						}
				}
			}
		}
*/
		break;
	case OPERATOR_curly_brackets:
		if( args ) args->getInstruction( vm );
		break;
	default:
		if( args )
		{
			if( args->expr[ 0 ] && args->expr[ 1 ] && op )
			{
				args->expr[ 0 ]->getInstruction( vm );
				op->getInstruction( vm );
				args->expr[ 1 ]->getInstruction( vm );
			}
			else if( args->expr[ 0 ] && op )
			{
				op->getInstruction( vm );
				args->expr[ 0 ]->getInstruction( vm );
			}
		}
	}

	getAfterInstruction( vm );	// tailing
}

/////////////////////////////////////////
//	Function	:	CompoundID, Separator, ArgList, Separator;
/////////////////////////////////////////
void CDsrAST_Function::getInstruction( VM_Dsr& vm )
{
	getBeforeInstruction( vm );	// heading

	if( name ) name->getInstruction( vm );
	if( lb ) lb->getInstruction( vm );
	if( args ) args->getInstruction( vm );
	if( rb ) rb->getInstruction( vm );

	getAfterInstruction( vm );	// tailing
}

/////////////////////////////////////////
//	IdxListItem	:	Separator, ArgList, Separator;
/////////////////////////////////////////
CDsrAST_IdxListItem::~CDsrAST_IdxListItem( void )
{
	if( lb ) delete lb;
	if( expr ) delete expr;
	if( rb ) delete rb;
}

void CDsrAST_IdxListItem::setObjectContextThroughTree( long o_id )
{
	if( expr ) expr->setObjectContextThroughTree( o_id );
}

void CDsrAST_IdxListItem::setLevelThroughTree( long lv )
{
	setLevel( lv );
	if( lb ) lb->setLevel( lv );
	if( expr ) expr->setLevel( lv );
	if( rb ) rb->setLevel( lv );
}

void CDsrAST_IdxListItem::getInstruction( VM_Dsr& vm )
{
	getBeforeInstruction( vm );	// heading

	if( lb ) lb->getInstruction( vm );
	if( expr ) expr->getInstruction( vm );
	if( rb ) rb->getInstruction( vm );

	getAfterInstruction( vm );	// tailing
}

/////////////////////////////////////////
//	CompoundID	:	PrimaryID*, Separator;
/////////////////////////////////////////
CDsrAST_CompoundID::~CDsrAST_CompoundID( void )
{
	for( long i = 0; i < (long) stmt.ArrayDim(); i++ )
	{
		CDsrAST_PrimaryID *ptr = stmt[ i ];
		if( ptr ) delete ptr;
	}
	if( sep ) delete sep;
}

void CDsrAST_CompoundID::setObjectContextThroughTree( long o_id )
{
	for( long i = 0; i < (long) stmt.ArrayDim(); i++ )
		if( stmt[ i ] ) stmt[ i ]->setObjectContextThroughTree( o_id );
}

void CDsrAST_CompoundID::setLevelThroughTree( long lv )
{
	setLevel( lv );
	if( sep ) sep->setLevel( lv );
	for( long i = 0; i < (long) stmt.ArrayDim(); i++ )
		if( stmt[ i ] ) stmt[ i ]->setLevelThroughTree( lv );
}

void CDsrAST_CompoundID::getInstruction( VM_Dsr& vm )
{
	getBeforeInstruction( vm );	// heading

	if( stmt.ArrayDim() )
	{
		for( long i = 0; i < (long) stmt.ArrayDim() - 1; i++ )
		{
			if( stmt[ i ] ) stmt[ i ]->getInstruction( vm );
			if( getSeparator() ) getSeparator()->getInstruction( vm );
		}
		if( stmt[ stmt.ArrayDim() - 1 ] ) 
			stmt[ stmt.ArrayDim() - 1 ]->getInstruction( vm );
	}
	getAfterInstruction( vm );	// tailing
}

void CDsrAST_CompoundID::setContext( long ct )
{
	for( long i = 0; i < (long) stmt.ArrayDim(); i++ )
		if( stmt[ i ] ) stmt[ i ]->context = ct;
}

/////////////////////////////////////////
//	PrimaryID	:	long, Special;
/////////////////////////////////////////
void CDsrAST_PrimaryID::getInstruction( VM_Dsr& vm )
{
	getBeforeInstruction( vm );	// heading

	if( id != -1 )
	{
		if( local_smbtable )
			vm.SymbolTableItem( local_smbtable->getSymbol( id ), getLine(), getLevel() );
		else
			vm.SymbolTableItem( id, getLine(), getLevel() );
	}
	else if( spec ) spec->getInstruction( vm );

	getAfterInstruction( vm );	// tailing
}

/////////////////////////////////////////
//	Special		:	long, Separator, ArgList, Separator;
/////////////////////////////////////////
void CDsrAST_Special::getInstruction( VM_Dsr& vm )
{
	getBeforeInstruction( vm );	// heading

	if( name ) name->getInstruction( vm );
	if( lb ) lb->getInstruction( vm );
	if( args ) args->getInstruction( vm );
	if( rb ) rb->getInstruction( vm );

	getAfterInstruction( vm );	// tailing
}

/////////////////////////////////////////
//	TypeSpec	:	Op, PrimaryID, Separator, ArgList, Separator;
/////////////////////////////////////////
void CDsrAST_TypeSpec::getInstruction( VM_Dsr& vm )
{
	getBeforeInstruction( vm );	// heading

	if( op ) op->getInstruction( vm );
	if( name ) name->getInstruction( vm );
	if( lb ) lb->getInstruction( vm );
	if( args ) args->getInstruction( vm );
	if( rb ) rb->getInstruction( vm );

	getAfterInstruction( vm );	// tailing
}

/////////////////////////////////////////
//	ObjDef_Assign	:	ObjHead, Op, Expr, Separator;
/////////////////////////////////////////
void CDsrAST_ObjDef_Assign::getInstruction( VM_Dsr& vm )
{
	getBeforeInstruction( vm );	// heading

	if( head ) head->getInstruction( vm );
	if( op ) op->getInstruction( vm );
	if( expr ) expr->getInstruction( vm );
	if( sep ) sep->getInstruction( vm );

	getAfterInstruction( vm );	// tailing
}

/////////////////////////////////////////
//	ObjDef_Compound	:	ObjHead, Compound;
/////////////////////////////////////////
void CDsrAST_ObjDef_Compound::getInstruction( VM_Dsr& vm )
{
	getBeforeInstruction( vm );	// heading

	if( head ) head->getInstruction( vm );
	if( compound ) compound->getInstruction( vm );

	getAfterInstruction( vm );	// tailing
}

/////////////////////////////////////////
//	ObjHead			:	Keyword, PrimaryID, ObjArg;
/////////////////////////////////////////
void CDsrAST_ObjHead::getInstruction( VM_Dsr& vm )
{
	getBeforeInstruction( vm );	// heading

	if( keyword ) keyword->getInstruction( vm );
	if( name ) name->getInstruction( vm );
	if( args ) args->getInstruction( vm );

	getAfterInstruction( vm );	// tailing
}

/////////////////////////////////////////
//	ObjArg			:	Separator, ArgList, Separator, TypeSpec;
/////////////////////////////////////////
void CDsrAST_ObjArg::getInstruction( VM_Dsr& vm )
{
	getBeforeInstruction( vm );	// heading

	if( lb ) lb->getInstruction( vm );
	if( args ) args->getInstruction( vm );
	if( rb ) rb->getInstruction( vm );
	if( typespec ) typespec->getInstruction( vm );

	getAfterInstruction( vm );	// tailing
}

/////////////////////////////////////////
//	Keyword		:	long;
/////////////////////////////////////////
void CDsrAST_Keyword::getInstruction( VM_Dsr& vm )
{
	getBeforeInstruction( vm );	// heading

	if( id != -1 ) vm.SymbolTableItem( id, getLine(), getLevel() );

	getAfterInstruction( vm );	// tailing
}

/////////////////////////////////////////
//	Op			:	long;
/////////////////////////////////////////
void CDsrAST_Op::getInstruction( VM_Dsr& vm )
{
	getBeforeInstruction( vm );	// heading

	if( id != -1 ) vm.Op( id, getLine(), getLevel() );

	getAfterInstruction( vm );	// tailing
}

/////////////////////////////////////////
//	Separator	:	long;
/////////////////////////////////////////
void CDsrAST_Separator::getInstruction( VM_Dsr& vm )
{
	getBeforeInstruction( vm );	// heading

	if( id != -1 ) vm.SeparatorGrammarIndex( id, getLine(), getLevel() );

	getAfterInstruction( vm );	// tailing
}

/////////////////////////////////////////
//	Remark		:	QString;
/////////////////////////////////////////
void CDsrAST_Remark::getInstruction( VM_Dsr& vm )
{
	getBeforeInstruction( vm );	// heading

	vm.Remark( value, getLine(), getLevel() );

	getAfterInstruction( vm );	// tailing
}

/////////////////////////////////////////
//	Blank		:	QString;
/////////////////////////////////////////
void CDsrAST_Blank::getInstruction( VM_Dsr& vm )
{
	getBeforeInstruction( vm );	// heading

	vm.Blank( value, getLine(), getLevel() );

	getAfterInstruction( vm );	// tailing
}

/////////////////////////////////////////
//	0d0a		:	;
/////////////////////////////////////////
void CDsrAST_0d0a::getInstruction( VM_Dsr& vm )
{
	getBeforeInstruction( vm );	// heading

	vm.EOL( value, getLine(), getLevel() );

	getAfterInstruction( vm );	// tailing
}

/////////////////////////////////////////
//	unknown		:	QString;
/////////////////////////////////////////
void CDsrAST_unknown::getInstruction( VM_Dsr& vm )
{
	getBeforeInstruction( vm );	// heading

	vm.Text( value, getLine(), getLevel() );

	getAfterInstruction( vm );	// tailing
}

///////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////
// Implementation
/////////////////////////////////////////

/////////////////////////////////////////
// 	Node		:	;
/////////////////////////////////////////

CDsrAST_Node::CDsrAST_Node( void )
{
	type = DsrASTT_None;
	line = 0;
	level = 0;
	setDataType( 0 );
	before_char = 0;
	after_char = 0;
	run_time_stack_overhead = 0;
}


CDsrAST_Node::~CDsrAST_Node( void )
{
	if( before_char ) delete before_char;
	if( after_char )  delete after_char;
}


void CDsrAST_Node::setObjectContextThroughTree( long /*o_id*/ )
{}


void CDsrAST_Node::AddBefore( CDsrAST_Node* n )
{
	if( !before_char ) 
		before_char = new CDsrAST_StmtList;
	before_char->Add( n );
}


void CDsrAST_Node::AddAfter( CDsrAST_Node* n )
{
	if( !after_char ) 
		after_char = new CDsrAST_StmtList;
	after_char->Add( n );
}


enum DsrAST_Types CDsrAST_Node::getType( void )
{
	return type;
}


void CDsrAST_Node::setLine( long ln )
{
	line = ln;
}


long CDsrAST_Node::getLine( void )
{
	return line;
}


void CDsrAST_Node::setLevel( long lv )
{
	level = lv;
}


long CDsrAST_Node::getLevel( void )
{
	return level;
}


void CDsrAST_Node::setLevelThroughTree( long lv )
{
	setLevel( lv );
}


void CDsrAST_Node::setDataType( CDsrDataType_Node* lv )
{
	data_type = lv;
}


CDsrDataType_Node* CDsrAST_Node::getDataType( void )
{
	return data_type;
}


long CDsrAST_Node::getRTStackOverhead( void )
{
	return run_time_stack_overhead;
}

/////////////////////////////////////////
//	StmtList	:	Node*;
/////////////////////////////////////////


int CDsrAST_StmtList::Add( CDsrAST_Node *node )
{
	return stmt.Add( node );
}


int CDsrAST_StmtList::AddLeft( CDsrAST_Node *node )
{
	stmt.insert( stmt.begin(), node );
	return 0;
}

/////////////////////////////////////////
//	ConstStringList	:	ConstString*;
/////////////////////////////////////////

CDsrAST_ConstStringList::CDsrAST_ConstStringList( void ) : 
	stmt(MIArray<CDsrAST_ConstString>::LeaveItems, 8, 0, 8)
{
	type = DsrASTT_ConstStringList;
}


int CDsrAST_ConstStringList::Add( CDsrAST_ConstString *node )
{
	return stmt.Add( node );
}

/////////////////////////////////////////
//	Compound	:	Keyword, StmtList, Keyword;
/////////////////////////////////////////

CDsrAST_Compound::CDsrAST_Compound( CDsrAST_StmtList *astmt, 
									CDsrAST_Keyword *ak_begin, 
									CDsrAST_Keyword	*ak_end )
{
	type = DsrASTT_Compound;
	stmt = astmt;
	k_begin = ak_begin;
	k_end = ak_end;
}


CDsrAST_Compound::~CDsrAST_Compound( void )
{
	if( stmt ) delete stmt;
	if( k_begin ) delete k_begin;
	if( k_end ) delete k_end;
}


void CDsrAST_Compound::setObjectContextThroughTree( long o_id )
{
	if( stmt ) stmt->setObjectContextThroughTree( o_id );
}


void CDsrAST_Compound::setLevelThroughTree( long lv )
{
	setLevel( lv );
	if( k_begin ) k_begin->setLevel( lv );
	if( k_end ) k_end->setLevel( lv );
	if( stmt ) stmt->setLevel( lv + 1 );
}


/////////////////////////////////////////
//	ArgList		:	Expr*, Separator;
/////////////////////////////////////////

CDsrAST_ArgList::CDsrAST_ArgList( CDsrAST_Separator *asep ) : 
	expr(MIArray<CDsrAST_Node>::LeaveItems, 8, 0, 8)
{
	type = DsrASTT_ArgList;
	sep = asep;
	setRecursiveDeleteStatus( 1 );
}


void CDsrAST_ArgList::setRecursiveDeleteStatus( bool rd )
{
	is_recursive_delete = rd;
}



long CDsrAST_ArgList::getArgNumber( void )
{
	return expr.ArrayDim();
}


int CDsrAST_ArgList::Add( CDsrAST_Node *node )
{
	return expr.Add( node );
}


void CDsrAST_ArgList::setSeparator( CDsrAST_Separator *asep )
{
	sep = asep;
}


CDsrAST_Separator* CDsrAST_ArgList::getSeparator( void )
{
	return sep;
}

/////////////////////////////////////////
//	NullStmt	:	Separator;
/////////////////////////////////////////

CDsrAST_NullStmt::CDsrAST_NullStmt( CDsrAST_Separator *asep )
{
	type = DsrASTT_NullStmt;
	sep = asep;
}


CDsrAST_NullStmt::~CDsrAST_NullStmt( void )
{
	if( sep ) delete sep;
}


void CDsrAST_NullStmt::setLevelThroughTree( long lv )
{
	setLevel( lv );
	if( sep ) sep->setLevel( lv );
}

/////////////////////////////////////////
//	If			:	Keyword, Expr, Keyword, Node, Keyword, Node;
/////////////////////////////////////////

CDsrAST_If::CDsrAST_If( CDsrAST_Node* _expr,
						CDsrAST_Node* _then,
						CDsrAST_Node* _else, 
						CDsrAST_Keyword	*_k_if, 
						CDsrAST_Keyword *_k_then, 
						CDsrAST_Keyword *_k_else )
{
	type = DsrASTT_If;
	n_expr = _expr;
	n_then = _then;
	n_else = _else;
	k_if = _k_if;
	k_then = _k_then;
	k_else = _k_else;
}


CDsrAST_If::~CDsrAST_If( void )
{
	if( n_expr ) delete n_expr;
	if( n_then ) delete n_then;
	if( n_else ) delete n_else;
	if( k_if ) delete k_if;
	if( k_then ) delete k_then;
	if( k_else ) delete k_else;
}


void CDsrAST_If::setElseNode( CDsrAST_Node* _else )
{
	if( n_else )
		delete n_else;
	n_else = _else;
}


void CDsrAST_If::setObjectContextThroughTree( long o_id )
{
	if( n_expr ) n_expr->setObjectContextThroughTree( o_id );
	if( n_then ) n_then->setObjectContextThroughTree( o_id );
	if( n_else ) n_else->setObjectContextThroughTree( o_id );
}


void CDsrAST_If::setLevelThroughTree( long lv )
{
	setLevel( lv );
	if( n_expr ) n_expr->setLevel( lv );
	if( n_then ) n_then->setLevel( lv );
	if( n_else ) n_else->setLevel( lv );
	if( k_if   ) k_if->setLevel( lv );
	if( k_then ) k_then->setLevel( lv );
	if( k_else ) k_else->setLevel( lv );
}

/////////////////////////////////////////
//	ConstInt	:	long;
/////////////////////////////////////////

CDsrAST_ConstInt::CDsrAST_ConstInt( long _value, QString _aspect )
{
	type = DsrASTT_ConstInt;
	value = _value;
	aspect = _aspect;
}


CDsrAST_ConstInt::~CDsrAST_ConstInt( void )
{
}


QString CDsrAST_ConstInt::getAspect( void )
{
	return aspect;
}

/////////////////////////////////////////
//	ConstReal	:	double;
/////////////////////////////////////////

CDsrAST_ConstReal::CDsrAST_ConstReal( double _value, QString _aspect )
{
	type = DsrASTT_ConstReal;
	value = _value;
	aspect = _aspect;
}


QString CDsrAST_ConstReal::getAspect( void )
{
	return aspect;
}


CDsrAST_ConstReal::~CDsrAST_ConstReal( void )
{
}

/////////////////////////////////////////
//	ConstString	:	QString;
/////////////////////////////////////////

CDsrAST_ConstString::CDsrAST_ConstString( QString _value )
{
	type = DsrASTT_ConstString;
	value = _value;
}


CDsrAST_ConstString::~CDsrAST_ConstString( void )
{
}


void CDsrAST_ConstString::append( const QString& str )
{
	value.append( str );
}


QString CDsrAST_ConstString::getAspect( void )
{
	return QString( _T("\"") ) + value + QString( _T("\"") );
}

/////////////////////////////////////////
//	Expr		:	Node, Op, Node;
/////////////////////////////////////////

CDsrAST_Expr::CDsrAST_Expr( CDsrAST_Op *_op, 
							CDsrAST_Node *_arg1, 
							CDsrAST_Node *_arg2 ) :
	m_used_xvar( 16, 0, 16 ), m_used_dxvar( 16, 0, 16 )
{
	type = DsrASTT_Expr;
	op = _op;
	args = new CDsrAST_ArgList();
	if( _arg1 ) args->Add( _arg1 );
	if( _arg2 ) args->Add( _arg2 );
	fun_id = FUNCTION_STD_NONE;
	fun_id_param = 0;
	is_koshi = 0;
}


CDsrAST_Expr::~CDsrAST_Expr( void )
{
	if( op ) delete op;
	if( args ) delete args;
}


void CDsrAST_Expr::setObjectContextThroughTree( long o_id )
{
	if( op ) op->setObjectContextThroughTree( o_id );
	if( args ) args->setObjectContextThroughTree( o_id );
}


void CDsrAST_Expr::setLevelThroughTree( long lv )
{
	setLevel( lv );
	if( op ) op->setLevel( lv );
	if( args ) args->setLevel( lv );
	/*if( arg1 ) arg1->setLevel( lv );
	if( arg2 ) arg2->setLevel( lv );*/
}

/////////////////////////////////////////
//	Function	:	CompoundID, Separator, ArgList, Separator;
/////////////////////////////////////////

CDsrAST_Function::CDsrAST_Function( CDsrAST_CompoundID* _name, CDsrAST_Separator *_lb, CDsrAST_Separator *_rb, CDsrAST_ArgList *_args )
{
	type = DsrASTT_Function;
	name = _name;
	lb = _lb;
	rb = _rb;
	args = _args;
	fun_id = FUNCTION_STD_NONE;
	fun_id_param = 0;
}


CDsrAST_Function::~CDsrAST_Function( void )
{
	if( name ) delete name;
	if( lb ) delete lb;
	if( rb ) delete rb;
	if( args ) delete args;
}


void CDsrAST_Function::setObjectContextThroughTree( long o_id )
{
	if( name ) name->setObjectContextThroughTree( o_id );
	if( args ) args->setObjectContextThroughTree( o_id );
}


void CDsrAST_Function::setLevelThroughTree( long lv )
{
	setLevel( lv );
	if( name ) name->setLevel( lv );
	if( lb ) lb->setLevel( lv );
	if( rb ) rb->setLevel( lv );
	if( args ) args->setLevel( lv );
}

/////////////////////////////////////////
//	IdxListItem	:	Separator, ArgList, Separator;
/////////////////////////////////////////

CDsrAST_IdxListItem::CDsrAST_IdxListItem( CDsrAST_Separator *alb, CDsrAST_Node *aexpr, CDsrAST_Separator *arb )
{
	type = DsrASTT_IdxListItem;
	lb = alb;
	expr = aexpr;
	rb = arb;
}

/////////////////////////////////////////
//	CompoundID	:	PrimaryID*, Separator;
/////////////////////////////////////////

CDsrAST_CompoundID::CDsrAST_CompoundID( CDsrAST_Separator *asep ) : 
	stmt(MIArray<CDsrAST_PrimaryID>::LeaveItems, 8, 0, 8)
{
	type = DsrASTT_CompoundID;
	sep = asep;
	setContext( CompoundID_CONTEXT__SINGLE );
}


void CDsrAST_CompoundID::setSeparator( CDsrAST_Separator *asep )
{
	sep = asep;
}


CDsrAST_Separator* CDsrAST_CompoundID::getSeparator( void )
{
	return sep;
}


int CDsrAST_CompoundID::Add( CDsrAST_PrimaryID *node )
{
	return stmt.Add( node );
}


CParseDsrSymbol* CDsrAST_CompoundID::getSymbol( CParseDsrSymbolTable& smbtable )
{
	if( stmt.ArrayDim() )
		return stmt[ stmt.ArrayDim() - 1 ]->getSymbol( smbtable );
	return 0;
}


/////////////////////////////////////////
//	PrimaryID	:	long, Special;
/////////////////////////////////////////

CDsrAST_PrimaryID::CDsrAST_PrimaryID( long _id, CDsrAST_Special *_spec )
{
	local_smbtable = 0;
	type = DsrASTT_PrimaryID;
	id = _id;
	spec = _spec;
	context = CompoundID_CONTEXT__SINGLE;
	is_push_by_address = 0;
}


int CDsrAST_PrimaryID::isLocalSmbTable( void )
{
	return local_smbtable != 0;
}


CDsrAST_PrimaryID::~CDsrAST_PrimaryID( void )
{
	if( spec ) delete spec;
}


void CDsrAST_PrimaryID::setObjectContextThroughTree( long o_id )
{
	if( spec ) spec->setObjectContextThroughTree( o_id );
}


void CDsrAST_PrimaryID::setLevelThroughTree( long lv )
{
	setLevel( lv );
	if( spec ) spec->setLevel( lv );
}


long CDsrAST_PrimaryID::getContext( void )
{
	return context;
}


CParseDsrSymbol* CDsrAST_PrimaryID::getSymbol( CParseDsrSymbolTable& smbtable )
{
	if( id != -1 )
		return local_smbtable ? local_smbtable->getSymbol( id ) : smbtable.getSymbol( id );
	else if( spec ) return spec->getSymbol( smbtable );
	else return 0;
}

/////////////////////////////////////////
//	Special		:	PrimaryID, Separator, ArgList, Separator;
/////////////////////////////////////////

CDsrAST_Special::CDsrAST_Special( CDsrAST_PrimaryID *_name, CDsrAST_Separator *_lb, CDsrAST_Separator *_rb, CDsrAST_ArgList *_args )
{
	type = DsrASTT_Special;
	name = _name;
	lb = _lb;
	rb = _rb;
	args = _args;
	fun_id = FUNCTION_STD_NONE;
	fun_id_param = 0;
}


CDsrAST_Special::~CDsrAST_Special( void )
{
	if( name ) delete name;
	if( lb ) delete lb;
	if( rb ) delete rb;
	if( args ) delete args;
}


void CDsrAST_Special::setObjectContextThroughTree( long o_id )
{
	if( name ) name->setObjectContextThroughTree( o_id );
	if( args ) args->setObjectContextThroughTree( o_id );
}


void CDsrAST_Special::setLevelThroughTree( long lv )
{
	setLevel( lv );
	if( lb ) lb->setLevel( lv );
	if( rb ) rb->setLevel( lv );
	if( args ) args->setLevel( lv );
}


CParseDsrSymbol* CDsrAST_Special::getSymbol( CParseDsrSymbolTable& smbtable )
{
	if( name ) return name->getSymbol( smbtable );
	else return 0;
}

/////////////////////////////////////////
//	TypeSpec	:	Op, PrimaryID, Separator, ArgList, Separator;
/////////////////////////////////////////

CDsrAST_TypeSpec::CDsrAST_TypeSpec( CDsrAST_Op *_op,
									CDsrAST_PrimaryID *_name,
									CDsrAST_Separator *_lb,
									CDsrAST_Separator *_rb,
									CDsrAST_ArgList *_args )
{
	type = DsrASTT_TypeSpec;
	op = _op;
	name = _name;
	lb = _lb;
	rb = _rb;
	args = _args;
}


CDsrAST_TypeSpec::~CDsrAST_TypeSpec( void )
{
	if( op ) delete op;
	if( name ) delete name;
	if( lb ) delete lb;
	if( rb ) delete rb;
	if( args ) delete args;
}


void CDsrAST_TypeSpec::setObjectContextThroughTree( long o_id )
{
	if( op ) op->setObjectContextThroughTree( o_id );
	if( name ) name->setObjectContextThroughTree( o_id );
	if( args ) args->setObjectContextThroughTree( o_id );
}
	

void CDsrAST_TypeSpec::setLevelThroughTree( long lv )
{
	setLevel( lv );
	if( op ) op->setLevel( lv );
	if( name ) name->setLevel( lv );
	if( lb ) lb->setLevel( lv );
	if( rb ) rb->setLevel( lv );
	if( args ) args->setLevel( lv );
}

/////////////////////////////////////////
//	ObjDef_Assign	:	ObjHead, Op, Expr, Separator;
/////////////////////////////////////////

CDsrAST_ObjDef_Assign::CDsrAST_ObjDef_Assign(	CDsrAST_ObjHead *_head, 
												CDsrAST_Op *_op, 
												CDsrAST_Node *_expr, 
												CDsrAST_Separator *_sep, 
												CParseDsrSymbolTable *_local_smbtable )
{
	type = DsrASTT_ObjDef_Assign;
	head = _head;
	op = _op;
	expr = _expr;
	sep = _sep;
	local_smbtable = _local_smbtable;
	fun_id = FUNCTION_STD_NONE;
	fun_id_param = 0;
}


CDsrAST_ObjDef_Assign::~CDsrAST_ObjDef_Assign( void )
{
	if( head ) delete head;
	if( op ) delete op;
	if( expr ) delete expr;
	if( sep ) delete sep;
}


void CDsrAST_ObjDef_Assign::setObjectContextThroughTree( long /*o_id*/ )
{
	if( head && head->name && expr )
		expr->setObjectContextThroughTree( head->name->id );
}


void CDsrAST_ObjDef_Assign::setLevelThroughTree( long lv )
{
	setLevel( lv );
	if( head ) head->setLevel( lv );
	if( op ) op->setLevel( lv );
	if( expr ) expr->setLevel( lv );
	if( sep ) sep->setLevel( lv );
}

/////////////////////////////////////////
//	ObjDef_Compound	:	ObjHead, Compound;
/////////////////////////////////////////

CDsrAST_ObjDef_Compound::CDsrAST_ObjDef_Compound(	CDsrAST_ObjHead *_head, 
													CDsrAST_Compound *_compound,
													CParseDsrSymbolTable *_local_smbtable )
{
	type = DsrASTT_ObjDef_Compound;
	head = _head;
	compound = _compound;
	local_smbtable = _local_smbtable;
}


CDsrAST_ObjDef_Compound::~CDsrAST_ObjDef_Compound( void )
{
	if( head ) delete head;
	if( compound ) delete compound;
}


void CDsrAST_ObjDef_Compound::setObjectContextThroughTree( long /*o_id*/ )
{
	if( head && head->name && compound )
		compound->setObjectContextThroughTree( head->name->id );
}


void CDsrAST_ObjDef_Compound::setLevelThroughTree( long lv )
{
	setLevel( lv );
	if( head ) head->setLevel( lv );
	if( compound ) compound->setLevel( lv );
}

/////////////////////////////////////////
//	ObjHead			:	Keyword, PrimaryID, ObjArg;
/////////////////////////////////////////

CDsrAST_ObjHead::CDsrAST_ObjHead(	CDsrAST_Keyword *_keyword, 
									CDsrAST_PrimaryID *_name, 
									CDsrAST_ObjArg *_args )
{
	type = DsrASTT_ObjHead;
	keyword = _keyword;
	name = _name;
	args = _args;
}


CDsrAST_ObjHead::~CDsrAST_ObjHead( void )
{
	if( keyword ) delete keyword;
	if( name ) delete name;
	if( args ) delete args;
}


void CDsrAST_ObjHead::setObjectContextThroughTree( long o_id )
{
	if( name ) name->setObjectContextThroughTree( o_id );
	if( args ) args->setObjectContextThroughTree( o_id );
}
	

void CDsrAST_ObjHead::setLevelThroughTree( long lv )
{
	setLevel( lv );
	if( keyword ) keyword->setLevel( lv );
	if( name ) name->setLevel( lv );
	if( args ) args->setLevel( lv );
}


CParseDsrSymbol* CDsrAST_ObjHead::getSymbol( CParseDsrSymbolTable& smbtable )
{
	if( name )
		return name->getSymbol( smbtable );
	return 0;
}

/////////////////////////////////////////
//	ObjArg			:	Separator, ArgList, Separator, TypeSpec;
/////////////////////////////////////////

CDsrAST_ObjArg::CDsrAST_ObjArg( CDsrAST_Separator *_lb, 
								CDsrAST_Separator *_rb, 
								CDsrAST_ArgList *_args, 
								CDsrAST_TypeSpec *_typespec )
{
	type = DsrASTT_ObjArg;
	lb = _lb;
	rb = _rb;
	args = _args;
	typespec = _typespec;
}


CDsrAST_ObjArg::~CDsrAST_ObjArg( void )
{
	if( lb ) delete lb;
	if( rb ) delete rb;
	if( args ) delete args;
	if( typespec ) delete typespec ;
}


void CDsrAST_ObjArg::setKeywordType( enum CDsrDataTypes tp )
{
	keyword_type = tp;
}


enum CDsrDataTypes CDsrAST_ObjArg::getKeywordType( void )
{
	return keyword_type;
}



void CDsrAST_ObjArg::setObjectContextThroughTree( long o_id )
{
	if( args ) args->setObjectContextThroughTree( o_id );
	if( typespec ) typespec->setObjectContextThroughTree( o_id );
}


void CDsrAST_ObjArg::setTypeSpec( CDsrAST_TypeSpec *aargs )
{
	typespec = aargs;
}


void CDsrAST_ObjArg::setLevelThroughTree( long lv )
{
	setLevel( lv );
	if( lb ) lb->setLevel( lv );
	if( rb ) rb->setLevel( lv );
	if( args ) args->setLevel( lv );
	if( typespec ) typespec->setLevel( lv );
}

/////////////////////////////////////////
//	Keyword		:	long;
/////////////////////////////////////////

CDsrAST_Keyword::CDsrAST_Keyword( long _id, enum CDsrDataTypes tp )
{
	type = DsrASTT_Keyword;
	id = _id;
	kw_type = tp;
}


CDsrAST_Keyword::~CDsrAST_Keyword( void )
{}


enum CDsrDataTypes CDsrAST_Keyword::getKeywordType( void )
{
	return kw_type;
}


void CDsrAST_Keyword::setLevelThroughTree( long lv )
{
	setLevel( lv );
}


CParseDsrSymbol* CDsrAST_Keyword::getSymbol( CParseDsrSymbolTable& smbtable )
{
	if( id != -1 )
		return smbtable.getSymbol( id );
	return 0;
}

/////////////////////////////////////////
//	Op			:	long;
/////////////////////////////////////////

CDsrAST_Op::CDsrAST_Op( long _id )
{
	type = DsrASTT_Op;
	id = _id;
	object_id = -1;
}


CDsrAST_Op::CDsrAST_Op( long _id, long _line )
{
	type = DsrASTT_Op;
	line = _line;
	id = _id;
	object_id = -1;
}


CDsrAST_Op::~CDsrAST_Op( void )
{}


void CDsrAST_Op::setObjectContextThroughTree( long o_id )
{
	object_id = o_id;
}


CParseDsrSymbol* CDsrAST_Op::getObjectContextSmb( CParseDsrSymbolTable& smbtable )
{
	if( object_id != -1 )
		return smbtable.getSymbol( object_id );
	return 0;
}


long CDsrAST_Op::getObjectContext( CParseDsrSymbolTable& /*smbtable*/ )
{
	return object_id;
}


void CDsrAST_Op::setLevelThroughTree( long lv )
{
	setLevel( lv );
}


CParseDsrSymbol* CDsrAST_Op::getSymbol( CParseDsrSymbolTable& smbtable )
{
	if( id != -1 )
	{
		long *pid = smbtable.is_find( ::ml_Op2String( id ) );
		if( pid )
			return smbtable.getSymbol( *pid );
	}
	return 0;
}

/////////////////////////////////////////
//	Separator	:	long;
/////////////////////////////////////////

CDsrAST_Separator::CDsrAST_Separator( long _id )
{
	type = DsrASTT_Separator;
	id = _id;
}


CDsrAST_Separator::~CDsrAST_Separator( void )
{}


void CDsrAST_Separator::setLevelThroughTree( long lv )
{
	setLevel( lv );
}
/////////////////////////////////////////
//	Remark		:	QString;
/////////////////////////////////////////

CDsrAST_Remark::CDsrAST_Remark( QString _value )
{
	type = DsrASTT_Remark;
	value = _value;
}


CDsrAST_Remark::~CDsrAST_Remark( void )
{}
	

void CDsrAST_Remark::setLevelThroughTree( long lv )
{
	setLevel( lv );
}
/////////////////////////////////////////
//	Blank		:	QString;
/////////////////////////////////////////

CDsrAST_Blank::CDsrAST_Blank( QString _value )
{
	type = DsrASTT_Blank;
	value = _value;
}


CDsrAST_Blank::~CDsrAST_Blank( void )
{}


void CDsrAST_Blank::setLevelThroughTree( long lv )
{
	setLevel( lv );
}
/////////////////////////////////////////
//	0d0a		:	;
/////////////////////////////////////////

CDsrAST_0d0a::CDsrAST_0d0a( QString _value )
{
	type = DsrASTT_0d0a;
	value = _value;
}


CDsrAST_0d0a::~CDsrAST_0d0a( void )
{}
	

void CDsrAST_0d0a::setLevelThroughTree( long lv )
{
	setLevel( lv );
}
/////////////////////////////////////////
//	unknown		:	QString;
/////////////////////////////////////////

CDsrAST_unknown::CDsrAST_unknown( QString _value )
{
	type = DsrASTT_unknown;
	value = _value;
}


CDsrAST_unknown::~CDsrAST_unknown( void )
{}


void CDsrAST_unknown::setLevelThroughTree( long lv )
{
	setLevel( lv );
}

///////////////////////////////////////////////////////////////////////////////
