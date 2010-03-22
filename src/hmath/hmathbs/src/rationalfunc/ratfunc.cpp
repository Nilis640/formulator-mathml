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

#include "../cdsrarrays/cdsrmvector.h"
#include "../cdsrarrays/cdsrmmatrix.h"
#include "../polynomial/polina.h"
#include "../la/slae/lu.h"
#include "ratfunc.h"
#include "rfinternal.h"

//////////////////////////////////////////////////////////////////////

MATHBASE_API 
void _calcFraction(CDSRArray<CDSRReal>* pNumerator, CDSRArray<CDSRReal>* pDenominator, CDSRReal rlArg, CDSRReal& rlResult)
{
	CDSRReal n, d;
	_calcPolinom(pNumerator, rlArg, n);
	_calcPolinom(pDenominator, rlArg, d);
	if( d == 0.0 ) throw;
	rlResult = n / d;
}

//////////////////////////////////////////////////////////////////////

MATHBASE_API 
void _calcFraction(CDSRArray<CDSRComplex>* pNumerator, CDSRArray<CDSRComplex>* pDenominator, CDSRComplex rlArg, CDSRComplex& rlResult)
{
	CDSRComplex n, d;
	_calcPolinom(pNumerator, rlArg, n);
	_calcPolinom(pDenominator, rlArg, d);
	if( abs(d) == 0.0 ) throw;
	rlResult = n / d;
}

//////////////////////////////////////////////////////////////////////

MATHBASE_API 
void _calcFraction(CDSRArray<CDSRReal>* pNumerator, CDSRArray<CDSRReal>* pDenominator, CDSRComplex rlArg, CDSRComplex& rlResult)
{
	CDSRComplex n, d;
	_calcPolinom(pNumerator, rlArg, n);
	_calcPolinom(pDenominator, rlArg, d);
	if( abs(d) == 0.0 ) throw;
	rlResult = n / d;
}

//////////////////////////////////////////////////////////////////////

