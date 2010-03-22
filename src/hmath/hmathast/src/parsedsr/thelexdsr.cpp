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

#include "HUtils/idfas.h"
#include "parserdsr.h"
#include "thelexdsr.h"

static QString __ParseDsr_startRem = _T("/*");
static QString __ParseDsr_endRem   = _T("*/");
static QString __ParseDsr_startRem_2 = _T("//");
static QString __ParseDsr_endRem_2   = _T("\x0d\x0a");

//*****************************************************************************
// ParseDsrLex::NextLexem (m) - returns next extracted lexem.
// Your should define the function yourself.
//*****************************************************************************
Lexem ParseDsrLex::NextLexem( void )
{
	Lexem ret;
	ret.line = GetLine();
	ret.col = GetCol();
	ret.shift = GetShift();

	double *p_d;
	QString *p_s;

	do
	{
		// ... Line feed, curret return
		if( isLA( __ParseDsr_endRem_2 ) ) // always the 1st check
		{
			ret.length = (long) _tcslen( __ParseDsr_endRem_2 );
			Inc( ret.length );
			ret.LexType = LEX_ignore_0d0a;
			//ret.ignore = true;
			continue;
		}
		// ... Continue with the remark breaked by "0d0a"
		else if( isRemarkMode() ) // always the 2nd check
		{
			ret.length = 0;
			while(  !_iseof( SoleChar() ) && 
					!isLA( __ParseDsr_endRem_2 ) && 
					!isLA( __ParseDsr_endRem ) )
			{
				NextChar();
				ret.length++;
			}
			if( _iseof( SoleChar() ) )
				lex_err = Lex::UnterminatedComment;
			else if( isLA( __ParseDsr_endRem ) )
			{
				ret.length += (long) _tcslen( __ParseDsr_endRem );
				Inc( (long) _tcslen(__ParseDsr_endRem) );
				setRemarkMode( false );
			}
			ret.LexType = LEX_ignore_remark;
			//ret.ignore = true;
			continue;
		}
		// ... Blanks
		else if( _isspace( SoleChar() ) )
		{
			for( ret.length = 0; 
				_isspace( SoleChar() ) && !isLA( __ParseDsr_endRem_2 ); 
				ret.length++ )
				NextChar();
			ret.LexType = LEX_ignore_blank;
			//ret.ignore = true;
			continue;
		}
		// ... Remark-2 ("//...0d0a")
		else if( isLA( __ParseDsr_startRem_2 ) )
		{
			ret.length = (long) _tcslen(__ParseDsr_startRem_2);
			Inc( ret.length );
			while(  !_iseof( SoleChar() ) && !isLA( __ParseDsr_endRem_2 ) )
			{
				NextChar();
				ret.length++;
			}
			// isn't nessessary -> if( _iseof( SoleChar() ) ) lex_err = Lex::UnterminatedComment;
			ret.LexType = LEX_ignore_remark;
			//ret.ignore = true;
			continue;
		}
		// ... Remark ("/*...*/", will be breaked by "0d0a")
		else if( isLA( __ParseDsr_startRem ) )
		{
			ret.length = (long) _tcslen(__ParseDsr_startRem);
			Inc( ret.length );
			while(  !_iseof( SoleChar() ) && 
					!isLA( __ParseDsr_endRem_2 ) && 
					!isLA( __ParseDsr_endRem ) )
			{
				NextChar();
				ret.length++;
			}
			if( _iseof( SoleChar() ) )
				lex_err = Lex::UnterminatedComment;
			else if( isLA( __ParseDsr_endRem_2 ) )
				setRemarkMode();
			else
			{
				ret.length += (long) _tcslen(__ParseDsr_endRem);
				Inc( (long) _tcslen(__ParseDsr_endRem) );
			}
			ret.LexType = LEX_ignore_remark;
			//ret.ignore = true;
			continue;
		}

		if( _isdigit( SoleChar() ) )
		{
			LDigit ldigit;
			ret.LexType = ldigit.Analisys( (const char*)(char*)(*this) );
			switch( ret.LexType )
			{
			case LDigit::Integer:
				ret.LexType = LEX_const_int;
				ret.Index = ldigit.mnt_int;
				break;
			case LDigit::Float:
				ret.LexType = LEX_const_real;
				ldigit.mnt_float *= pow( 10.0, ldigit.power );
				ret.Smth = d_autoheap.Add2List( p_d = new double( ldigit.mnt_float ) );
				break;
			case LDigit::FloatIEEE:
				ret.LexType = LEX_const_real;
				ldigit.mnt_float *= pow( 10.0, ldigit.power + ldigit.sign * ldigit.powerE );
				ret.Smth = d_autoheap.Add2List( p_d = new double( ldigit.mnt_float ) );
				break;
			default:
				lex_err = Lex::BadNumericFormat;
				ret.LexType = LEX_error_or_unknown;
				ret.Smth = 0;
			}
			ret.length = ldigit.GetRead();
			Inc( ret.length );
			continue;
		}

		if( _isalpha( SoleChar() ) || ( SoleChar() == '_' ) )
		{
			LDsr2Var lvar;
			ret.LexType = lvar.Analisys( (LPCTSTR)(TCHAR*)(*this) );
			switch( ret.LexType )
			{
			case LDsr2Var::Identifier:
				ret.LexType = LEX_identifier;
				ret.Smth = s_autoheap.Add2List( p_s = new QString( lvar.identifier + _T('$') + lvar.modifier ) );
				break;
			case LDsr2Var::KW_begin:
				ret.LexType = LEX_kw_begin;
				break;
			case LDsr2Var::KW_end:
				ret.LexType = LEX_kw_end;
				break;
			case LDsr2Var::KW_if:
				ret.LexType = LEX_kw_if;
				break;
			case LDsr2Var::KW_then:
				ret.LexType = LEX_kw_then;
				break;
			case LDsr2Var::KW_else:
				ret.LexType = LEX_kw_else;
				break;
			case LDsr2Var::KW_function:
				ret.LexType = LEX_kw_function;
				break;
			case LDsr2Var::KW_group:
				ret.LexType = LEX_kw_group;
				break;
			case LDsr2Var::KW_polynomial:
				ret.LexType = LEX_kw_polynomial;
				break;
			case LDsr2Var::KW_rational_function:
				ret.LexType = LEX_kw_rational_function;
				break;
			default:
				lex_err = Lex::UnknownError;
				ret.LexType = LEX_error_or_unknown;
				ret.Smth = 0;
			}
			ret.length = lvar.GetRead();
			Inc( ret.length );
			continue;
		}

		if( SoleChar() == _T('"') )
		{
			LUnbrokenString lstring;
			if( ( ret.LexType = lstring.Analisys( (LPCTSTR)(TCHAR*)(*this) ) ) != 0 )
			{
				ret.LexType = LEX_const_string;
				ret.Smth = s_autoheap.Add2List( p_s = new QString( lstring.read_string ) );
			}
			else
				lex_err = Lex::UnknownSymbol;
			ret.length = lstring.GetRead();
			Inc( ret.length );
			continue;
		}

		if( SoleChar() == _T('@') )
		{
			LDsr2SpecialID lspec;
			if( ( ret.LexType = lspec.Analisys( (LPCTSTR)(TCHAR*)(*this) ) ) != 0 )
			{
				long pid = -1, *pword;
				CParseDsrSymbol *dsmb;
				if( getSymbolTablePtr() && 
					(pword = getSymbolTablePtr()->is_find( lspec.identifier )) != 0 &&
					(dsmb = getSymbolTablePtr()->getSymbol( *pword )) != 0 )
					pid = dsmb->param1;
				if( pid != -1 )
				{
					switch( pid )
					{
					case FORMULA_times:
					case OPERATOR_op_mul:
						ret.LexType = LEX_op_mul;
						ret.Index = OPERATOR_op_mul;
						break;
					case FORMULA_minus:
					case OPERATOR_op_sub:
						ret.LexType = LEX_op_add;
						ret.Index = OPERATOR_op_sub;
						break;
					case FORMULA_sadd:
					case OPERATOR_op_setadd:
						ret.LexType = LEX_op_setadd;
						ret.Index = OPERATOR_op_setadd;
						break;
					case FORMULA_smul:
					case OPERATOR_op_setmul:
						ret.LexType = LEX_op_setmul;
						ret.Index = OPERATOR_op_setmul;
						break;
					case FORMULA_in:
					case OPERATOR_op_rel_in:
						ret.LexType = LEX_op_rel_in;
						ret.Index = OPERATOR_op_rel_in;
						break;
					case FORMULA_not_in:
					case OPERATOR_op_rel_not_in:
						ret.LexType = LEX_op_rel_not_in;
						ret.Index = OPERATOR_op_rel_not_in;
						break;
					case FORMULA_set_in:
					case OPERATOR_op_rel_set_in:
						ret.LexType = LEX_op_rel_set_in;
						ret.Index = OPERATOR_op_rel_set_in;
						break;
					case FORMULA_set_in_inv:
					case OPERATOR_op_rel_set_in_inv:
						ret.LexType = LEX_op_rel_set_in_inv;
						ret.Index = OPERATOR_op_rel_set_in_inv;
						break;
					case FORMULA_set_in_eq:
					case OPERATOR_op_rel_set_in_eq:
						ret.LexType = LEX_op_rel_set_in_eq;
						ret.Index = OPERATOR_op_rel_set_in_eq;
						break;
					case FORMULA_set_in_eq_inv:
					case OPERATOR_op_rel_set_in_eq_inv:
						ret.LexType = LEX_op_rel_set_in_eq_inv;
						ret.Index = OPERATOR_op_rel_set_in_eq_inv;
						break;
					case FORMULA_set_not_in:
					case OPERATOR_op_rel_set_not_in:
						ret.LexType = LEX_op_rel_set_not_in;
						ret.Index = OPERATOR_op_rel_set_not_in;
						break;
					default:
						ret.LexType = LEX_sp_identifier;
						ret.Smth = s_autoheap.Add2List( p_s = new QString( lspec.identifier ) );
					}
				}
				else
				{
					ret.LexType = LEX_sp_identifier;
					ret.Smth = s_autoheap.Add2List( p_s = new QString( lspec.identifier ) );
				}
			}
			else
				lex_err = Lex::UnknownSymbol;
			ret.length = lspec.GetRead();
			Inc( ret.length );
			continue;
		}

        switch( NextChar() )
        {
		case _T('(') :
			ret.LexType = LEX_lb;
			ret.length = 1;
			break;
		case _T(')') :
			ret.LexType = LEX_rb;
			ret.length = 1;
			break;
		case _T('[') :
			ret.LexType = LEX_lab;
			ret.length = 1;
			break;
		case _T(']') :
			ret.LexType = LEX_rab;
			ret.length = 1;
			break;
		case _T('{') :
			ret.LexType = LEX_lfb;
			ret.length = 1;
			break;
		case _T('}') :
			ret.LexType = LEX_rfb;
			ret.length = 1;
			break;
		case _T('.') :
			ret.LexType = LEX_dot;
			ret.length = 1;
			break;
		case _T('?') :
			ret.LexType = LEX_ask;
			ret.length = 1;
			break;
		case _T(',') :
			ret.LexType = LEX_coma;
			ret.length = 1;
			break;
		case _T('+') :
			ret.LexType = LEX_op_add;
			ret.length = 1;
			ret.Index = OPERATOR_op_add;
			break;
		case _T('-') :
			ret.LexType = LEX_op_add;
			ret.length = 1;
			ret.Index = OPERATOR_op_sub;
			break;
		case _T('*') :
			ret.LexType = LEX_op_mul;
			ret.length = 1;
			ret.Index = OPERATOR_op_mul;
			break;
		case _T('/') :
			ret.LexType = LEX_op_mul;
			ret.length = 1;
			ret.Index = OPERATOR_op_div;
			break;
		case _T('%') :
			ret.LexType = LEX_op_mul;
			ret.length = 1;
			ret.Index = OPERATOR_op_mod;
			break;
		case _T(';') :
			ret.LexType = LEX_eol;
			ret.length = 1;
			break;
		case _T('<') :
            if( NextChar() != '=' )
            {
				ret.LexType = LEX_op_rel_l;
				ret.Index = OPERATOR_op_rel_l;
				ret.length = 1;
				PutBack();
			}
			else
			{
				ret.LexType = LEX_op_rel_le;
				ret.Index = OPERATOR_op_rel_le;
				ret.length = 2;
			}
			break;
		case _T('>') :
			if( NextChar() != '=' )
			{
				ret.LexType = LEX_op_rel_g;
				ret.Index = OPERATOR_op_rel_g;
				ret.length = 1;
				PutBack();
			}
			else
			{
				ret.LexType = LEX_op_rel_ge;
				ret.Index = OPERATOR_op_rel_ge;
				ret.length = 2;
			}
			break;
		case _T('&') :
			ret.LexType = LEX_op_bool_and;
			ret.Index = OPERATOR_op_bool_and;
			ret.length = 1;
			break;
		case _T('|') :
			ret.LexType = LEX_op_bool_or;
			ret.Index = OPERATOR_op_bool_or;
			ret.length = 1;
			break;
		case _T('^') :
			ret.LexType = LEX_op_pow;
			ret.Index = OPERATOR_op_pow;
			ret.length = 1;
			break;
		case _T('!') :
			if( NextChar() != '=' )
			{
				ret.LexType = LEX_op_bool_not;
				ret.Index = OPERATOR_op_bool_not;
				ret.length = 1;
				PutBack();
			}
			else
			{
				ret.LexType = LEX_op_compare_ne;//LEX_op_compare;//LEX_op_compare_ne;
				ret.Index = OPERATOR_op_compare_ne;
				ret.length = 2;
			}
			break;
		case _T('=') :
			if( NextChar() != _T('=') )
			{
				ret.LexType = LEX_op_assign;
				ret.Index = OPERATOR_op_assign;
				ret.length = 1;
				PutBack();
			}
			else
			{
				ret.LexType = LEX_op_compare_e;//LEX_op_compare;//LEX_op_compare_e;
				ret.Index = OPERATOR_op_compare_e;
				ret.length = 2;
			}
			break;
		case EOF_ASCII_1A :					// !!! eof !!!
		case EOF_ASCII_NULL :				// !!! eof !!!
			ret.LexType = LEX_eof;
			ret.length = 1;
			PutBack();
			if( isRemarkMode() )
			{
				lex_err = Lex::UnterminatedComment;
				setRemarkMode( false );
			}
			break;
		default :
			lex_err = Lex::UnknownSymbol;
			ret.LexType = LEX_error_or_unknown;
			ret.length = 1;
		}
	}
	while( 0 );

	return ret;
}
