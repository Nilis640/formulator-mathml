#ifndef _ROOTINFO_H_
#define _ROOTINFO_H_

#include "cdsrreal.h"
#include "cdsrcomplex.h"

typedef struct tagROOT_INFO
{
	CDSRReal 	real;
	CDSRReal 	image;
	int			isComplex;
	long		multiplicity;

	tagROOT_INFO()
	{
		real		= 0.0;
		image		= 0.0;
		isComplex	= FALSE;
		multiplicity	= 1;
	}
	CDSRComplex getComplexValue( void )
	{
		return isComplex ? CDSRComplex( real, image ) : CDSRComplex( real, 0.0 );
	}
} ROOT_INFO;

#endif