MATHBASE_API
void _partFractDecompos(CDSRArray<CDSRReal>* pNumerator, CDSRArray<CDSRReal>* pDenominator, CDSRArray<CDSRArray<CDSRReal>*>& partNums, CDSRArray<CDSRArray<CDSRReal>*>& partDenoms, CDSRArray<unsigned long>& MultiplicityArray)
{
	long i, j, k, l, m;
	long rows = pDenominator->size() - 1;
	CDSRArray<CDSRArray<CDSRReal>*> denomFactors;
	CDSRArray<unsigned long> degrees;
	CDSRMatrix<CDSRReal> MatrixA;
	CDSRMVector<CDSRReal> vectorB;
	CDSRMVector<CDSRReal> vectorX;
	CDSRArray<CDSRReal> auxPoly;
	CDSRArray<CDSRReal>* pAuxPoly = &auxPoly;
	CDSRArray<CDSRReal> mulPoly;
	CDSRArray<CDSRReal>* pMulPoly = &mulPoly;

	vectorB.resize(rows);
	for( i = 0; i < rows; i++ )
		if( int(i) - int(rows + pNumerator->size() + 1) > 0 )
			vectorB[ i ] = (*pNumerator)[ i - rows + pNumerator->size() ];
		else
			vectorB[ i ] = 0;

	_factorisationOfPolinom(pDenominator, denomFactors, degrees);
	for( i = 0; i < (long) denomFactors.size(); i++ )
	{
		if( denomFactors[ i ]->size() > 1 )
			for( j = 0; j < (long) degrees[ i ]; j++ )
			{
				mulPoly.resize(1);
				mulPoly[ 0 ] = 1;
				for( k = 0; k < (long) denomFactors.size(); k++ )
					if( k != i )
					{
						for( l = 0; l < (long) degrees[ k ]; l++ )
						{
							_multyOfPoly(denomFactors[ k ], pMulPoly, pAuxPoly);
							mulPoly.resize(auxPoly.size());
							for( m = 0; m < (long) auxPoly.size(); m++ )
								mulPoly[ m ] = auxPoly[ m ];
						}
					}
				for( k = 1; k < (long) degrees[ i ] - j; k++ )
				{
					_multyOfPoly(denomFactors[ i ], pMulPoly, pAuxPoly);
					mulPoly.resize(auxPoly.size());
					for( m = 0; m < (long) auxPoly.size(); m++ )
						mulPoly[ m ] = auxPoly[ m ];
				}
				if( denomFactors[ i ]->size() == 2 )
				{
					MatrixA.resize_matrix(rows, MatrixA.n_column() + 1);
					for( k = 0; k < rows; k++ )
						if( int(k) - int(rows + mulPoly.size() + 1) > 0 )
							MatrixA( k, MatrixA.n_column() - 1 ) = mulPoly[ k - rows + mulPoly.size() ];
						else
							MatrixA( k, MatrixA.n_column() - 1 ) = 0;
				}
				else
				{
					MatrixA.resize_matrix(rows, MatrixA.n_column() + 2);
					for( k = 0; k < rows; k++ )
						if( int(k) - int(rows + mulPoly.size() + 1) > 0 )
							MatrixA( k, MatrixA.n_column() - 1 ) = mulPoly[ k - rows + mulPoly.size() ];
						else
							MatrixA( k, MatrixA.n_column() - 1 ) = 0;
					mulPoly.push_back(0);
					for( k = 0; k < rows; k++ )
						if( int(k) - int(rows + mulPoly.size() + 1) > 0 )
							MatrixA( k, MatrixA.n_column() - 2 ) = mulPoly[ k - rows + mulPoly.size() ];
						else
							MatrixA( k, MatrixA.n_column() - 2 ) = 0;
				}
			}
	}
	CDSRMMatrix<CDSRReal> A(rows, MatrixA.n_column());

	for( i = 0; i < rows; i++ )
		for( j = 0; j < MatrixA.n_column(); j++ )
			A(i, j) = MatrixA(i, j);
	vectorX.resize(rows);
	_LU(&A, &vectorB, &vectorX);
	partNums.resize(0);
	partDenoms.resize(0);
	MultiplicityArray.resize(0);

	//k = 0;
	for( i = 0, k = 0; i < (long) denomFactors.size(); i++ )
		if( denomFactors[ i ]->size() > 1 )
			for( j = 0; j < (long) degrees[ i ]; j++, k++ )
				if( denomFactors[ i ]->size() == 2 )
				{
					//k++;
					partNums.add(new CDSRArray<CDSRReal>);
					partDenoms.add(new CDSRArray<CDSRReal>);
					partNums[ partNums.size() - 1 ]->resize(1);
					(*partNums[ partNums.size() - 1 ])[ 0 ] = vectorX[ k ];
					partDenoms[ partDenoms.size() - 1 ]->resize(2);
					(*partDenoms[ partDenoms.size() - 1 ])[ 0 ] = (*denomFactors[ i ])[ 0 ];
					(*partDenoms[ partDenoms.size() - 1 ])[ 1 ] = (*denomFactors[ i ])[ 1 ];
					MultiplicityArray.add((unsigned long) j + 1);
				}
				else
				{
					partNums.add(new CDSRArray<CDSRReal>);
					partDenoms.add(new CDSRArray<CDSRReal>);
					partNums[ partNums.size() - 1 ]->resize(2);
					(*partNums[ partNums.size() - 1 ])[ 0 ] = vectorX[ k ];
					(*partNums[ partNums.size() - 1 ])[ 1 ] = vectorX[ ++k ];
					partDenoms[ partDenoms.size() - 1 ]->resize(3);
					(*partDenoms[ partDenoms.size() - 1 ])[ 0 ] = (*denomFactors[ i ])[ 0 ];
					(*partDenoms[ partDenoms.size() - 1 ])[ 1 ] = (*denomFactors[ i ])[ 1 ];
					(*partDenoms[ partDenoms.size() - 1 ])[ 2 ] = (*denomFactors[ i ])[ 2 ];
					MultiplicityArray.add((unsigned long) j + 1);
				}
}

//////////////////////////////////////////////////////////////////////

MATHBASE_API
void _partFractDecomposComplex(CDSRArray<CDSRReal>* pNumerator, CDSRArray<CDSRReal>* pDenominator, CDSRArray<CDSRComplex>& numCoefs, CDSRArray<CDSRComplex>& denomRoots, CDSRArray<unsigned long>& MultiplicityArray)
{
	CDSRArray<ROOT_INFO> RootsArray, _denomRoots;
	CDSRArray<ROOT_INFO>* pRootsArray = &RootsArray;
	CDSRArray<CDSRComplex> _denom, _nom;
	CDSRComplex res;
	long i, j, rootsQnt;

	numCoefs.resize(0);
	MultiplicityArray.resize(0);
	denomRoots.resize(0);
	_rootsOfPolinom(pDenominator, pRootsArray);
	rootsQnt = RootsArray.size();

	for( i = 0, _nom.resize(0); i < (long) pNumerator->size(); i++ )
		_nom.push_back( (*pNumerator)[ i ] );
	for( i = 0; i < rootsQnt; i++ )
	{
		for( j = 0, _denomRoots.resize(0); j < rootsQnt; j++ )
			if( j != i ) _denomRoots.push_back( RootsArray[ j ] );
		_poly4ComplexRoots(&_denomRoots, &_denom);
		for( j = 0; j < (long)RootsArray[ i ].multiplicity; j++ )
		{
			_fractDeriativeComplex(&_nom, &_denom, (unsigned long) j, CDSRComplex(RootsArray[ i ].real, RootsArray[ i ].image), res);
			numCoefs.push_back( res / ( CDSRComplex( factor( (long) j ), 0.0 ) * (*pDenominator)[ 0 ] ) );
			denomRoots.push_back( CDSRComplex(RootsArray[ i ].real, RootsArray[ i ].image) );
			MultiplicityArray.push_back( (unsigned long) (RootsArray[ i ].multiplicity - j) );
		}
	}
}

