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

#if !defined( __COMMON_LA_KEYWORD_H__ )
#define __COMMON_LA_KEYWORD_H__

#include "HUtils/ihkmap.h"
#include "../limauto.h"
#include <vector>

// class LKeyword
// ~~~~~ ~~~~~~~~
class HUTILS_EXPORT LKeyword : public LAuto
{
protected:
	HKMap<QString, long> prefix_map;
	std::vector<QString> prefix;
	QString identifier;
	std::vector<std::vector<long> > prefix2keyword;
	int isValid_prefix2keyword;

public :
	LKeyword( void );
	LKeyword( const QString *str_list, long n, int buildPrefix2Keyword = 0 );
	QString& getID( void );
	void Reset( void );
	const QString& getPrefix( long i );
	const std::vector<long>& getKeywordList( long i = -1 );	// returns a list of keywords for the current state

protected:
	void BuildDFA( const QString *str_list, long n );
	void BuildPrefixList( const QString *str_list, long n, std::vector<long>& es );
	void BuildEndStates( const QString *str_list, long n, std::vector<long>& es );
	void BuildJmp( const QString *str_list, long n );

	virtual long What( const QChar S );
	virtual void Deeds( void );
};

inline
QString& LKeyword::getID( void )
{
	return identifier;
}

inline
const QString& LKeyword::getPrefix( long i )
{
	return prefix[ i ];
}

inline
long LKeyword::What( const QChar S )
{
	return S.unicode() + 1;
}

#endif
