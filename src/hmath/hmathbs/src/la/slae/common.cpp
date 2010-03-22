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

#include <string>
#include <cstring>

#include "common.h"
#include "slae_internal.h"
#include "gauss.h"
#include "jordangauss.h"
#include "lu.h"
#include "reflection.h"
#include "relaxation.h"
#include "rotation.h"
#include "siter.h"
#include "ziter.h"

//////////////////////////////////////////////////////////////////////

MATHBASE_API 
void _Inverse( CDSRMMatrix<CDSRComplex> * /*pA*/, CDSRMMatrix<CDSRComplex> * /*pInv*/ )
{
	throw "Function is not supported in the current version";
}
MATHBASE_API 
void _DeterminantCR( CDSRMMatrix<CDSRInteger> * /*pA*/, CDSRInteger& /*rlDet*/ )
{
	throw "Function is not supported in the current version";
}
MATHBASE_API 
void _DeterminantCR( CDSRMMatrix<CDSRComplex> * /*pA*/, CDSRComplex& /*rlDet*/ )
{
	throw "Function is not supported in the current version";
}

MATHBASE_API 
void _Inverse( CDSRMMatrix<CDSRReal> *pA, CDSRMMatrix<CDSRReal> *pInv )
{
	if( pA->n_row() != pA->n_column() || pInv->n_row() != pInv->n_column() )
		throw "The matrix is not square";
	if( pA->n_row() != pInv->n_row() )
		throw "Internal problem - sizes of the source and destination matrix are not equal";

	long i, j, k;
	CDSRReal value;
	CDSRMMatrix<CDSRReal> matrix( (*pA) );
	
	for( i = 0; i < pA->n_row(); i++ )
	{
		for( j = 0; j < i; j++ )
			(*pInv)( i, j ) = 0.0;
		(*pInv)( i, j ) = 1.0;
		for( j = i + 1; j < pA->n_row(); j++ )
			(*pInv)( i, j ) = 0.0;
	}

	//
	for( i = 0; i < pA->n_row(); i++ )
	{
		if( IsNull( matrix( i, i ) ) )
			throw "SLAE cannot be solved by this method or matrix is singular";

		for( k = i; k < pA->n_row(); k++ )
		{
			value = matrix( k, i );
			if( IsNotNull( value ) && IsNotOne( value ) )
			{
				for( j = i + 1; j < pA->n_row(); j++ )
					matrix( k, j ) /= value;
				matrix( k, i ) = 1.0;

				for( j = 0; j < pA->n_row(); j++ )
					(*pInv)( k, j ) /= value;
			}
		}

		//
		for( k = i + 1; k < pA->n_row(); k++ )
		{
			value = matrix( k, i );
			if( IsNotNull( value ) )
			{
				for( j = i + 1; j < pA->n_row(); j++ )
					matrix( k, j ) -= matrix( i, j );
				matrix( k, i ) = 0.0;

				for( j = 0; j < pA->n_row(); j++ )
					(*pInv)( k, j ) -= (*pInv)( i, j );
			}
		}
	}

	// Up
	for( i = pA->n_row() - 1; i > 0; i-- )
	{
		for( k = 0; k <= i; k++ )
		{
			value = matrix( k, i );
			if( IsNotNull( value ) && IsNotOne( value ) )
			{
				for( j = k; j < i; j++ )
					matrix( k, j ) /= value;
				matrix( k, i ) = 1.0;

				for( j = 0; j < pA->n_row(); j++ )
					(*pInv)( k, j ) /= value;
			}
		}

		//
		for( k = 0; k < i; k++ )
		{
			value = matrix( k, i );
			if( IsNotNull( value ) )
			{
				for( j = k; j < i; j++ )
					matrix( k, j ) -= matrix( i, j );
				matrix( k, i ) = 0.0;

				for( j = 0; j < pA->n_row(); j++ )
					(*pInv)( k, j ) -= (*pInv)( i, j );
			}
		}
	}

	for( i = 0; i < pA->n_row(); i++ )
	{
		value = matrix( i, i );
		if( IsNotOne( value ) )
			for( j = 0; j < pA->n_row(); j++ )
				(*pInv)( i, j ) /= value;
	}
}

//////////////////////////////////////////////////////////////////////

