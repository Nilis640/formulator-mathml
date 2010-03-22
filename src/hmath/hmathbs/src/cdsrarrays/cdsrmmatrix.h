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

#ifndef __COMMON_CDSRMMATRIX_H__
#define __COMMON_CDSRMMATRIX_H__

#include "cdsrbasematrix.h"
#include "cdsrmvector.h"

template<class T> class CDSRMMatrix : public CDSRBaseMatrix<T>
{
public:
	CDSRMMatrix( const CDSRMMatrix& matrix )
		: CDSRBaseMatrix<T>( matrix )
	{
	}
	CDSRMMatrix( typename CDSRBaseMatrix<T>::iterator first, long row, long col )
		: CDSRBaseMatrix<T>( first, row, col )
	{
	}
	CDSRMMatrix( typename CDSRBaseMatrix<T>::const_iterator first, long row, long col )
		: CDSRBaseMatrix<T>( first, row, col )
	{
	}
	CDSRMMatrix( long row = 0, long col = 0, const T& val = T() )
		: CDSRBaseMatrix<T>( row, col, val )
	{
	}
	CDSRMMatrix( T **first, long row, long col )
		: CDSRBaseMatrix<T>( first, row, col )
	{
	}
	CDSRMMatrix( const T **first, long row, long col )
		: CDSRBaseMatrix<T>( first, row, col )
	{
	}
	virtual ~CDSRMMatrix()
	{
	}

	template<typename T2> friend CDSRMMatrix<T2> operator + ( CDSRMMatrix<T2>& matr1, CDSRMMatrix<T2>& matr2 );
	template<typename T2> friend CDSRMMatrix<T2> operator - ( CDSRMMatrix<T2>& matr1, CDSRMMatrix<T2>& matr2 );
	template<typename T2> friend CDSRMMatrix<T2> operator * ( CDSRMMatrix<T2>& matr1, CDSRMMatrix<T2>& matr2 );
	template<typename T2> friend CDSRMVector<T2> operator * ( CDSRMVector<T2>& vec, CDSRMMatrix<T2>& matr );
	template<typename T2> friend CDSRMVector<T2> operator * ( CDSRMMatrix<T2>& matr, CDSRMVector<T2>& vec );
	template<typename T2> friend CDSRMMatrix<T2> operator * ( const T2& value, CDSRMMatrix<T2>& matr );
};

//////////////////////////////////////////////////////////////////////

template<class T> CDSRMMatrix<T> operator + ( CDSRMMatrix<T>& matr1, CDSRMMatrix<T>& matr2 )
{
	long i, j;
	CDSRMMatrix<T> ret( matr1.row(), matr1.n_column() );

	if ( matr1.row() != matr2.row() || matr1.n_col() != matr2.n_col() )
		throw "Matrixs must be of the same dimension";

	for ( i = 0; i < matr1.row(); i++ )
		for ( j = 0; j < matr1.n_col(); j++ )
			ret( i, j ) = matr1( i, j ) + matr2( i, j );
	return ret;
};

//////////////////////////////////////////////////////////////////////

template<class T> CDSRMMatrix<T> operator - ( CDSRMMatrix<T>& matr1, CDSRMMatrix<T>& matr2 )
{
	long i, j;
	CDSRMMatrix<T> ret( matr1.row(), matr1.n_column() );

	if ( matr1.row() != matr2.row() || matr1.n_col() != matr2.n_col() )
		throw "Matrixs must be of the same dimension";

	for ( i = 0; i < matr1.row(); i++ )
		for ( j = 0; j < matr1.n_col(); j++ )
			ret( i, j ) = matr1( i, j ) - matr2( i, j );
	return ret;
};

//////////////////////////////////////////////////////////////////////

template<class T> CDSRMMatrix<T> operator * ( CDSRMMatrix<T>& matr1, CDSRMMatrix<T>& matr2 )
{
	T value;
	long i, j, k;
	CDSRMMatrix<T> ret( matr1.row(), matr2.col() );

	if ( matr1.row() != matr2.col() || matr1.col() != matr2.row() )
		throw "Matrixs must be of the same dimension";

	for ( i = 0; i < matr1.row(); i++ )
		for ( j = 0; j < matr2.col(); j++ )
		{
			value = 0;
			for ( k = 0; k < matr1.col(); k++ )
				value += matr1( i, k ) * matr2( k, j );
			ret( i, j ) = value;
		}
	return ret;
};

//////////////////////////////////////////////////////////////////////

template<class T> CDSRMVector<T> operator * ( CDSRMVector<T>& vec, CDSRMMatrix<T>& matr )
{
	long i, j;
	CDSRMVector<T> ret( matr.col() );

	if ( vec.size() != matr.row() )
		throw CDSRCompareException( vec.size(),	matr.row() * matr.col() );
	for ( i = 0; i < matr.col(); i++ )
	{
		ret[ i ] = 0;
		for ( j = 0; j < vec.size(); j++ )
			ret[ i ] = ret[ i ] + vec[ j ] * matr( j, i );
	}
	return ret;
};

//////////////////////////////////////////////////////////////////////

template<class T> CDSRMVector<T> operator * ( CDSRMMatrix<T>& matr, CDSRMVector<T>& vec )
{
	long i, j;
	CDSRMVector<T> ret( matr.row() );

	if ( vec.size() != matr.col() )
		throw CDSRCompareException( vec.size(),	matr.row() * matr.col() );
	for ( i = 0; i < matr.row(); i++ )
	{
		ret[ i ] = 0;
		for ( j = 0; j < matr.col(); j++ )
			ret[ i ] +=  matr( i, j ) * vec[ j ];
	}
	return ret;
};

//////////////////////////////////////////////////////////////////////

template<class T> CDSRMMatrix<T> operator * ( const T& value, CDSRMMatrix<T>& matr )
{
	long n = matr.row();
	long m = matr.col();
	long i, j;
	CDSRMMatrix<T> ret( n, m );

	for ( i = 0; i < n; i++ )
		for ( j = 0; j < m; j++ )
			ret( i, j ) = value * matr( i, j );
	return ret;
};

//////////////////////////////////////////////////////////////////////

#endif // __COMMON_CDSRMMATRIX_H__
