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

#if !defined( __PARSEDSR_OPERATOR_TYPEMODI_H__ )
#define __PARSEDSR_OPERATOR_TYPEMODI_H__

#include "../hmathast_global.h"
#include "HUtils/ihkflag.h"

#define CDsrDataType_Modifier_LVALUE	0x00000001
#define CDsrDataType_Modifier_XVALUE	0x00000002
#define CDsrDataType_Modifier_LOCAL		0x00000004
#define CDsrDataType_Modifier_DYNAMIC	0x00000008
#define CDsrDataType_Modifier_RETURNS	0x00000010
#define CDsrDataType_Modifier_LINEAR_EQ	0x00000020
#define CDsrDataType_Modifier_USED		0x00000040

/////////////////////////////////////////
//	CDsrDataType_Modifier
/////////////////////////////////////////
struct __HMATHLNGDLL__ CDsrDataType_Modifier
{
protected:
	HKLongFlag modifier;
public:
	CDsrDataType_Modifier( long v = 0 )
	{
		modifier.setValue( v );
	}
	
	HKLongFlag& getModifier( void )
	{
		return modifier;
	}
	
	int isLValue( void )
	{
		return modifier.isSetFlag( CDsrDataType_Modifier_LVALUE );
	}
	
	void setLValue( void )
	{
		modifier.setFlag( CDsrDataType_Modifier_LVALUE );
	}
	
	void clearLValue( void )
	{
		modifier.clearFlag( CDsrDataType_Modifier_LVALUE );
	}
	
	int isLocal( void )
	{
		return modifier.isSetFlag( CDsrDataType_Modifier_LOCAL );
	}
	
	void setLocal( void )
	{
		modifier.setFlag( CDsrDataType_Modifier_LOCAL );
	}
	
	int isXValue( void )
	{
		return modifier.isSetFlag( CDsrDataType_Modifier_XVALUE );
	}
	
	void setXValue( void )
	{
		modifier.setFlag( CDsrDataType_Modifier_XVALUE );
	}
	
	int isDynamic( void )
	{
		return modifier.isSetFlag( CDsrDataType_Modifier_DYNAMIC );
	}
	
	void setDynamic( void )
	{
		modifier.setFlag( CDsrDataType_Modifier_DYNAMIC );
	}
	
	int isReturns( void )
	{
		return modifier.isSetFlag( CDsrDataType_Modifier_RETURNS );
	}
	
	void setReturns( void )
	{
		modifier.setFlag( CDsrDataType_Modifier_RETURNS );
	}
	
	int isLinearEQ( void )
	{
		return modifier.isSetFlag( CDsrDataType_Modifier_LINEAR_EQ );
	}
	
	void setLinearEQ( void )
	{
		modifier.setFlag( CDsrDataType_Modifier_LINEAR_EQ );
	}
	
	int isUsed( void )
	{
		return modifier.isSetFlag( CDsrDataType_Modifier_USED );
	}
	
	void setUsed( void )
	{
		modifier.setFlag( CDsrDataType_Modifier_USED );
	}
	
	void copyModifier( CDsrDataType_Modifier *modi )
	{
		if( modi ) modifier.setValue( modi->getValue() );
	}
	
	long getValue( void )
	{
		return modifier.getValue();
	}
	
	void resetModifier( void )
	{
		modifier.resetFlag();
	}

};

#endif
	
