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

#include "object.h"

///////////////////////////////////////////////////////////////////////////////


MMD_Object::MMD_Object( CParseDsrSymbolTable& _smbtable, CDsrDataType_Node *_type ) :
	m_smbtable( &_smbtable )
{
	if( _type )
		m_type = getSmbTable().getTypeTable().cloneDataTypeNode( _type );
	else
		m_type = 0;
}


enum CDsrDataTypes MMD_Object::getType( void )
{
	return getDataType() && getDataType()->getNestedType() ?
			getDataType()->getNestedType()->getType() : DSRDATA_TYPE_NONE;
}


QString MMD_Object::getTypeName( void )
{
	return getDataType() ? getDataType()->getTypeName() : ::getTypeName( DSRDATA_TYPE_NONE );
}


MMD_Object::MMD_Object( CParseDsrSymbolTable& _smbtable, enum CDsrDataTypes _type ) :
	m_smbtable( &_smbtable )
{
	m_type = getSmbTable().getTypeTable().makeDataTypeNode( _type );
}


MMD_Object::MMD_Object( const MMD_Object& o ) :
	m_smbtable( &o.getSmbTable() )
{
	m_type = getSmbTable().getTypeTable().cloneDataTypeNode( o.getDataType() );
}


MMD_Object::~MMD_Object( void )
{
}


MMD_Object* MMD_Object::copy( const MMD_Object *o )
{
	m_smbtable = &o->getSmbTable();
	m_type = getSmbTable().getTypeTable().cloneDataTypeNode( o->getDataType() );
	return this;
}


MMD_Object MMD_Object::operator=( const MMD_Object& o )
{
	return *copy( &o );
}


CParseDsrSymbolTable& MMD_Object::getSmbTable( void ) const
{
	return *m_smbtable;
}


CDsrDataType_Node* MMD_Object::getDataType( void ) const
{
	return m_type;
}


void MMD_Object::setDataType( CDsrDataType_Node *tp )
{
	m_type = tp;
}


QString MMD_Object::getTextView( void )
{
	return _T("");
}


QString MMD_Object::getFormulatorView( void )
{
	return _T("");
}


QString MMD_Object::getMmlView( void )
{
	return _T("");
}

///////////////////////////////////////////////////////////////////////////////

MMD_ObjectHeap::~MMD_ObjectHeap( void )
{
	for( long i = 0; i < (long) ArrayDim(); i++ )
	{
		if( at( i ) )
			delete at( i );
	}
}

///////////////////////////////////////////////////////////////////////////////
