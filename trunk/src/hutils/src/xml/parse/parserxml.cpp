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

/////////////////////////////////////////////////////////////////////////////

#include "parserxml.h"
#include "thelexxml.h"

//*****************************************************************************
// TXML_Parser::SetSemanticError - redefines base-class function to hook
//                                 error state and reallocate memory
//*****************************************************************************
void TXML_Parser::SetSemanticError( void )
{
	for( long i = 0; i < astn_index; i++ )
	{
		if( astn[ i ] )
		{
			delete astn[ i ];
			astn[ i ] = 0;
		}
	}
	BaseParser::SetSemanticError();
}

//*****************************************************************************
// TXML_Parser::NewLex - define pointer to class-scaner.
//*****************************************************************************
void TXML_Parser::NewLex( Source& aSource )
{
  scaner = new TXML_Lex( aSource );
}

//*****************************************************************************
// TheParser::BeforeMake (m) - function is called before parsing.
//*****************************************************************************
void TXML_Parser::BeforeMake( void )
{
	SemanticErrorHappened = false;
	BaseParser::BeforeMake();
	astn_index = 0;
	astn.Flush();
}

//*****************************************************************************
// TXML_Parser::AfterMake (m) - function is called if parsing successfully finished.
//*****************************************************************************
void TXML_Parser::AfterMake( void )
{
}

//*****************************************************************************
// TXML_Parser::GenCode (m) - generate code while syntax analiser works.
// Params  : RuleNumber - number of grammar rule, which is 
//                        used by parser in reduce.
//
// Your should define function's actions ( depending of rule number ) if your 
// wish your compiler to do something more than analise syntax text structure.
//*****************************************************************************
#define _LEXSTR(c)		(*(LexStack[LexPoint - (c)].getString()))
#define _LEXSTRPTR(c)	(LexStack[LexPoint - (c)].getString())
#define _LEXLN(c)		(LexStack[LexPoint - (c)].GetLine())
#define _LEXCOL(c)		(LexStack[LexPoint - (c)].GetCol())
#define _LEXSHIFT(c)	(LexStack[LexPoint - (c)].GetShift())
#define _MATCHERR(a,b)	(_LEXSTR(a) != _LEXSTR(b))
#define _CHECKMATCH(a,b)		if(_MATCHERR(a, b)) \
								{ \
									QString buf = QString("mismatch of starting tag %1 and ending tag %2").arg(_LEXSTR(2)).arg(_LEXSTR(6)); \
									SetError( buf, _LEXLN(b) ); \
									SetSemanticError(); \
									return; \
								}

