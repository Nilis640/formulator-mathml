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

#include "stdafx.h"
#include "hmath\hmathlng\src\parsedsr\parserdsr.h"
#include "hmath\hmathlng\src\parsedsr\thelexdsr.h"

#define IS_IGNORE_LEXEM( c ) ((c) >= LEX_ignore_remark && (c) < LEX_eof)
#define IS_TERMINAL_LEXEM( c ) ((c) >= LEX_dot)

extern QString __femTooManyErrors;	// !!! change to error\er_mess.h

//*****************************************************************************
// DSRText_Parser::GetLexError (m) - returns error message on lex error.
//*****************************************************************************
const QString& DSRText_Parser::GetLexError( void )
{
	static const QString LexError[] = {
		_T("Unknown error"),
    	_T("Unknown symbol"),
    	_T("Unterminated comment"),
    	_T("Bad numeric format")
	};
	return LexError[ ( int )(scaner->GetError()) ];
}

void DSRText_Parser::SetSemanticError( void )
{
	for( long i = 0; i < astn_index; i++ )
	{
		if( astn[ i ] )
		{
			delete astn[ i ];
			astn[ i ] = 0;
		}
	}
	ClearFlag( PF_LOCALS__PREPARE_FUNCTION_DEF__LVALUE );
	ClearFlag( PF_LOCALS__PREPARE_FUNCTION_DEF );
	ClearFlag__PF_LOCALS__FUNCTION_DEF();
	ClearFlag( PF_LOCALS__FUNCTION_ARGS_DEF );
	BaseParser::SetSemanticError();
}

inline
DSRText_Parser::~DSRText_Parser( void )
{
}

//*****************************************************************************
// DSRText_Parser::NewLex - define pointer to class-scaner.
//*****************************************************************************
void DSRText_Parser::NewLex( Source& aSource )
{
  scaner = new ParseDsrLex( aSource, Info, getSymbolTablePtr() );
}

//*****************************************************************************
// DSRText_Parser::BeforeMake (m) - function is called before parsing.
//*****************************************************************************
void DSRText_Parser::BeforeMake( void )
{
	SemanticErrorHappened = false;
	BaseParser::BeforeMake();
	astn_index = 0;
	astn.Flush();

	LexPoint = StatePoint = 0;
	LexStack.Flush();
	StateStack.Flush();
	IgnoreStack.Flush();
	StatePush( 1 );

	setLexerStatus( LEXSTATUS_CURRENT );

	ClearFlag( PF_LOCALS__PREPARE_FUNCTION_DEF__LVALUE );
	ClearFlag( PF_LOCALS__PREPARE_FUNCTION_DEF );
	ClearFlag__PF_LOCALS__FUNCTION_DEF();
	ClearFlag( PF_LOCALS__FUNCTION_ARGS_DEF );
}

void DSRText_Parser::ConstructAfterChain( DSRText_LexemVector& lv, CDsrAST_Node *node )
{
	for( int i = 0; i < (long) lv.ArrayDim(); i++ )
	{
		Lexem& Smb = lv[ i ];
		switch( Smb.LexType )
		{
		case LEX_ignore_remark:                      
			node->AddAfter( new CDsrAST_Remark( ((ParseDsrLex*)scaner)->getLexemText( Smb ) ) );
			break;
		case LEX_ignore_blank:
			node->AddAfter( new CDsrAST_Blank( ((ParseDsrLex*)scaner)->getLexemText( Smb ) ) );
			break;
		case LEX_ignore_0d0a:
			node->AddAfter( new CDsrAST_0d0a( ((ParseDsrLex*)scaner)->getLexemText( Smb ) ) );
			break;
		case LEX_error_or_unknown:
			node->AddAfter( new CDsrAST_unknown( ((ParseDsrLex*)scaner)->getLexemText( Smb ) ) );
			break;
		}
	}
	lv.Flush();//lev ???
}

//*****************************************************************************
// DSRText_Parser::AfterMake (m) - function is called if parsing successfully finished.
//*****************************************************************************
void DSRText_Parser::AfterMake( void )
{
}

//*****************************************************************************
// DSRText_Parser::Make - parsing.
//*****************************************************************************
int DSRText_Parser::OnReadLexem( void )
{
	int ret = 0;
	for(;;)
	{
		switch( getLexerStatus() )
		{
		case LEXSTATUS_CURRENT:
			ClearFlag( PF_ERROR_REPORT_SILENT_MODE_4CURRENT_SMB );
			setLexerBuffer( scaner->NextLexem(), LEXSTATUS_LEXEM );
			LexBuffer[ LEXSTATUS_CURRENT ] = LexBuffer[ LEXSTATUS_LEXEM ];
			if( getCurrentSmb().LexType == LEXEM( 0 ) || 
				getCurrentSmb().LexType == LEX_error_or_unknown )
				ret = SetError( GetLexError(), GetLine() );
			break;
		case LEXSTATUS_LEXEM:
			LexBuffer[ LEXSTATUS_CURRENT ] = LexBuffer[ LEXSTATUS_LEXEM ];
			setLexerStatus( LEXSTATUS_CURRENT );
			break;
		case LEXSTATUS_NT:
			LexBuffer[ LEXSTATUS_CURRENT ] = LexBuffer[ LEXSTATUS_NT ];
			setLexerStatus( LEXSTATUS_LEXEM );
			break;
		}
		if( IS_IGNORE_LEXEM( getCurrentSmb().LexType ) )
		{
			IgnoreStack[ LexPoint ].Add( getCurrentSmb() );
			continue;
		}
		else
			break;
	}
	return ret;
}

void DSRText_Parser::OnReduce( long _RuleNumber, long _RuleLength, long _RuleLeft )
{
	GenCode( _RuleNumber );
	Lexem NonTermStorage( (LEXEM) _RuleLeft );
	if( _RuleLength )
	{
		NonTermStorage.line = LexStack[ LexPoint - _RuleLength ].GetLine();
		NonTermStorage.col  = LexStack[ LexPoint - _RuleLength ].GetCol();
	}
	else
	{
		NonTermStorage.line = GetLine();
		NonTermStorage.col  = GetCol();
	}
	IgnorePop( (unsigned short) _RuleLength );
	Pop( (unsigned short) _RuleLength );
	setLexerBuffer( NonTermStorage, LEXSTATUS_NT );
	setLexerStatus( LEXSTATUS_NT );
}

