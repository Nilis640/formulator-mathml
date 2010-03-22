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

#ifndef _CDSRRational_H_
#define _CDSRRational_H_

//#define NO_CHECK  //проверки на переполнение определено не будет

#include "../hmathbs_global.h"

#define AUTO_SHORTING

MATHBASE_API long EvklidMaxBinDiv( long a, long b );
MATHBASE_API long EvklidMinMul( long a, long b );

typedef long integer;

struct CDSRRationalStorage
{
	integer c,z;

};

class CDSRRational : public CDSRRationalStorage
{
public:
	CDSRRational( integer _c = 0, integer _z=1 )
	{ 
#ifdef AUTO_SHORTING
		if( _z == 0 ) throw "Division by zero";
		long mm = EvklidMaxBinDiv(_c,_z);
		c = _c / mm;
		z = _z / mm; 
#else
		c = _c;
		z = _z; 
#endif
	};

	CDSRRational( const CDSRRationalStorage& i )
	{ 
		c = i.c;
		z = i.z;

#ifdef AUTO_SHORTING
		if( z == 0 ) 
			throw "Division by zero";
		long mm = EvklidMaxBinDiv(c,z);
		c /= mm;
		z /= mm;
#endif
	};

	double get()
	{
		return (double) c / (double) z;
	};

	CDSRRational operator = ( const CDSRRational& i )
	{
		c = i.c;
		z = i.z;
		return *this;
	};

	CDSRRational operator = ( CDSRRationalStorage& i )
	{
		c = i.c;
		z = i.z;
		return *this;
	};

	CDSRRational operator = ( integer& val )
	{
		c = val;
		z = 1;
		return *this;
	};

	friend inline CDSRRational operator + ( CDSRRational& i1, CDSRRational& i2 )
	{
		integer c1, c2, z1, z2, mm, i, j;
		mm = EvklidMinMul( i1.z, i2.z );
		z1 = mm / i1.z; z2 = mm / i2.z;
		c1 = i1.c * z1; 

		//__asm jno mmm;
		//throw "Result of the operation is out of range";
		//__asm mmm: nop;

		c2 = i2.c * z2;

		//__asm jno mmm2;
		//throw "Result of the operation is out of range";
		//__asm mmm2: nop;

		j = mm;
		i = c1 + c2;

		//__asm jno mmm3;
		//throw "Result of the operation is out of range";
		//__asm mmm3: nop;

		return CDSRRational( i, j );
	};

	friend inline CDSRRational operator - ( CDSRRational& i1, CDSRRational& i2 )
	{
		integer c1, c2, z1, z2, mm, i, j;
		mm = EvklidMinMul( i1.z, i2.z );
		z1 = mm / i1.z; 
		z2 = mm / i2.z;
		c1 = i1.c * z1; 

		//__asm jno mmm;
		//throw "Result of the operation is out of range";
		//__asm mmm: nop;

		c2 = i2.c * z2;

		//__asm jno mmm2;
		//throw "Result of the operation is out of range";
		//__asm mmm2: nop;

		j = mm;
		i = c1 - c2;

		//__asm jno mmm3;
		//throw "Result of the operation is out of range";
		//__asm mmm3: nop;

		return CDSRRational( i, j );
	};
	friend CDSRRational operator * ( CDSRRational& i1, CDSRRational& i2 )
	{
		integer mc, mz;
		mc = i1.c * i2.c;

		//__asm jno mmm1;
		//throw "Result of the operation is out of range";
		//__asm mmm1: nop;

		mz = i1.z * i2.z ;

		//__asm jno mmm3;
		//throw "Result of the operation is out of range";
		//__asm mmm3: nop;

		return CDSRRational( mc, mz);
	};

	friend CDSRRational operator / ( CDSRRational& i1, CDSRRational& i2 )
	{
		integer mc, mz;
		mc = i1.c * i2.z;

		//__asm jno mmm1;
		//throw "Result of the operation is out of range";
		//__asm mmm1: nop;

		mz = i1.z * i2.c ;

		//__asm jno mmm3;
		//throw "Result of the operation is out of range";
		//__asm mmm3: nop;

		return CDSRRational( mc, mz);
	};

	friend int operator == ( CDSRRational& i1, CDSRRational& i2 )
	{
		return( i1.get() == i2.get() );
	};
	friend int operator < ( CDSRRational& i1, CDSRRational& i2 )
	{
		return( i1.get() < i2.get() );
	};
	friend int operator > ( CDSRRational& i1, CDSRRational& i2 )
	{
		return( i1.get() > i2.get() );
	};
	friend int operator <= ( CDSRRational& i1, CDSRRational& i2 )
	{
		return( i1.get() <= i2.get() );
	};
	friend int operator >= ( CDSRRational& i1, CDSRRational& i2 )
	{
		return( i1.get() >= i2.get() );
	};
};

#endif
