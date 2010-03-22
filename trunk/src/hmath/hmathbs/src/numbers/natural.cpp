
#include <math.h>
#include "natural.h"
#include "bitsett.h"

/////////////////////////////////////////////////////////////////
// For export


/////////////////////////////////////////////////////////////////
// 
MATHBASE_API int IsNaturalSimple(CDSRNatural &n)
{
	long k;
	CDSRArray<CDSRNatural> m;
	
	k = Eratosfen(n, m);

	if(m[k-1] == n)
		return 1;
	else
		return 0;
};


/////////////////////////////////////////////////////////////////
// 
MATHBASE_API long Eratosfen(CDSRNatural& n, CDSRArray<CDSRNatural>& m)
{
	unsigned long i, j;
	unsigned long value = n.get();
	bitsetT* f;

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
				(*f)[j] = 0;
		}//end if
	}//end for i

	delete f;
	return (long) m.size();
};

/////////////////////////////////////////////////////////////////
// 
MATHBASE_API long EratosfenACM(CDSRNatural& n, CDSRArray<CDSRNatural>& m)
{
	register long value = n.get();
	double s; 
	register long i, k;

	if(value <= 1) return 0;
	
	m.add(CDSRNatural(2));
	if(value >= 3)
	{
		m.add(CDSRNatural(3));
		if(value == 3)
			return (long) m.size();
	}//end if

	for(k = 5; k <= value; k += 2)
	{
		i = 0;
		s = sqrt( (double) k);
		do 
		{
			i++;
			if(m[i].get() > s) 
			{ 
				m.add(CDSRNatural(k));
				break; 
			}//end if
		}
		while((k % m[i].get()) != 0);
	}//end for

	return (long) m.size();
};

/////////////////////////////////////////////////////////////////
// 
MATHBASE_API long GetNaturalSimplyDivider(CDSRNatural &digit, CDSRArray<CDSRNatural> &ar, CDSRArray<long> &count)
{
	unsigned long i, j, d;
	long p;
	bitsetT* f;
	unsigned long n = digit.get();
	
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
				};//end for
			};//end if
		};//end if
		if(d == 0) break;
	};	//end for

	delete f;
	return (long) ar.size();
};

/////////////////////////////////////////////////////////////////
// 
MATHBASE_API long EratosfenRange(CDSRNatural& n1, CDSRNatural& n2, CDSRArray<CDSRNatural>& m)
{
	CDSRArray<CDSRNatural> m1, m2;
	long in1,in2;
	long i;

	if(n1 > n2)
	{
		CDSRNatural n = n1;
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


/////////////////////////////////////////////////////////////////
// 
MATHBASE_API CDSRNatural Euqlid(CDSRNatural& n1, CDSRNatural& n2)
{
	long in1, in2;
	long ir;

	if(n1 < n2)
	{
		CDSRNatural n = n1;
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

	return CDSRNatural(in2);
};

/////////////////////////////////////////////////////////////////
// 
MATHBASE_API CDSRNatural EuqlidArray(CDSRArray<CDSRNatural>& m)
{
	CDSRNatural d(0);
	size_t i, n = m.size();

	if( n < 2) return d;

	d = Euqlid(m[0], m[1]);
	for( i = 2; i < n; i++)
	{
		if(m[i] > d)
			d = Euqlid(m[i], d);
		else
			d = Euqlid(d, m[i]);
	};
	return d;
};

/////////////////////////////////////////////////////////////////
// 
MATHBASE_API CDSRNatural GetLeastCommonMultiple(CDSRNatural& n1, CDSRNatural& n2)
{
	CDSRNatural q, d;

	d = Euqlid(n1, n2);
	CDSRNatural nn = n1 * n2;
	q = nn / d;
	return q;
};

/////////////////////////////////////////////////////////////////
// 
MATHBASE_API CDSRNatural GetLeastCommonMultipleArray(CDSRArray<CDSRNatural>& m)
{
	CDSRNatural q(0);
	size_t i, n = m.size();
	
	if( n < 2) return q;

	q = GetLeastCommonMultiple(m[0], m[1]);
	for( i = 2; i < n; i++)
	{
		if(m[i] > q)
			q = GetLeastCommonMultiple(m[i], q);
		else
			q = GetLeastCommonMultiple(q, m[i]);
	};
	return q;
};

/////////////////////////////////////////////////////////////////
// 
MATHBASE_API int IsDivider(CDSRNatural& n1, CDSRNatural& n2)
{
	if(n1.get() % n2.get() == 0)
		return 1;
	return 0;
};

/////////////////////////////////////////////////////////////////
// 
MATHBASE_API long SystemConvertion(CDSRNatural& n, long base, CDSRArray<long>& m)
{
	long in1, in2;
	
	in1 = n.get();
	in2 = base;

	do
	{
		m.add(in1 % in2);
		in1 = in1 / in2;
	}
	while(in1 >= base);

	m.add(in1);
	return (long) m.size();
};

