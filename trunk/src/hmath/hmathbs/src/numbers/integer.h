#ifndef _INTEGER_H_
#define _INTEGER_H_

#include "../hmathbs_global.h"
#include "../cdsrnumbers/number.h"
#include "../cdsrarrays/cdsrarray.h"

MATHBASE_API long SystemConvertion(CDSRInteger& n, long p, CDSRArray<long>& m);
MATHBASE_API long Diofant1(long n, CDSRArray<CDSRInteger>& m, CDSRArray<CDSRInteger>& root);
MATHBASE_API void Diofant2(CDSRInteger& a, CDSRInteger& b, CDSRInteger& c, CDSRArray<CDSRInteger>& x, CDSRArray<CDSRInteger>& y);

///////////////////////////////////////////////////////////////
//	Copy from Natural
MATHBASE_API long GetIntegerSimplyDivider(CDSRInteger &digit, CDSRArray<CDSRInteger> &ar, CDSRArray<long> &count);
MATHBASE_API long Eratosfen(CDSRInteger& n, CDSRArray<CDSRInteger>& m);
MATHBASE_API int IsIntegerSimple(CDSRInteger &n);
MATHBASE_API long EratosfenRange(CDSRInteger& n1, CDSRInteger& n2, CDSRArray<CDSRInteger>& m);
MATHBASE_API int IsDivider(CDSRInteger& n1, CDSRInteger& n2);
MATHBASE_API CDSRInteger Euqlid(CDSRInteger& n1, CDSRInteger& n2);
MATHBASE_API CDSRInteger EuqlidArray(CDSRArray<CDSRInteger>& m);
MATHBASE_API CDSRInteger GetLeastCommonMultiple(CDSRInteger& n1, CDSRInteger& n2);
MATHBASE_API CDSRInteger GetLeastCommonMultipleArray(CDSRArray<CDSRInteger>& m);

#endif
