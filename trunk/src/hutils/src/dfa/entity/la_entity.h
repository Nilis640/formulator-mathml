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

#if !defined( __MATHLANG_LIMAUTO_LA_ENTITY_H__ )
#define __MATHLANG_LIMAUTO_LA_ENTITY_H__

#include "../limauto.h"

#define LCEntityStates   9
#define LCEntityAlphabet 7

// class LCEntity
// ~~~~~ ~~~~~~~~
class HUTILS_EXPORT LCEntity : public LAuto
{
public :
	QString identifier;	// Идентификатор переменной
	unsigned short pre, value;
	short int is_big;

	enum LCEntityTypes
	{
		Identifier = 1,
		Number
	};

public :
	LCEntity( void );
	~LCEntity(){}

	QString& getID( void );
	unsigned short getVal( void );
	int isBig( void );

protected:
	virtual long What( const QChar S );
	virtual void Deeds( void );
};

inline
int LCEntity::isBig( void )
{
	return is_big;
}

inline
QString& LCEntity::getID( void )
{
	return identifier;
}

inline
unsigned short LCEntity::getVal( void )
{
	return value;
}

#endif
