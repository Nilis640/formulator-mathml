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

#include "baselex.h"
#include "HUtils/iustring.h"

Lex::~Lex( void )
{
}

//*****************************************************************************
// Lex::IsSpaceNextChar - checks is the next character space and skip it if 
//                        true; correctly deals with the lines' counter.
// Returns : 1 if space character, 0 otherwise.
//*****************************************************************************
int Lex::IsSpaceNextChar( void )
{
	QChar ch = NextChar();
	if( _isspace( ch ) )
		return 1;
	else
	{
		PutBack();
		return 0;
	}  
}

//*****************************************************************************
// Lex::NextLexem (m) - returns next extracted lexem.
// Your should define the function yourself.
//*****************************************************************************
Lexem Lex::NextLexem( void )
{
	return Lexem( 0 );
}


//*****************************************************************************
// Lex::RemTermination - Terminates remarks.
// Returns : -1 if <NextLexem> must returns something, 0 otherwise.
//*****************************************************************************
int Lex::RemTermination( Lexem& ret )
//       ~~~~~~~~~~~~~~
{
	bool is_slsl;
	QString startRem, endRem;

	//*** Remarks' termination ***
	while( NextChar() == '/' )
	{
		if( NextChar() == '*' )         // Remark like this  =>  /*...*/
		{
			startRem = QString("/*");
			endRem = QString("*/");
			is_slsl = false;
		}
		else
		{
			PutBack();
			if( NextChar() == _T('/') )      // Remark like this  =>  //...
			{
				startRem = QString("//");
				endRem = QString("\r\n");
				is_slsl = true;
			}
			else
			{
				PutBack();
				break;
			}
		}

		QChar ch;
		while( !_iseof( SoleChar() ) && !isLA( endRem ) )
		{
			ch = NextChar();
			if( !is_slsl && (ch == _T('\x0D')) )
			{
				if( NextChar() == _T('\x0A') )
					line++;
				else
					PutBack();
			}
		}
		if( _iseof( SoleChar() ) )
		{
			if( is_slsl )
			{
				ret.LexType = __LEX_eof;
				PutBack();
			}
			else
			{
				lex_err = Lex::UnterminatedComment;
				ret.LexType = 0 /*LEX_NONE*/;
				PutBack();
			}
			return -1;
		}
		else
		{
			Inc( endRem.length() );
			if( is_slsl ) line++;
		}

		//*** Space symbols' termination ***
                while( IsSpaceNextChar() ) ;
	}
	PutBack();
	return 0;
}
