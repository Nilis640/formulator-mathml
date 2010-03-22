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

#include <math.h>
#include "../cdsrnumbers/cdsrcomplex.h"
#include "complex.h"

#if !defined(M_PI)
#define M_PI        3.14159265358979323846
#endif
#if !defined(M_PI_2)
#define M_PI_2      1.57079632679489661923
#endif
//#define M_PI_4      0.785398163397448309616

/////////////////////////////////////////////////////////////////////////////////////////////
//	Internal Functions

/////////////////////////////////////////////////////////////////////////////////////////////
//
double GetComplexMagnitude(CDSRComplex& c)
{
	return (sqrt(c.real()*c.real() + c.imag()*c.imag()));
}

/////////////////////////////////////////////////////////////////////////////////////////////
//
double GetComplexPhaseAngle(CDSRComplex& c)
{
	double r, im;

	r = c.real(); im = c.imag();
	if(r == 0)
	{
		if(im >= 0)
			return (M_PI_2); // r == 0 && im > 0
		else
			return (-M_PI_2); // r == 0 && im < 0
	}
	else
	{
		if(r < 0)
		{
			if(im >= 0)
				return (atan(im / r) + M_PI); // r < 0 && im >=0
			else
				return (atan(im / r) - M_PI); // r < 0 && im < 0
		}
		else
			return atan(im / r);  // r > 0;
	}
	return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////

MATHBASE_API void ConvertComplexNumberForm(CDSRComplex& src, CDSRComplex& dest, COMPLEX_NUMBER_FORM destForm, COMPLEX_NUMBER_FORM srcForm)
{
	if(srcForm == destForm)
	{
		dest = src;
		return;
	}//end if

	switch(srcForm)
	{
	case CNF_ALGERAIC:

		switch(destForm)
		{
		case CNF_EXPONENTIAL:
		case CNF_TRIGONOMETRIC:
			{
				dest = CDSRComplex(GetComplexMagnitude(src), GetComplexPhaseAngle(src));
				break;
			}//ens case CNF_EXPONENTIAL
		default:
			dest = src;
		}//end switch(destForm)

		break;

	case CNF_EXPONENTIAL:
	case CNF_TRIGONOMETRIC:

		switch(destForm)
		{
		case CNF_ALGERAIC:
			{
				dest = CDSRComplex(std::polar(src.real(), src.imag()));
				break;
			}//ens case CNF_ALGERAIC
		default:
			dest = src;
		}//end switch(destForm)

		break;
	}//end switch(srcForm)
}