//////////////////////////////////////////////////////////////////////

MATHBASE_API
void _rationalFuncDeriv(CDSRArray<CDSRReal>* pNum, CDSRArray<CDSRReal>* pDen, CDSRArray<CDSRReal>* pNumRes, CDSRArray<CDSRReal>* pDenRes)
{
	CDSRArray<CDSRReal> GCD, deriv, poly1, poly2;
	long i;

	_derivativePolinom(pNum, 1, &deriv);
	_multyOfPoly(&deriv, pDen, &poly1);

	_derivativePolinom(pDen, 1, &deriv);
	_multyOfPoly(&deriv, pNum, &poly2);

	for( i = 0; i < (long) poly2.size(); i++ )
		poly2[ i ] = -poly2[ i ];
	_sumOfPoly(&poly1, &poly2, &poly1);
	_multyOfPoly(pDen, pDen, &poly2);
	_greatCommonDevider(&poly1, &poly2, &GCD);
	_divOfPoly(&poly1, &GCD, pNumRes, &deriv);
	_divOfPoly(&poly2, &GCD, pDenRes, &deriv);
}

//////////////////////////////////////////////////////////////////////

MATHBASE_API
void _fractDeriativeComplex(CDSRArray<CDSRComplex>* pNumerator, CDSRArray<CDSRComplex>* pDenominator, CDSRArray<CDSRComplex>* pResNum, CDSRArray<CDSRComplex>* pResDenom)
{
	CDSRArray<CDSRComplex> deriv, poly1, poly2, poly3;
	long i, _size1, _size2, _size;

	_derivativePolinom(pNumerator, 1, &deriv);
	_multyOfPoly(&deriv, pDenominator, &poly1);
	_derivativePolinom(pDenominator, 1, &deriv);
	_multyOfPoly(&deriv, pNumerator, &poly2);
	_size1 = (long) poly1.size();
	_size2 = (long) poly2.size();
	_size = std::max( _size1, _size2 );
	deriv.resize( _size );
	for( i = 0; i < _size; i++ )
		deriv[ i ]  = ( _size1 - _size + i >= 0 ? poly1[ _size1 - _size + i ] : CDSRComplex(0.0, 0.0) ) - ( _size2 - _size + i >= 0 ? poly2[ _size2 - _size + i ] : CDSRComplex(0.0, 0.0) );
	
	// Trim left nulls
	_size = (long) deriv.size();
	for( i = 0; i < _size; i++ )
		if( !IsNoll( deriv[ i ] ) )
		{
			deriv.assign( deriv.begin() + i, deriv.end( ));
			break;
		}
	if( i == (long) deriv.size() )
		deriv.resize(1);

	_multyOfPoly(pDenominator, pDenominator, &poly2);

	_greatCommonDevider(&deriv, &poly2, &poly1);
	_divOfPoly(&deriv, &poly1, pResNum, &poly3);
	_divOfPoly(&poly2, &poly1, pResDenom, &poly3);
}

//////////////////////////////////////////////////////////////////////

MATHBASE_API
void _fractDeriativeComplex(CDSRArray<CDSRComplex>* pNumerator, CDSRArray<CDSRComplex>* pDenominator, long nDerivative, CDSRComplex arg, CDSRComplex& res)
{
	CDSRArray<CDSRComplex> resNum, resDenom, _num, _denom;
	CDSRComplex res1, res2;
	long i, j;

	for( i = 0; i < (long) pNumerator->size(); i++ )
         resNum.push_back( (*pNumerator)[ i ] );
	for( i = 0; i < (long) pDenominator->size(); i++ )
         resDenom.push_back( (*pDenominator)[ i ] );

	for( i = 0; i < (long) nDerivative; i++ )
	{
		_fractDeriativeComplex(&resNum, &resDenom, &_num, &_denom);
		resNum.resize(0);
		for( j = 0; j < (long) _num.size(); j++ )
			resNum.push_back(_num[ j ]);
		resDenom.resize(0);
		for( j = 0; j < (long) _denom.size(); j++ )
			resDenom.push_back(_denom[ j ]);
	}
	_calcPolinom(&resNum, arg, res1);
	_calcPolinom(&resDenom, arg, res2);
	res = res1 / res2;
}