void DSRText_Parser::OnShift( long _state )
{
	StatePush( (unsigned short) _state );
	LexPush( getCurrentSmb() );
}

unsigned DSRText_Parser::Make( void )
{
	BeforeMake();

	for(;;) 
	{
		// continue even if error number is too big
		while( OnReadLexem() == -1 );

		ActionItem *act = ( *actions )( TopState(), getCurrentSmb().LexType );
		CheckDeed( act );
		switch( act->select )
		{
		case _Shift :
			OnShift( act->value );
			break;
		case _Reduce :
			OnReduce( act->value, actions->RuleLen[ act->value - 1 ], actions->NonTerms[ act->value - 1 ] );
			break;
		case _Halt :
			if( !IsError() )
			{
				AfterMake();
				return 0;
			}  
			return 0xFFFF;
        case _Error :
			if( !IsFlag( PF_ERROR_REPORT_SILENT_MODE_4CURRENT_SMB ) )
				SetError( *act, GetLine() );
			ClearFlag( PF_LOCALS__PREPARE_FUNCTION_DEF__LVALUE );
			ClearFlag( PF_LOCALS__PREPARE_FUNCTION_DEF );
			ClearFlag__PF_LOCALS__FUNCTION_DEF();
			ClearFlag( PF_LOCALS__FUNCTION_ARGS_DEF );
			if( OnSyntaxError() == -1 )
				SetSemanticError();
			break;
		}
	}
}

CDsrAST_Node* DSRText_Parser::ConstructAST4Lexem( int shift )
{
	CDsrAST_Node* node = 0;
	switch( LexStack[ LexPoint - shift ].LexType )
	{
	case LEX_eol:
	case LEX_dot:
	case LEX_ask:
	case LEX_coma:
	case LEX_lb:
	case LEX_rb:
	case LEX_lab:
	case LEX_rab:
	case LEX_lfb:
	case LEX_rfb:
		node = ConstructSeparator( shift );
		break;
	case LEX_op_assign:
	case LEX_op_bool_or:
	case LEX_op_bool_and:
	case LEX_op_compare_e:
	case LEX_op_compare_ne:
	case LEX_op_setadd:
	case LEX_op_setmul:
	case LEX_op_rel_g:
	case LEX_op_rel_ge:
	case LEX_op_rel_l:
	case LEX_op_rel_le:
	case LEX_op_rel_in:
	case LEX_op_rel_not_in:
	case LEX_op_rel_set_in:
	case LEX_op_rel_set_in_inv:
	case LEX_op_rel_set_in_eq:
	case LEX_op_rel_set_in_eq_inv:
	case LEX_op_rel_set_not_in:
	case LEX_op_add:
	case LEX_op_mul:
	case LEX_op_pow:
	case LEX_op_bool_not:
	case LEX_op_arrow:
		node = ConstructOp( shift );
		break;
	case LEX_const_int:
		node = ConstructConstInt( shift );
		break;
//	case LEX_const_big_int:
//		ConstructConstBigInt( shift );
//		break;
	case LEX_const_real:
		node = ConstructConstReal( shift );
		break;
	case LEX_const_string:
		node = ConstructConstString( shift );
		break;
	case LEX_identifier:
	case LEX_sp_identifier:
		node = ConstructPrimaryID_identifier( shift );
		break;
	case LEX_kw_begin:
	case LEX_kw_end:
	case LEX_kw_if:
	case LEX_kw_then:
	case LEX_kw_else:
	case LEX_kw_function:
	case LEX_kw_group:
	case LEX_kw_polynomial:
	case LEX_kw_rational_function:
		node = ConstructKeyword( shift );
		break;
	case LEX_ignore_remark:
		node = new CDsrAST_Remark( ((ParseDsrLex*)scaner)->getLexemText( LexStack[ LexPoint - shift ] ) );
		ConstructAfterChain( shift, node );
		break;
	case LEX_ignore_blank:
		node = new CDsrAST_Blank( ((ParseDsrLex*)scaner)->getLexemText( LexStack[ LexPoint - shift ] ) );
		ConstructAfterChain( shift, node );
		break;
	case LEX_ignore_0d0a:
		node = new CDsrAST_0d0a( ((ParseDsrLex*)scaner)->getLexemText( LexStack[ LexPoint - shift ] ) );
		ConstructAfterChain( shift, node );
		break;
	case LEX_error_or_unknown:
		node = new CDsrAST_unknown( ((ParseDsrLex*)scaner)->getLexemText( LexStack[ LexPoint - shift ] ) );
		ConstructAfterChain( shift, node );
		break;
	case LEX_eof:
		node = new CDsrAST_Blank( _T("") );
		ConstructAfterChain( shift, node );
		break;
	}
	return node;
}

