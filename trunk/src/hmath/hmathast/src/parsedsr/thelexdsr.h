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

#ifndef __PARSEDSR_THELEXER_H__
#define __PARSEDSR_THELEXER_H__

#include "HUtils/ihkstring.h"
#include "HUtils/ibaselexer.h"
#include "HUtils/iautoheap.h"
#include "../hmathast_global.h"
#include "../operator/operator.h"
#include "../symbol/symbol_table_dsr.h"

//=============================================================================

class __HMATHLNGDLL__ ParseDsrLex : public Lex {
protected:
	HU_Heap2Free<QString> s_autoheap;
	HU_Heap2Free<double> d_autoheap;
	bool remark_mode;
	CParseDsrSymbolTable *ptr_symbol_table;

public :
	ParseDsrLex( Source& Src, Common* aInfo, CParseDsrSymbolTable *_ptr_symbol_table = 0 )
		: Lex( Src, aInfo )
	{
		remark_mode = false;
		ptr_symbol_table = _ptr_symbol_table;
	}

	CParseDsrSymbolTable* getSymbolTablePtr( void )
		{ return ptr_symbol_table; }
	void setRemarkMode( bool value = true )
		{ remark_mode = value; }
	bool isRemarkMode( void )
		{ return remark_mode; }

	const QString& getLexemText( Lexem& r )
	{
		static QString str = _T("");
		if( r.length )
			str.assign( Source::getText( r.shift ), r.length );
		else
			str = _T("");
		return str;
	}

	virtual Lexem NextLexem( void );
};

#endif
