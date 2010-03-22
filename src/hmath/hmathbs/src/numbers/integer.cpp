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

#include "integer.h"
#include "rational.h"
#include "bitsett.h"

CDSRInteger Polinom(CDSRArray<CDSRInteger>& p, long x);

////////////////////////////////////////////////////////////////////////////////////////////
//
MATHBASE_API long SystemConvertion(CDSRInteger& n, long base, CDSRArray<long>& m)
{
	long in1, in2;
	
	in1 = n.get();
	in2 = base;

	do
	{
		m.add(in1 % in2);
		in1 = in1 / in2;
	}while(in1 >= base);

	m.add(in1);
	return (long) m.size();
};

////////////////////////////////////////////////////////////////////////////////////////////
//	an*x^n + ... +a1*x + a0
MATHBASE_API long Diofant1(long n, CDSRArray<CDSRInteger>& m, CDSRArray<CDSRInteger>& root)
{
	CDSRArray<CDSRInteger>	prime;
	CDSRArray<long>			primeC;
	CDSRInteger				p_x;
	long					i, p;
	
	p = GetIntegerSimplyDivider(m[n], prime, primeC);
	prime.insert(0, CDSRInteger(1));
	primeC.insert(0, (long) 1);
	p++;

	for(i = 1; i <= p; i++)
	{
		p_x = Polinom(m, prime[i].get());
		if(p_x.get() == 0)
			root.add(prime[i]);

		p_x = Polinom(m, -prime[i].get());
		if(p_x.get() == 0)
			root.add(CDSRInteger(-prime[i].get()));
	};

	return (long) root.size();
};

////////////////////////////////////////////////////////////////////////////////////////////
//	a*x + b*y + c = 0
MATHBASE_API void Diofant2(CDSRInteger& a, CDSRInteger& b, CDSRInteger& c, CDSRArray<CDSRInteger>& x, CDSRArray<CDSRInteger>& y)
{
	CDSRArray<long>	m;
	long	ix0, iy0, ix1, iy1, ix, iy;
	long	i, k;
	
	CDSRRational rV(abs(a.get()), abs(b.get()));
	k = CircFraction(rV, m);
		
	ix0 = m[0]; iy0 = 1L;
	if(k > 2)
	{
		ix1 = ix0; iy1 = iy0;
		ix0 = m[0] * m[1] + 1L;
		iy0 = m[1];
		if(k > 3)
		{
			for(i = 2; i < (k - 1); i++)
			{
				ix = ix0 * m[i] + ix1;
				iy = iy0 * m[i] + iy1;
				ix1 = ix0; iy1 = iy0;
				ix0 = ix; iy0 = iy;
			}
		}
	}

	CDSRInteger x0 = (long) (pow( -1.0, k - 1 ) * c.value * iy0);
	CDSRInteger y0 = (long) (pow( -1.0, k ) * c.value * ix0);
	CDSRInteger x1 = x0 - b;
	CDSRInteger y1 = y0 + a;
	x.add(x0); x.add(x1);
	y.add(y0); y.add(y1);
};


///////////////////////////////////////////////////////////////////////////////////////////////////
// Polinom - this is an intenal function
CDSRInteger Polinom(CDSRArray<CDSRInteger>& p, long x)
{
	long value = 0;
	long i, n = (long) p.size();
	for(i = 0; i < n; i++)
		value += p[i].get() * (long) pow( (double) x, (n - i - 1));
	
	return CDSRInteger( value );
};


///////////////////////////////////////////////////////////////////////////////////////////////////
//
MATHBASE_API int IsIntegerSimple(CDSRInteger &n)
{
	CDSRArray<CDSRInteger> m;
	long k = Eratosfen(n, m);
	if( k < 1 ) 
		return 0;
	return (m[ k - 1].get() == abs( n.get() ));
};

///////////////////////////////////////////////////////////////////////////////////////////////////
//
MATHBASE_API long Eratosfen(CDSRInteger& n, CDSRArray<CDSRInteger>& m)
{
	unsigned long i, j;
	unsigned long value = abs(n.get());
	bitsetT* f;

	if(value == 0)
		return 0;

	f = new bitsetT(value + 1);

	for(i = 2; i <= value; i++) 
		(*f)[i] = 1;

	(*f)[1] = 0;
	
	for(i = 2; i <= value; i++)
	{
		if((*f)[i] == 1)
		{
			m.add(i);
			for(j = i; j <= value; j += i)
			{
				(*f)[j] = 0;
			};
		};
	};

	delete f;
	return (long) m.size();
};