//*****************************************************************************
// DSRText_Parser::GenCode (m) - generate code while syntax analiser works.
// Params  : RuleNumber - number of grammar rule, which is 
//                        used by parser in reduce.
//
// Your should define function's actions ( depending of rule number ) if your 
// wish your compiler to do something more than analise syntax text structure.
//*****************************************************************************
void DSRText_Parser::GenCode( unsigned int RuleNumber )
{
	if( IsSemanticError() ) return;

	CDsrAST_Node *node, *node1;
	CDsrAST_ArgList *args;
	long i;
	QString str = _T("");

	try
	{
		switch( RuleNumber )
		{
		case 1: //	Main		->	 StmtList 	// Orig. rule 1
			astn_index--;
			if( astn[ astn_index ]->getType() != DsrASTT_StmtList )
				throw EMID_INTERNAL_ERROR;
			if( IgnoreStack[ 0 ].ArrayDim() )
			{
				ConstructAfterChain( IgnoreStack[ 0 ], node = new CDsrAST_Node() );
				((CDsrAST_StmtList*)astn[ astn_index ])->AddLeft( node );
			}
			*the_storage = (CDsrAST_StmtList*)astn[ astn_index ];
			break;
		case 2: //	StmtList	->	 StmtList  Stmt 	// Orig. rule 2
			astn_index--;
			node = astn[ astn_index - 1 ];
			if( node->getType() != DsrASTT_StmtList )
				throw EMID_INTERNAL_ERROR;
			((CDsrAST_StmtList*)node)->Add( astn[ astn_index ] );
			break;
		case 3: //	StmtList	->	 Stmt 	// Orig. rule 3
			node = new CDsrAST_StmtList();
			node->line = LexStack[ LexPoint - 1 ].GetLine();
			((CDsrAST_StmtList*)node)->Add( astn[ astn_index - 1 ] );
			astn[ astn_index - 1 ] = node;
			break;
		case 4: //	Stmt		->	 IfStmt 	// Orig. rule 5
			if( astn[ astn_index - 1 ]->getType() != DsrASTT_If )
				throw EMID_INTERNAL_ERROR;
			break;
		case 5: //	Stmt		->	 eol 	// Orig. rule 7
			astn[ astn_index++ ] = ConstructSeparator( 1 );
			break;
		case 6: //	Stmt		->	 ObjDef 	// Orig. rule 8
			break;
		case 7: //	Stmt		->	 Compound 	// Orig. rule 4
			if( astn[ astn_index - 1 ]->getType() != DsrASTT_Compound )
				throw EMID_INTERNAL_ERROR;
			break;
		case 8: //	Stmt		->	 ArgList  eol 	// Orig. rule 6
			if( astn[ astn_index - 1 ]->getType() != DsrASTT_ArgList )
				throw EMID_INTERNAL_ERROR;
			node = new CDsrAST_StmtList();
			node->line = LexStack[ LexPoint - 1 ].GetLine();
			((CDsrAST_StmtList*)node)->Add( astn[ astn_index - 1 ] );
			((CDsrAST_StmtList*)node)->Add( ConstructSeparator( 1 ) );
			astn[ astn_index - 1 ] = node;
			break;
		case 9: //	Compound	->	 kw_begin  StmtList  kw_end 	// Orig. rule 9
			if( astn[ astn_index - 1 ]->getType() != DsrASTT_StmtList )
				throw EMID_INTERNAL_ERROR;
			node = new CDsrAST_Compound(
						(CDsrAST_StmtList*)astn[ astn_index - 1 ],
						ConstructKeyword( 3 ),
						ConstructKeyword( 1 )
				  					  );
			node->line = LexStack[ LexPoint - 1 ].GetLine();
			astn[ astn_index - 1 ] = node;
			break;
		case 10: //	IfStmt		->	 IfHead  Stmt  Else 	// Orig. rule 11
			if( astn[ astn_index - 5 ]->getType() != DsrASTT_Keyword ||
				astn[ astn_index - 4 ]->getType() != DsrASTT_Keyword ||
				astn[ astn_index - 1 ]->getType() != DsrASTT_Keyword )
				throw EMID_INTERNAL_ERROR;
			node = new CDsrAST_If(
						astn[ astn_index - 6 ],
						astn[ astn_index - 3 ],
						astn[ astn_index - 2 ],
						(CDsrAST_Keyword*)astn[ astn_index - 5 ],
						(CDsrAST_Keyword*)astn[ astn_index - 4 ],
						(CDsrAST_Keyword*)astn[ astn_index - 1 ]
				  					  );
			node->line = LexStack[ LexPoint - 1 ].GetLine();
			astn_index -= 5;
			astn[ astn_index - 1 ] = node;
			break;
		case 11: //	IfStmt		->	 IfHead  Stmt 	// Orig. rule 10
			if( astn[ astn_index - 3 ]->getType() != DsrASTT_Keyword ||
				astn[ astn_index - 2 ]->getType() != DsrASTT_Keyword )
				throw EMID_INTERNAL_ERROR;
			node = new CDsrAST_If(
						astn[ astn_index - 4 ],
						astn[ astn_index - 1 ],
						(CDsrAST_Node*)0,
						(CDsrAST_Keyword*)astn[ astn_index - 3 ],
						(CDsrAST_Keyword*)astn[ astn_index - 2 ],
						(CDsrAST_Keyword*)0
				  					  );
			node->line = LexStack[ LexPoint - 1 ].GetLine();
			astn_index -= 3;
			astn[ astn_index - 1 ] = node;
			break;
		case 12: //	ArgList		->	 Expression 	// Orig. rule 56
			node = new CDsrAST_ArgList();
			node->line = LexStack[ LexPoint - 1 ].GetLine();
			((CDsrAST_ArgList*)node)->Add( astn[ astn_index - 1 ] );
			astn[ astn_index - 1 ] = node;
			break;
		case 13: //	ArgList		->	 ArgList  coma  Expression 	// Orig. rule 55
			astn_index--;
			node = astn[ astn_index - 1 ];
			if( node->getType() != DsrASTT_ArgList )
				throw EMID_INTERNAL_ERROR;
			if( !((CDsrAST_ArgList*)node)->getSeparator() )
				((CDsrAST_ArgList*)node)->setSeparator( ConstructSeparator( 2 ) );
			((CDsrAST_ArgList*)node)->Add( astn[ astn_index ] );
			break;
		case 14: //	ObjDef		->	 ObjHead  Compound 	// Orig. rule 84
			if( astn[ astn_index - 1 ]->getType() != DsrASTT_Compound ||
				astn[ astn_index - 2 ]->getType() != DsrASTT_ObjHead )
				throw EMID_INTERNAL_ERROR;
			node = new CDsrAST_ObjDef_Compound(
						(CDsrAST_ObjHead*)astn[ astn_index - 2 ],
						(CDsrAST_Compound*)astn[ astn_index - 1 ],
						ptr_local_symbol_table
									  );
			node->line = LexStack[ LexPoint - 1 ].GetLine();
			astn_index --;
			astn[ astn_index - 1 ] = node;
			ClearFlag__PF_LOCALS__FUNCTION_DEF();
			break;
		case 15: //	ObjDef		->	 ObjHead  AssignOP  Expression  eol 	// Orig. rule 83
			if( astn[ astn_index - 3 ]->getType() != DsrASTT_ObjHead ||
				astn[ astn_index - 2 ]->getType() != DsrASTT_Op )
				throw EMID_INTERNAL_ERROR;
			node = new CDsrAST_ObjDef_Assign(
						(CDsrAST_ObjHead*)astn[ astn_index - 3 ],
						(CDsrAST_Op*)astn[ astn_index - 2 ],
						astn[ astn_index - 1 ],
						ConstructSeparator( 1 ),
						ptr_local_symbol_table
									  );
			node->line = LexStack[ LexPoint - 1 ].GetLine();
			astn_index -= 2;
			astn[ astn_index - 1 ] = node;
			ClearFlag__PF_LOCALS__FUNCTION_DEF();
			break;
		case 16: //	IfHead		->	 kw_if  Expression  kw_then 	// Orig. rule 13
			/*if( astn[ astn_index - 1 ]->getType() != DsrASTT_Expr )
				throw EMID_INTERNAL_ERROR;*/
			astn[ astn_index++ ] = ConstructKeyword( 3 );
			astn[ astn_index++ ] = ConstructKeyword( 1 );
			break;
		case 17: //	Else		->	 kw_else  Stmt 	// Orig. rule 14
			astn[ astn_index++ ] = ConstructKeyword( 2 );
			break;
		case 18: //	Expression	->	 BoolOrExpr 	// Orig. rule 16
		case 20: //	BoolOrExpr	->	 BoolAndExpr 	// Orig. rule 18
		case 25: //	BoolAndExpr	->	 EqualExpr 	// Orig. rule 20
		case 28: //	EqualExpr	->	 RelExpr 	// Orig. rule 22
		case 31: //	RelExpr		->	 AddExpr 	// Orig. rule 24
		case 44: //	AddExpr		->	 MulExpr 	// Orig. rule 26
		case 49: //	MulExpr		->	 PowExpr 	// Orig. rule 28
		case 52: //	PowExpr		->	 UnaryExpr 	// Orig. rule 30
		case 56: //	UnaryExpr	->	 PostFixExpr 	// Orig. rule 32
			break;
		case 19: //	Expression	->	 BoolOrExpr  AssignOP  Expression 	// Orig. rule 15
		case 21: //	BoolOrExpr	->	 BoolOrExpr  BoolOrOP  BoolAndExpr 	// Orig. rule 17
		case 24: //	BoolAndExpr	->	 BoolAndExpr  BoolAndOP  EqualExpr 	// Orig. rule 19
		case 27: //	EqualExpr	->	 EqualExpr  EqualOP  RelExpr 	// Orig. rule 21
		case 32: //	RelExpr		->	 RelExpr  RelOP  AddExpr 	// Orig. rule 23
		case 45: //	AddExpr		->	 AddExpr  AddOP  MulExpr 	// Orig. rule 25
		case 48: //	MulExpr		->	 MulExpr  MulOP  PowExpr 	// Orig. rule 27
		case 53: //	PowExpr		->	 PowExpr  PowOP  UnaryExpr 	// Orig. rule 29
			if( astn[ astn_index - 2 ]->getType() != DsrASTT_Op )
				throw EMID_INTERNAL_ERROR;
			node = new CDsrAST_Expr(
						(CDsrAST_Op*)astn[ astn_index - 2 ],
						astn[ astn_index - 3 ],
						astn[ astn_index - 1 ]
									);
			node->line = LexStack[ LexPoint - 1 ].GetLine();
			astn_index -= 2;
			astn[ astn_index - 1 ] = node;
			break;
		case 22: //	AssignOP	->	 op_assign 	// Orig. rule 57
		case 23: //	BoolOrOP	->	 op_bool_or 	// Orig. rule 58
		case 26: //	BoolAndOP	->	 op_bool_and 	// Orig. rule 59
		case 29: //	EqualOP		->	 op_compare_e 	// Orig. rule 60
		case 30: //	EqualOP		->	 op_compare_ne 	// Orig. rule 61
		case 33: //	RelOP		->	 op_rel_g 	// Orig. rule 62
		case 34: //	RelOP		->	 op_rel_set_in 	// Orig. rule 68
		case 35: //	RelOP		->	 op_rel_not_in 	// Orig. rule 67
		case 36: //	RelOP		->	 op_rel_in 	// Orig. rule 66
		case 37: //	RelOP		->	 op_rel_le 	// Orig. rule 65
		case 38: //	RelOP		->	 op_rel_set_in_inv 	// Orig. rule 69
		case 39: //	RelOP		->	 op_rel_set_in_eq_inv 	// Orig. rule 71
		case 40: //	RelOP		->	 op_rel_set_not_in 	// Orig. rule 72
		case 41: //	RelOP		->	 op_rel_l 	// Orig. rule 64
		case 42: //	RelOP		->	 op_rel_ge 	// Orig. rule 63
		case 43: //	RelOP		->	 op_rel_set_in_eq 	// Orig. rule 70
		case 46: //	AddOP		->	 op_setadd 	// Orig. rule 74
		case 47: //	AddOP		->	 op_add 	// Orig. rule 73
		case 50: //	MulOP		->	 op_setmul 	// Orig. rule 76
		case 51: //	MulOP		->	 op_mul 	// Orig. rule 75
		case 54: //	PowOP		->	 op_pow 	// Orig. rule 77
		case 57: //	UnaryOP		->	 op_add 	// Orig. rule 79
		case 58: //	UnaryOP		->	 op_bool_not 	// Orig. rule 78
			astn[ astn_index++ ] = ConstructOp( 1 );
			break;
		case 55: //	UnaryExpr	->	 UnaryOP  UnaryExpr 	// Orig. rule 31
			if( astn[ astn_index - 2 ]->getType() != DsrASTT_Op )
				throw EMID_INTERNAL_ERROR;
			node = new CDsrAST_Expr(
						(CDsrAST_Op*)astn[ astn_index - 2 ],
						0,
						astn[ astn_index - 1 ]
									);
			node->line = LexStack[ LexPoint - 1 ].GetLine();
			astn_index --;
			astn[ astn_index - 1 ] = node;
			break;
		case 59: //	PostFixExpr	->	 Primary  IdxList 	// Orig. rule 34
			if( astn[ astn_index - 1 ]->getType() != DsrASTT_ArgList )
				throw EMID_INTERNAL_ERROR;
			args = (CDsrAST_ArgList*)(astn[ astn_index - 1 ]);
			node1 = astn[ astn_index - 2 ];
			for( i = 0; i < args->getArgNumber(); i++ )
			{
				//if( !args->expr[ i ] ) throw EMID_INTERNAL_ERROR;
				node = new CDsrAST_Op( OPERATOR_square_brackets );
				node->line = LexStack[ LexPoint - 1 ].GetLine();
				node = new CDsrAST_Expr(
							(CDsrAST_Op*)node,
							node1,
							args->expr[ i ] );
				node->line = LexStack[ LexPoint - 1 ].GetLine();
				node1 = node;
				args->expr[ i ] = 0;
			}
			delete args;
			astn_index --;
			astn[ astn_index - 1 ] = node;
			break;
		case 60: //	PostFixExpr	->	 Primary 	// Orig. rule 33
			// Empty; Primary must just be successor of the type AST_Node
			break;
		case 61: //	Primary		->	 const_real 	// Orig. rule 36
			astn[ astn_index++ ] = ConstructConstReal( 1 );
			break;
		case 62: //	Primary		->	 ConstStringList 	// Orig. rule 37
			if( astn[ astn_index - 1 ]->getType() != DsrASTT_ConstStringList )
				throw EMID_INTERNAL_ERROR;
			break;
		case 67: //	Primary		->	 lb  Expression  rb 	// Orig. rule 38
			astn[ astn_index - 1 ]->AddBefore( ConstructSeparator( 3 ) );
			astn[ astn_index - 1 ]->AddAfter( ConstructSeparator( 1 ) );

			node = new CDsrAST_Op( OPERATOR_round_brackets );
			node->line = LexStack[ LexPoint - 1 ].GetLine();

			node =  new CDsrAST_Expr(
						(CDsrAST_Op*)node,
						astn[ astn_index - 1 ],
						0 );
			node->line = LexStack[ LexPoint - 1 ].GetLine();

			astn[ astn_index - 1 ] = node;
			break;
		case 66: //	Primary		->	 lfb  ArgList  rfb 	// Orig. rule 39
			astn[ astn_index - 1 ]->AddBefore( ConstructSeparator( 3 ) );
			astn[ astn_index - 1 ]->AddAfter( ConstructSeparator( 1 ) );

			node = new CDsrAST_Op( OPERATOR_curly_brackets );
			node->line = LexStack[ LexPoint - 1 ].GetLine();

			node =  new CDsrAST_Expr(
						(CDsrAST_Op*)node,
						astn[ astn_index - 1 ],
						0 );
			node->line = LexStack[ LexPoint - 1 ].GetLine();

			astn[ astn_index - 1 ] = node;
			break;
		case 63: //	Primary		->	 CompoundId  lb  rb 	// Orig. rule 43
			if( astn[ astn_index - 1 ]->getType() != DsrASTT_CompoundID )
				throw EMID_INTERNAL_ERROR;
			((CDsrAST_CompoundID*)astn[ astn_index - 1 ])->setContext( CompoundID_CONTEXT__FUNCTION );
			node = new CDsrAST_Function(
						(CDsrAST_CompoundID*)astn[ astn_index - 1 ],
						ConstructSeparator( 2 ),
						ConstructSeparator( 1 ),
						0
									);
			node->line = LexStack[ LexPoint - 1 ].GetLine();
			astn[ astn_index - 1 ] = node;
			break;
		case 64: //	Primary		->	 CompoundId  lb  ArgList  rb 	// Orig. rule 42
			if( astn[ astn_index - 2 ]->getType() != DsrASTT_CompoundID ||
				astn[ astn_index - 1 ]->getType() != DsrASTT_ArgList )
				throw EMID_INTERNAL_ERROR;
			((CDsrAST_CompoundID*)astn[ astn_index - 2 ])->setContext( CompoundID_CONTEXT__FUNCTION );
			node = new CDsrAST_Function(
						(CDsrAST_CompoundID*)astn[ astn_index - 2 ],
						ConstructSeparator( 3 ),
						ConstructSeparator( 1 ),
						(CDsrAST_ArgList*)astn[ astn_index - 1 ]
									);
			node->line = LexStack[ LexPoint - 1 ].GetLine();
			astn_index --;
			astn[ astn_index - 1 ] = node;
			break;
		case 65: //	Primary		->	 CompoundId  TypeSpec 	// Orig. rule 41
			if( astn[ astn_index - 2 ]->getType() != DsrASTT_CompoundID || 
				astn[ astn_index - 1 ]->getType() != DsrASTT_TypeSpec )
				throw EMID_INTERNAL_ERROR;

			((CDsrAST_CompoundID*)astn[ astn_index - 2 ])->setContext( CompoundID_CONTEXT__TYPESPEC );

			node = new CDsrAST_Op( OPERATOR_op_type_spec );
			//node = new CDsrAST_Op( OPERATOR_op_rel_in );
			node->line = LexStack[ LexPoint - 1 ].GetLine();

			node = new CDsrAST_Expr(
						(CDsrAST_Op*)node,
						astn[ astn_index - 2 ],
						astn[ astn_index - 1 ]
									);
			node->line = LexStack[ LexPoint - 1 ].GetLine();
			astn_index --;
			astn[ astn_index - 1 ] = node;
			break;
		case 68: //	Primary		->	 const_int 	// Orig. rule 35
			astn[ astn_index++ ] = ConstructConstInt( 1 );
			break;
		case 69: //	Primary		->	 CompoundId 	// Orig. rule 40
			if( astn[ astn_index - 1 ]->getType() != DsrASTT_CompoundID )
				throw EMID_INTERNAL_ERROR;
			((CDsrAST_CompoundID*)astn[ astn_index - 1 ])->setContext( CompoundID_CONTEXT__SINGLE );
			break;
		case 70: //	IdxList		->	 lab  Expression  rab 	// Orig. rule 54
			astn[ astn_index - 1 ]->AddBefore( ConstructSeparator( 3 ) );
			astn[ astn_index - 1 ]->AddAfter( ConstructSeparator( 1 ) );

			node = new CDsrAST_ArgList();
			node->line = LexStack[ LexPoint - 1 ].GetLine();
			((CDsrAST_ArgList*)node)->Add( astn[ astn_index - 1 ] );
			astn[ astn_index - 1 ] = node;
			break;
		case 71: //	IdxList		->	 IdxList  lab  Expression  rab 	// Orig. rule 53
			astn_index--;
			node = astn[ astn_index - 1 ];
			if( node->getType() != DsrASTT_ArgList )
				throw EMID_INTERNAL_ERROR;
			astn[ astn_index ]->AddBefore( ConstructSeparator( 3 ) );
			astn[ astn_index ]->AddAfter( ConstructSeparator( 1 ) );
			((CDsrAST_ArgList*)node)->Add( astn[ astn_index ] );
			break;
		case 72: //	ConstString	->	 ConstString  const_string 	// Orig. rule 51
			if( astn[ astn_index - 1 ]->getType() != DsrASTT_ConstStringList )
				throw EMID_INTERNAL_ERROR;
			((CDsrAST_ConstStringList*)astn[ astn_index - 1 ])->
					Add( ConstructConstString( 1 ) );
			break;
		case 73: //	ConstString	->	 const_string 	// Orig. rule 52
			node = new CDsrAST_ConstStringList();
			node->line = LexStack[ LexPoint - 1 ].GetLine();
			((CDsrAST_ConstStringList*)node)->Add( ConstructConstString( 1 ) );
			astn[ astn_index ] = node;
			astn_index++;
			break;
		case 75: //	CompoundId	->	 CompoundId  dot  PrimaryId 	// Orig. rule 44
			astn_index--;
			node = astn[ astn_index - 1 ];
			if( node->getType() != DsrASTT_CompoundID || 
				astn[ astn_index ]->getType() != DsrASTT_PrimaryID )
				throw EMID_INTERNAL_ERROR;
			if( !((CDsrAST_ArgList*)node)->getSeparator() )
				((CDsrAST_ArgList*)node)->setSeparator( ConstructSeparator( 2 ) );
			((CDsrAST_CompoundID*)node)->Add( (CDsrAST_PrimaryID*)astn[ astn_index ] );
			break;
		case 74: //	CompoundId	->	 PrimaryId 	// Orig. rule 45
			if( astn[ astn_index - 1 ]->getType() != DsrASTT_PrimaryID )
				throw EMID_INTERNAL_ERROR;
			node = new CDsrAST_CompoundID();
			node->line = LexStack[ LexPoint - 1 ].GetLine();
			((CDsrAST_CompoundID*)node)->Add( (CDsrAST_PrimaryID*)astn[ astn_index - 1 ] );
			astn[ astn_index - 1 ] = node;
			break;
		case 77: //	TypeSpec	->	 op_rel_in  PrimaryId  lb  ArgList  rb 	// Orig. rule 49
			if( astn[ astn_index - 2 ]->getType() != DsrASTT_PrimaryID ||
				astn[ astn_index - 1 ]->getType() != DsrASTT_ArgList )
				throw EMID_INTERNAL_ERROR;
			node = new CDsrAST_TypeSpec(
						ConstructOp( 5 ),
						(CDsrAST_PrimaryID*)astn[ astn_index - 2 ],
						ConstructSeparator( 3 ),
						ConstructSeparator( 1 ),
						(CDsrAST_ArgList*)astn[ astn_index - 1 ] );
			node->line = LexStack[ LexPoint - 1 ].GetLine();
			astn_index --;
			astn[ astn_index - 1 ] = node;
			break;
		case 76: //	TypeSpec	->	 op_rel_in  PrimaryId 	// Orig. rule 50
			if( astn[ astn_index - 1 ]->getType() != DsrASTT_PrimaryID )
				throw EMID_INTERNAL_ERROR;
			node = new CDsrAST_TypeSpec(
						ConstructOp( 2 ),
						(CDsrAST_PrimaryID*)astn[ astn_index - 1 ],
						(CDsrAST_Separator*)0,
						(CDsrAST_Separator*)0,
						(CDsrAST_ArgList*)0 );
			node->line = LexStack[ LexPoint - 1 ].GetLine();
			astn[ astn_index - 1 ] = node;
			break;
		case 78: //	PrimaryId	->	 SpecialOP 	// Orig. rule 47
			if( astn[ astn_index - 1 ]->getType() != DsrASTT_Special )
				throw EMID_INTERNAL_ERROR;
			node = new CDsrAST_PrimaryID( 
							-1, 
							(CDsrAST_Special*)astn[ astn_index - 1 ] );
			node->line = LexStack[ LexPoint - 1 ].GetLine();
			astn[ astn_index - 1 ] = node;
			break;
		case 79: //	PrimaryId	->	 identifier 	// Orig. rule 46
		case 80: //	PrimaryId	->	 ask 	// Orig. rule 48
			if( IsFlag( PF_LOCALS__PREPARE_FUNCTION_DEF ) )
			{
				astn[ astn_index++ ] = ConstructPrimaryID_identifier( 1, IsFlag( PF_LOCALS__PREPARE_FUNCTION_DEF__LVALUE ) );
				ClearFlag( PF_LOCALS__PREPARE_FUNCTION_DEF__LVALUE );
				ClearFlag( PF_LOCALS__PREPARE_FUNCTION_DEF );
				SetFlag__PF_LOCALS__FUNCTION_DEF();
				SetFlag( PF_LOCALS__FUNCTION_ARGS_DEF );
			}
			else
				astn[ astn_index++ ] = ConstructPrimaryID_identifier( 1 );
			break;
		case 82: //	SpecialOP	->	 sp_identifier  lb  rb 	// Orig. rule 81
			astn[ astn_index++ ] = 	
						new CDsrAST_Special(	
								ConstructPrimaryID_identifier( 3 ), 
								ConstructSeparator( 2 ),
								ConstructSeparator( 1 ),
								(CDsrAST_ArgList*)0
				 		);
			astn[ astn_index - 1 ]->line = LexStack[ LexPoint - 1 ].GetLine();
			break;
		case 81: //	SpecialOP	->	 sp_identifier  lb  ArgList  rb 	// Orig. rule 80
			if( astn[ astn_index - 1 ]->getType() != DsrASTT_ArgList )
				throw EMID_INTERNAL_ERROR;
			astn[ astn_index - 1 ] = new CDsrAST_Special(	
										ConstructPrimaryID_identifier( 4 ), 
										ConstructSeparator( 3 ),
										ConstructSeparator( 1 ),
										(CDsrAST_ArgList*)astn[ astn_index - 1 ]
				 					 );
			astn[ astn_index - 1 ]->line = LexStack[ LexPoint - 1 ].GetLine();
			break;
		case 83: //	SpecialOP	->	 sp_identifier 	// Orig. rule 82
			astn[ astn_index++ ] = 	
						new CDsrAST_Special(	
								ConstructPrimaryID_identifier( 1 ), 
								(CDsrAST_Separator*)0,
								(CDsrAST_Separator*)0,
								(CDsrAST_ArgList*)0
				 		);
			astn[ astn_index - 1 ]->line = LexStack[ LexPoint - 1 ].GetLine();
			break;
		case 85: //	ObjHead		->	 ObjModifier  PrimaryId  ObjArg 	// Orig. rule 86
			if( astn[ astn_index - 3 ]->getType() != DsrASTT_Keyword ||
				astn[ astn_index - 2 ]->getType() != DsrASTT_PrimaryID ||
				astn[ astn_index - 1 ]->getType() != DsrASTT_ObjArg)
				throw EMID_INTERNAL_ERROR;
			node = new CDsrAST_ObjHead(
						(CDsrAST_Keyword*)astn[ astn_index - 3 ],
						(CDsrAST_PrimaryID*)astn[ astn_index - 2 ],
						(CDsrAST_ObjArg*)astn[ astn_index - 1 ]
									);
			((CDsrAST_ObjArg*)astn[ astn_index - 1 ])->setKeywordType( ((CDsrAST_Keyword*)astn[ astn_index - 3 ])->getKeywordType() );
			node->line = LexStack[ LexPoint - 1 ].GetLine();
			astn_index -= 2;
			astn[ astn_index - 1 ] = node;
			ClearFlag( PF_LOCALS__FUNCTION_ARGS_DEF );
			break;
		case 84: //	ObjHead		->	 ObjModifier  PrimaryId 	// Orig. rule 85
			if( astn[ astn_index - 2 ]->getType() != DsrASTT_Keyword ||
				astn[ astn_index - 1 ]->getType() != DsrASTT_PrimaryID )
				throw EMID_INTERNAL_ERROR;
			node = new CDsrAST_ObjHead(
						(CDsrAST_Keyword*)astn[ astn_index - 2 ],
						(CDsrAST_PrimaryID*)astn[ astn_index - 1 ],
						(CDsrAST_ObjArg*)0
									);
			node->line = LexStack[ LexPoint - 1 ].GetLine();
			astn_index --;
			astn[ astn_index - 1 ] = node;
			ClearFlag( PF_LOCALS__FUNCTION_ARGS_DEF );
			break;
		case 86: //	ObjModifier	->	 kw_rational_function 	// Orig. rule 90
			if( IsFlag( PF_LOCALS__FUNCTION_DEF ) )
			{
				SetError( _T("local rational function definitions are illegal"), LexStack[ LexPoint - 1 ].GetLine() );
				/*if( OnSyntaxError() == -1 )
					SetSemanticError();
				break;*/
			}
			SetFlag( PF_LOCALS__PREPARE_FUNCTION_DEF__LVALUE );
			SetFlag( PF_LOCALS__PREPARE_FUNCTION_DEF );
			astn[ astn_index++ ] = ConstructKeyword( 1, DSRDATA_TYPE_RATIONAL_FUN );
			break;
		case 87: //	ObjModifier	->	 kw_polynomial 	// Orig. rule 89
			if( IsFlag( PF_LOCALS__FUNCTION_DEF ) )
			{
				SetError( _T("local polynomial definitions are illegal"), LexStack[ LexPoint - 1 ].GetLine() );
				/*if( OnSyntaxError() == -1 )
					SetSemanticError();
				break;*/
			}
			SetFlag( PF_LOCALS__PREPARE_FUNCTION_DEF__LVALUE );
			SetFlag( PF_LOCALS__PREPARE_FUNCTION_DEF );
			astn[ astn_index++ ] = ConstructKeyword( 1, DSRDATA_TYPE_POLYNOM );
			break;
		case 88: //	ObjModifier	->	 kw_group 	// Orig. rule 88
			if( IsFlag( PF_LOCALS__FUNCTION_DEF ) )
			{
				SetError( _T("local group definitions are illegal"), LexStack[ LexPoint - 1 ].GetLine() );
				/*if( OnSyntaxError() == -1 )
					SetSemanticError();
				break;*/
			}
			SetFlag( PF_LOCALS__PREPARE_FUNCTION_DEF__LVALUE );
			SetFlag( PF_LOCALS__PREPARE_FUNCTION_DEF );
			astn[ astn_index++ ] = ConstructKeyword( 1, DSRDATA_TYPE_ABS_GROUP );
			break;
		case 89: //	ObjModifier	->	 kw_function 	// Orig. rule 87
			if( IsFlag( PF_LOCALS__FUNCTION_DEF ) )
			{
				SetError( _T("local function definitions are illegal"), LexStack[ LexPoint - 1 ].GetLine() );
				/*if( OnSyntaxError() == -1 )
					SetSemanticError();
				break;*/
			}
			ClearFlag( PF_LOCALS__PREPARE_FUNCTION_DEF__LVALUE );
			SetFlag( PF_LOCALS__PREPARE_FUNCTION_DEF );
			astn[ astn_index++ ] = ConstructKeyword( 1, DSRDATA_TYPE_FUNCTION );
			break;
		case 90: //	ObjArg		->	 ObjParamList  TypeSpec 	// Orig. rule 92
			if( astn[ astn_index - 2 ]->getType() != DsrASTT_ObjArg ||
				astn[ astn_index - 1 ]->getType() != DsrASTT_TypeSpec )
				throw EMID_INTERNAL_ERROR;
			((CDsrAST_ObjArg*)astn[ astn_index - 2 ])->setTypeSpec( (CDsrAST_TypeSpec*)astn[ astn_index - 1 ] );
			astn_index --;
			break;
		case 91: //	ObjArg		->	 ObjParamList 	// Orig. rule 91
			if( astn[ astn_index - 1 ]->getType() != DsrASTT_ObjArg )
				throw EMID_INTERNAL_ERROR;
			break;
		case 92: //	ObjParamList	->	 lb  rb 	// Orig. rule 94
			node = new CDsrAST_ObjArg(
						ConstructSeparator( 2 ),
						ConstructSeparator( 1 ),
						(CDsrAST_ArgList*)0
									);
			node->line = LexStack[ LexPoint - 1 ].GetLine();
			astn[ astn_index ] = node;
			astn_index ++;
			break;
		case 93: //	ObjParamList	->	 lb  ArgList  rb 	// Orig. rule 93
			if( astn[ astn_index - 1 ]->getType() != DsrASTT_ArgList )
				throw EMID_INTERNAL_ERROR;
			node = new CDsrAST_ObjArg(
						ConstructSeparator( 3 ),
						ConstructSeparator( 1 ),
						(CDsrAST_ArgList*)astn[ astn_index - 1 ]
									);
			node->line = LexStack[ LexPoint - 1 ].GetLine();
			astn[ astn_index - 1 ] = node;
			break;
		}
	}
	catch( enum ErrorMessageID emid )
	{
		long _line;
		if( actions->RuleLen[ RuleNumber - 1 ] )
			_line = LexStack[ LexPoint - actions->RuleLen[ RuleNumber - 1 ] ].GetLine();
		else
			_line = GetLine();
		SetError( ErrorSpeaker.getMessage(emid), _line );
		SetSemanticError();
	}
}

