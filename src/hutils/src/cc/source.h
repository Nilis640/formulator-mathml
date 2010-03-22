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

#ifndef __HUTILS_SOURCE_H__
#define __HUTILS_SOURCE_H__

#include "../basexxx.h"
#include "HUtils/ihkarray.h"

//////////////////////////////////////////////////////////////////////

typedef unsigned short LEXEM;
class HUTILS_EXPORT Lexem 
{
public:
	union 
	{
		unsigned long Index;
		void*         Smth;
	};

	LEXEM LexType;
	long line;
	long col;
	long shift;
	long length;

	long GetLine( void ) { return line; }
	long GetCol( void ) { return col; }
	long GetShift( void ) { return shift; }
	long GetLength( void ) { return length; }
	QString* getString() { return (QString*)Smth; }

	Lexem( void ) 
		: line( 0 ), col( 0 ), shift( -1 ), length( 0 )
	{
	}
	Lexem( LEXEM lxm, unsigned long ind = 0 ) 
		: Index( ind ), LexType( lxm ), line( 0 ), col( 0 ), shift( -1 ), length( 0 )
	{
	}
	Lexem( LEXEM lxm, void *smth ) 
		: Smth( smth ), LexType( lxm ), line( 0 ), col( 0 ), shift( -1 ), length( 0 )
	{
	}
	
	int operator == ( const Lexem& eel ) const
	{
		return ( LexType == eel.LexType ) && ( Index == eel.Index );
	}
	
	int operator < ( const Lexem& eel ) const
	{
		return ( LexType < eel.LexType ) && ( Index < eel.Index );
	}
	
	Lexem& operator = ( const Lexem& eel )
	{
		memcpy( this, &eel, sizeof( Lexem ) );
		return *(this);
	}
};

//////////////////////////////////////////////////////////////////////

#define LEX_NONE	0
#define __LEX_eof	0xFFFF

class HUTILS_EXPORT Source
{
protected :
	MArray<unsigned long> maxlen;
	int state;

	unsigned long line;
	unsigned long col;

	unsigned long cur;
	QString	txt;

public :
	Source( const Source& aSrc );
	Source( const QString& aTxt );

	const QString& getText()
		{ return txt; }
	QStringRef getCurrentText()
		{ return getText( cur ); }
	QStringRef getText( unsigned long shift )
		{ return txt.midRef( shift ); }

	int isLA( const QString& str );
	QChar SoleChar( void );
	QChar NextChar( void );
	QChar LastChar( void );
	void PutBack( void );
	void PutBack( unsigned quant );
	void Inc( void );
	void Inc( int inc );
	void mvState( void );
	void mvbackState( void );
	void Reset( void );

	unsigned long GetLine( void );
	unsigned long GetCol( void );
	unsigned long GetShift( void );

	int isBOF( void );
};

inline
int Source::isLA( const QString& str )
{
	return !QStringRef::compare( txt.midRef( cur, str.length() ), str );
}

//***************************************************************************
// Moves column, line counters and the current state forward and backward.
//***************************************************************************
inline
void Source::mvState( void )
{
	if( (long) cur < txt.length() )
	{
		if( txt.at(cur) == QChar(13) && state == 0 )
		{
			state = 1;
			col++;
		}
		else if( txt.at(cur) == QChar(10) && state == 1 )
		{
			state = 0;
			if( maxlen.ArrayDim() < (long) line )
				maxlen[ line - 1 ] = col;
			line++;
			col = 1;
		}
		else
		{
			col++;
			state = 0;
		}
	}
	else
		state = 0;
}

inline
void Source::mvbackState( void )
{
	if( !cur )
	{
		line = col = 1;
		state = 0;
	}
	else
	{
		if( txt.at(cur - 1) == QChar(13) && txt.at(cur) == QChar(10) )
		{
			state = 1;
			line--;
			if( line > 0 )
				col = maxlen[ line - 1 ];
		}
		else if( txt.at(cur) == QChar(13) && (long(cur + 1) < txt.length() && txt.at(cur + 1) == QChar(10)) )
		{
			state = 0;
			col--;
		}
		else
			col--;
	}
}

inline
Source::Source( const Source& aSrc ) 
	: maxlen( 64, 0, 64 )
{
	cur = 0;
	line = 1;
	col = 1;
	state = 0;
	txt = aSrc.txt;
}

inline
Source::Source( const QString& aTxt )
	: maxlen( 64, 0, 64 )
{
	cur = 0;
	line = 1;
	col = 1;
	state = 0;
	txt = aTxt;
	if( txt.length() )
	{
		if( txt.at( txt.length() - 1 ) != QChar(0) )
			txt.append( QChar(0) );
	}
}

//***************************************************************************
// Returns current character without cursor's shifting.
//***************************************************************************
inline
QChar Source::SoleChar( void )
{
	return ((long) cur < txt.length()) ? txt.at(cur) : QChar(0);
}

//***************************************************************************
// Returns current character and shifts cursor.
//***************************************************************************
inline
QChar Source::NextChar( void )
{
	mvState();
	return ((long) cur < txt.length()) ? txt.at(cur++) : QChar(0);
}

//***************************************************************************
// Returns last read character.
//***************************************************************************
inline
QChar Source::LastChar( void )
{
	return cur ? txt.at(cur - 1) : QChar(0);
}

//***************************************************************************
// Puts back last read character.
//***************************************************************************
inline
void Source::PutBack( void )
{
	if( cur ) cur--;
	mvbackState();
}

//***************************************************************************
// Puts back <quant> last read characters.
//***************************************************************************
inline
void Source::PutBack( unsigned quant )
{
	for( ; quant > 0; quant-- )
		PutBack();
}

//***************************************************************************
// Skips sole character.
//***************************************************************************
inline
void Source::Inc( void )
{
	NextChar();
}

//***************************************************************************
// Skips <inc> characters.
//***************************************************************************
inline
void Source::Inc( int inc )
{
	for( ; inc > 0; inc-- )
		Inc();
}

//***************************************************************************
// Resets class.
//***************************************************************************
inline
void Source::Reset( void )
{
	cur  = 0;
	line = 1;
	col = 1;
	state = 0;
}

//***************************************************************************
// Returns current line number.
//***************************************************************************
inline
unsigned long Source::GetLine( void )
{
	return line;
}

//***************************************************************************
// Returns current column number.
//***************************************************************************
inline
unsigned long Source::GetCol( void )
{
	return col;
}

inline
unsigned long Source::GetShift( void )
{
	return cur;
}

inline
int Source::isBOF( void )
{
	return cur == 0;
}

//////////////////////////////////////////////////////////////////////

#endif
