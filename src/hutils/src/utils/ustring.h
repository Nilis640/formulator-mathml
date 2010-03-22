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

#if !defined( __COMMON_BASET_H__ )
#define __COMMON_BASET_H__

#include <math.h>
#include "HUtils/ihkstring.h"
#include "../hutils_global.h"

#define EOL_ASCII		_T('\x0D')
#define EOF_ASCII		_T('\x1A')
#define EOF_ASCII_NULL	_T('\x00')
#define EOF_ASCII_1A	_T('\x1A')

inline
int _iseof( QChar c )
{
	return c == EOF_ASCII_NULL || c == EOF_ASCII_1A;
}

#define _isalpha( c ) ( ((c) >= _T('a') && (c) <= _T('z')) || ((c) >= _T('A') && (c) <= _T('Z')) )
#define _isalnum( c ) ( _isalpha( c ) || _isdigit( c ) )
#define _isdigit( c ) ( (c) >= _T('0') && (c) <= _T('9'))
#define _isspace( c ) ( (c) == _T(' ') || (c) == _T('\x09') || (c) == _T('\x0d') || (c) == _T('\x0a') )
#define _isprint( c ) ( isRu( c ) || ( (char)(c) > 0 && isprint( c ) ) )
//#define _iseof( c ) ( (c) == EOF_ASCII_NULL || (c) == EOF_ASCII_1A )
#define _iscntrl( c ) ( iscntrl( c ) && !isRu( c ) )
#define _isxdigit( c ) ( ( ((c) >= _T('a') && (c) <= _T('f')) || ((c) >= _T('A') && (c) <= _T('F')) ) || _isdigit( c ) )

inline
short int hex2dec( QChar S )
{
	if( S.isDigit() )
		return S.digitValue();
	else if( _isalpha( S.toAscii() ) )
		return S.toUpper().toAscii() - _T('A') + 10;
	return 0;
}

inline
long hround( double src )
{
	if( fabs( src - long(src) ) < 0.5 )
	{
		if( src > 0 )
			return (long)floor( src );
		else
			return (long)ceil( src );
	}
	else
	{
		if( src > 0 )
			return (long)ceil( src );
		else
			return (long)floor( src );
	}
}

inline
int xml_strcmp( const QString& a, const QString& b )
{
	return a.compare( b, Qt::CaseSensitive );
}

inline
int html_strcmp( const QString& a, const QString& b )
{
	return a.compare( b, Qt::CaseInsensitive );
}

inline
void print2blank( QString& dest, long n )
{
	dest.append( QString(n, _T(' ')) );
}

HUTILS_EXPORT QString generateTextSeed( unsigned long nSize );
HUTILS_EXPORT QString hstr_escape_blank( const QString& a );
HUTILS_EXPORT QString hstr_escape( const QString& a );
HUTILS_EXPORT const QString& hstr_escape_htmlamp( const QString& src );
HUTILS_EXPORT void hstr_0a_to_0d0a( QString& str );
HUTILS_EXPORT void hstr_del_redundant_0d0a( QString& str );

#endif
