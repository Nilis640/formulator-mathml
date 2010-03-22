
#ifndef __COMPLEX_H__
#define __COMPLEX_H__

#include "../hmathbs_global.h"
#include "../cdsrnumbers/cdsrcomplex.h"

MATHBASE_API void ConvertComplexNumberForm(CDSRComplex& src, CDSRComplex& dest, COMPLEX_NUMBER_FORM destForm = CNF_ALGERAIC, COMPLEX_NUMBER_FORM srcForm = CNF_ALGERAIC);

inline double GetComplexMagnitude(CDSRComplex& c);
inline double GetComplexPhaseAngle(CDSRComplex& c);

#endif //__COMPLEX_H__