MATHBASE_API 
void _InverseC( CDSRMMatrix<CDSRReal> *pA, CDSRMMatrix<CDSRReal> *pInv )
{
	if( pA->n_row() != pA->n_column() || pInv->n_row() != pInv->n_column() )
		throw "The matrix is not square";
	if( pA->n_row() != pInv->n_row() )
		throw "Internal problem - sizes of the source and destination matrix are not equal";

	long i, j, k;
	CDSRReal value;
	CDSRMMatrix<CDSRReal> matrix( (*pA) );
	CDSRMVector<long> col( pA->n_row() );

	for( i = 0; i < pA->n_row(); i++ )
		col[ i ] = i;

	for( i = 0; i < pA->n_row(); i++ )
	{
		for( j = 0; j < i; j++ )
			(*pInv)( i, j ) = 0.0;
		(*pInv)( i, j ) = 1.0;
		for( j = i + 1; j < pA->n_row(); j++ )
			(*pInv)( i, j ) = 0.0;
	}

	//
	for( i = 0; i < pA->n_row(); i++ )
	{
		if( IsNull( matrix( i, col[ i ] ) ) )
			_IndexC( i, &matrix, &col );

		for( k = i; k < pA->n_row(); k++ )
		{
			value = matrix( k, col[ i ] );
			if( IsNotNull( value ) && IsNotOne( value ) )
			{
				for( j = i + 1; j < pA->n_row(); j++ )
					matrix( k, col[ j ] ) /= value;
				matrix( k, col[ i ] ) = 1.0;

				for( j = 0; j < pA->n_row(); j++ )
					(*pInv)( k, col[ j ] ) /= value;
			}
		}

		//
		for( k = i + 1; k < pA->n_row(); k++ )
		{
			value = matrix( k, col[ i ] );
			if( IsNotNull( value ) )
			{
				for( j = i + 1; j < pA->n_row(); j++ )
					matrix( k, col[ j ] ) -= matrix( i, col[ j ] );
				matrix( k, col[ i ] ) = 0.0;

				for( j = 0; j < pA->n_row(); j++ )
					(*pInv)( k, col[ j ] ) -= (*pInv)( i, col[ j ] );
			}
		}
	}

	// Up
	for( i = pA->n_row() - 1; i > 0; i-- )
	{
		for( k = 0; k <= i; k++ )
		{
			value = matrix( k, col[ i ] );
			if( IsNotNull( value ) && IsNotOne( value ) )
			{
				for( j = k; j < i; j++ )
					matrix( k, col[ j ] ) /= value;
				matrix( k, col[ i ] ) = 1.0;

				for( j = 0; j < pA->n_row(); j++ )
					(*pInv)( k, col[ j ] ) /= value;
			}
		}

		//
		for( k = 0; k < i; k++ )
		{
			value = matrix( k, col[ i ] );
			if( IsNotNull( value ) )
			{
				for( j = k; j < i; j++ )
					matrix( k, col[ j ] ) -= matrix( i, col[ j ] );
				matrix( k, col[ i ] ) = 0.0;

				for( j = 0; j < pA->n_row(); j++ )
					(*pInv)( k, col[ j ] ) -= (*pInv)( i, col[ j ] );
			}
		}
	}

	for( i = 0; i < pA->n_row(); i++ )
	{
		value = matrix( i, col[ i ] );
		if( IsNotOne( value ) )
			for( j = 0; j < pA->n_row(); j++ )
				(*pInv)( i, col[ j ] ) /= value;
	}
}

//////////////////////////////////////////////////////////////////////

