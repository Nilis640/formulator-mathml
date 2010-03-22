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

#include "HUtils/imessage.h"
#include "template.h"

int HKMatchTemplate::Treate( const QString& src, const QVector<QString>& args )
{
	static const QString mark = __HERMITECH_TEMPLATE_MARK;
	static const int delta[ 9 ][ 2 ] = {{0, 0},
										{2, 1},
										{3, 1},
										{4, 1},
										{1, 5},
										{1, 6},
										{1, 0},
										{0, 0}};
	buffer.clear();
	int i, a = 0, cb, state;
	for( i = cb = 0; cb < args.size() && i < src.length(); )
	{
		a = i;
		state = 1;
		while( state && i < src.length() )
		{
			if( src.at(i) == QChar('/') )
				state = delta[ state ][ 0 ];
			else
			{
				if( src.at(i) == QChar('$') )
					state = delta[ state ][ 1 ];
				else
					state = 1;
			}
			i++;
		}
		if( state )
			break;
		else
		{
			buffer.append( src.mid(a, i - mark.length() - a) );
			buffer.append( args[ cb ] );
			cb++;

			state = 1;
			while( i < src.length() )
			{
				if( src.at(i) == QChar('\x0d') )
				{
					state = 2;
					i++;
					continue;
				}
				if( src[ i++ ] == QChar('\x0a') && state == 2 )
				{
					i -= 2;
					break;
				}
				else
					state = 1;
			}
		}
	}

	for( ; i < src.length(); )
	{
		a = i;
		state = 1;
		while( state && (i < src.length()) )
		{
			if( src.at(i) == QChar('/') )
				state = delta[ state ][ 0 ];
			else
			{
				if( src.at(i) == QChar('$') )
					state = delta[ state ][ 1 ];
				else
					state = 1;
			}
			i++;
		}
		if( state )
			break;
		else
		{
			cb++;
			state = 1;
			while( i < src.length() )
			{
				if( src.at(i) == QChar('\x0d') )
				{
					state = 2;
					i++;
					continue;
				}
				if( src.at(i++) == QChar('\x0a') && state == 2 )
				{
					i -= 2;
					break;
				}
				else
					state = 1;
			}
		}
	}
	buffer.append( src.mid(a, i - a) );
	return (cb != args.size()) ? -1 : 0;
}
