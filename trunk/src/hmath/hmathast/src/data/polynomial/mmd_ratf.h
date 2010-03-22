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

#ifndef __PARSEDSR_DATA_MMD_RATF_H__
#define __PARSEDSR_DATA_MMD_RATF_H__

#include "mmd_poly.h"

///////////////////////////////////////////////////////////////////////////////

class __HMATHLNGDLL__ MMD_RationalFun :  public MMD_Object
{
public :
	MMD_Polynomial *hi;
	MMD_Polynomial *lo;

	MMD_RationalFun( CParseDsrSymbolTable& _smbtable, enum CDsrDataTypes nested_type = DSRDATA_TYPE_REAL );
	MMD_RationalFun( CParseDsrSymbolTable& _smbtable, MMD_Polynomial *_hi, MMD_Polynomial *_lo );
	MMD_RationalFun( const MMD_RationalFun& ms );
	virtual MMD_Object* copy( const MMD_RationalFun *src );
	MMD_RationalFun* clone( void );
	virtual ~MMD_RationalFun( void );

	int isNormal( void );
	void Normalize( void );
	void getValue( UniWord& ret, const UniWord& x );
	virtual QString getTextView();
	virtual QString getFormulatorView();
	virtual QString getMmlView();
};

///////////////////////////////////////////////////////////////////////////////

inline
MMD_RationalFun::MMD_RationalFun( CParseDsrSymbolTable& _smbtable, MMD_Polynomial *_hi, MMD_Polynomial *_lo ) :
	MMD_Object( 
		_smbtable, 
		_smbtable.getTypeTable().makeDataTypeNode( 
			new CDsrDataType_RationalFun( 
				_smbtable.getTypeTable().makeDataTypeNode( _hi->getType() ) 
			) ) ),
	hi( _hi ), lo( _lo )
{
	if( _hi->getType() != _lo->getType() )
		throw _T("Internal error");
}

inline
MMD_RationalFun::MMD_RationalFun( const MMD_RationalFun& ms ) :
	MMD_Object( ms ), hi( ms.hi->clone() ), lo( ms.lo->clone() )
{
}

inline
MMD_Object* MMD_RationalFun::copy( const MMD_RationalFun *src )
{
	MMD_Object::copy( src );
	hi = src->hi->clone();
	lo = src->lo->clone();
	return this;
}

inline
MMD_RationalFun::~MMD_RationalFun( void )
{}

inline
MMD_RationalFun* MMD_RationalFun::clone( void )
{
	return getSmbTable().makeUniWord( new MMD_RationalFun( *this ) );
}

inline
int MMD_RationalFun::isNormal( void )
{
	if( hi->isNormal() && lo->isNormal() )
		return 1;
	return 0;
}

inline
void MMD_RationalFun::Normalize( void )
{
	if( !hi->isNormal() ) hi->Normalize();
	if( !lo->isNormal() ) lo->Normalize();
}

///////////////////////////////////////////////////////////////////////////////

typedef MMD_RationalFun* CDSRRFunStorage;

#endif
