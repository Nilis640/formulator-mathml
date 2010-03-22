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

#include "../parsedsr/grammar.h"
#include "../ast/astdsr.h"
#include "../operator/operator.h"
#include "vmidsr.h"

/////////////////////////////////////////
// Implementation
/////////////////////////////////////////

/////////////////////////////////////////
// AST-to-Code API
/////////////////////////////////////////

void VM_Dsr::Nop( long /*line*/, long /*level*/ )
{}

void VM_Dsr::ConstInt( long value, long line, long level )
{
	PrintText( value, line, level );
}

void VM_Dsr::ConstReal( double /*value*/, QString aspect, long line, long level )
{
	PrintText( aspect, line, level );
}

void VM_Dsr::ConstString( QString value, long line, long level )
{
	PrintText( _T("\""), line, level );
	PrintText( value, line, level );
	PrintText( _T("\""), line, level );
}

QString VM_Dsr::SymbolTableItem2String( long id )
{
	CParseDsrSymbol *smb = ptr_symbol_table->getSymbol( id );
	if( smb )
		return smb->name;
#if _DEBUG
	else
		return QString( _T("/*!!! internal error: unknown id !!!*/") );
#endif
	return _T("");
}

void VM_Dsr::SymbolTableItem( CParseDsrSymbol *smb, long line, long level )
{
	if( smb )
		PrintText( smb->name, line, level );
#if _DEBUG
	else
		PrintText( QString( _T("/*!!! internal error: null symbol record !!!*/") ), line, level );
#endif
}

void VM_Dsr::SymbolTableItem( long id, long line, long level )
{
	PrintText( SymbolTableItem2String( id ), line, level );
}

QString VM_Dsr::SeparatorGrammarIndex2String( long id )
{
	static QString value = _T("");

	value = _T("");
	switch( id )
	{
	case LEX_eol:
		value = _T(");");
		break;
	case LEX_lb:
		value = _T("(");
		break;
	case LEX_rb:
		value = _T(")");
		break;
	case LEX_lfb:
		value = _T("{");
		break;
	case LEX_rfb:
		value = _T("}");
		break;
	case LEX_dot:
		value = _T(".");
		break;
	case LEX_ask:
		value = _T("?");
		break;
	case LEX_lab:
		value = _T("[");
		break;
	case LEX_rab:
		value = _T("]");
		break;
	case LEX_coma:
		value = _T(",");
		break;
#if _DEBUG
	default:
		value = _T("/*!!! internal error: unknown separator !!!*/");
#endif
	}
	return value;
}

void VM_Dsr::SeparatorGrammarIndex( long id, long line, long level )
{
	PrintText( SeparatorGrammarIndex2String( id ), line, level );
}

QString VM_Dsr::Op2String( long id )
{
	return ml_Op2String( id );
}

void VM_Dsr::Op( long id, long line, long level )
{
	PrintText( Op2String( id ), line, level );
}

void VM_Dsr::Remark( QString value, long line, long level )
{
	PrintText( value, line, level );
}

void VM_Dsr::Blank( QString value, long line, long level )
{
	PrintText( value, line, level );
}

void VM_Dsr::EOL( QString /*value*/, long line, long level )
{
	PrintEOL( line, level );
}

void VM_Dsr::Text( QString value, long line, long level )
{
	PrintText( value, line, level );
}

void VM_Dsr::LeftRoundBracket( long line, long level )
{
	PrintText( _T("("), line, level );
}

void VM_Dsr::RightRoundBracket( long line, long level )
{
	PrintText( _T(")"), line, level );
}

void VM_Dsr::LeftSquareBracket( long line, long level )
{
	PrintText( _T("["), line, level );
}

void VM_Dsr::RightSquareBracket( long line, long level )
{
	PrintText( _T("]"), line, level );
}

void VM_Dsr::LeftCurlyBracket( long line, long level )
{
	PrintText( _T("{"), line, level );
}

void VM_Dsr::RightCurlyBracket( long line, long level )
{
	PrintText( _T("}"), line, level );
}

/////////////////////////////////////////
// Printing
/////////////////////////////////////////
void VM_Dsr::PrintTAB( long /*line*/, long level )
{
	if( modifier.isSetFlag( VM_Dsr_FLAG__JUST_AFTER_EOL ) )
	{
		if( level > 0 )	text_buffer.append( QString(level, _T('\t')) );
		modifier.clearFlag( VM_Dsr_FLAG__JUST_AFTER_EOL );
	}
}

void VM_Dsr::PrintEOL( long /*line*/, long /*level*/ )
{
	text_buffer.append( _T("\n") );
	modifier.setFlag( VM_Dsr_FLAG__JUST_AFTER_EOL );
}

void VM_Dsr::PrintText( QString value, long line, long level )
{
	PrintTAB( line, level );
	text_buffer.append( value );
}

void VM_Dsr::PrintText( long value, long line, long level )
{
	PrintTAB( line, level );
	text_buffer.append( QString("%1").arg( value ) );
}

void VM_Dsr::PrintText( double value, long line, long level )
{
	PrintTAB( line, level );
	text_buffer.append( QString("%1").arg( value ) );
}

/////////////////////////////////////////
// Implementation
/////////////////////////////////////////


VM_Dsr::VM_Dsr( CParseDsrSymbolTable*& _ptr_symbol_table, long init_mode ) :
	ptr_symbol_table ( _ptr_symbol_table )
{
	if( !ptr_symbol_table )
		throw "Internal error";
	mode = init_mode;
	setTextBuffer();
	modifier.resetFlag();
}


VM_Dsr::~VM_Dsr()
{
	if( ptr_symbol_table )
	{
		delete ptr_symbol_table;
		ptr_symbol_table = 0;
	}
}


CParseDsrSymbolTable& VM_Dsr::getSmbTable( void )
{
	return *ptr_symbol_table;
}


void VM_Dsr::setTextBuffer( QString src )
{
	text_buffer = src;
}


const QString& VM_Dsr::getTextBuffer( void )
{
	return text_buffer;
}

/////////////////////////////////////////
