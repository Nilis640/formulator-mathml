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

#include "vmi.h"
#include "parser.h"

//*****************************************************************************
// BulanParser::GetLexError (m) - returns error message on lex error.
//*****************************************************************************
const QString& BulanParser::GetLexError()
{
	static const QString LexError[] = {
		_T("Unknown error"),
		_T("Unknown symbol"),
		_T("Unterminated comment"),
		_T("Bad numeric format")
	};
	return LexError[ ( int )(scaner->GetError()) ];
}

void BulanParser::SetSemanticError()
{
	for( long i = 0; i < astn_index; i++ )
		if( astn[ i ] ) delete astn[ i ];
	BaseParser::SetSemanticError();
}

//*****************************************************************************
// BulanParser::NewLex - define pointer to class-scaner.
//*****************************************************************************
void BulanParser::NewLex( Source& aSource )
{
	scaner = new BulanLex( aSource );
}

//*****************************************************************************
// BulanParser::BeforeMake (m) - function is called before parsing.
//*****************************************************************************
void BulanParser::BeforeMake()
{
	SemanticErrorHappened = false;
	BaseParser::BeforeMake();
	astn_index = 0;
	astn.Flush();
}

//*****************************************************************************
// BulanParser::AfterMake (m) - function is called if parsing successfully finished.
//*****************************************************************************
void BulanParser::AfterMake()
{
}