int DSRText_Parser::OnSyntaxError( void )
{
	ActionItem *act = 0;
	int i = 0, j, k, st2 = 0, is_final = 0;
/*
	// for general case only -> is final (in the rule) state?
	for( i = 0; i < LEX_eof; i++ )
	{
		if( ( *actions )( TopState(), i )->select == _Reduce )
		{
			is_final = 1;
			break;
		}
	}
*/
	act = ( *actions )( TopState(), LEX_eol );
	if( /*is_final && */act->select == _Reduce )
	{// state is final and ';' is in the FOLLOW set of this state
		SetFlag( PF_ERROR_REPORT_SILENT_MODE_4CURRENT_SMB );
		OnReduce( act->value, 
				  actions->RuleLen[ act->value - 1 ], 
				  actions->NonTerms[ act->value - 1 ] );
	}// state is final
	else
	{// state is not final
		// try to find a Stmt-acceptable state
		for( j = StatePoint - 1; j >= 0; j-- )
		{
			act = ( *actions )( StateStack[ j ], LEX_Stmt );
			if( act->select == _Shift )
			{
				st2 = act->value;
				break;
			}
		}
		if( j < 0 )		// it is necessary to have a state "Stmt start"
			return -1;	// in the stack to restore after syntax error

		for(;;)
		{
			act = ( *actions )( st2, getCurrentSmb().LexType );
			if( act->select != _Error )
			{//=== erroneous symbol is acceptable in a state "Stmt start"
				OnError_CleanParseStack2Stmt( j );
				break;
			}
			else if( getCurrentSmb().LexType == LEX_eof )
			{
				for( k = j - 1; k >= 0; k-- )
				{
					act = ( *actions )( StateStack[ k ], LEX_Stmt );
					if( act->select == _Shift )
					{
						st2 = act->value;
						act = ( *actions )( st2, LEX_eof );
						if( act->select != _Error )
							break;
					}
				}
				if( k < 0 )		// it is necessary to have a state "Stmt start"
					return -1;	// in the stack to restore after syntax error
								// also there must be non-erroneous responce
								// to the EOF lexem after Stmt shift
				OnError_CleanParseStack2Stmt( k );
				break;
			}
			else if( act->select == _Error )
			{//=== erroneous symbol is not eof and is not acceptable in a state "Stmt start"
				// push erroneous symbol
				StatePush( 0/*erroneous state*/ );
				LexPush( getCurrentSmb() );
				// read the next lexem
				while( OnReadLexem() == -1 );
				// continue;
			} 
		}
	}// state is not final
	return 0;
}