///////////////////////////////////////////////////////////////////////////////////////////////////
//
MATHBASE_API long EratosfenRange(CDSRInteger& n_1, CDSRInteger& n_2, CDSRArray<CDSRInteger>& m)
{
	CDSRArray<CDSRInteger> m1, m2;
	CDSRInteger n1(abs(n_1.get())), n2(abs(n_2.get()));
	long in1,in2;
	long i;

	if(abs(n1.get()) > abs(n2.get()))
	{
		CDSRInteger n = n1;
		n1 = n2;
		n2 = n;
	};
	
	in2 = Eratosfen(n2, m2);
	in1 = n1.get();
	if(in1 != 1)
		in1 = Eratosfen(n1, m1);
	else
		in1 = 0;
	
	for(i = in1; i < in2; i++)
		m.add(m2[i]);
	
	return (long) m.size();
};

///////////////////////////////////////////////////////////////////////////////////////////////////
//
MATHBASE_API long GetIntegerSimplyDivider(CDSRInteger &digit, CDSRArray<CDSRInteger> &ar, CDSRArray<long> &count)
{
	long p;
	bitsetT* f;
	unsigned long n = abs(digit.get());
	unsigned long i, j, d;

	f = new bitsetT(n + 1);
	
	for(i = 1; i <= n; i++) 
		(*f)[i] = 1;

	(*f)[1] = 0;
	d = n;
	for(i = 2; i <= n; i++)
	{
		if((*f)[i] == 1)
		{
			p = 0;
			if((d % i) == 0)
			{
				while((d % i) == 0)
				{
					p++;
					d = d / i;
				};
				ar.add(i);
				count.add(p);
				if(d <= 1) break;
				for(j = i; j <= n; j += i)
				{
					if((*f)[j] == 1)
						(*f)[j] = 0;
				}
			}
		}
	}

	delete f;
	return (long) ar.size();
};


///////////////////////////////////////////////////////////////////////////////////////////////////
//
MATHBASE_API CDSRInteger Euqlid(CDSRInteger& n_1, CDSRInteger& n_2)
{
	long in1, in2;
	long ir;
	CDSRInteger n1(abs(n_1.get())), n2(abs(n_2.get()));

	if(n1 < n2)
	{
		CDSRInteger n = n1;
		n1 = n2;
		n2 = n;
	};

	in1 = n1.get();
	in2 = n2.get();
	while((ir = in1 % in2) != 0)
	{
		in1 = in2;
		in2 = ir;
	};

	return CDSRInteger(abs(in2));
};

///////////////////////////////////////////////////////////////////////////////////////////////////
//
MATHBASE_API CDSRInteger EuqlidArray(CDSRArray<CDSRInteger>& m)
{
	CDSRInteger d(0);
	size_t i, n = m.size();

	// Отсортировать массив m по-убыванию m[1]>m[2]>m[3]... - по теории
	// Реально она осуществляется ниже

	if( n < 2) return d;

	d = Euqlid(m[0], m[1]);
	for( i = 2; i < n; i++)
	{
		//Сортировка массива здесь
		if(abs(m[i].get()) > d.get())
			d = Euqlid(m[i], d);
		else
			d = Euqlid(d, m[i]);
	};
	return d;
};

///////////////////////////////////////////////////////////////////////////////////////////////////
//
MATHBASE_API CDSRInteger GetLeastCommonMultiple(CDSRInteger& n1, CDSRInteger& n2)
{
	CDSRInteger q, d;

	d = Euqlid(n1, n2);
	q = n1.get() * n2.get() / d.get();
	return CDSRInteger(abs(q.get()));
};

///////////////////////////////////////////////////////////////////////////////////////////////////
//
MATHBASE_API CDSRInteger GetLeastCommonMultipleArray(CDSRArray<CDSRInteger>& m)
{
	CDSRInteger q(0);
	size_t i, n = m.size();
	
	if( n < 2) return q;

	q = GetLeastCommonMultiple(m[0], m[1]);

	for( i = 2; i < n; i++)
	{
		if(abs(m[i].get()) > q.get())
			q = GetLeastCommonMultiple(m[i], q);
		else
			q = GetLeastCommonMultiple(q, m[i]);
	};
	return q;
};

///////////////////////////////////////////////////////////////////////////////////////////////////
//
MATHBASE_API int IsDivider(CDSRInteger& n1, CDSRInteger& n2)
{
	if(abs(n1.get()) % abs(n2.get()) == 0)
		return 1;
	return 0;
};
