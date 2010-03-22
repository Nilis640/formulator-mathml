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

#if !defined(__MATHEMATICAL_CONST_H__)
#define __MATHEMATICAL_CONST_H__

#include <float.h>

//////////////////////////////////////////////////////////////////////
#define MATH_2_PI			6.283185307179586476925286766559
#define MATH_3_PI_2			4.7123889803846898576939650749193
#define MATH_PI				3.1415926535897932384626433832795
#define MATH_PI_2			1.5707963267948966192313216916398
#define MATH_PI_3			1.0471975511965977461542144610932
#define MATH_PI_4			0.78539816339744830961566084581988
#define MATH_PI_6			0.52359877559829887307710723054658

//////////////////////////////////////////////////////////////////////

#define MATH_REAL_EPSILON	(1e3 * DBL_EPSILON)

//////////////////////////////////////////////////////////////////////
#define POLYNOM_MAXITERS				1024
#define POLYNOM_PRECISION				1e-6

#define ALMASLAE_SITER_PREC				1e-4
#define ALMASLAE_MAX_ITER_COUNT			1024

#define ALMAFUNC_ITERATIONCOUNT			1024
#define ALMAFUNC_PRECISION				1e-3
#define ALMAFUNC_FIRST_APPR				1e-1

#define ALMA_ODE_PREC					1e-3
#define ALMA_ODE_PREC_MUL				1e-2

#define EIGENVALUES_PRECISION			1e-3

//////////////////////////////////////////////////////////////////////

#endif // __MATHEMATICAL_CONST_H__
