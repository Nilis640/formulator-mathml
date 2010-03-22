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
#include "HUtils/idfas.h"

#include "thelex.h"
#include "la_iden.h"

//*****************************************************************************
// BulanLex::RemTermination - Terminates remarks.
// Returns : -1 if <NextLexem> must returns something, 0 otherwise.
//*****************************************************************************

int BulanLex::RemTermination( Lexem& ret )
{
	bool is_slsl;
	QString startRem, endRem;

	//*** Remarks' termination ***
	while( NextChar() == _T('/') )
	{
		if( NextChar() == _T('*') )         // Remark like this  =>  /*...*/
		{
			startRem = "/*";
			endRem = "*/";
			is_slsl = false;
		}
		else
		{
			PutBack();
			if( NextChar() == _T('/') )      // Remark like this  =>  //...
			{
				startRem = "//";
				endRem = "\r\n";
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
				ret.LexType = LEX_eof;
			else
			{
				lex_err = Lex::UnterminatedComment;
				ret.LexType = LEX_NONE;
			}
			PutBack();
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

//*****************************************************************************
// BulanLex::NextLexem (m) - returns next extracted lexem.
// Your should define the function yourself.
//*****************************************************************************
Lexem BulanLex::NextLexem()
{
	Lexem ret;
	double *p_d;
	QString *p_s;

        while( IsSpaceNextChar() ) ;

	do
	{
		//*** Space symbols' termination ***
                while( IsSpaceNextChar() ) ;
		//*** Remarks' termination ***
		if( RemTermination( ret ) == -1 )
		{
			ret.line = GetLine();
			ret.col = GetCol();
			continue;
		}

		ret.line = GetLine();
		ret.col = GetCol();

		if( SoleChar().isDigit() )
		{
			LDigit ldigit;
			ret.LexType = ldigit.Analisys( getCurrentText() );
			switch( ret.LexType )
			{
			case LDigit::Integer:
				ret.LexType = LEX_const_real;
				ret.Smth = d_autoheap.Add2List( p_d = new double( ldigit.mnt_int ) );
				break;
			case LDigit::Float:
				ret.LexType = LEX_const_real;
				ldigit.mnt_float *= pow( 10.0, (int) ldigit.power );
				ret.Smth = d_autoheap.Add2List( p_d = new double( ldigit.mnt_float ) );
				break;
			case LDigit::FloatIEEE:
				ret.LexType = LEX_const_real;
				ldigit.mnt_float *= pow( 10.0, (int) (ldigit.power + ldigit.sign * ldigit.powerE) );
				ret.Smth = d_autoheap.Add2List( p_d = new double( ldigit.mnt_float ) );
				break;
			default:
				lex_err = Lex::BadNumericFormat;
				ret.LexType = LEX_NONE;
				ret.Smth = 0;
			}
			Inc( ldigit.GetRead() );
			continue;
		}

		if( SoleChar().isLetter() || (SoleChar() == QChar('_')) )
		{
			LIden lvar;
			ret.LexType = lvar.Analisys( getCurrentText() );
			switch( ret.LexType )
			{
			case LIden::Identifier:
				ret.LexType = LEX_ident;
				ret.Smth = s_autoheap.Add2List( p_s = new QString( lvar.identifier ) );
				break;

			case LIden::KW_Abs:
				ret.LexType = LEX_kw_abs;
				break;

			case LIden::KW_Av:
				ret.LexType = LEX_kw_av;
				break;

			case LIden::KW_Min:
				ret.LexType = LEX_kw_min;
				break;

			case LIden::KW_Max:
				ret.LexType = LEX_kw_max;
				break;

			default:
				throw "Internal error";
			}
			Inc( lvar.GetRead() );
			continue;
		}

		QChar tch = NextChar();
		switch( tch.unicode() )
		{
		case _T('(') :
			ret.LexType = LEX_lb;
			break;
		case _T(')') :
			ret.LexType = LEX_rb;
			break;
		case _T('.') :
			ret.LexType = LEX_dot;
			break;
		case _T(',') :
			ret.LexType = LEX_coma;
			break;
		case _T('+') :
			ret.LexType = LEX_addsub;
			ret.Index = _T('+');
			break;
		case _T('-') :
			ret.LexType = LEX_addsub;
			ret.Index = _T('-');
			break;
		case _T('*') :
			ret.LexType = LEX_muldiv;
			ret.Index = _T('*');
			break;
		case _T('/') :
			ret.LexType = LEX_muldiv;
			ret.Index = _T('/');
			break;
		case _T('=') :
			ret.LexType = LEX_assign;
			ret.Index = _T('=');
			break;
		case _T(';') :
			ret.LexType = LEX_eol;
			break;
		case EOF_ASCII_1A :					// !!! eof !!!
		case EOF_ASCII_NULL :				// !!! eof !!!
			ret.LexType = LEX_eof;
			PutBack();
			break;
		default :
			lex_err = Lex::UnknownSymbol;
			ret.LexType = LEX_NONE;
		}
	}
	while( 0 );

	return ret;
}
