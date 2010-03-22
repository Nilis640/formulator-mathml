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

#ifndef __PARSEDSR_DATA_MMD_VEC_H__
#define __PARSEDSR_DATA_MMD_VEC_H__

#include "../object/obj_vec.h"

///////////////////////////////////////////////////////////////////////////////

class __HMATHLNGDLL__ MMD_Vector :  public MMD_ObjectVec
{
protected :
	long m_rows;
	long m_columns;

public :
	MMD_Vector( CParseDsrSymbolTable& _smbtable );
	MMD_Vector( CParseDsrSymbolTable& _smbtable, enum CDsrDataTypes nested_type, long r = 0, long c = 0 );
	MMD_Vector( const MMD_Vector& ms );
	MMD_Vector* clone( void );
	virtual MMD_Object* copy( const MMD_Vector *src );
	virtual ~MMD_Vector( void );
	void set( long idx, UniWord& uw );
	void set( long r, long c, UniWord& uw );
	void set_natural( long i, long j, UniWord& uw );
	void set_integer( long i, long j, UniWord& uw );
	void set_rational( long i, long j, UniWord& uw );
	void set_real( long i, long j, UniWord& uw );
	void set_complex( long i, long j, UniWord& uw );
	UniWord get( long idx );
	UniWord& get( long r, long c );
	int isValidIdx( long idx );
	int isValidIdx( long r, long c );
	CDsrDataType_Node* getNestedType( void );
	long getRows( void );
	void setRows( long _rows );
	long getColumns( void );
	void setColumns( long _columns );
	QString getTextView_Row( long r );
	virtual QString getTextView( void );
	virtual QString getFormulatorView( void );
	virtual QString getMmlView( void );
	//int operator==( const MMD_Vector& b );
};

///////////////////////////////////////////////////////////////////////////////

inline
MMD_Vector::~MMD_Vector( void )
{}

inline
MMD_Vector* MMD_Vector::clone( void )
{
	return (MMD_Vector*)getSmbTable().makeUniWord( new MMD_Vector( *this ) );
}

inline
long MMD_Vector::getRows( void )
{
	return m_rows;
}

inline
void MMD_Vector::setRows( long _rows )
{
	m_rows = _rows;
}

inline
long MMD_Vector::getColumns( void )
{
	return m_columns;
}

inline
void MMD_Vector::setColumns( long _columns )
{
	m_columns = _columns;
}

inline
CDsrDataType_Node* MMD_Vector::getNestedType( void )
{
	return getDataType() ? getDataType()->getNestedType() : 0;
}

inline
UniWord& MMD_Vector::get( long r, long c )
{
	return ItemAt( (r - 1) * getColumns() + (c - 1) );
}

inline
void MMD_Vector::set( long r, long c, UniWord& uw )
{
	get( r, c ) = uw;
}

inline
void MMD_Vector::set_natural( long i, long j, UniWord& uw )
{
	get( i, j ) = uw.v_natural;
}

inline
void MMD_Vector::set_integer( long i, long j, UniWord& uw )
{
	get( i, j ) = uw.v_integer;
}

inline
void MMD_Vector::set_rational( long i, long j, UniWord& uw )
{
	get( i, j ) = uw.v_rational;
}

inline
void MMD_Vector::set_real( long i, long j, UniWord& uw )
{
	get( i, j ) = uw.v_real;
}

inline
void MMD_Vector::set_complex( long i, long j, UniWord& uw )
{
	get( i, j ) = uw.v_complex;
}

inline
int MMD_Vector::isValidIdx( long r, long c )
{
	return r > 0 && c > 0 && r <= getRows() && c <= getColumns();
}

inline
int MMD_Vector::isValidIdx( long idx )
{
	if( idx < 1 ) return 0;
	if( getRows() == 1 || getColumns() == 1 )
		return idx <= (long) getCount();
	return idx <= getRows();
}

///////////////////////////////////////////////////////////////////////////////

typedef MMD_Vector* CDSRVectorStorage;

#endif