MATHBASE_API 
void _InverseR( CDSRMMatrix<CDSRReal> *pA, CDSRMMatrix<CDSRReal> *pInv )
{
	if( pA->n_row() != pA->n_column() || pInv->n_row() != pInv->n_column() )
		throw "The matrix is not square";
	if( pA->n_row() != pInv->n_row() )
		throw "Internal problem - sizes of the source and destination matrix are not equal";

	long i, j, k;
	CDSRReal value;
	CDSRMMatrix<CDSRReal> matrix( (*pA) );
	CDSRMVector<long> row( pA->n_row() );

	for( i = 0; i < pA->n_row(); i++ )
		row[ i ] = i;

	for( i = 0; i < pA->n_row(); i++ )
	{
		for( j = 0; j < i; j++ )
			(*pInv)( i, j ) = 0.0;
		(*pInv)( i, j ) = 1.0;
		for( j = i + 1; j < pA->n_row(); j++ )
			(*pInv)( i, j ) = 0.0;
	}

	//
	for( i = 0; i < pA->n_row(); i++ )
	{
		if( IsNull( matrix( row[ i ], i ) ) )
			_IndexR( i, &matrix, &row );

		for( k = i; k < pA->n_row(); k++ )
		{
			value = matrix( row[ k ], i );
			if( IsNotNull( value ) && IsNotOne( value ) )
			{
				for( j = i + 1; j < pA->n_row(); j++ )
					matrix( row[ k ], j ) /= value;
				matrix( row[ k ], i ) = 1.0;

				for( j = 0; j < pA->n_row(); j++ )
					(*pInv)( row[ k ], j ) /= value;
			}
		}

		//
		for( k = i + 1; k < pA->n_row(); k++ )
		{
			value = matrix( row[ k ], i );
			if( IsNotNull( value ) )
			{
				for( j = i + 1; j < pA->n_row(); j++ )
					matrix( row[ k ], j ) -= matrix( row[ i ], j );
				matrix( row[ k ], i ) = 0.0;

				for( j = 0; j < pA->n_row(); j++ )
					(*pInv)( row[ k ], j ) -= (*pInv)( row[ i ], j );
			}
		}
	}

	// Up
	for( i = pA->n_row() - 1; i > 0; i-- )
	{
		for( k = 0; k <= i; k++ )
		{
			value = matrix( row[ k ], i );
			if( IsNotNull( value ) && IsNotOne( value ) )
			{
				for( j = k; j < i; j++ )
					matrix( row[ k ], j ) /= value;
				matrix( row[ k ], i ) = 1.0;

				for( j = 0; j < pA->n_row(); j++ )
					(*pInv)( row[ k ], j ) /= value;
			}
		}

		//
		for( k = 0; k < i; k++ )
		{
			value = matrix( row[ k ], i );
			if( IsNotNull( value ) )
			{
				for( j = k; j < i; j++ )
					matrix( row[ k ], j ) -= matrix( row[ i ], j );
				matrix( row[ k ], i ) = 0.0;

				for( j = 0; j < pA->n_row(); j++ )
					(*pInv)( row[ k ], j ) -= (*pInv)( row[ i ], j );
			}
		}
	}

	for( i = 0; i < pA->n_row(); i++ )
	{
		value = matrix( row[ i ], i );
		if( IsNotOne( value ) )
			for( j = 0; j < pA->n_row(); j++ )
				(*pInv)( row[ i ], j ) /= value;
	}
}

//////////////////////////////////////////////////////////////////////

MATHBASE_API 
void _InverseCR( CDSRMMatrix<CDSRReal> *pA, CDSRMMatrix<CDSRReal> *pInv )
{
	if( pA->n_row() != pA->n_column() || pInv->n_row() != pInv->n_column() )
		throw "The matrix is not square";
	if( pA->n_row() != pInv->n_row() )
		throw "Internal problem - sizes of the source and destination matrix are not equal";

	long i, j, k;
	CDSRReal value;
	CDSRMMatrix<CDSRReal> matrix( (*pA) );
	CDSRMVector<long> row( pA->n_row() );
	CDSRMVector<long> col( pA->n_row() );

	for( i = 0; i < pA->n_row(); i++ )
		row[ i ] = col[ i ] = i;

	// ‘ормируем E матрицу
	for( i = 0; i < pA->n_row(); i++ )
	{
		for( j = 0; j < i; j++ )
			(*pInv)( i, j ) = 0.0;
		(*pInv)( i, j ) = 1.0;
		for( j = i + 1; j < pA->n_row(); j++ )
			(*pInv)( i, j ) = 0.0;
	}

	//
	for( i = 0; i < pA->n_row(); i++ )
	{
		if( IsNull( matrix( row[ i ], col[ i ] ) ) )
			_IndexCR( i, &matrix, &row, &col );

		for( k = i; k < pA->n_row(); k++ )
		{
			value = matrix( row[ k ], col[ i ] );
			if( IsNotNull( value ) && IsNotOne( value ) )
			{
				for( j = i + 1; j < pA->n_row(); j++ )
					matrix( row[ k ], col[ j ] ) /= value;
				matrix( row[ k ], col[ i ] ) = 1.0;

				for( j = 0; j < pA->n_row(); j++ )
					(*pInv)( row[ k ], col[ j ] ) /= value;
			}
		}

		//
		for( k = i + 1; k < pA->n_row(); k++ )
		{
			value = matrix( row[ k ], col[ i ] );
			if( IsNotNull( value ) )
			{
				for( j = i + 1; j < pA->n_row(); j++ )
					matrix( row[ k ], col[ j ] ) -= matrix( row[ i ], col[ j ] );
				matrix( row[ k ], col[ i ] ) = 0.0;

				for( j = 0; j < pA->n_row(); j++ )
					(*pInv)( row[ k ], col[ j ] ) -= (*pInv)( row[ i ], col[ j ] );
			}
		}
	}

	// Up
	for( i = pA->n_row() - 1; i > 0; i-- )
	{
		for( k = 0; k <= i; k++ )
		{
			value = matrix( row[ k ], col[ i ] );
			if( IsNotNull( value ) && IsNotOne( value ) )
			{
				for( j = k; j < i; j++ )
					matrix( row[ k ], col[ j ] ) /= value;
				matrix( row[ k ], col[ i ] ) = 1.0;

				for( j = 0; j < pA->n_row(); j++ )
					(*pInv)( row[ k ], col[ j ] ) /= value;
			}
		}

		//
		for( k = 0; k < i; k++ )
		{
			value = matrix( row[ k ], col[ i ] );
			if( IsNotNull( value ) )
			{
				for( j = k; j < i; j++ )
					matrix( row[ k ], col[ j ] ) -= matrix( row[ i ], col[ j ] );
				matrix( row[ k ], col[ i ] ) = 0.0;

				for( j = 0; j < pA->n_row(); j++ )
					(*pInv)( row[ k ], col[ j ] ) -= (*pInv)( row[ i ], col[ j ] );
			}
		}
	}

	for( i = 0; i < pA->n_row(); i++ )
	{
		value = matrix( row[ i ], col[ i ] );
		if( IsNotOne( value ) )
			for( j = 0; j < pA->n_row(); j++ )
				(*pInv)( row[ i ], col[ j ] ) /= value;
	}
}

