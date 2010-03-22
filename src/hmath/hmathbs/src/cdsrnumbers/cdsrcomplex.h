#ifndef _CSDRCOMPLEX_H_
#define _CSDRCOMPLEX_H_

#include <complex>
#include "cdsrreal.h"

typedef enum
{
	CNF_ALGERAIC,
	CNF_EXPONENTIAL,
	CNF_TRIGONOMETRIC
} COMPLEX_NUMBER_FORM;

typedef std::complex<CDSRReal> CDSRComplex;

#endif
