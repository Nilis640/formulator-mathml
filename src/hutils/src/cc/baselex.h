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

#ifndef __HUTILS_BASELEX_H__
#define __HUTILS_BASELEX_H__

#include "source.h"

//////////////////////////////////////////////////////////////////////

class HUTILS_EXPORT Lex : public Source
{
public :
	enum LexErrors
	{
		UnknownError,
		UnknownSymbol,
		UnterminatedComment,
		BadNumericFormat,
		UnknownWord
	};

protected :  
	Lex::LexErrors lex_err;
	virtual int IsSpaceNextChar( void );

public :
	Lex( Source& Src );
	Lex::LexErrors GetError();
	virtual void Reset( void );
	virtual Lexem NextLexem();
	virtual int RemTermination( Lexem& ret );
	virtual ~Lex();
};

//////////////////////////////////////////////////////////////////////

inline
Lex::Lex( Source& Src )
	: Source( Src )
{
	lex_err = UnknownError;
}

inline
Lex::LexErrors Lex::GetError()
{
	return lex_err;
}

inline
void Lex::Reset( void )
{
	Source::Reset();
}

//////////////////////////////////////////////////////////////////////

#endif
