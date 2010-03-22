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

#ifndef __PARSEDSR_DATA_MMD_OBJ_VEC_H__
#define __PARSEDSR_DATA_MMD_OBJ_VEC_H__

#include "../uniword/uniword.h"

///////////////////////////////////////////////////////////////////////////////

class __HMATHLNGDLL__ MMD_ObjectVec : public MArray<UniWord>,
											  public MMD_Object
{
public :
	MMD_ObjectVec( CParseDsrSymbolTable& _smbtable, CDsrDataType_Node *_type = 0, long count = 0 );
	MMD_ObjectVec( CParseDsrSymbolTable& _smbtable, enum CDsrDataTypes _type, long count = 0 );
	MMD_ObjectVec( const MMD_ObjectVec& ms );
	virtual MMD_Object* copy( const MMD_ObjectVec *src );
	virtual ~MMD_ObjectVec( void );
	unsigned long getCount( void ) const;
};

///////////////////////////////////////////////////////////////////////////////

inline
MMD_ObjectVec::MMD_ObjectVec( CParseDsrSymbolTable& _smbtable, CDsrDataType_Node *_type, long count )
	: MArray<UniWord>( 32, 0, 32 )
	, MMD_Object( _smbtable, _type )
	
{
	if( count ) resize( count );
}

inline
MMD_ObjectVec::MMD_ObjectVec( CParseDsrSymbolTable& _smbtable, enum CDsrDataTypes _type, long count )
	: MArray<UniWord>( 32, 0, 32 )
	, MMD_Object( _smbtable, _type )
	
{
	if( count ) resize( count );
}

inline
MMD_ObjectVec::MMD_ObjectVec( const MMD_ObjectVec& ms )
	: MArray<UniWord>( 32, 0, 32 )
	, MMD_Object( ms )
	
{
	MMD_Object::copy( &ms );
	MArray<UniWord>::Copy( ms );
}

inline
MMD_Object* MMD_ObjectVec::copy( const MMD_ObjectVec *src )
{
	MMD_Object::copy( src );
	MArray<UniWord>::Copy( *src );
	return this;
}

inline
unsigned long MMD_ObjectVec::getCount( void ) const
{
	return (unsigned long) ArrayDim();
}

inline
MMD_ObjectVec::~MMD_ObjectVec( void )
{
}

///////////////////////////////////////////////////////////////////////////////

#endif
