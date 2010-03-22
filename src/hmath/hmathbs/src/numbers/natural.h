#ifndef _NATURAL_H_
#define _NATURAL_H_

#include "../hmathbs_global.h"
#include "../cdsrnumbers/cdsrnatural.h"
#include "../cdsrarrays/cdsrarray.h"

MATHBASE_API long Eratosfen(CDSRNatural& n, CDSRArray<CDSRNatural>& m);
MATHBASE_API long EratosfenACM(CDSRNatural& n, CDSRArray<CDSRNatural>& m);
MATHBASE_API int IsNaturalSimple(CDSRNatural &n);
MATHBASE_API long GetNaturalSimplyDivider(CDSRNatural &digit, CDSRArray<CDSRNatural> &ar, CDSRArray<long> &count);
MATHBASE_API long EratosfenRange(CDSRNatural& n1, CDSRNatural& n2, CDSRArray<CDSRNatural>& m);
MATHBASE_API int IsDivider(CDSRNatural& n1, CDSRNatural& n2);
MATHBASE_API CDSRNatural Euqlid(CDSRNatural& n1, CDSRNatural& n2);
MATHBASE_API CDSRNatural EuqlidArray(CDSRArray<CDSRNatural>& m);
MATHBASE_API CDSRNatural GetLeastCommonMultiple(CDSRNatural& n1, CDSRNatural& n2);
MATHBASE_API CDSRNatural GetLeastCommonMultipleArray(CDSRArray<CDSRNatural>& m);

MATHBASE_API long SystemConvertion(CDSRNatural& n, long base, CDSRArray<long>& m);

#endif
