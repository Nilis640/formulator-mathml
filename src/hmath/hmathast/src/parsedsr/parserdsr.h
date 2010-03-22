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

#ifndef __PARSEDSR_PARSER_H__
#define __PARSEDSR_PARSER_H__

#include "common\error\err_mess.h"

#include "common\compiler\base2\basepars.h"
#include "hmath\hmathlng\src\ast\astdsr.h"
#include "hmath\hmathlng\src\parsedsr\thelexdsr.h"
#include "hmath\hmathlng\src\symbol\symbol_table_dsr.h"

//*** Parser's flags ***
#define PF_ERROR_REPORT_SILENT_MODE_4CURRENT_SMB	0x00010000
#define PF_LOCALS__PREPARE_FUNCTION_DEF				0x00020000
#define PF_LOCALS__FUNCTION_DEF						0x00040000
#define PF_LOCALS__FUNCTION_ARGS_DEF				0x00080000
#define PF_LOCALS__PREPARE_FUNCTION_DEF__LVALUE		0x00100000
//**********************

#define DSRText_LexemVector_DefaultLength 16
class __HMATHLNGDLL__ DSRText_LexemVector : public MArray<Lexem>
{
public:
	DSRText_LexemVector( int cb = DSRText_LexemVector_DefaultLength ) : 
		MArray<Lexem>( cb, 0, cb )
	{}
};

#define LEXSTATUS_CURRENT	0
#define LEXSTATUS_LEXEM		1
#define LEXSTATUS_NT		2

class __HMATHLNGDLL__ DSRText_Parser : public BaseParser {
protected :
	MArray<DSRText_LexemVector> IgnoreStack;

	MIArray<CDsrAST_Node> astn;
	long astn_index;

	Lexem LexBuffer[ LEXSTATUS_NT + 1 ];
	long CurrentStatus;

	virtual void GenCode( unsigned int RuleNumber );
	virtual void NewLex( Source& aSource );
	virtual void BeforeMake( void );
	virtual void AfterMake ( void );
	virtual void SetSemanticError( void );

	void IgnorePop( unsigned short anRuleLength );
	void OnReduce( long _RuleNumber, long _RuleLength, long _RuleLeft );
	void OnShift( long _state );

	int OnReadLexem( void );
	int OnSyntaxError( void );
	void OnError_CleanParseStack2Stmt( long stmt_acceptable_state );

	void setLexerStatus( long st );
	long getLexerStatus( void );
	void setLexerBuffer( Lexem smb, long status );
	Lexem& getCurrentSmb( void );

public :
	CDsrASTStorage** the_storage;
	CParseDsrSymbolTable *ptr_symbol_table, *ptr_local_symbol_table;

	const QString& GetLexError( void );
	DSRText_Parser( Source& src, 
					ActionT *aActionTable, 
					Common* Info,
					CDsrASTStorage** st,
					CParseDsrSymbolTable*& smbtable );
	~DSRText_Parser( void );

	unsigned Make( void );

	// Symbol table
	CParseDsrSymbolTable* getSymbolTablePtr( void );
	long LookupSTable( QString str );
	long LookupOrAdd2STable( QString str, CParseDsrSymbolTable*& wheretable, int is_lvalue );
	void ClearFlag__PF_LOCALS__FUNCTION_DEF( void );
	void SetFlag__PF_LOCALS__FUNCTION_DEF( void );

	void ConstructAfterChain( int shift, CDsrAST_Node *node );
	void ConstructAfterChain( DSRText_LexemVector& lv, CDsrAST_Node *node );
	CDsrAST_Separator* ConstructSeparator( int shift );
	CDsrAST_Keyword* ConstructKeyword( int shift, enum CDsrDataTypes tp = DSRDATA_TYPE_NONE );
	CDsrAST_Op* ConstructOp( int shift );
	CDsrAST_ConstReal* ConstructConstReal( int shift );
	CDsrAST_ConstString* ConstructConstString( int shift );
	CDsrAST_ConstInt* ConstructConstInt( int shift );
	CDsrAST_PrimaryID* ConstructPrimaryID_identifier( int shift, int is_lvalue = 1 );

	CDsrAST_Node* ConstructAST4Lexem( int shift );
};

inline
DSRText_Parser::DSRText_Parser( Source& src, 
								ActionT *aActionTable, 
								Common* Info,
								CDsrASTStorage** st,
								CParseDsrSymbolTable*& smbtable ) :
  	BaseParser( src, aActionTable, Info ),
	astn( MIArray<CDsrAST_Node>::LeaveItems, 8, 0, 8 ),
	IgnoreStack( 16, 0, 16 )
{
	if( !smbtable ) smbtable = new CParseDsrSymbolTable();
	ptr_symbol_table = smbtable;
	ptr_local_symbol_table = 0;
	the_storage = st;
	astn_index = 0;
	SemanticErrorHappened = false;
	NewLex( src );
}

inline
void DSRText_Parser::ClearFlag__PF_LOCALS__FUNCTION_DEF( void )
{
	ptr_local_symbol_table = 0;
	ClearFlag( PF_LOCALS__FUNCTION_DEF );
}