void TXML_Parser::GenCode( unsigned int RuleNumber )
{
	if( IsSemanticError() ) return;

	QString empty;
	AST_TXML_Node *node;

	try
	{
		switch( RuleNumber )
		{
		case 3:	// Document	->	 Prolog  Item  Misc 	// Orig. rule 3
		case 5:	// Document	->	 Item  Misc 	// Orig. rule 4
		case 2:	// Document	->	 Item 	// Orig. rule 5
		case 4:	// Document	->	 Prolog  Item 	// Orig. rule 2
			--astn_index;
			switch( astn[ astn_index ]->getNodeType() )
			{
			case ASTT_TXML_Tag:
				*the_storage = new AST_TXML_Compound();
				(*the_storage)->Add( astn[ astn_index ] );
				break;
			case ASTT_TXML_Compound:
				*the_storage = (AST_TXML_Compound*)astn[ astn_index ];
				break;
			default:
				break;
			}
		break;

		case 9:	// Item		->	 <  id  >  Body  '</'  id  > 	// Orig. rule 16
				_CHECKMATCH(2, 6);
				if( astn[ astn_index - 1 ]->getNodeType() == ASTT_TXML_Compound ) 
					node = new AST_TXML_Tag( _LEXSTRPTR(2), 0, (AST_TXML_Compound*)astn[ astn_index - 1 ] );
				else if( astn[ astn_index - 1 ]->getNodeType() == ASTT_TXML_Value )
					node = new AST_TXML_Tag( _LEXSTRPTR(2), 0, (AST_TXML_Value*)astn[ astn_index - 1 ] );
				else
					throw "Internal error";
				node->column = _LEXCOL(6);
				node->line = _LEXLN(6);
				node->shift1 = _LEXSHIFT(6);
				node->shift2 = _LEXSHIFT(2);
				astn[ astn_index - 1 ] = node;
		break;

		case 13:	// Item		->	 <  id  AttL  >  Body  '</'  id  > 	// Orig. rule 19
				_CHECKMATCH(2, 7);
				if( astn[ astn_index - 2 ]->getNodeType() != ASTT_TXML_Compound ) 
					throw "Internal error";
				if( astn[ astn_index - 1 ]->getNodeType() == ASTT_TXML_Compound ) 
					node = new AST_TXML_Tag( _LEXSTRPTR(2), (AST_TXML_Compound*)astn[ astn_index - 2 ],
											(AST_TXML_Compound*)astn[ astn_index - 1 ] );
				else
				{
					if( astn[ astn_index - 1 ]->getNodeType() == ASTT_TXML_Value ) 
						node = new AST_TXML_Tag( _LEXSTRPTR(2), (AST_TXML_Compound*)astn[ astn_index - 2 ],
										 		(AST_TXML_Value*)astn[ astn_index - 1 ] );
					else
						throw "Internal error";
				}
				node->column = _LEXCOL(7);
				node->line = _LEXLN(7);
				node->shift1 = _LEXSHIFT(7);
				node->shift2 = _LEXSHIFT(2);
				astn[ astn_index - 2 ] = node;
				astn_index --;
		break;

		case 11:	// Item		->	 <  id  >  '</'  id  > 	// Orig. rule 17
				_CHECKMATCH(2, 5);
				node = new AST_TXML_Tag( _LEXSTRPTR(2), 0, (AST_TXML_Compound*)0 );
				node->column = _LEXCOL(5);
				node->line = _LEXLN(5);
				node->shift1 = _LEXSHIFT(5);
				node->shift2 = _LEXSHIFT(2);
				astn[ astn_index++ ] = node;
		break;

		case 14:	// Item		->	 <  id  AttL  >  '</'  id  > 	// Orig. rule 20
				_CHECKMATCH(2, 6);
				if( astn[ astn_index - 1 ]->getNodeType() != ASTT_TXML_Compound ) 
					throw "Internal error";
				node = new AST_TXML_Tag( _LEXSTRPTR(2), (AST_TXML_Compound*)astn[ astn_index - 1 ],
									 	(AST_TXML_Compound*)0 );
				node->column = _LEXCOL(6);
				node->line = _LEXLN(6);
				node->shift1 = _LEXSHIFT(6);
				node->shift2 = _LEXSHIFT(2);
				astn[ astn_index - 1 ] = node;
		break;

		case 12:	// Item		->	 <  id  '/>' 	// Orig. rule 18
				node = new AST_TXML_Tag( _LEXSTRPTR(2), 0, (AST_TXML_Compound*)0, FL_XML_SELF_CLOSE_TAG );
				node->column = _LEXCOL(2);
				node->line = _LEXLN(2);
				node->shift1 = _LEXSHIFT(2);
				node->shift2 = -1;
				astn[ astn_index++ ] = node;
		break;

		case 10:	// Item		->	 <  id  AttL  '/>' 	// Orig. rule 21
				if( astn[ astn_index - 1 ]->getNodeType() != ASTT_TXML_Compound ) 
					throw "Internal error";
				node = new AST_TXML_Tag( _LEXSTRPTR(3), (AST_TXML_Compound*)astn[ astn_index - 1 ],
										 (AST_TXML_Compound*)0 );
				node->column = _LEXCOL(3);
				node->line = _LEXLN(3);
				node->shift1 = _LEXSHIFT(3);
				node->shift2 = -1;
				astn[ astn_index - 1 ] = node;
		break;

		case 29:	// AttLPI	->	 AttRPI 	// Orig. rule 46
		case 50:	// AttL		->	 AttR 	// Orig. rule 29
			node = new AST_TXML_Compound();
			node->column = _LEXCOL(1);
			node->line = _LEXLN(1);
			((AST_TXML_Compound*)node)->Add( astn[ astn_index - 1 ] );
			astn[ astn_index - 1 ] = node;
		break;

		case 28:	// AttLPI	->	 AttLPI  AttRPI 	// Orig. rule 45
		case 49:	// AttL		->	 AttL  AttR 	// Orig. rule 28
			astn_index--;
			node = astn[ astn_index - 1 ];
			if( node->getNodeType() != ASTT_TXML_Compound ) 
				throw "Internal error";
			((AST_TXML_Compound*)node)->Add( astn[ astn_index ] );
		break;

		case 17:	// XMLDecl		->	 '<?'  kw_xml  '/>' 	// Orig. rule 21
		case 20:	// XMLDecl		->	 '<?'  kw_xml  '?>' 	// Orig. rule 19
		break;
		case 18:	// XMLDecl		->	 '<?'  kw_xml  AttLPI  '/>' 	// Orig. rule 20
		case 19:	// XMLDecl		->	 '<?'  kw_xml  AttLPI  '?>' 	// Orig. rule 18
			if( astn[ astn_index - 1 ]->getNodeType() != ASTT_TXML_Compound )
				throw "Internal error";
			delete astn[ astn_index - 1 ];
			astn_index--;
		break;

		case 31:	// PI		->	 '<?'  id  '?>' 	// Orig. rule 15
				node = new AST_TXML_Tag( _LEXSTRPTR(2), 0, (AST_TXML_Compound*)0, FL_XML_PROCESSING_INSTRUCTION );
				node->column = _LEXCOL(2);
				node->line = _LEXLN(2);
				node->shift1 = _LEXSHIFT(2);
				node->shift2 = -1;
				astn[ astn_index++ ] = node;
		break;
		
		case 32:	// PI		->	 '<?'  id  '/>' 	// Orig. rule 17
				node = new AST_TXML_Tag( _LEXSTRPTR(2), 0, (AST_TXML_Compound*)0, FL_XML_PROCESSING_INSTRUCTION | FL_XML_SELF_CLOSE_TAG );
				node->column = _LEXCOL(2);
				node->line = _LEXLN(2);
				node->shift1 = _LEXSHIFT(2);
				node->shift2 = -1;
				astn[ astn_index++ ] = node;
		break;

		case 33:	// PI		->	 '<?'  id  AttLPI  '?>' 	// Orig. rule 14
				if( astn[ astn_index - 1 ]->getNodeType() != ASTT_TXML_Compound ) 
					throw "Internal error";
				node = new AST_TXML_Tag( _LEXSTRPTR(3), (AST_TXML_Compound*)astn[ astn_index - 1 ],
										 (AST_TXML_Compound*)0, FL_XML_PROCESSING_INSTRUCTION );
				node->column = _LEXCOL(3);
				node->line = _LEXLN(3);
				node->shift1 = _LEXSHIFT(3);
				node->shift2 = -1;
				astn[ astn_index - 1 ] = node;
		break;

		case 30:	// PI		->	 '<?'  id  AttLPI  '/>' 	// Orig. rule 16
				if( astn[ astn_index - 1 ]->getNodeType() != ASTT_TXML_Compound ) 
					throw "Internal error";
				node = new AST_TXML_Tag( _LEXSTRPTR(3), (AST_TXML_Compound*)astn[ astn_index - 1 ],
										 (AST_TXML_Compound*)0, FL_XML_PROCESSING_INSTRUCTION | FL_XML_SELF_CLOSE_TAG );
				node->column = _LEXCOL(3);
				node->line = _LEXLN(3);
				node->shift1 = _LEXSHIFT(3);
				node->shift2 = -1;
				astn[ astn_index - 1 ] = node;
		break;

		case 43:	// Body		->	 text 	// Orig. rule 26
			astn[ astn_index ] = new AST_TXML_Value( _LEXSTRPTR(1) );
			astn[ astn_index ]->column = _LEXCOL(1);
			astn[ astn_index ]->line = _LEXLN(1);
			astn_index++;
			//break;	//!!! - reuse the next statement
		case 44:	// Body		->	 PI 	// Orig. rule 44
		case 45:	// Body		->	 Item 	// Orig. rule 25
			node = new AST_TXML_Compound();
			node->column = _LEXCOL(1);
			node->line = _LEXLN(1);
			((AST_TXML_Compound*)node)->Add( astn[ astn_index - 1 ] );
			astn[ astn_index - 1 ] = node;
		break;

		case 47:	// Body		->	 Body  text 	// Orig. rule 23
			astn[ astn_index ] = new AST_TXML_Value( _LEXSTRPTR(1) );
			astn[ astn_index ]->column = _LEXCOL(1);
			astn[ astn_index ]->line = _LEXLN(1);
			astn_index++;
			//break;	//!!! - reuse the next statement
		case 46:	// Body		->	 Body  PI 	// Orig. rule 41
		case 48:	// Body		->	 Body  Item 	// Orig. rule 22
			astn_index--;
			node = astn[ astn_index - 1 ];
			if( node->getNodeType() == ASTT_TXML_Tag )
			{
				SetError( QString("syntax error: expected ending tag %1").arg( ((AST_TXML_Tag*)node)->getTagName() ), _LEXLN(1) );
				SetSemanticError();
			}
			else if( node->getNodeType() == ASTT_TXML_Value )
			{
				SetError( QString("syntax error: expected ending tag"), _LEXLN(1) );
				SetSemanticError();
			}
			else if( node->getNodeType() == ASTT_TXML_Compound )
				((AST_TXML_Compound*)node)->Add( astn[ astn_index ] );
			else
			{
				SetError( QString("syntax error"), _LEXLN(1) );
				SetSemanticError();
			}
			break;
		break;

		case 40:	// CSTR		->	 CSTR  const_string 	// Orig. rule 55
			node = astn[ astn_index - 1 ];
			if( node->getNodeType() != ASTT_TXML_Value || LexStack[ LexPoint - 1 ].Smth == 0 )
				throw "Internal error";
			((AST_TXML_Value*)node)->getText() += _LEXSTR(1);
		break;
		case 41:	// CSTR		->	 const_string 	// Orig. rule 56
			astn[ astn_index ] = new AST_TXML_Value( _LEXSTRPTR(1) );
			astn[ astn_index ]->column = _LEXCOL(1);
			astn[ astn_index ]->line = _LEXLN(1);
			astn_index++;
		break;

		case 52:	// AttRPI		->	 id 	// Orig. rule 48
			astn[ astn_index ] = new AST_TXML_Attribute( &empty, _LEXSTRPTR(1) );
			astn[ astn_index ]->column = _LEXCOL(1);
			astn[ astn_index ]->line = _LEXLN(1);
			astn_index++;
		break;

		case 53:	// AttR		->	 id  =  CSTR 	// Orig. rule 32
		{
			if( astn[ astn_index - 1 ]->getNodeType() != ASTT_TXML_Value )
				throw "Internal error";
			AST_TXML_Value *nodeval = (AST_TXML_Value*)(astn[ astn_index - 1 ]);
			node = new AST_TXML_Attribute( _LEXSTRPTR(3), &(nodeval->getText()) );
			node->column = _LEXCOL(3);
			node->line = _LEXLN(3);
			delete astn[ astn_index - 1 ];
			astn[ astn_index - 1 ] = node;
		}
		break;

		case 54:	// AttR		->	 id  =  id 	// Orig. rule 32
			astn[ astn_index ] = new AST_TXML_Attribute( _LEXSTRPTR(3), _LEXSTRPTR(1) );
			astn[ astn_index ]->column = _LEXCOL(3);
			astn[ astn_index ]->line = _LEXLN(3);
			astn_index++;
		break;                                             

		case 55:	// AttR		->	 id  =  const_real 	// Orig. rule 31
			astn[ astn_index ] = new AST_TXML_Attribute( _LEXSTRPTR(3), (double*)(LexStack[ LexPoint - 1 ].Smth) );
			astn[ astn_index ]->column = _LEXCOL(3);
			astn[ astn_index ]->line = _LEXLN(3);
			astn_index++;
		break;

		case 56:	// AttR		->	 id  =  const_int 	// Orig. rule 30
			astn[ astn_index ] = new AST_TXML_Attribute( _LEXSTRPTR(3), (long*)&(LexStack[ LexPoint - 1 ].Index) );
			astn[ astn_index ]->column = _LEXCOL(3);
			astn[ astn_index ]->line = _LEXLN(3);
			astn_index++;
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

/////////////////////////////////////////////////////////////////////////////