void DSRText_Parser::OnError_CleanParseStack2Stmt( long stmt_acceptable_state )
{
	long i, k;
	// construct "dummy" Stmt nonterminal
	CDsrAST_StmtList *stmt;
	CDsrAST_Node *node;
	stmt = new CDsrAST_StmtList();
	for( k = 1, i = LexPoint - 1; i >= stmt_acceptable_state && astn_index >= k; i-- )
	{
		if( IS_TERMINAL_LEXEM( LexStack[ i ].LexType ) )
			node = ConstructAST4Lexem( LexPoint - i );
		else
		{
			node = astn[ astn_index - k ];
			k++;
		}
		stmt->AddLeft( node );
	}
	astn_index -= k - 1;
	astn[ astn_index++ ] = stmt;

	k = LexPoint - stmt_acceptable_state;
	IgnorePop( (unsigned short) k );
	Pop( (unsigned short) k );

	setLexerBuffer( Lexem( LEX_Stmt ), LEXSTATUS_NT );
	setLexerStatus( LEXSTATUS_NT );
}

long DSRText_Parser::LookupOrAdd2STable( QString str, 
										 CParseDsrSymbolTable*& wheretable,
										 int is_lvalue )
{
	long *id;

	if( IsFlag( PF_LOCALS__FUNCTION_ARGS_DEF ) )
	{
		if( IsFlag( PF_LOCALS__FUNCTION_DEF ) )
		{
			if( !ptr_local_symbol_table )
				throw EMID_INTERNAL_ERROR;
			if( (id = ptr_local_symbol_table->is_find( str )) != 0 )
			{
				wheretable = ptr_local_symbol_table;
				return *id;
			}
			else if( (id = ptr_symbol_table->is_find( str )) != 0 &&
					  ptr_symbol_table->is_predefined( *id ) )
			{
				wheretable = 0;
				return *id;
			}
			else
			{
				wheretable = ptr_local_symbol_table;
				return ptr_local_symbol_table->Add( CParseDsrSymbol( str, is_lvalue ? SMBTABLE_TYPE_VARIABLE : SMBTABLE_TYPE_FUNCTION ) );
			}
		}
		else
			throw EMID_INTERNAL_ERROR;
	}
	else
	{	
		if( IsFlag( PF_LOCALS__FUNCTION_DEF ) )
		{
			if( !ptr_local_symbol_table )
				throw EMID_INTERNAL_ERROR;
			if( (id = ptr_local_symbol_table->is_find( str )) != 0 )
			{
				wheretable = ptr_local_symbol_table;
				return *id;
			}
		}
		wheretable = 0;
		if( (id = ptr_symbol_table->is_find( str )) == 0 )
			return ptr_symbol_table->Add( CParseDsrSymbol( str, is_lvalue ? SMBTABLE_TYPE_VARIABLE : SMBTABLE_TYPE_FUNCTION ) );
		else
			return *id;
	}
}
