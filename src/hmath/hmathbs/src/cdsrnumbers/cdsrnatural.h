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

#ifndef _CDSRNATURAL_H_
#define _CDSRNATURAL_H_

typedef /*unsigned*/ long natural;

struct CDSRNaturalStorage
{
	natural value;
	natural& operator = ( double v ) { return value = (natural) v; }
};

class CDSRNatural : public CDSRNaturalStorage
{
public:
	CDSRNatural( natural val = 1 )
	{
		if( val < 1 )
			throw "Result of the operation is out of range";
		value = val;
	};

	natural get(){ return value; };

	CDSRNatural operator = (const CDSRNatural& n )
	{
		value = n.value;
		return *this;
	};
	CDSRNatural operator = ( natural& val )
	{
		if( val < 1 )
			throw "Result of the operation is out of range";
		value = val;
		return *this;
	};

	friend CDSRNatural operator + ( CDSRNatural& n1, CDSRNatural& n2 )
	{
		long r = n1.get() + n2.get();

		//__asm jno mmm;
		//throw "Result of the operation is out of range";
		//__asm mmm: nop;

		return CDSRNatural( r );
	};

	friend CDSRNatural operator + ( CDSRNatural& n1, long n2 )
	{
		long r = n1.get() + n2;

		//__asm jno mmm;
		//throw "Result of the operation is out of range";
		//__asm mmm: nop;

		return CDSRNatural( r );
	};

	friend CDSRNatural operator + ( long n1, CDSRNatural& n2 )
	{
		long r = n1 + n2.get();

		//__asm jno mmm;
		//throw "Result of the operation is out of range";
		//__asm mmm: nop;

		return CDSRNatural( r );
	};

	friend CDSRNatural operator - ( CDSRNatural& n1, CDSRNatural& n2 )
	{
		if( n1.get() <= n2.get() )
			throw "Result of the operation is out of range";
		return CDSRNatural( n1.get() - n2.get() );
	};

	friend CDSRNatural operator - ( CDSRNatural& n1, long n2 )
	{
		if( n1.get() <= n2 )
			throw "Result of the operation is out of range";
		return CDSRNatural( n1.get() - n2 );
	};

	friend CDSRNatural operator - ( long n1, CDSRNatural& n2 )
	{
		if( n1 <= n2.get() )
			throw "Result of the operation is out of range";
		return CDSRNatural( n1 - n2.get() );
	};

	friend CDSRNatural operator * ( CDSRNatural& n1, CDSRNatural& n2 )
	{
		long r = n1.get() * n2.get();

		//__asm jno mmm;
		//throw "Result of the operation is out of range";
		//__asm mmm: nop;

		return CDSRNatural( r );
	};

	friend CDSRNatural operator * ( CDSRNatural& n1, long n2 )
	{
		long r = n1.get() * n2;

		//__asm jno mmm;
		//throw "Result of the operation is out of range";
		//__asm mmm: nop;

		return CDSRNatural( r );
	};

	friend CDSRNatural operator * (long n1, CDSRNatural& n2 )
	{
		long r = n1 * n2.get();

		//__asm jno mmm;
		//throw "Result of the operation is out of range";
		//__asm mmm: nop;

		return CDSRNatural( r );
	};

	friend CDSRNatural operator / ( CDSRNatural& n1, CDSRNatural& n2 )
	{
		if( n1.get() < n2.get() )
			throw "Result of the operation is out of range";
		return CDSRNatural( n1.get() / n2.get() );
	};

	friend int operator == ( CDSRNatural& n1, CDSRNatural& n2 )
	{
		return( n1.get() == n2.get() );
	};
	friend int operator < ( CDSRNatural& n1, CDSRNatural& n2 )
	{
		return( n1.get() < n2.get() );
	};
	friend int operator > ( CDSRNatural& n1, CDSRNatural& n2 )
	{
		return( n1.get() > n2.get() );
	};
	friend int operator <= ( CDSRNatural& n1, CDSRNatural& n2 )
	{
		return( n1.get() <= n2.get() );
	};
	friend int operator >= ( CDSRNatural& n1, CDSRNatural& n2 )
	{
		return( n1.get() >= n2.get() );
	};

};

#endif
