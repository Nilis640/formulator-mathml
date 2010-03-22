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

#if !defined( __HMATHLNG_MM_DOMAIN_H__ )
#define __HMATHLNG_MM_DOMAIN_H__

///////////////////////////////////////////////////////////////////////////////
// TASK ID

enum CDsrMathModel
{
	DSRMM_NONE = 0,
	DSRMM_SET,
	DSRMM_DIGRAPH,
	DSRMM_NET,
	DSRMM_BOOL_FUN,
	DSRMM_R_BOOL_FUN,
	DSRMM_DFA,
	DSRMM_CFG,
	DSRMM_PETRI_NET,
	DSRMM_NEURON_NET,
	DSRMM_NUMERY,
	DSRMM_FUNCTION,
	DSRMM_FRACTAL,
	DSRMM_VECTOR_FUNCTION,
	DSRMM_GROUPS,
	DSRMM_POLYNOM,
	DSRMM_RATIONAL,
	DSRMM_GALUA,
	DSRMM_VECTOR,
	DSRMM_SLAU,
	DSRMM_SNAU,
	DSRMM_ODE,
	DSRMM_EXTREMUM,
	DSRMM_PM,
	DSRMM_TAU,

	//===
	DSRMM_LAST
};

///////////////////////////////////////////////////////////////////////////////
// SUBTASK ID

#define INT_MM_ID_SETS_SPECIFIC			101
#define INT_MM_ID_SETS_ABSTRACT			102
#define INT_MM_ID_NUMERIC_NATURAL		201
#define INT_MM_ID_NUMERIC_INTEGER		202
#define INT_MM_ID_NUMERIC_INTEGER_CONGR	0
#define INT_MM_ID_NUMERIC_RATIONAL		203
#define INT_MM_ID_NUMERIC_REAL			204
#define INT_MM_ID_NUMERIC_COMPLEX		205
#define INT_MM_ID_NUMERIC_BIG_NUMBER	2010

#define INT_MM_ID_NUMERIC_DIOFANT_1		2011
#define INT_MM_ID_NUMERIC_DIOFANT_2		2012

#define INT_MM_ID_FUNCTION_DISCRETE_NR	3111
#define INT_MM_ID_FUNCTION_DISCRETE_R	3112
#define INT_MM_ID_FUNCTION_REAL_E		3121
#define INT_MM_ID_FUNCTION_REAL_I		3122
#define INT_MM_ID_FUNCTION_REAL_P		3123
#define INT_MM_ID_ODE					801

#define INT_MM_ID_TAU_LINEAR			1300
#define INT_MM_ID_TAU_ORDINAL			1310
#define INT_MM_ID_TAU_STRUCT			1311
#define INT_MM_ID_TAU_DISCRETE_STRUCT	1312
#define INT_MM_ID_TAU_NONLINEAR_STRUCT	1320

#define AGW_CONGRUENCE_1				2020001
#define AGW_CONGRUENCE_2				2020002
#define AGW_CONGRUENCE_3				2020003
#define AGW_CONGRUENCE_4				2020004
#define AGW_CONGRUENCE_5				2020005
#define AGW_CONGRUENCE_6				2020006
#define AGW_CONGRUENCE_11				20200011

///////////////////////////////////////////////////////////////////////////////
// TASK PROPERTY ID

enum CDsrMMProperty
{
	DSR_HAVE_NONE = 0,
	DSR_HAVE_VAR_DECL,
	DSR_HAVE_VAR_TYPE,
	DSR_HAVE_VECTOR,
	DSR_HAVE_EQUATION,
	DSR_HAVE_INEQUATION,
	DSR_HAVE_FUNCTION_E,
	DSR_HAVE_FUNCTION_I,
	DSR_HAVE_FUNCTION_P,
	DSR_HAVE_FUNCTION_D,
	DSR_HAVE_FRACTAL,
	DSR_HAVE_RANGE,
	DSR_HAVE_CYCLE,
	DSR_HAVE_SET,
	
	//===

	DSR_HAVE_LAST
};

///////////////////////////////////////////////////////////////////////////////

#endif
