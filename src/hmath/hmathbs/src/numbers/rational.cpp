
#include "rational.h"

int GetDegree(long n1 , long n2);

MATHBASE_API long DecimalFraction(CDSRRational& r, long kmax, CDSRArray<long>&m)
{
	long n1 = r.c;
	long n2 = r.z;

	while(n1 / n2 == 0)
	{
		m.add((long) 0);
		n1 *= 10;
	};//end while

	while(n1 / n2 != 0 && m.size() <= (size_t) kmax)
	{
		m.add(n1 / n2);
		n1 = (n1 % n2) * (long) pow(10.0, GetDegree(n1 % n2, n2));
	};//end while

	return (long) m.size();
};

MATHBASE_API long CircFraction(CDSRRational& r, CDSRArray<long>&m)
{
	long ir;
	long in1 = r.c;
	long in2 = r.z;

	m.add(in1 / in2);
	ir = in1 % in2; in1 = in2; in2 = ir;
	if(ir == 0) return (long) m.size();
	ir = in1 % in2;
	while(ir != 0)
	{
		m.add(in1 / in2);
		in1 = in2;
		in2 = ir;
		ir = in1 % in2;
	};
	m.add(in1);

	return (long) m.size();
};

int GetDegree(long n1 , long n2)
{
	int degree = 0;
	while(n1 * pow(10.0, degree) < n2)
		degree++;
	return degree;
};