inline
void DSRText_Parser::SetFlag__PF_LOCALS__FUNCTION_DEF( void )
{
	ptr_local_symbol_table = ptr_symbol_table->getNewLocalSmbTable();
	SetFlag( PF_LOCALS__FUNCTION_DEF );
}

inline
void DSRText_Parser::setLexerStatus( long st )
{
	CurrentStatus = st;
}

inline
long DSRText_Parser::getLexerStatus( void )
{
	return CurrentStatus;
}

inline
void DSRText_Parser::setLexerBuffer( Lexem smb, long status = LEXSTATUS_NT )
{
	switch( status )
	{
	case LEXSTATUS_LEXEM:
	case LEXSTATUS_NT:
		LexBuffer[ status ] = smb;
		break;
	}
}

inline
Lexem& DSRText_Parser::getCurrentSmb( void )
{
	return LexBuffer[ LEXSTATUS_CURRENT ];
}

inline
void DSRText_Parser::IgnorePop( unsigned short anRuleLength )
{
	for( int i = LexPoint - anRuleLength + 1; i < LexPoint + 1; i++ )
		IgnoreStack[ i ].Flush();
}

inline
long DSRText_Parser::LookupSTable( QString str )
{
	long *id;
	if( (id = ptr_symbol_table->is_find( str )) == 0 )
		return -1;
	else
		return *id;
}

inline
CParseDsrSymbolTable* DSRText_Parser::getSymbolTablePtr( void )
{
	return ptr_symbol_table;
}

///////////////////////////////////////////////////////////////////////////////

inline
void DSRText_Parser::ConstructAfterChain( int shift, CDsrAST_Node *node )
{
	ConstructAfterChain( IgnoreStack[ LexPoint - shift + 1 ], node );
}

inline
CDsrAST_Separator* DSRText_Parser::ConstructSeparator( int shift )
{
	CDsrAST_Separator *node = 
		new CDsrAST_Separator( LexStack[ LexPoint - shift ].LexType );
	ConstructAfterChain( shift, node );
	node->line = LexStack[ LexPoint - shift ].GetLine();
	return node;
}

inline
CDsrAST_Keyword* DSRText_Parser::ConstructKeyword( int shift, enum CDsrDataTypes tp )
{
	CDsrAST_Keyword *node = 
		new CDsrAST_Keyword( LookupSTable( ((ParseDsrLex*)scaner)->getLexemText( LexStack[ LexPoint - shift ] ) ), tp );
	ConstructAfterChain( shift, node );
	node->line = LexStack[ LexPoint - shift ].GetLine();
	return node;
}

inline
CDsrAST_Op* DSRText_Parser::ConstructOp( int shift )
{

	CDsrAST_Op *node = 
		new CDsrAST_Op( LexStack[ LexPoint - shift ].Index );
	ConstructAfterChain( shift, node );
	node->line = LexStack[ LexPoint - shift ].GetLine();
	return node;
}

inline
CDsrAST_ConstReal* DSRText_Parser::ConstructConstReal( int shift )
{
	CDsrAST_ConstReal *node = 
		new CDsrAST_ConstReal( 
			*(double*)(LexStack[ LexPoint - shift ].Smth), 
			((ParseDsrLex*)scaner)->getLexemText( LexStack[ LexPoint - shift ] ) );
	ConstructAfterChain( shift, node );
	node->line = LexStack[ LexPoint - shift ].GetLine();
	return node;
}

inline
CDsrAST_ConstString* DSRText_Parser::ConstructConstString( int shift )
{
	CDsrAST_ConstString *node = 
		new CDsrAST_ConstString( *(QString*)(LexStack[ LexPoint - shift ].Smth) );
	ConstructAfterChain( shift, node );
	node->line = LexStack[ LexPoint - shift ].GetLine();
	return node;
}

inline
CDsrAST_ConstInt* DSRText_Parser::ConstructConstInt( int shift )
{
	CDsrAST_ConstInt *node = 
		new CDsrAST_ConstInt( LexStack[ LexPoint - shift ].Index, 
			((ParseDsrLex*)scaner)->getLexemText( LexStack[ LexPoint - shift ] ) );
	ConstructAfterChain( shift, node );
	node->line = LexStack[ LexPoint - shift ].GetLine();
	return node;
}

inline
CDsrAST_PrimaryID* DSRText_Parser::ConstructPrimaryID_identifier( int shift, int is_lvalue )
{
	CParseDsrSymbolTable* wheretable = 0;
	CDsrAST_PrimaryID *node = 
		new CDsrAST_PrimaryID( LookupOrAdd2STable( ((ParseDsrLex*)scaner)->getLexemText( LexStack[ LexPoint - shift ] ), wheretable, is_lvalue ),
							   (CDsrAST_Special*)0 );
	node->local_smbtable = wheretable;
	ConstructAfterChain( shift, node );
	node->line = LexStack[ LexPoint - shift ].GetLine();
	return node;
}

#endif
