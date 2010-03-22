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
#include "HUtils/ihkarray.h"
#include "la_keyword.h"

//#include <io.h>
//#include <fcntl.h>
//#include <sys\stat.h>

void LKeyword::Deeds( void )
{
	switch( CurrentState )
	{
	case 1 :
		identifier = _T("");
		break;
	default:
		identifier += character;
		break;
	}
}

void LKeyword::BuildDFA( const QString *str_list, long n )
{
	std::vector<long> es;
	BuildPrefixList( str_list, n, es );
	nStates = prefix.size();
	nAlphabet = 256;
	BuildEndStates( str_list, n, es );
	BuildJmp( str_list, n );
	prefix.clear();
}

void LKeyword::BuildPrefixList( const QString *str_list, long n, std::vector<long>& es )
{
	long i, j, l, *id = 0;
	long tmp = 0;
	QString to_add = _T("");

	es.clear();
	es.push_back( tmp );

	if( isValid_prefix2keyword )
	{
		prefix2keyword.erase( prefix2keyword.begin(), prefix2keyword.end() );
		prefix2keyword.push_back( std::vector<long>() );
	}
	prefix_map.erase( prefix_map.begin(), prefix_map.end() );
	prefix.clear();
	l = prefix.size();
	prefix.push_back( to_add );
	prefix_map[ to_add ] = l;

	for( i = 0; i < n; i++ )
	{
		for( j = 1; j <= (long) str_list[ i ].length(); j++ )
		{
			to_add = str_list[ i ].mid( 0, j );
			id = prefix_map.is_find( to_add );
			if( id == NULL )
			{
				l = prefix.size();
				prefix.push_back( to_add );
				prefix_map[ to_add ] = l;
				tmp = (j == str_list[ i ].length() ? i + 1 : 0);
				es.push_back( tmp );
				if( isValid_prefix2keyword )
					prefix2keyword.push_back( std::vector<long>(1, i + 1) );
			}
			else
			{
				if( *id >= 0 && *id < (long) es.size() && es[ *id ] == 0 && j == str_list[ i ].length() )
					es[ *id ] = i + 1;
				if( isValid_prefix2keyword && *id >= 0 && *id < (long) prefix2keyword.size() )
					prefix2keyword[ *id ].push_back( i + 1 );
			}
		}
	}
}

void LKeyword::BuildEndStates( const QString * /*str_list*/, long /*n*/, std::vector<long>& es )
{
	EndStates = new long[ prefix.size() ];
	copy( es.begin(), es.end(), EndStates );
}

void LKeyword::BuildJmp( const QString *str_list, long n )
{
	Jmp = new long[ nAlphabet * nStates ];
	memset( Jmp, 0, sizeof(long) * nAlphabet * nStates);

	QString pre;
	long row, cell, *id = 0;
	unsigned short column;
	long i, j;
	for( i = 0; i < n; i++ )
	{
		pre = _T("");
		for( j = 0; j < (long) str_list[ i ].length(); j++ )
		{
			id = prefix_map.is_find( pre );
			row = id ? *id : INT_MAX;
			pre = str_list[ i ].mid( 0, j + 1 );
			id = prefix_map.is_find( pre );
			cell = id ? *id : INT_MAX;
			if( row == INT_MAX || cell == INT_MAX ) break;
			column = (str_list[ i ].at(j)).unicode();
			Jmp[ column + row * nAlphabet ] = cell + 1;
		}
	}
}

const std::vector<long>& LKeyword::getKeywordList( long i )
{
	static std::vector<long> dummy_list;
	if( isValid_prefix2keyword == 0 )
		return dummy_list;
	if( i < 0 || i >= (long)prefix2keyword.size() )
		i = CurrentState - 1;
	return (i >= 0 && i < (long)prefix2keyword.size()) ? prefix2keyword[ i ] : dummy_list;
}

LKeyword::LKeyword( const QString *str_list, long n, int buildPrefix2Keryword )
	: identifier("")
{
	isValid_prefix2keyword = buildPrefix2Keryword;
	BuildDFA( str_list, n );
}

LKeyword::LKeyword( void )
	: identifier("")
{
	isValid_prefix2keyword = 0;
}

void LKeyword::Reset( void )
{
	LAuto::Reset();
	identifier = "";
}
