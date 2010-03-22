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

#ifndef __COMMON_CDSRMATRIX_H__
#define __COMMON_CDSRMATRIX_H__

#include "cdsrbasematrix.h"

template<class T> class CDSRMatrix : public CDSRBaseMatrix<T>
{
public:
	CDSRMatrix( const CDSRMatrix& matrix )
		: CDSRBaseMatrix<T>( matrix )
	{
	}
	CDSRMatrix( typename CDSRBaseMatrix<T>::iterator first, long row, long col )
		: CDSRBaseMatrix<T>( first, row, col )
	{
	}
	CDSRMatrix( typename CDSRBaseMatrix<T>::const_iterator first, long row, long col )
		: CDSRBaseMatrix<T>( first, row, col )
	{
	}
	CDSRMatrix( long row = 0, long col = 0, const T& val = T() )
		: CDSRBaseMatrix<T>( row, col, val )
	{
	}
	CDSRMatrix( T **first, long row, long col )
		: CDSRBaseMatrix<T>( first, row, col )
	{
	}
	CDSRMatrix( const T **first, long row, long col )
		: CDSRBaseMatrix<T>( first, row, col )
	{
	}
	virtual ~CDSRMatrix()
	{
	}
};

//////////////////////////////////////////////////////////////////////

#endif // __COMMON_ÑDSRMATRIX_H__
