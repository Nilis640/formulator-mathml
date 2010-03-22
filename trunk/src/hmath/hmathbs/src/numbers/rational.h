#ifndef _RATIONAL_H_
#define _RATIONAL_H_

#include "../hmathbs_global.h"
#include "../cdsrnumbers/number.h"
#include "../cdsrarrays/cdsrarray.h"

MATHBASE_API long DecimalFraction(CDSRRational& r, long kmax, CDSRArray<long>&m);
MATHBASE_API long CircFraction(CDSRRational& r, CDSRArray<long>&m);

#endif
