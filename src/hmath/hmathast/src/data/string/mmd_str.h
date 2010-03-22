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

#ifndef __PARSEDSR_DATA_MMD_STR_H__
#define __PARSEDSR_DATA_MMD_STR_H__

#include "../object/obj_vec.h"

///////////////////////////////////////////////////////////////////////////////

class __HMATHLNGDLL__ MMD_String : public MMD_Object, public QString
{
public :
	MMD_String( CParseDsrSymbolTable& _smbtable );
	MMD_String( CParseDsrSymbolTable& _smbtable, const QString& arg_str );
	MMD_String( const MMD_String& ms );
	virtual MMD_Object* copy( const MMD_String *src );
	MMD_String* clone( void );
	virtual ~MMD_String( void );

	virtual QString getTextView();
	virtual QString getFormulatorView();
	virtual QString getMmlView();

	friend MMD_String* operator+( const MMD_String& a, const MMD_String& b );
	friend int operator==( const MMD_String& a, const MMD_String& b );
	friend int operator!=( const MMD_String& a, const MMD_String& b );
};

///////////////////////////////////////////////////////////////////////////////

inline
MMD_String::MMD_String( CParseDsrSymbolTable& _smbtable ) :
	MMD_Object( _smbtable, _smbtable.getTypeTable().makeDataTypeNode( DSRDATA_TYPE_STRING ) )
{
	(QString&)*this = QString( _T("") );
}

	inline
MMD_String::MMD_String( CParseDsrSymbolTable& _smbtable, const QString& arg_str ) :
	MMD_Object( _smbtable, _smbtable.getTypeTable().makeDataTypeNode( DSRDATA_TYPE_STRING ) )
{
	(QString&)*this = QString( arg_str );
}

inline
MMD_String::MMD_String( const MMD_String& ms )
    : MMD_Object( ms )
    , QString( ms )
{
        //(QString&)*this = (QString)ms;
}

inline
MMD_Object* MMD_String::copy( const MMD_String *ms )
{
	MMD_Object::copy( ms );
	(QString&)*this = (QString)*ms;
	return this;
}

inline
MMD_String::~MMD_String( void )
{}

inline
MMD_String* MMD_String::clone( void )
{
	return getSmbTable().makeUniWord( new MMD_String( *this ) );
}

inline
QString MMD_String::getTextView( void )
{
	return QString( _T("\"") ) + (const QString&)*this + QString( _T("\"") );
}

inline
QString MMD_String::getFormulatorView( void )
{
	return getTextView();
}

inline
QString MMD_String::getMmlView( void )
{
	return QString("<cstring>%1</cstring>").arg((const QString&)*this);
}

///////////////////////////////////////////////////////////////////////////////

inline
MMD_String* operator+( const MMD_String& a, const MMD_String& b )
{
	return a.getSmbTable().makeUniWord( 
		new MMD_String( a.getSmbTable(), (QString)a + (QString)b ) );
}

inline
int operator==( const MMD_String& a, const MMD_String& b )
{
	return (QString)a == (QString)b;
}

inline
int operator!=( const MMD_String& a, const MMD_String& b )
{
	return (QString)a != (QString)b;
}

///////////////////////////////////////////////////////////////////////////////

#endif
