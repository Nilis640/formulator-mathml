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

#include "HMathML/immldefs.h"
#include "../uniword/uniword_op.h"
#include "mmd_vec.h"

MMD_Vector::MMD_Vector( CParseDsrSymbolTable& _smbtable ) :
	MMD_ObjectVec( _smbtable, DSRDATA_TYPE_VECTOR )
{
	m_rows = 0;
	m_columns = 0;
}

MMD_Vector::MMD_Vector( CParseDsrSymbolTable& _smbtable, enum CDsrDataTypes nested_type, long r, long c ) :
	MMD_ObjectVec( _smbtable, _smbtable.getTypeTable().makeDataTypeNode( new CDsrDataType_Vector( _smbtable.getTypeTable().makeDataTypeNode( nested_type ) ) ), r * c )
{
	m_rows = r;
	m_columns = c;
}

MMD_Vector::MMD_Vector( const MMD_Vector& ms ) :
	MMD_ObjectVec( ms )
{
	//copy( &ms );
	m_columns = ms.m_columns;
	m_rows = ms.m_rows;
}

MMD_Object* MMD_Vector::copy( const MMD_Vector *src )
{
	MMD_ObjectVec::copy( src );
	m_columns = src->m_columns;
	m_rows = src->m_rows;
	return this;
}
/*
int MMD_Vector::operator==( const MMD_Vector& b )
{
	if( getRows() != b.getRows() || getColumns() != b.getColumns() )
		return 0;
	long i, j;
	for( i = 1; i <= getRows(); i++ )
		for( j = 1; j <= getColumns(); j++ )
			if( get( i, j ) != b.get( i, j ) )
}
*/
UniWord MMD_Vector::get( long idx )
{
	if( getRows() == 1 || getColumns() == 1 )
		return ItemAt( idx - 1 );
	enum CDsrDataTypes nt = 
		(getDataType() && getDataType()->getNestedType()) ?
		getDataType()->getNestedType()->getType() : DSRDATA_TYPE_ERROR;
	MMD_Vector *rv = (MMD_Vector*)getSmbTable().makeUniWord( new MMD_Vector( getSmbTable(), nt, 1, getColumns() ) );
	for( long j = 0; j < getColumns(); j++ )
		(*rv)[ j ] = get( idx, j + 1 );
	return UniWord( rv );
}

void MMD_Vector::set( long idx, UniWord& uw )
{
	if( getRows() == 1 || getColumns() == 1 )
		ItemAt( idx - 1 ) = uw;
	else
	{
		MMD_Vector *rv = uw.getVector();
		if( rv && rv->isValidIdx( getColumns() - 1 ) )
		{
			for( long j = 0; j < getColumns(); j++ )
				set( idx, j + 1, rv->ItemAt( j ) );
		}
	}
}

QString MMD_Vector::getTextView_Row( long r )
{
	QString result = _T("");
	if( getColumns() == 0 )
		return _T("()");
	else
	{
		result = QString( _T("(") ) + get( r, 1 ).getTextView( getSmbTable() );
		for( long i = 2; i <= getColumns(); i++ )
			result += QString( _T(", ") ) + get( r, i ).getTextView( getSmbTable() );
		result += QString( _T(")") );
	}
	return result;
}

QString MMD_Vector::getTextView( void )
{
	QString result = _T("");
	if( getRows() == 0 )
		return _T("()");
	else if( getRows() == 1 )
		return getTextView_Row( 1 );
	else
	{
		result = QString( _T("(") ) + getTextView_Row( 1 );
		for( long i = 2; i <= getRows(); i++ )
			result += QString( _T(", ") ) + getTextView_Row( i );
		result += QString( _T(")") );
	}
	return result;
}

QString MMD_Vector::getFormulatorView( void )
{
	QString result = _T("");
	if( getRows() == 0 || getColumns() == 0 )
		return _T("");
	else
	{
		result = QString("@table( %1, %2, ").arg( getRows() ).arg( getColumns() ) + ItemAt( 0 ).getFormulatorView( getSmbTable() );
		for( long i = 0; i < (long) getCount(); i++ )
			result += QString( _T(", ") ) + ItemAt( i ).getFormulatorView( getSmbTable() );
		result += QString( _T(" )") );
	}
	return result;
}

QString MMD_Vector::getMmlView( void )
{
	QString result;
	if( getRows() == 0 || getColumns() == 0 )
		return result;
	else if( getColumns() == 1 )
	{
		result += QString("<%1>\n").arg(FBL_TAG_vector);
		for( long r = 1; r <= getRows(); r++ )
			result += (get( r, 1 ).getMmlView( getSmbTable() ) + "\n");
		result += QString("</%1>\n").arg(FBL_TAG_vector);
	}
	else
	{
		result += QString("<%1>\n").arg(FBL_TAG_matrix);
		for( long r = 1; r <= getRows(); r++ )
		{
			result += QString("<%1>\n").arg(FBL_TAG_matrixrow);
			for( long c = 1; c <= getColumns(); c++ )
				result += (get( r, c ).getMmlView( getSmbTable() ) + "\n");
			result += QString("</%1>\n").arg(FBL_TAG_matrixrow);
		}
		result += QString("</%1>\n").arg(FBL_TAG_matrix);
	}
	return result;
}

///////////////////////////////////////////////////////////////////////////////
