
#include <math.h>
#include <cstdlib>
#include "cdsrrational.h"

//Максимальний дільник ділення навпіл
MATHBASE_API long EvklidMaxBinDiv( long in1, long in2 ) 
{
	if(in1 == 0 || in2 == 0) return 1;

	long ir;
	in1 = abs(in1); in2 = abs(in2);
	if(in1 < in2)
	{
		long n = in1;
		in1 = in2;
		in2 = n;
	}//end if

	while((ir = in1 % in2) != 0)
	{
		in1 = in2;
		in2 = ir;
	}//end while

	return in2;

	/*long r=1,s=1,c=0,k,t;
	if( (a == 0) || ( b == 0 ) ) 
		return abs( a+b);
	while( (a % 2) == 0 ) { a/=2; r*=2; }	//120,130
	while( (b % 2) == 0 ) { b/=2; s*=2; }	//140,150
	if( s > r ) k = r; else k = s;			//160
	while ( a != b )						//170
	{
		if( a > b )							//180
		{
			a = a - b;
		} 
		else
		{
			b = b - a;
			t=a; a=b; b=t;
		}
		do {
			a /= 2;
		} while(a % 2 == 0);
	}
	return a * k;*/
}


//Мінімальний множник 
MATHBASE_API long EvklidMinMul( long a, long b ) {
	long MaxDiv=EvklidMaxBinDiv(a,b);
	if( MaxDiv == 0 ) 
		throw "Division by zero";
    return a*b/MaxDiv;
}
