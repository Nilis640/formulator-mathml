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
#include "../cdsrnumbers/cdsrreal.h"
#include "../cdsrarrays/cdsrmmatrix.h"
#include "matrix_internal.h"

double SumOffdiagonalSqrs( CDSRMMatrix<CDSRReal>& A )
{
	CDSRReal	t2 = 0.0;
	long i, j;

	for( i = 0; i < A.n_row() - 1 ; i++ )
		for( j = i + 1; j < A.n_column(); j++ )
			t2 += A(i, j) * A(i, j);
	t2 *= 2;
	return t2;
}