//*****************************************************************************
// BulanParser::GenCode (m) - generate code while syntax analiser works.
// Params  : RuleNumber - number of grammar rule, which is 
//                        used by parser in reduce.
//
// Your should define function's actions ( depending of rule number ) if your 
// wish your compiler to do something more than analise syntax text structure.
//*****************************************************************************
void BulanParser::GenCode( unsigned int RuleNumber )
{
	if( IsSemanticError() ) return;

	FBAST_Node *node;

	try
	{
		switch( RuleNumber )
		{
		case  1:	//	MAIN		->	 STMT_LIST 	// Orig. rule 1
			*the_storage = (FBAST_Compound*)astn[ --astn_index ];
			break;
		case  2:	//	STMT_LIST	->	 STMT_LIST  STMT 	// Orig. rule 2
			astn_index--;
			node = astn[ astn_index - 1 ];
			if( node->getType() != ASTT_Compound ) throw "Internal error";
			((FBAST_Compound*)node)->Add( astn[ astn_index ] );
			break;
		case  3:	//	STMT_LIST	->	 STMT 	// Orig. rule 3
			node = new FBAST_Compound();
			node->line = LexStack[ LexPoint - 1 ].GetLine();
			((FBAST_Compound*)node)->Add( astn[ astn_index - 1 ] );
			astn[ astn_index - 1 ] = node;
			break;
		case  4:	//	STMT		->	 ident  dot  ident  assign  EXPR  eol 	// Orig. rule 5
			node = new FBAST_Assign(  new FBAST_Variable(*(QString*)(LexStack[ LexPoint - 6 ].Smth),
				*(QString*)(LexStack[ LexPoint - 4 ].Smth)),
				(FBAST_Expr*)astn[ astn_index - 1 ] );
			node->line = LexStack[ LexPoint - 4 ].GetLine();
			astn[ astn_index - 1 ] = node;
			break;
		case  5:	//	STMT		->	 ident  assign  EXPR  eol 	// Orig. rule 4
			node = new FBAST_Assign(  new FBAST_Variable( QString( _T("") ),
				*(QString*)(LexStack[ LexPoint - 4 ].Smth)),
				(FBAST_Expr*)astn[ astn_index - 1 ] );
			node->line = LexStack[ LexPoint - 4 ].GetLine();
			astn[ astn_index - 1 ] = node;
			break;
		case  6:	//	EXPR		->	 TERM 	// Orig. rule 23
			// empty
			break;
		case  7:	//	EXPR		->	 EXPR  addsub  TERM 	// Orig. rule 22
			astn_index--;
			if( LexStack[ LexPoint - 2 ].Index == _T('+') )
				node = new FBAST_Expr( ASTO_Add, astn[ astn_index - 1 ], astn[ astn_index ] );
			else
				node = new FBAST_Expr( ASTO_Sub, astn[ astn_index - 1 ], astn[ astn_index ] );
			node->line = LexStack[ LexPoint - 3 ].GetLine();
			astn[ astn_index - 1 ] = node;
			break;
		case  8:	//	TERM		->	 FACTOR 	// Orig. rule 25
			// empty
			break;
		case  9:	//	TERM		->	 TERM  muldiv  FACTOR 	// Orig. rule 24
			astn_index--;
			if( LexStack[ LexPoint - 2 ].Index == _T('*') )
				node = new FBAST_Expr( ASTO_Mul, astn[ astn_index - 1 ], astn[ astn_index ] );
			else
				node = new FBAST_Expr( ASTO_Div, astn[ astn_index - 1 ], astn[ astn_index ] );
			node->line = LexStack[ LexPoint - 3 ].GetLine();
			astn[ astn_index - 1 ] = node;
			break;
		case  10:	//	TERM		->	 addsub  TERM 	// Orig. rule 26
			if( LexStack[ LexPoint - 2 ].Index == _T('-') )
			{
				node = new FBAST_Expr( ASTO_Minus, astn[ astn_index - 1 ] );
				node->line = LexStack[ LexPoint - 2 ].GetLine();
				astn[ astn_index - 1 ] = node;
			}
			break;
		case  11:	//	FACTOR		->	 OPERAND 	// Orig. rule 27
			// empty
			break;
		case  12:	//	OPERAND		->	 const_real 	// Orig. rule 12
			astn[ astn_index ] = new FBAST_Value( 0, (double*)(LexStack[ LexPoint - 1 ].Smth) );
			astn[ astn_index ]->line = LexStack[ LexPoint - 1 ].GetLine();
			astn_index++;
			break;
		case  13:	//	OPERAND		->	 ident 	// Orig. rule 13
			astn[ astn_index ] = new FBAST_Variable( _T(""),
				*(QString*)(LexStack[ LexPoint - 1 ].Smth) );
			astn[ astn_index ]->line = LexStack[ LexPoint - 1 ].GetLine();
			astn_index++;
			break;
		case  14:	//	OPERAND		->	 ident  dot  ident 	// Orig. rule 14
			astn[ astn_index ] = new FBAST_Variable(
				*(QString*)(LexStack[ LexPoint - 3 ].Smth),
				*(QString*)(LexStack[ LexPoint - 1 ].Smth) );
			astn[ astn_index ]->line = LexStack[ LexPoint - 3 ].GetLine();
			astn_index++;
			break;
		case  15:	//	OPERAND		->	 kw_abs  lb  EXPR  rb 	// Orig. rule 15
			node = new FBAST_Function( ASTOF_Abs, (FBAST_Expr*)astn[ astn_index - 1 ] );
			node->line = LexStack[ LexPoint - 4 ].GetLine();
			astn[ astn_index - 1 ] = node;
			break;
		case  16:	//	OPERAND		->	 kw_av  lb  EXPR  coma  EXPR  rb 	// Orig. rule 16
			astn_index--;
			node = new FBAST_Function(ASTOF_Average, 
				(FBAST_Expr*)astn[ astn_index - 1 ],
				(FBAST_Expr*)astn[ astn_index ] );
			node->line = LexStack[ LexPoint - 6 ].GetLine();
			astn[ astn_index - 1 ] = node;
			break;
		case  17:	//	OPERAND		->	 kw_min  lb  EXPR  coma  EXPR  rb 	// Orig. rule 17
			astn_index--;
			node = new FBAST_Function(ASTOF_Min, 
				(FBAST_Expr*)astn[ astn_index - 1 ],
				(FBAST_Expr*)astn[ astn_index ] );
			node->line = LexStack[ LexPoint - 6 ].GetLine();
			astn[ astn_index - 1 ] = node;
			break;
		case  18:	//	OPERAND		->	 kw_max  lb  EXPR  coma  EXPR  rb 	// Orig. rule 18
			astn_index--;
			node = new FBAST_Function(ASTOF_Max, 
				(FBAST_Expr*)astn[ astn_index - 1 ],
				(FBAST_Expr*)astn[ astn_index ] );
			node->line = LexStack[ LexPoint - 6 ].GetLine();
			astn[ astn_index - 1 ] = node;
			break;
		case  19:	//	OPERAND		->	 lb  EXPR  rb 	// Orig. rule 19
			// empty
			break;
		default:
			break;
		}
	}
	catch( QString errMsg )
	{
		long _line;
		if( actions->RuleLen[ RuleNumber - 1 ] )
			_line = LexStack[ LexPoint - actions->RuleLen[ RuleNumber - 1 ] ].GetLine();
		else
			_line = GetLine();
		SetError( errMsg, _line );
		SetSemanticError();
	}
}
