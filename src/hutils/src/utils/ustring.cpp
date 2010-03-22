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

#include <cstdlib>
#include "ustring.h"

QString generateTextSeed( unsigned long nSize )
{
	static QString bricks("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");
	QString sum("");
	long idx;
	double randValue;
	for( unsigned long i = 0; i < nSize; i++ )
	{
		randValue = ((double) std::rand()) / RAND_MAX;
		idx = long(randValue * (bricks.length() - 1));
		sum.append( bricks.at(idx) );
	}
	return sum;
}

QString hstr_escape_blank( const QString& src )
{
	QString ret;
	for( long i = 0; i < src.length(); i++ )
	{
		switch( src.at(i).toAscii() )
		{
		case _T('\t'):
			ret += "\\t";
			break;
		case _T('\r'):
			ret += "\\r";
			break;
		case _T('\n'):
			ret += "\\n";
			break;
		default:
			ret.append( src.at(i) );
		}
	}
	return ret;
}

QString hstr_escape( const QString& src )
{
	QString ret;
	for( long i = 0; i < src.length(); i++ )
	{
		switch( src.at(i).toAscii() )
		{
		case _T('\\'):
		case _T('\''):
		case _T('\"'):
			ret.append( QChar('\\') );
			break;
		}
		ret.append( src.at(i) );
	}
	return ret;
}

HUTILS_EXPORT
const QString& hstr_escape_htmlamp( const QString& src )
{
	static const int ampstr_len = 5;
	static const QString ampstr = _T("&amp;");
	static QString ret;

	ret = "";
	for( long offs = 0, fnd = 0; offs < src.length(); )
	{
		fnd = src.indexOf( ampstr, offs );
		if( fnd == -1 )
		{
			ret.append( src.mid(offs, src.length() - offs) );
			break;
		}
		ret.append( src.mid(offs, fnd - offs) );
		ret.append( QChar('&') );
		offs = fnd + ampstr_len;
	}
	return ret;
}

HUTILS_EXPORT
void hstr_0a_to_0d0a( QString& str )
{
	// replace "\n" -> "\r\n"
	long pos = 0;
	while( (pos = str.indexOf( '\n', pos )) != -1 )
	{
		if( pos && (pos - 1 < str.length()) && str.at(pos - 1) != '\r' )
		{
			str.insert( pos, "\r" );
			pos += 2;
		}
		else
		{
			pos++;
		}
	}
}

HUTILS_EXPORT
void hstr_del_redundant_0d0a( QString& str )
{
	// replace "\r\n\r\n" -> "\r\n"
	long pos = 0;
	while( (pos = str.indexOf( "\r\n\r\n", pos )) != -1 )
		str.remove( pos, 2 );
}

/////////////////////////////////////////////////////////////////////////////