//////////////////////////////////////////////////////////////////////

MATHBASE_API
void _rationalFuncsSum(CDSRArray<CDSRReal>* pNum1, CDSRArray<CDSRReal>* pDen1, CDSRArray<CDSRReal>* pNum2, CDSRArray<CDSRReal>* pDen2, CDSRArray<CDSRReal>* pNumRes, CDSRArray<CDSRReal>* pDenRes)
{
	CDSRArray<CDSRReal> GCD, poly1, poly2, poly3;
	long i;

	_greatCommonDevider(pDen1, pDen2, &GCD);
	_multyOfPoly(pNum1, pDen2, &poly1);
	_multyOfPoly(pNum2, pDen1, &poly2);
	_sumOfPoly(&poly1, &poly2, &poly1);
	_multyOfPoly(pDen1, pDen2, &poly2);

	// Trim left nulls
	for( i = 0; i < (long) poly1.size(); i++ )
		if( !IsNoll(poly1[ i ]) )
		{
			poly1.assign( poly1.begin() + i, poly1.end() );
			break;
		}
	if( i == (long) poly1.size() )
		poly1.resize(1);
	
	_divOfPoly(&poly1, &GCD, pNumRes, &poly3);
	_divOfPoly(&poly2, &GCD, pDenRes, &poly3);
}

//////////////////////////////////////////////////////////////////////

MATHBASE_API
void _rationalFuncsSum(CDSRArray<CDSRComplex>* pNum1, CDSRArray<CDSRComplex>* pDen1, CDSRArray<CDSRComplex>* pNum2, CDSRArray<CDSRComplex>* pDen2, CDSRArray<CDSRComplex>* pNumRes, CDSRArray<CDSRComplex>* pDenRes)
{
	CDSRArray<CDSRComplex> GCD, poly1, poly2, poly3;
	long i;

	_greatCommonDevider(pDen1, pDen2, &GCD);
	_multyOfPoly(pNum1, pDen2, &poly1);
	_multyOfPoly(pNum2, pDen1, &poly2);
	_sumOfPoly(&poly1, &poly2, &poly1);
	_multyOfPoly(pDen1, pDen2, &poly2);

	// Trim left nulls
	for( i = 0; i < (long) poly1.size(); i++ )
		if( !IsNoll(poly1[ i ]) )
		{
			poly1.assign(poly1.begin() + i, poly1.end());
			break;
		}
	if( i == (long) poly1.size() )
		poly1.resize(1);

	_divOfPoly(&poly1, &GCD, pNumRes, &poly3);
	_divOfPoly(&poly2, &GCD, pDenRes, &poly3);
}

//////////////////////////////////////////////////////////////////////

MATHBASE_API
void _rationalFuncsMult(CDSRArray<CDSRReal>* pNum1, CDSRArray<CDSRReal>* pDen1, CDSRArray<CDSRReal>* pNum2, CDSRArray<CDSRReal>* pDen2, CDSRArray<CDSRReal>* pNumRes, CDSRArray<CDSRReal>* pDenRes)
{
	CDSRArray<CDSRReal> GCD, poly1, poly2, poly3;

	_multyOfPoly(pNum1, pNum2, &poly1);
	_multyOfPoly(pDen1, pDen2, &poly2);
	_greatCommonDevider(&poly1, &poly2, &GCD);
	_divOfPoly(&poly1, &GCD, pNumRes, &poly3);
	_divOfPoly(&poly2, &GCD, pDenRes, &poly3);
}

//////////////////////////////////////////////////////////////////////

MATHBASE_API
void _rationalFuncReduce(CDSRArray<CDSRReal>* pNum, CDSRArray<CDSRReal>* pDen, CDSRArray<CDSRReal>* pNumRes, CDSRArray<CDSRReal>* pDenRes)
{
	CDSRArray<CDSRReal> GCD, ost;

	_greatCommonDevider(pNum, pDen, &GCD);
	_divOfPoly(pNum, &GCD, pNumRes, &ost);
	_divOfPoly(pDen, &GCD, pDenRes, &ost);
}

//////////////////////////////////////////////////////////////////////
