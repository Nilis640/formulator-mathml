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

#ifndef _CDSRINTEGER_H_
#define _CDSRINTEGER_H_

typedef long integer;

struct CDSRIntegerStorage
{
	integer value;
	integer& operator= ( double v ) { return value = (integer) v; }
};

class CDSRInteger : public CDSRIntegerStorage
{
public:
	CDSRInteger( integer val = 0 ){ value = val; };

	integer get(){ return value; };

	const CDSRInteger& operator = ( const CDSRInteger& i )
	{
		value = i.value;
		return *this;
	};
	const CDSRInteger& operator = ( const integer& val )
	{
		value = val;
		return *this;
	};

	friend inline CDSRInteger operator + ( CDSRInteger& i1, CDSRInteger& i2 )
	{
		integer result;
		result = i1.value + i2.value;

		//__asm jno mmm;
		//throw "Result of the operation is out of range";
		//__asm mmm: nop;

		return CDSRInteger( result );
	};
	friend inline CDSRInteger operator + ( CDSRInteger& i1, integer i2 )
	{
		integer result;
		result = i1.value + i2;

		//__asm jno mmm;
		//throw "Result of the operation is out of range";
		//__asm mmm: nop;

		return CDSRInteger( result );
	};
	friend inline CDSRInteger operator + ( integer i1, CDSRInteger& i2 )
	{
		integer result;
		result = i1 + i2.value;

		//__asm jno mmm;
		//throw "Result of the operation is out of range";
		//__asm mmm: nop;

		return CDSRInteger( result );
	};

	friend inline CDSRInteger operator - ( CDSRInteger& i1, CDSRInteger& i2 )
	{
		integer result;
		result = i1.value - i2.value;

		//__asm jno mmm;
		//throw "Result of the operation is out of range";
		//__asm mmm: nop;

		return CDSRInteger( result );
	};
	friend inline CDSRInteger operator - ( CDSRInteger& i1, integer i2 )
	{
		integer result;
		result = i1.value - i2;

		//__asm jno mmm;
		//throw "Result of the operation is out of range";
		//__asm mmm: nop;

		return CDSRInteger( result );
	};
	friend inline CDSRInteger operator - ( integer i1, CDSRInteger& i2 )
	{
		integer result;
		result = i1 - i2.value;

		//__asm jno mmm;
		//throw "Result of the operation is out of range";
		//__asm mmm: nop;

		return CDSRInteger( result );
	};
	friend CDSRInteger operator * ( CDSRInteger& i1, CDSRInteger& i2 )
	{
		integer result;
		result = i1.value * i2.value;

		//__asm jno mmm;
		//throw "Result of the operation is out of range";
		//__asm mmm: nop;

		return CDSRInteger( result );
	};

	friend CDSRInteger operator * ( CDSRInteger& i1, integer i2 )
	{
		integer result;
		result = i1.value * i2;

		//__asm jno mmm;
		//throw "Result of the operation is out of range";
		//__asm mmm: nop;

		return CDSRInteger( result );
	};

	friend CDSRInteger operator * ( integer i1, CDSRInteger& i2 )
	{
		integer result;
		result = i1 * i2.value;

		//__asm jno mmm;
		//throw "Result of the operation is out of range";
		//__asm mmm: nop;

		return CDSRInteger( result );
	};

	friend CDSRInteger operator / ( CDSRInteger& i1, CDSRInteger& i2 )
	{
		integer result;
		if( i2.get() == 0 )
			throw "Result of the operation is out of range";
		result = i1.get() / i2.get();
		return CDSRInteger( result );
	};
	friend CDSRInteger operator / ( CDSRInteger& i1, integer i2 )
	{
		integer result;
		if( i2 == 0 )
			throw "Division by zero";
		result = i1.get() / i2;
		return CDSRInteger( result );
	};
	friend CDSRInteger operator / ( integer i1, CDSRInteger& i2 )
	{
		integer result;
		if( i2.get() == 0 )
			throw "Result of the operation is out of range";
		result = i1 / i2.get();
		return CDSRInteger( result );
	};
	friend int operator==( CDSRInteger& i1, CDSRInteger& i2 )
	{
		return( i1.get() == i2.get() );
	};
	friend int operator<( CDSRInteger& i1, CDSRInteger& i2 )
	{
		return( i1.get() < i2.get() );
	};
	friend int operator>( CDSRInteger& i1, CDSRInteger& i2 )
	{
		return( i1.get() > i2.get() );
	};
	friend int operator<=( CDSRInteger& i1, CDSRInteger& i2 )
	{
		return( i1.get() <= i2.get() );
	};
	friend int operator>=( CDSRInteger& i1, CDSRInteger& i2 )
	{
		return( i1.get() >= i2.get() );
	};
};

#endif
