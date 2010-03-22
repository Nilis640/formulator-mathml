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

#ifndef __COMMON_CDSRMVECTOR_H__
#define __COMMON_CDSRMVECTOR_H__

#include "cdsrarray.h"

template <class T> class CDSRMVector : public CDSRBaseVector<T>  
{
public:
	CDSRMVector( long size = 0, const T& value = T() )
		: CDSRBaseVector<T>( size, value )
	{
	}
	CDSRMVector( const CDSRMVector& vec )
		: CDSRBaseVector<T>( vec )
	{
	}
	CDSRMVector( typename CDSRBaseVector<T>::iterator first, long size )
		: CDSRBaseVector<T>( first, size )
	{
	}
	CDSRMVector( typename CDSRBaseVector<T>::const_iterator first, long size )
		: CDSRBaseVector<T>( first, size )
	{
	}
	virtual ~CDSRMVector()
	{
	}

	const CDSRMVector<T>& operator += ( const CDSRMVector<T>& vec )
	{
		if ( CDSRBaseVector<T>::size() != vec.size() )
			throw "Vectors must be of the same dimension";
		for ( long i = 0; i < (long) CDSRBaseVector<T>::size(); i++ )
			(*this)[ i ] += vec[ i ];
		return (*this);
	}

	const CDSRMVector<T>& operator -= ( const CDSRMVector<T>& vec )
	{
		if ( CDSRBaseVector<T>::size() != vec.size() )
			throw "Vectors must be of the same dimension";
		for ( long i = 0; i < (long) CDSRBaseVector<T>::size(); i++ )
			(*this)[ i ] -= vec[ i ];
		return (*this);
	}

	const CDSRMVector<T>& operator *= ( const T& value )
	{
		for ( long i = 0; i < (long) CDSRBaseVector<T>::size(); i++ )
			(*this)[ i ] *= value;
		return (*this);
	}

	const CDSRMVector<T>& operator /= ( const T& value )
	{
		for ( long i = 0; i < (long) CDSRBaseVector<T>::size(); i++ )
			(*this)[ i ] /= value;
		return (*this);
	}

	template<typename T2> friend CDSRMVector<T2> operator + ( CDSRMVector<T2>& vec1, CDSRMVector<T2>& vec2 );
	template<typename T2> friend CDSRMVector<T2> operator - ( CDSRMVector<T2>& vec1, CDSRMVector<T2>& vec2 );
	template<typename T2> friend T2              operator * ( CDSRMVector<T2>& vec1, CDSRMVector<T2>& vec2 );
	template<typename T2> friend CDSRMVector<T2> operator * ( CDSRMVector<T2>& vec1, T2 value );
};

//////////////////////////////////////////////////////////////////////

template<typename T> CDSRMVector<T> operator + ( CDSRMVector<T>& vec1, CDSRMVector<T>& vec2 )
{
	if ( vec1.size() != vec2.size() )
		throw "Vectors must be of the same dimension";

	long i;
	CDSRMVector<T> ret( vec1.size() );

	for ( i = 0; i < vec1.size(); i++ )
		ret[ i ] = vec1[ i ] + vec2[ i ];
			
	return ret;
}

template<typename T> CDSRMVector<T> operator - ( CDSRMVector<T>& vec1, CDSRMVector<T>& vec2 )
{
	if ( vec1.size() != vec2.size() )
		throw "Vectors must be of the same dimension";

	long i;
	CDSRMVector<T> ret( vec1.size() );

	for ( i = 0; i < vec1.size(); i++ )
		ret[ i ] = vec1[ i ] - vec2[ i ];
			
	return ret;
}

template<typename T> T operator * ( CDSRMVector<T>& vec1, CDSRMVector<T>& vec2 )
{
	if ( vec1.size() != vec2.size() )
		throw "Vectors must be of the same dimension";

	T ret = 0;
	long i;

	for ( i = 0; i < vec1.size(); i++ )
		ret += vec1[ i ] * vec2[ i ];
		
	return ret;
}

template<typename T> CDSRMVector<T> operator * ( CDSRMVector<T>& vec1, T value )
{
	CDSRMVector<T> ret( vec1 );
	long i;

	for ( i = 0; i < ret.size(); i++ )
		ret[ i ] *= value;
		
	return ret;
}

//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
#endif //__COMMON_CDSRMVECTOR_H__