//////////////////////////////////////////////////////////////////////

MATHBASE_API 
void _Determinant( CDSRMMatrix<CDSRReal> *pA, CDSRReal& rlDet )
{
	if( pA->n_row() != pA->n_column() )
		throw "Internal problem - sizes of the source and destination matrix are not equal";

	long i, j, k;
	CDSRReal value;
	CDSRMMatrix<CDSRReal> matrix( (*pA) );

	//	
	rlDet = 1.0;
	for( i = 0; i < pA->n_row(); i++ )
	{
		if( IsNull( matrix( i, i ) ) )
		{
			rlDet = 0.0;
			break;
		}

		for( k = i; k < pA->n_row(); k++ )
		{
			value = matrix( k, i );
			if( IsNotNull( value ) && IsNotOne( value ) )
			{
				rlDet *= value;

				for( j = i + 1; j < pA->n_row(); j++ )
					matrix( k, j ) /= value;
				matrix( k, i ) = 1.0;
			}
		}

		for( k = i + 1; k < pA->n_row(); k++ )
		{
			value = matrix( k, i );
			if( IsNotNull( value ) )
			{
				for( j = i + 1; j < pA->n_row(); j++ )
					matrix( k, j ) -= matrix( i, j );
				matrix( k, i ) = 0.0;
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////

MATHBASE_API 
void _DeterminantC( CDSRMMatrix<CDSRReal> *pA, CDSRReal& rlDet )
{
	if( pA->n_row() != pA->n_column() )
		throw "Internal problem - sizes of the source and destination matrix are not equal";

	long i, j, k;
	CDSRReal value;
	CDSRMMatrix<CDSRReal> matrix( (*pA) );
	CDSRMVector<long> col( pA->n_row() );

	for( i = 0; i < pA->n_row(); i++ )
		col[ i ] = i;

	//	
	rlDet = 1.0;
	for( i = 0; i < pA->n_row(); i++ )
	{
		if( IsNull( matrix( i, col[ i ] ) ) )
			_IndexC( i, &matrix, &col );

		for( k = i; k < pA->n_row(); k++ )
		{
			value = matrix( k, col[ i ] );
			if( IsNotNull( value ) && IsNotOne( value ) )
			{
				rlDet *= value;

				for( j = i + 1; j < pA->n_row(); j++ )
					matrix( k, col[ j ] ) /= value;
				matrix( k, col[ i ] ) = 1.0;
			}
		}

		for( k = i + 1; k < pA->n_row(); k++ )
		{
			value = matrix( k, col[ i ] );
			if( IsNotNull( value ) )
			{
				for( j = i + 1; j < pA->n_row(); j++ )
					matrix( k, col[ j ] ) -= matrix( i, col[ j ] );
				matrix( k, col[ i ] ) = 0.0;
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////

MATHBASE_API 
void _DeterminantR( CDSRMMatrix<CDSRReal> *pA, CDSRReal& rlDet )
{
	if( pA->n_row() != pA->n_column() )
		throw "Internal problem - sizes of the source and destination matrix are not equal";

	long i, j, k;
	CDSRReal value;
	CDSRMMatrix<CDSRReal> matrix( (*pA) );
	CDSRMVector<long> row( pA->n_row() );

	for( i = 0; i < pA->n_row(); i++ )
		row[ i ] = i;

	//	
	rlDet = 1.0;
	for( i = 0; i < pA->n_row(); i++ )
	{
		if( IsNull( matrix( row[ i ], i ) ) )
			_IndexR( i, &matrix, &row );

		for( k = i; k < pA->n_row(); k++ )
		{
			value = matrix( row[ k ], i );
			if( IsNotNull( value ) && IsNotOne( value ) )
			{
				rlDet *= value;

				for( j = i + 1; j < pA->n_row(); j++ )
					matrix( row[ k ], j ) /= value;
				matrix( row[ k ], i ) = 1.0;
			}
		}

		for( k = i + 1; k < pA->n_row(); k++ )
		{
			value = matrix( row[ k ], i );
			if( IsNotNull( value ) )
			{
				for( j = i + 1; j < pA->n_row(); j++ )
					matrix( row[ k ], j ) -= matrix( row[ i ], j );
				matrix( row[ k ], i ) = 0.0;
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////

MATHBASE_API 
void _DeterminantCR( CDSRMMatrix<CDSRReal> *pA, CDSRReal& rlDet )
{
	if( pA->n_row() != pA->n_column() )
		throw "Internal problem - sizes of the source and destination matrix are not equal";

	long i, j, k;
	CDSRReal value;
	CDSRMMatrix<CDSRReal> matrix( (*pA) );
	CDSRMVector<long> row( pA->n_row() );
	CDSRMVector<long> col( pA->n_row() );

	// заполн€ем массив индексов
	for( i = 0; i < pA->n_row(); i++ )
		row[ i ] = col[ i ] = i;

	//	
	rlDet = 1.0;
	for( i = 0; i < pA->n_row(); i++ )
	{
		if( IsNull( matrix( row[ i ], col[ i ] ) ) )
			_IndexCR( i, &matrix, &row, &col );

		for( k = i; k < pA->n_row(); k++ )
		{
			value = matrix( row[ k ], col[ i ] );
			if( IsNotNull( value ) && IsNotOne( value ) )
			{
				rlDet *= value;

				for( j = i + 1; j < pA->n_row(); j++ )
					matrix( row[ k ], col[ j ] ) /= value;
				matrix( row[ k ], col[ i ] ) = 1.0;
			}
		}

		for( k = i + 1; k < pA->n_row(); k++ )
		{
			value = matrix( row[ k ], col[ i ] );
			if( IsNotNull( value ) )
			{
				for( j = i + 1; j < pA->n_row(); j++ )
					matrix( row[ k ], col[ j ] ) -= matrix( row[ i ], col[ j ] );
				matrix( row[ k ], col[ i ] ) = 0.0;
			}
		}
	}
}

MATHBASE_API
void _LSolve(CDSRMMatrix<CDSRReal> *pA, CDSRMVector<CDSRReal> *pB, CDSRMVector<CDSRReal> *pX, std::string _method )
{
        if( _method == "" || _method == "Rotation" )
		_Rotation( pA, pB, pX );
        else if( _method == "Gauss" )
		_GaussCR( pA, pB, pX );
        else if( _method == "JordanGauss" || _method == "Jordan-Gauss" )
		_JordanGaussCR( pA, pB, pX );
        else if( _method == "LU" )
		_LU( pA, pB, pX );
        else if( _method == "Reflection" )
		_Reflection( pA, pB, pX );
        else if( _method == "Relaxation" )
		_RelaxationCR( pA, pB, pX );
        else if( _method == "SimpleIterations" || _method == "Simple Iterations" )
		_SimpleIterationsCR( pA, pB, pX );
        else if( _method == "ZeidelIterations" || _method == "Zeidel Iterations" || _method == "Zeidel" )
		_ZeidIterationsCR( pA, pB, pX );
}

//////////////////////////////////////////////////////////////////////
