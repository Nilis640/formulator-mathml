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

#if !defined( __COMMON_LA_DSR2VAR_H__ )
#define __COMMON_LA_DSR2VAR_H__

#include "HUtils/ihkarray.h"
#include "../limauto.h"

#define LDsr2VarStates   4
#define LDsr2VarAlphabet 3

// class LDsr2Var
// ~~~~~ ~~~~~
class HUTILS_EXPORT LDsr2Var : public LAuto
{
protected:
	MArray<QString> keywords;

public :
	QString identifier;	// Идентификатор переменной
	QString modifier;		// Модификатор переменной ('g' = greek, etc.)

public :

	enum LDsr2VarTypes
	{
		Identifier = 1,
		KW_begin,
		KW_end,
		KW_if,
		KW_then,		
		KW_else,
		KW_function,		
		KW_group,		
		KW_polynomial,		
		KW_rational_function		
	};

	LDsr2Var( void );
	~LDsr2Var(){}
	QChar getModifierChar( void );
	QString& getID( void );
	QString& getModifier( void );
	int	Analisys( const QStringRef& aText );

	long getKeywordNumber( void )
		{ return (long) keywords.ArrayDim(); }
	long getKeywordStartIdx( void )
		{ return LDsr2Var::KW_begin; }
	const QString& getKeyword( long i )
		{ return keywords[ i ]; }

protected:
	virtual long What( const QChar S );
	virtual void Deeds( void );
};

inline
QString& LDsr2Var::getID( void )
{
	return identifier;
}

inline
QString& LDsr2Var::getModifier( void )
{
	return modifier;
}

inline
QChar LDsr2Var::getModifierChar( void )
{
	return _T('$');
}

#endif
