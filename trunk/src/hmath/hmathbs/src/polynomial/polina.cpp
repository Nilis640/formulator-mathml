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

#include <algorithm>
#include <math.h>
#include "polina.h"
#include "internalfunc.h"
#include "../cdsrarrays/cdsrarray.h"
#include "../la/matrix_internal.h"

//////////////////////////////////////////////////////////////////////

MATHBASE_API
void _calcPolinom(CDSRArray<CDSRReal>*& pPolinom, CDSRReal rlArg, CDSRReal& rlResult)
{
	if( !pPolinom->size() )
		throw "Size of polynom can't be 0";

	rlResult = (*pPolinom)[ 0 ];
	for( unsigned long i = 1; i < pPolinom->size(); i++ )
		rlResult = (*pPolinom)[ i ] + rlArg * rlResult;
}

//////////////////////////////////////////////////////////////////////

MATHBASE_API
void _calcPolinom(CDSRArray<CDSRReal>*& pPolinom, CDSRComplex cxArg, CDSRComplex& cxResult)
{
	if( !pPolinom->size() )
		throw "Size of polynom can't be 0";

	cxResult = CDSRComplex((*pPolinom)[ 0 ], 0.0);
	for( unsigned long i = 1; i < pPolinom->size(); i++ )
		cxResult = CDSRComplex((*pPolinom)[ i ], 0.0) + cxArg * cxResult;
}

//////////////////////////////////////////////////////////////////////

MATHBASE_API
void _calcPolinom(CDSRArray<CDSRComplex>* pPolinom, CDSRComplex cxArg, CDSRComplex& cxResult)
{
	if( !pPolinom->size() )
		throw "Size of polynom can't be 0";

	cxResult = (*pPolinom)[ 0 ];
	for( unsigned long i = 1; i < pPolinom->size(); i++ )
		cxResult = (*pPolinom)[ i ] + cxArg * cxResult;
}

//////////////////////////////////////////////////////////////////////

MATHBASE_API
void _calcPolinom(CDSRArray<CDSRReal>*& pPolinom, CDSRArray<CDSRReal>*& pArg, CDSRArray<CDSRReal>*& pResPolinom)
{
	pResPolinom->resize(pArg->size());
	for( unsigned long i = 0; i < pArg->size(); i++ )
		_calcPolinom(pPolinom, (*pArg)[ i ], (*pResPolinom)[ i ]);
}

//////////////////////////////////////////////////////////////////////

MATHBASE_API
void _calcPolinom(CDSRArray<CDSRReal>*& pPolinom, CDSRArray<CDSRComplex>*& pArg, CDSRArray<CDSRComplex>*& pResPolinom)
{
	pResPolinom->resize(pArg->size());
	for( unsigned long i = 0; i < pArg->size(); i++ )
		_calcPolinom(pPolinom, (*pArg)[ i ], (*pResPolinom)[ i ]);
}

//////////////////////////////////////////////////////////////////////

MATHBASE_API
void _derivativePolinom(CDSRArray<CDSRReal>* pPolinom, unsigned long nDerivative, CDSRArray<CDSRReal>* pDerivative)
{
	if( nDerivative <= pPolinom->size() )
	{
		unsigned long i, j;
		unsigned long _size_pol = (unsigned long) pPolinom->size();
		unsigned long _size = (unsigned long) std::max( (unsigned long) 1, (unsigned long) (pPolinom->size() - nDerivative) );
		pDerivative->resize( _size );

		for( i = 0; i < _size; i++ )
		{
			(*pDerivative)[ i ] = 1.0;
			for( j = 0; j < nDerivative; j++ )
				(*pDerivative)[ i ] *= (_size_pol - 1 - i - j);
			(*pDerivative)[ i ] *= (*pPolinom)[ i ];
		}
	}
	else
		pDerivative->resize(1, 0.0);
}

//////////////////////////////////////////////////////////////////////

MATHBASE_API
void _derivativePolinom(CDSRArray<CDSRReal>*& pPolinom, unsigned long nDerivative, CDSRReal rlArg, CDSRReal& rlResult)
{
	CDSRArray<CDSRReal> derivative;
	CDSRArray<CDSRReal>* pDerivative = &derivative;
	_derivativePolinom(pPolinom, nDerivative, pDerivative);
	_calcPolinom(pDerivative, rlArg, rlResult);
}

//////////////////////////////////////////////////////////////////////

MATHBASE_API
void _derivativePolinom(CDSRArray<CDSRReal>*& pPolinom, unsigned long nDerivative, CDSRComplex cxArg, CDSRComplex& cxResult)
{
	CDSRArray<CDSRReal> derivative;
	CDSRArray<CDSRReal>* pDerivative = &derivative;
	_derivativePolinom(pPolinom, nDerivative, pDerivative);
	_calcPolinom(pDerivative, cxArg, cxResult);
}

//////////////////////////////////////////////////////////////////////

MATHBASE_API
void _derivativePolinom(CDSRArray<CDSRReal>*& pPolinom, unsigned long nDerivative, CDSRArray<CDSRReal>*& pArg, CDSRArray<CDSRReal>*& pResult)
{
	CDSRArray<CDSRReal> derivative;
	CDSRArray<CDSRReal>* pDerivative = &derivative;

	_derivativePolinom(pPolinom, nDerivative, pDerivative);

	for( unsigned long i = 0; i < pArg->size(); i++ )
		_calcPolinom(pDerivative, (*pArg)[ i ], (*pResult)[ i ]);
}

//////////////////////////////////////////////////////////////////////

MATHBASE_API
void _derivativePolinom(CDSRArray<CDSRReal>*& pPolinom, unsigned long nDerivative, CDSRArray<CDSRComplex>*& pArg, CDSRArray<CDSRComplex>*& pResult)
{
	CDSRArray<CDSRReal> derivative;
	CDSRArray<CDSRReal>* pDerivative = &derivative;

	_derivativePolinom(pPolinom, nDerivative, pDerivative);

	for( unsigned long i = 0; i < pArg->size(); i++ )
		_calcPolinom(pDerivative, (*pArg)[ i ], (*pResult)[ i ]);
}

//////////////////////////////////////////////////////////////////////

MATHBASE_API
void _derivativePolinom(CDSRArray<CDSRComplex>* pPolinom, unsigned long nDerivative, CDSRArray<CDSRComplex>* pDerivative)
{
	if( nDerivative <= pPolinom->size() )
	{
		unsigned long i, j;
		unsigned long _size_pol = (unsigned long) pPolinom->size();
		unsigned long _size = (unsigned long) std::max( (unsigned long) 1, pPolinom->size() - nDerivative );
		pDerivative->resize(_size);

		for( i = 0; i < _size; i++ )
		{
			(*pDerivative)[ i ] = CDSRComplex(1.0, 0.0);
			for( j = 0; j < nDerivative; j++ )
				(*pDerivative)[ i ] *= (_size_pol - 1 - i - j);
			(*pDerivative)[ i ] *= (*pPolinom)[ i ];
		}
	}
	else
		pDerivative->resize(1, CDSRComplex(0.0, 0.0));
}

//////////////////////////////////////////////////////////////////////

MATHBASE_API
void _derivativePolinom(CDSRArray<CDSRComplex>* pPolinom, unsigned long nDerivative, CDSRComplex cxArg, CDSRComplex& cxResult)
{
	CDSRArray<CDSRComplex> derivative;
	_derivativePolinom(pPolinom, nDerivative, &derivative);
	_calcPolinom(&derivative, cxArg, cxResult);
}

//////////////////////////////////////////////////////////////////////

MATHBASE_API
void _newtonPolinomsRoot(CDSRArray<CDSRReal>*& pPolinom, CDSRReal& rlRoot, unsigned long nMaxItrs, CDSRReal rlPrec)
{
	unsigned long i;
	unsigned long _itrs = 0;
	unsigned long _size = (unsigned long) pPolinom->size();
	CDSRArray<CDSRReal> polinomDeriv(_size - 1);
	CDSRArray<CDSRReal> *pPolinomDeriv = &polinomDeriv;
	CDSRReal _root, rlMeanPoli, rlMeanDeriv;

	for( i = 0; i < _size - 1; i++ )
		polinomDeriv[ i ] = (*pPolinom)[ i ] * (_size - 1 - i); 

	do
	{
		_root = rlRoot; 
		_calcPolinom(pPolinom, rlRoot, rlMeanPoli);
		_calcPolinom(pPolinomDeriv, rlRoot, rlMeanDeriv);
		rlRoot -= rlMeanPoli / rlMeanDeriv;
		if( fabs(rlMeanDeriv) < rlPrec )
		{
			_newtonPolinomsRoot(pPolinomDeriv, _root, nMaxItrs, rlPrec);
			rlRoot = _root;
			break;
		}
		_itrs++; 
		if( _itrs > nMaxItrs )
			throw "Iteration counter limit exceeded";
	}
	while( fabs(_root - rlRoot) > rlPrec ); 
}

//////////////////////////////////////////////////////////////////////

MATHBASE_API
void _newtonPolinomsRoot(CDSRArray<CDSRReal>*& pPolinom, CDSRComplex& cxRoot, unsigned long nMaxItrs, CDSRReal rlPrec)
{
	unsigned long i;
	unsigned long _itrs = 0;
	unsigned long _size = (unsigned long) pPolinom->size();
	CDSRArray<CDSRReal> polinomDeriv(_size - 1);
	CDSRArray<CDSRReal> *pPolinomDeriv = &polinomDeriv;
	CDSRComplex _root, cxMeanPoli, cxMeanDeriv;

	for( i = 0; i < _size - 1; i++ )
		polinomDeriv[ i ] = (*pPolinom)[ i ] * (_size - 1 - i); 

	do
	{
		_root = cxRoot; 
		_calcPolinom(pPolinom, cxRoot, cxMeanPoli);
		_calcPolinom(pPolinomDeriv, cxRoot, cxMeanDeriv);
		cxRoot -= cxMeanPoli / cxMeanDeriv; 
		if( fabs(cxMeanDeriv.real()) < rlPrec && fabs(cxMeanDeriv.imag()) < rlPrec )
		{
			_newtonPolinomsRoot(pPolinomDeriv, _root, nMaxItrs, rlPrec);
			cxRoot = _root;
			break;
		}

		_itrs++;
		if( _itrs > nMaxItrs )
			throw "Iteration counter limit exceeded";

	} while( abs(_root - cxRoot) > rlPrec ); 
}

//////////////////////////////////////////////////////////////////////

MATHBASE_API
void _newtonPolinomsRoot(CDSRArray<CDSRComplex>* pPolinom, CDSRComplex& cxRoot, unsigned long nMaxItrs, CDSRReal rlPrec)
{
	unsigned long i;
	unsigned long _itrs = 0;
	unsigned long _size = (unsigned long) pPolinom->size();
	CDSRArray<CDSRComplex> polinomDeriv(_size - 1);
	CDSRArray<CDSRComplex> *pPolinomDeriv = &polinomDeriv;
	CDSRComplex _root, cxMeanPoli, cxMeanDeriv;

	for( i = 0; i < _size - 1; i++ )
		polinomDeriv[ i ] = (*pPolinom)[ i ] * (double)(_size - 1 - i); 

	do
	{
		_root = cxRoot;
		_calcPolinom(pPolinom, cxRoot, cxMeanPoli);
		_calcPolinom(pPolinomDeriv, cxRoot, cxMeanDeriv);
		if( fabs(cxMeanDeriv.real()) < rlPrec && fabs(cxMeanDeriv.imag()) < rlPrec )
		{
			_newtonPolinomsRoot(pPolinomDeriv, _root, nMaxItrs, rlPrec);
			cxRoot = _root;
			break;
		}
		else
			cxRoot -= cxMeanPoli / cxMeanDeriv;
		_itrs++;
		if( _itrs > nMaxItrs )
			throw "Iteration counter limit exceeded";

	} while( abs(_root - cxRoot) > rlPrec ); 
}

//////////////////////////////////////////////////////////////////////

MATHBASE_API
void _divOnRoot(CDSRArray<CDSRReal>*& pPolinom, CDSRReal rlRoot, CDSRArray<CDSRReal>*& pResPolinom)
{
	unsigned long i; 
	unsigned long _size = (unsigned long) pPolinom->size(); 

	if( _size )
	{
		pResPolinom->resize(_size - 1); 

		(*pResPolinom)[ 0 ] = (*pPolinom)[ 0 ];
		for( i = 1; i < _size - 1; i++ )
			(*pResPolinom)[ i ] = (*pResPolinom)[ i - 1 ] * rlRoot + (*pPolinom)[ i ]; 
	}
}

//////////////////////////////////////////////////////////////////////

MATHBASE_API
void _divOnRoot(CDSRArray<CDSRComplex>* pPolinom, CDSRComplex root, CDSRArray<CDSRComplex>* pResPolinom)
{
	unsigned long i; 
	unsigned long _size = (unsigned long) pPolinom->size(); 

	if( _size )
	{
		pResPolinom->resize(_size - 1); 

		(*pResPolinom)[ 0 ] = (*pPolinom)[ 0 ];
		for( i = 1; i < _size - 1; i++ )
			(*pResPolinom)[ i ] = (*pResPolinom)[ i - 1 ] * root + (*pPolinom)[ i ]; 
	}
}

//////////////////////////////////////////////////////////////////////

MATHBASE_API
void _replaceX(CDSRArray<CDSRReal>*& pPolinom, CDSRArray<CDSRReal>*& pResPolinom)
{
	unsigned long i; 
	unsigned long _size = (unsigned long) pPolinom->size(); 

	pResPolinom->resize( _size ); 

	for( i = 0; i < _size; i++ )
		(*pResPolinom)[ i ] = ((_size - 1 - i) % 2) ? -(*pPolinom)[ i ] : (*pPolinom)[ i ]; 
}

//////////////////////////////////////////////////////////////////////

MATHBASE_API
CDSRReal _maxElement(CDSRArray<CDSRReal>*& pPolinom)
{
	unsigned long i; 
	unsigned long _size = (unsigned long) pPolinom->size(); 

	CDSRReal _max = 0.0; 

	if( _size )
		_max = fabs((*pPolinom)[ 0 ]);

	for( i = 1; i < _size; i++ )
		if( fabs((*pPolinom)[ i ]) > _max )
			_max = fabs((*pPolinom)[ i ]); 
	return _max; 
}

//////////////////////////////////////////////////////////////////////

MATHBASE_API
void _sumOfPoly(CDSRArray<CDSRReal>* pPolinom1, CDSRArray<CDSRReal>* pPolinom2, CDSRArray<CDSRReal>* pResPolinom)
{
        long i, size1, size2;
	size1 = pPolinom1->size();
	size2 = pPolinom2->size();
	CDSRArray<CDSRReal> poly;

	if( size1 > size2 )
	{
		poly.resize(size1);
		for( i = 0; i < size1; i++ )
			if( i < size2 )
				poly[ size1 - i - 1 ] = (*pPolinom1)[ size1 - i - 1 ] + (*pPolinom2)[ size2 - i - 1 ];
			else
				poly[ size1 - i - 1 ] = (*pPolinom1)[ size1 - i - 1 ];
	}
	else
	{
		poly.resize(size2);
		for( i = 0; i < size2; i++ )
			if( i < size1 )
				poly[ size2 - i - 1 ] = (*pPolinom1)[ size1 - i - 1 ] + (*pPolinom2)[ size2 - i - 1 ];
			else
				poly[ size2 - i - 1 ] = (*pPolinom2)[ size2 - i - 1 ];
	}

	// Trim left nulls
        for( i = 0; i < (long) poly.size(); i++ )
		if( !IsNoll(poly[ i ]) )
		{
			poly.assign(poly.begin() + i, poly.end());
			break;
		}
                if( i == (long) poly.size() )
			poly.resize(1);

		pResPolinom->assign(poly.begin(), poly.end());
}

//////////////////////////////////////////////////////////////////////

MATHBASE_API
void _sumOfPoly(CDSRArray<CDSRComplex>* pPolinom1, CDSRArray<CDSRComplex>* pPolinom2, CDSRArray<CDSRComplex>* pResPolinom)
{
        long i, size1, size2;
	size1 = pPolinom1->size();
	size2 = pPolinom2->size();
	CDSRArray<CDSRComplex> poly;

	if( size1 > size2 )
	{
		poly.resize(size1);
		for( i = 0; i < size1; i++ )
			if( i < size2 )
				poly[ size1 - i - 1 ] = (*pPolinom1)[ size1 - i - 1 ] + (*pPolinom2)[ size2 - i - 1 ];
			else
				poly[ size1 - i - 1 ] = (*pPolinom1)[ size1 - i - 1 ];
	}
	else
	{
		poly.resize(size2);
		for( i = 0; i < size2; i++ )
			if( i < size1 )
				poly[ size2 - i - 1 ] = (*pPolinom1)[ size1 - i - 1 ] + (*pPolinom2)[ size2 - i - 1 ];
			else
				poly[ size2 - i - 1 ] = (*pPolinom2)[ size2 - i - 1 ];
	}

	// Trim left nulls
        for( i = 0; i < (long) poly.size(); i++ )
		if( !IsNoll(poly[ i ]) )
		{
			poly.assign(poly.begin() + i, poly.end());
			break;
		}
                if( i == (long) poly.size() )
			poly.resize(1);

		pResPolinom->assign(poly.begin(), poly.end());
}

//////////////////////////////////////////////////////////////////////

MATHBASE_API
void _divOfPoly(CDSRArray<CDSRReal>* pDivPolinom, CDSRArray<CDSRReal>* pDevPolinom, CDSRArray<CDSRReal>* pResPolinom, CDSRArray<CDSRReal>* pOstPolinom)
{
	if( !pDivPolinom->size() || !pDevPolinom->size() )
		throw "Size of polynom can't be 0";

        long i, j;
        long _size_div = pDivPolinom->size();
        long _size_dev = pDevPolinom->size();
        long _size_res = _size_div - _size_dev + 1;
        long _size_ost = _size_dev - 1;
        long nullCount = 0;

	if( _size_res < 1 )
	{
		pResPolinom->resize(1);
		(*pResPolinom)[ 0 ] = 0.0;
		pOstPolinom->resize(_size_div);
		for( i = 0; i < _size_div; i++ )
			(*pOstPolinom)[ i ] = (*pDivPolinom)[ i ];
		return;
	}

	pResPolinom->resize(_size_res);
	pOstPolinom->resize(_size_ost);

	CDSRReal rlSum;

	(*pResPolinom)[ 0 ] = (*pDivPolinom)[ 0 ] / (*pDevPolinom)[ 0 ];
	for( i = 1; i < _size_res; i++ )
	{
		rlSum = 0.0;
		for( j = 0; j < i; j++ )
			if( i - j < _size_dev )
				rlSum += (*pResPolinom)[ j ] * (*pDevPolinom)[ i - j ];
		(*pResPolinom)[ i ] = ((*pDivPolinom)[ i ] - rlSum) / (*pDevPolinom)[ 0 ];
	}

	for( i = 0; i < _size_ost; i++ )
	{
		rlSum = 0.0;
		for( j = 0; j <= i; j++ )
			if( _size_dev - 1 - j > 0 && _size_res + j - i - 1 >= 0 )
				rlSum += (*pResPolinom)[ _size_res + j - i - 1 ] * (*pDevPolinom)[ _size_dev - 1 - j ];
		(*pOstPolinom)[ _size_dev - 2 - i ] = (*pDivPolinom)[ _size_div - i - 1 ] - rlSum;
	}
	for( i = 0; i < _size_ost; i++ )
	{
		if( (*pOstPolinom)[ i ] != 0.0 )
			break;
		nullCount++;
	}
	if( nullCount < _size_ost )
	{
		for( i = 0; i < _size_ost - nullCount; i++ )
			(*pOstPolinom)[ i ] = (*pOstPolinom)[ i + nullCount ];
		pOstPolinom->resize(_size_ost - nullCount);
	}
	else
		pOstPolinom->resize(1);

}

//////////////////////////////////////////////////////////////////////

MATHBASE_API
void _divOfPoly(CDSRArray<CDSRComplex>* pDivPolinom, CDSRArray<CDSRComplex>* pDevPolinom, CDSRArray<CDSRComplex>* pResPolinom, CDSRArray<CDSRComplex>* pOstPolinom)
{
	if( !pDivPolinom->size() || !pDevPolinom->size() )
		throw "Size of polynom can't be 0";

        long i, j;
        long _size_div = pDivPolinom->size();
        long _size_dev = pDevPolinom->size();
        long _size_res = _size_div - _size_dev + 1;
        long _size_ost = _size_dev - 1;
        long nullCount = 0;

	if( _size_res < 1 )
	{
		pResPolinom->resize(1);
		(*pResPolinom)[ 0 ] = 0.0;
		pOstPolinom->resize(_size_div);
		for( i = 0; i < _size_div; i++ )
			(*pOstPolinom)[ i ] = (*pDivPolinom)[ i ];
		return;
	}

	pResPolinom->resize(_size_res);
	pOstPolinom->resize(_size_ost);

	CDSRComplex sum;

	(*pResPolinom)[ 0 ] = (*pDivPolinom)[ 0 ] / (*pDevPolinom)[ 0 ];
	for( i = 1; i < _size_res; i++ )
	{
		sum = 0.0;
		for( j = 0; j < i; j++ )
			if( i - j < _size_dev )
				sum += (*pResPolinom)[ j ] * (*pDevPolinom)[ i - j ];
		(*pResPolinom)[ i ] = ((*pDivPolinom)[ i ] - sum) / (*pDevPolinom)[ 0 ];
	}

	for( i = 0; i < _size_ost; i++ )
	{
		sum = 0.0;
		for( j = 0; j <= i; j++ )
			if( _size_dev - 1 - j > 0 && _size_res + j - i - 1 >= 0 )
				sum += (*pResPolinom)[ _size_res + j - i - 1 ] * (*pDevPolinom)[ _size_dev - 1 - j ];
		(*pOstPolinom)[ _size_dev - 2 - i ] = (*pDivPolinom)[ _size_div - i - 1 ] - sum;
	}
	for( i = 0; i < _size_ost; i++ )
	{
		if( (*pOstPolinom)[ i ] != 0.0 )
			break;
		nullCount++;
	}
	if( nullCount < _size_ost )
	{
		for( i = 0; i < _size_ost - nullCount; i++ )
			(*pOstPolinom)[ i ] = (*pOstPolinom)[ i + nullCount ];
		pOstPolinom->resize(_size_ost - nullCount);
	}
	else
		pOstPolinom->resize(1);

}

//////////////////////////////////////////////////////////////////////

MATHBASE_API
void _greatCommonDevider(CDSRArray<CDSRReal>* pFstPolinom, CDSRArray<CDSRReal>* pSecPolinom, CDSRArray<CDSRReal>* pResPolinom)
{
	unsigned long i;
	bool bRepeat;
	bool bStdPolSize = pFstPolinom->size() >= pSecPolinom->size();

	unsigned long _size_div, _size_res;
	unsigned long _size_fst = (unsigned long) (bStdPolSize ? pFstPolinom->size() : pSecPolinom->size());
	unsigned long _size_sec = (unsigned long) (bStdPolSize ? pSecPolinom->size() : pFstPolinom->size());

	CDSRArray<CDSRReal> divPolinom(_size_fst);
	CDSRArray<CDSRReal> *pDivPolinom = &divPolinom;
	CDSRArray<CDSRReal> divResPolinom(_size_fst - _size_sec);
	CDSRArray<CDSRReal> *pDivResPolinom = &divResPolinom;
	CDSRArray<CDSRReal> ostPolinom(_size_sec - 1);
	CDSRArray<CDSRReal> *pOstPolinom = &ostPolinom;
	CDSRReal fstElement, Norm, Norm1, Norm2;

	pResPolinom->resize(_size_sec);

	_size_res = _size_sec;
	_size_div = _size_fst;

	Norm1 = 0.0;
	for( i = 0; i < pFstPolinom->size(); i++ )
		Norm1 += (*pFstPolinom)[ i ] * (*pFstPolinom)[ i ];
	Norm1 = sqrt(Norm1);

	Norm2 = 0.0;
	for( i = 0; i < pSecPolinom->size(); i++ )
		Norm2 += (*pSecPolinom)[ i ] * (*pSecPolinom)[ i ];
	Norm2 = sqrt(Norm2);

	for( i = 0; i < _size_div; i++ )
		divPolinom[ i ] = bStdPolSize ? (*pFstPolinom)[ i ] / Norm1 : (*pSecPolinom)[ i ] / Norm2;

	for( i = 0; i < _size_res; i++ )
		(*pResPolinom)[ i ] = bStdPolSize ? (*pSecPolinom)[ i ] / Norm2 : (*pFstPolinom)[ i ] / Norm1;

	do
	{
		bRepeat = false;
		_divOfPoly(pDivPolinom, pResPolinom, pDivResPolinom, pOstPolinom);

		Norm = 0.0;
		for( i = 0; i < pOstPolinom->size(); i++ )
			Norm += (*pOstPolinom)[ i ] * (*pOstPolinom)[ i ];

		if( !IsNoll(Norm) )
		{
			bRepeat = true;
			divPolinom.resize(_size_res);
			for( i = 0; i < _size_res; i++ )
				divPolinom[ i ] = (*pResPolinom)[ i ];

			_size_div = _size_res;
			_size_res = (unsigned long) ostPolinom.size();
			pResPolinom->assign( ostPolinom.begin(), ostPolinom.end() );
		}
	} while( bRepeat );

	fstElement = (*pResPolinom)[ 0 ];
	for( i = 0; i < _size_res; i++ )
		(*pResPolinom)[ i ] = (*pResPolinom)[ i ] / fstElement;
}

//////////////////////////////////////////////////////////////////////

MATHBASE_API
void _greatCommonDevider(CDSRArray<CDSRComplex>* pFstPolinom, CDSRArray<CDSRComplex>* pSecPolinom, CDSRArray<CDSRComplex>* pResPolinom)
{
	unsigned long i;
	bool bRepeat;
	bool bStdPolSize = pFstPolinom->size() >= pSecPolinom->size();

	unsigned long _size_div, _size_res;
	unsigned long _size_fst = (unsigned long) (bStdPolSize ? pFstPolinom->size() : pSecPolinom->size());
	unsigned long _size_sec = (unsigned long) (bStdPolSize ? pSecPolinom->size() : pFstPolinom->size());

	CDSRArray<CDSRComplex> divPolinom(_size_fst);
	CDSRArray<CDSRComplex> divResPolinom(_size_fst - _size_sec);
	CDSRArray<CDSRComplex> ostPolinom(_size_sec - 1);
	CDSRComplex fstElement, Norm, Norm1, Norm2;

	pResPolinom->resize(_size_sec);

	_size_res = _size_sec;
	_size_div = _size_fst;

	Norm1 = 0.0;
	for( i = 0; i < pFstPolinom->size(); i++ )
		Norm1 += abs( (*pFstPolinom)[ i ] ) * abs( (*pFstPolinom)[ i ] );
	Norm1 = sqrt(Norm1);

	Norm2 = 0.0;
	for( i = 0; i < pSecPolinom->size(); i++ )
		Norm2 += abs( (*pSecPolinom)[ i ] ) * abs( (*pSecPolinom)[ i ] );
	Norm2 = sqrt(Norm2);

	for( i = 0; i < _size_div; i++ )
		divPolinom[ i ] = bStdPolSize ? (*pFstPolinom)[ i ] / Norm1 : (*pSecPolinom)[ i ] / Norm2;

	for( i = 0; i < _size_res; i++ )
		(*pResPolinom)[ i ] = bStdPolSize ? (*pSecPolinom)[ i ] / Norm2 : (*pFstPolinom)[ i ] / Norm1;

	do
	{
		bRepeat = false;
		_divOfPoly(&divPolinom, pResPolinom, &divResPolinom, &ostPolinom);

		Norm = 0.0;
		for( i = 0; i < ostPolinom.size(); i++ )
			Norm += abs( ostPolinom[ i ] ) * abs( ostPolinom[ i ] );

		if( !IsNoll(Norm) )
		{
			bRepeat = true;
			divPolinom.resize(_size_res);
			for( i = 0; i < _size_res; i++ )
				divPolinom[ i ] = (*pResPolinom)[ i ];


			_size_div = _size_res;
			_size_res = (unsigned long) ostPolinom.size();
			pResPolinom->assign( ostPolinom.begin(), ostPolinom.end() );
		}
	} while( bRepeat );

	fstElement = (*pResPolinom)[ 0 ];
	for( i = 0; i < _size_res; i++ )
		(*pResPolinom)[ i ] = (*pResPolinom)[ i ] / fstElement;
}

//////////////////////////////////////////////////////////////////////

MATHBASE_API
void _linSqrPolinoms(CDSRArray<CDSRReal>*& pPolinom, CDSRArray<CDSRReal>*& pSqrPolinom, CDSRArray<CDSRReal>*& pResPolinom, CDSRReal rlPres, unsigned long nMaxItrs)
{
	long _size = (long) pPolinom->size();
	unsigned long nItrs;
	CDSRReal rlb1cur, rlb2cur, rlErr;
	CDSRArray<CDSRReal> ostPolinom(2); 
	CDSRArray<CDSRReal> *pOstPolinom = &ostPolinom;

	pResPolinom->resize(_size - 2);
	rlb1cur = (*pSqrPolinom)[ 1 ];
	rlb2cur = (*pSqrPolinom)[ 2 ];
	nItrs = 0;
	do
	{
		_divOfPoly(pPolinom, pSqrPolinom, pResPolinom, pOstPolinom);
		(*pSqrPolinom)[ 2 ] = (*pPolinom)[ _size - 1 ] / (*pResPolinom)[ _size - 3 ];
		if( _size - 4 >= 0 )
			(*pSqrPolinom)[ 1 ] = ((*pPolinom)[ _size - 2 ] - (*pSqrPolinom)[ 2 ] * (*pResPolinom)[ _size - 4 ] ) / (*pResPolinom)[ _size - 3 ];
		else
			(*pSqrPolinom)[ 1 ] = (*pPolinom)[ _size - 2 ] / (*pResPolinom)[ _size - 3 ];
		rlErr = sqrt((rlb1cur - (*pSqrPolinom)[ 1 ]) * (rlb1cur - (*pSqrPolinom)[ 1 ]) + (rlb2cur - (*pSqrPolinom)[ 2 ]) * (rlb2cur - (*pSqrPolinom)[ 2 ]));
		rlb1cur = (*pSqrPolinom)[ 1 ];
		rlb2cur = (*pSqrPolinom)[ 2 ];
		nItrs++;
		if( nItrs > nMaxItrs )
			throw "Iteration counter limit exceeded";
	}
	while( rlErr > rlPres );
}

//////////////////////////////////////////////////////////////////////

MATHBASE_API
void _multyOfPoly(CDSRArray<CDSRReal>* pFstPolinom, CDSRArray<CDSRReal>* pSecPolinom, CDSRArray<CDSRReal>* pResPolinom)
{
        long i, j;
        long _sizeFst = (long) pFstPolinom->size();
        long _sizeSec = (long) pSecPolinom->size();
	pResPolinom->resize(_sizeFst + _sizeSec - 1);

	CDSRReal rlSum;
	for( i = 0; i < _sizeFst + _sizeSec - 1 ; i++ )
	{
		rlSum = 0.0;
		for( j = 0; j <= i; j++ )
			if( i - j >= 0 && i - j < _sizeFst && j < _sizeSec )
				rlSum += (*pFstPolinom)[ i - j ] * (*pSecPolinom)[ j ];
		(*pResPolinom)[ i ] = rlSum;
	}
}

//////////////////////////////////////////////////////////////////////

MATHBASE_API
void _multyOfPoly(CDSRArray<CDSRComplex>* pFstPolinom, CDSRArray<CDSRComplex>* pSecPolinom, CDSRArray<CDSRComplex>* pResPolinom)
{
        long i, j;
	CDSRComplex _Sum;
        long _sizeFst = (long) pFstPolinom->size();
        long _sizeSec = (long) pSecPolinom->size();
	pResPolinom->resize(_sizeFst + _sizeSec - 1);

	for( i = 0; i < _sizeFst + _sizeSec - 1 ; i++ )
	{
		_Sum = CDSRComplex(0.0, 0.0);
		for( j = 0; j <= i; j++ )
			if( i - j >= 0 && i - j < _sizeFst && j < _sizeSec )
				_Sum += (*pFstPolinom)[ i - j ] * (*pSecPolinom)[ j ];
		(*pResPolinom)[ i ] = _Sum;
	}
}

//////////////////////////////////////////////////////////////////////

MATHBASE_API
void _leastCommonMultiple(CDSRArray<CDSRReal>*& pFstPolinom, CDSRArray<CDSRReal>*& pSecPolinom, CDSRArray<CDSRReal>*& pNokPolinom)
{
	unsigned long nNODPor;
	unsigned long _sizeFst = (unsigned long) pFstPolinom->size();
	unsigned long _sizeSec = (unsigned long) pSecPolinom->size();
	CDSRArray<CDSRReal> nodPolinom(_sizeFst); 
	CDSRArray<CDSRReal> *pNodPolinom = &nodPolinom;
	CDSRArray<CDSRReal> multPolinom(_sizeFst + _sizeSec - 1); 
	CDSRArray<CDSRReal> *pMultPolinom = &multPolinom;

	_multyOfPoly(pFstPolinom, pSecPolinom, pMultPolinom);
	_greatCommonDevider(pFstPolinom, pSecPolinom, pNodPolinom);
	nNODPor = (unsigned long) nodPolinom.size() - 1;
	CDSRArray<CDSRReal> ostPolinom(0); 
	CDSRArray<CDSRReal> *pOstPolinom = &ostPolinom;
	_divOfPoly(pMultPolinom, pNodPolinom, pNokPolinom, pOstPolinom);
}

//////////////////////////////////////////////////////////////////////

MATHBASE_API
void _sqrPoly(CDSRArray<CDSRReal>*& pPolinom, CDSRReal& rlFstRoot, CDSRReal& rlSecRoot, bool& bFeature)
{
	CDSRReal D = (*pPolinom)[ 1 ] * (*pPolinom)[ 1 ] - 4 * (*pPolinom)[ 0 ] * (*pPolinom)[ 2 ];
	if( D >= 0 )
	{
		rlFstRoot = ( - (*pPolinom)[ 1 ] + sqrt(D)) / ( 2 * (*pPolinom)[ 0 ] );
		rlSecRoot = ( - (*pPolinom)[ 1 ] - sqrt(D)) / ( 2 * (*pPolinom)[ 0 ] );
		bFeature = 0;
	}
	else
	{
		rlFstRoot = - (*pPolinom)[ 1 ] / ( 2 * (*pPolinom)[ 0 ] );
		rlSecRoot = sqrt( - D) / ( 2 * (*pPolinom)[ 0 ] );
		bFeature = 1;
	}
}

//////////////////////////////////////////////////////////////////////


MATHBASE_API
void _divOnSquarePoli(CDSRArray<CDSRReal>*& pPolinom, CDSRArray<CDSRReal>*& pSqrPolinom, CDSRArray<CDSRReal>*& pResPolinom)
{
	unsigned long _size = (unsigned long) pPolinom->size();
	pResPolinom->resize(_size - 2);
	(*pResPolinom)[ 0 ] = (*pPolinom)[ 0 ];
	(*pResPolinom)[ 1 ] = (*pPolinom)[ 1 ] - (*pPolinom)[ 0 ] * (*pSqrPolinom)[ 1 ];
	if( _size > 4 )
		for( unsigned long i = 2; i < _size - 2; i++ )
			(*pResPolinom)[ i ] = (*pPolinom)[ i ] - (*pSqrPolinom)[ 1 ] * (*pResPolinom)[ i - 1 ] - (*pSqrPolinom)[ 2 ] * (*pResPolinom)[ i - 2 ];
}

//////////////////////////////////////////////////////////////////////

MATHBASE_API
void _mutuallySimple(CDSRArray<CDSRReal>*& pFstPolinom, CDSRArray<CDSRReal>*& pSecPolinom, bool& bSimple)
{
	CDSRArray<CDSRReal> nodPolinom;
	CDSRArray<CDSRReal> *pNodPolinom = &nodPolinom;
	_greatCommonDevider(pFstPolinom, pSecPolinom, pNodPolinom);
	bSimple  = (nodPolinom.size() == 1);
}

//////////////////////////////////////////////////////////////////////

MATHBASE_API
void _isSimple(CDSRArray<CDSRReal>*& pPolinom, bool& bSimple)
{
	unsigned long _size = (unsigned long) pPolinom->size();
	CDSRReal rlFstRoot, rlSecRoot;

	bSimple = true;
	if( _size > 3)
		bSimple = false;
	else
		if( _size == 3)
			_sqrPoly(pPolinom, rlFstRoot, rlSecRoot, bSimple);
}

//////////////////////////////////////////////////////////////////////

MATHBASE_API
void _rootsOfPolinom(CDSRArray<CDSRReal>*& pPolinom, CDSRArray<ROOT_INFO>*& pRootsArray, CDSRReal rlPrec)
{
	bool bKratn, bFeature;
	bool Break=0;
	unsigned long i, j, nRootsQnt, nRealRootsQnt;
	unsigned long nRealQuant = 0; 
	unsigned long nCpxQuant = 0;
	unsigned long _sizeRes;
	unsigned long _size = (unsigned long) pPolinom->size();
	unsigned long nPorad = _size - 1;
	unsigned long nPoliPorad = nPorad;	
	CDSRReal maxElement, reCpxRoot, imCpxRoot, root;
	CDSRComplex cpxRoot;
	CDSRArray<CDSRReal> RootPolinom(nPorad); 
	//CDSRArray<CDSRReal> *pRootPolinom = &RootPolinom;
	CDSRArray<CDSRReal> workPolinom(_size); 
	CDSRArray<CDSRReal> *pWorkPolinom = &workPolinom;
	CDSRArray<CDSRReal> resPolinom; 
	CDSRArray<CDSRReal> *pResPolinom = &resPolinom;
	CDSRArray<CDSRReal> sqrPolinom(3);
	CDSRArray<CDSRReal> *pSqrPolinom = &sqrPolinom;
	sqrPolinom[ 0 ] = 1;

	for( i = 0; i < nPorad+1; i++ )
		workPolinom[ i ] = (*pPolinom)[ i ];

	while( nPorad )
	{
		if( Break )
			break;
		switch( nPorad )
		{
		case 1:
			RootPolinom[ nRealQuant ] = - workPolinom[ 1 ] / workPolinom[ 0 ];
			_newtonPolinomsRoot(pPolinom, RootPolinom[ nRealQuant ], POLYNOM_MAXITERS, rlPrec);
			nPorad --;	
			nRealQuant ++;
			break;

		case 2:
			_sqrPoly(pWorkPolinom, reCpxRoot, imCpxRoot, bFeature);
			if( bFeature )
			{
				cpxRoot = CDSRComplex(reCpxRoot,imCpxRoot);
				_newtonPolinomsRoot(pPolinom, cpxRoot, POLYNOM_MAXITERS, rlPrec);
				if( fabs(imag(cpxRoot)) > rlPrec )
				{
					RootPolinom[ nRealQuant ] = fabs(cpxRoot.real()) > rlPrec ? real(cpxRoot) : 0.0;
					RootPolinom[ nRealQuant + 1 ] = imag(cpxRoot);
				}
				else
				{
					RootPolinom[ nRealQuant ] = real(cpxRoot);
					RootPolinom[ nRealQuant + 1 ] = real(cpxRoot);
					nRealQuant += 2;
				}
			}
			else
			{
				_newtonPolinomsRoot(pPolinom, reCpxRoot, POLYNOM_MAXITERS, rlPrec);
				RootPolinom[ nRealQuant ] = reCpxRoot;

				_newtonPolinomsRoot(pPolinom, imCpxRoot, POLYNOM_MAXITERS, rlPrec);
				RootPolinom[ nRealQuant + 1] = imCpxRoot;

				nRealQuant += 2;
			}
			nPorad -= 2;
			break;

		default:
			maxElement = fabs(workPolinom[ 0 ]);
			for( i = 1; i < nPorad - 1; i++ )
				if( fabs(workPolinom[ i ]) > maxElement )
					maxElement = fabs(workPolinom[ i ]);
			reCpxRoot = fabs( 1 / ( 1 + maxElement / fabs(workPolinom[ nPorad ] ) ) );
			cpxRoot = CDSRComplex(reCpxRoot, workPolinom[ nPorad ] < 0 ? 0.0 : reCpxRoot);
			_newtonPolinomsRoot(pWorkPolinom, cpxRoot, POLYNOM_MAXITERS, rlPrec);
			_newtonPolinomsRoot(pPolinom, cpxRoot, POLYNOM_MAXITERS, rlPrec);
			if( fabs(imag(cpxRoot)) > rlPrec) 
			{
				sqrPolinom[ 1 ] = - 2 * real(cpxRoot);
				sqrPolinom[ 2 ] = real(cpxRoot) * real(cpxRoot) + imag(cpxRoot) * imag(cpxRoot);
				_divOnSquarePoli(pWorkPolinom, pSqrPolinom, pResPolinom);
				_sizeRes = (unsigned long) resPolinom.size();	
				workPolinom.resize(_sizeRes);
				for( i = 0; i < _sizeRes; i++ )
					workPolinom[ i ] = resPolinom[ i ];
				nPorad -= 2;
				RootPolinom[ nPoliPorad - 1 - nCpxQuant * 2 ] = imag(cpxRoot);
				RootPolinom[ nPoliPorad - 2 - nCpxQuant * 2 ] = fabs(cpxRoot.real()) > rlPrec ? real(cpxRoot) : 0.0;
				nCpxQuant++;
			}
			else
			{
				_divOnRoot(pWorkPolinom, real(cpxRoot), pResPolinom);
				_sizeRes = (unsigned long) resPolinom.size();	
				workPolinom.resize(_sizeRes);
				for( i = 0; i < _sizeRes; i++ )
					workPolinom[ i ] = resPolinom[ i ];
				nPorad--;
				RootPolinom[ nRealQuant ] = real(cpxRoot);
				nRealQuant++;

			}
		}
	}

	if( !Break )
	{
		pRootsArray->resize(nPoliPorad);
		nRootsQnt = 0;
		nRealRootsQnt = 0;

		if( nRealQuant )
		{
			for( i = 0; i < nRealQuant; i++ )
			{
				root = RootPolinom[ i ];
				bKratn = 0;
				(*pRootsArray)[ nRootsQnt ].isComplex = false;
				(*pRootsArray)[ nRootsQnt ].multiplicity = 1;

				for( j = 0; j < nRootsQnt; j++ )
					if( fabs((*pRootsArray)[ j ].real - root) < rlPrec )
					{
						(*pRootsArray)[ j ].multiplicity++;
						bKratn = 1;
					}
					if( !bKratn )
					{
						(*pRootsArray)[ nRootsQnt ].real = root;
						nRootsQnt++;
					}
			}
			nRealRootsQnt = nRootsQnt;
		}

		if( nRealQuant < nPoliPorad )
		{
			for( i = nRealQuant; i < nPoliPorad; i += 2)
			{
				bKratn = 0;
				(*pRootsArray)[ nRootsQnt ].isComplex = true;
				(*pRootsArray)[ nRootsQnt ].multiplicity = 1;
				(*pRootsArray)[ nRootsQnt + 1].isComplex = true;
				(*pRootsArray)[ nRootsQnt + 1].multiplicity = 1;

				for( j = nRealRootsQnt; j < nRootsQnt; j += 2 )
					if( fabs((*pRootsArray)[ j ].real - RootPolinom[ i ] ) < rlPrec 
						&& fabs( fabs((*pRootsArray)[ j ].image)- fabs(RootPolinom[ i + 1 ]) ) < rlPrec )
					{
						(*pRootsArray)[ j ].multiplicity++;
						(*pRootsArray)[ j + 1 ].multiplicity++;
						bKratn = 1;
					}
					if( !bKratn )
					{
						(*pRootsArray)[ nRootsQnt ].real = RootPolinom[ i ];
						(*pRootsArray)[ nRootsQnt ].image = fabs(RootPolinom[ i + 1 ]);
						(*pRootsArray)[ nRootsQnt +1 ].real = RootPolinom[ i ];
						(*pRootsArray)[ nRootsQnt + 1].image = - fabs(RootPolinom[ i + 1 ]);

						nRootsQnt += 2;
					}
			}
		}
		pRootsArray->resize(nRootsQnt);
	}
}
//////////////////////////////////////////////////////////////////////

MATHBASE_API 
void _integral(CDSRArray<CDSRReal>*& pPolinom, CDSRArray<CDSRReal>*& pIntegral, CDSRReal rlBegX, CDSRReal rlBegY)
{
	unsigned long i;
	unsigned long _size_pol = (unsigned long) pPolinom->size();
	CDSRReal _sum;
	pIntegral->resize(_size_pol + 1);
	for( i = 0; i < _size_pol; i++ )
	{
		(*pIntegral)[ i ] = (*pPolinom)[ i ] / (_size_pol - i);
	}
	(*pIntegral)[ _size_pol ] = 0;
	_calcPolinom(pIntegral, rlBegX, _sum);
	(*pIntegral)[ _size_pol ] = rlBegY - _sum;
}

//////////////////////////////////////////////////////////////////////

MATHBASE_API 
void _defIntegral(CDSRArray<CDSRReal>*& pPolinom, CDSRReal rlLo, CDSRReal rlHi, CDSRReal& rlRes)
{
	CDSRReal _x;
	CDSRArray<CDSRReal> NdefIntegral;
	CDSRArray<CDSRReal>* pNdefIntegral = &NdefIntegral;
	_integral(pPolinom, pNdefIntegral);
	_calcPolinom(pNdefIntegral, rlLo, _x);
	_calcPolinom(pNdefIntegral, rlHi, rlRes);
	rlRes -= _x;
}

//////////////////////////////////////////////////////////////////////

MATHBASE_API 
void _extremum(CDSRArray<CDSRReal>*& /*pPolinom*/, CDSRMatrix<CDSRReal>*& /*pResMatrix*/)
{
/*
	CDSRArray<CDSRReal> Deriative;
	CDSRArray<CDSRReal>* pDeriative = &Deriative;
	unsigned long _rows, i, _extrQnt;
	CDSRReal _extr, _pol;

	_derivativePolinom(pPolinom, 1, pDeriative);
	_rootsOfPolinom(pDeriative, 0.001, pResMatrix);
	_derivativePolinom(pPolinom, 2, pDeriative);
	_rows = pResMatrix->n_row();
	_extrQnt = 0;
	for( i = 0; i < _rows; i++)
	{
		if( (*pResMatrix)(i, 0) == 0 && (unsigned long)(*pResMatrix)(i, 1) % 2 )
		{
			_extr = (*pResMatrix)(i, 2);
			_calcPolinom(pDeriative, _extr, _pol);
			if( _pol > 0 )
				(*pResMatrix)(_extrQnt, 2) = 0;
			else
				(*pResMatrix)(_extrQnt, 2) = 1;
			(*pResMatrix)(_extrQnt, 0) = _extr;
			_calcPolinom(pPolinom, _extr, _pol);
			(*pResMatrix)(_extrQnt, 1) = _pol;
			_extrQnt++;
		}
		}
	pResMatrix->resize_matrix(_extrQnt, 3);
*/
}

MATHBASE_API 
void _undefIntegral(CDSRArray<CDSRReal>*& pPolinom, CDSRArray<CDSRReal>*& pIntegral, CDSRArray<CDSRReal>*& pBegPolinom)
{
	unsigned long i;
	unsigned long _size_pol = (unsigned long) pPolinom->size();
	unsigned long _size_beg = (unsigned long) pBegPolinom->size();
	CDSRArray<CDSRReal> auxPolinom = (*pPolinom);
	CDSRArray<CDSRReal>* pAuxPolinom = &auxPolinom;

	for( i = 0; i < _size_beg - 1; i++ )
	{
		_integral(pAuxPolinom, pIntegral, (*pBegPolinom)[ 0 ], (*pBegPolinom)[ _size_beg - i - 1 ]);
		auxPolinom.resize(_size_pol + i + 1); 
		auxPolinom = (*pIntegral);
	}
}

//////////////////////////////////////////////////////////////////////

MATHBASE_API 
void _inequality(CDSRArray<CDSRReal>*& pPolinom, CDSRArray<CDSRReal>*& pChangeSignPoints, CDSRReal rlPrec )
{
	CDSRArray<ROOT_INFO> RootsArray;
	CDSRArray<ROOT_INFO>* pRootsArray = &RootsArray;
	unsigned long i, _rootsQnt, _pointsQnt;

	_rootsOfPolinom(pPolinom, pRootsArray, rlPrec);
	_rootsQnt = (unsigned long) RootsArray.size();
	_pointsQnt = 0;
	pChangeSignPoints->resize(0);
	for( i = 0; i < _rootsQnt; i++ )
	{
		if( !RootsArray[ i ].isComplex && RootsArray[ i ].multiplicity % 2 )
		{
			pChangeSignPoints->resize(_pointsQnt + 1);
			(*pChangeSignPoints)[ _pointsQnt ] = RootsArray[ i ].real;
			_pointsQnt++;
		}
	}
}

//////////////////////////////////////////////////////////////////////

MATHBASE_API
void _factorisationOfPolinom(CDSRArray<CDSRReal>*& pPolinom, CDSRArray<CDSRArray<CDSRReal>*>& MultipliersArray, CDSRArray<unsigned long>& MultiplicityArray, CDSRReal rlPrec)
{
	CDSRArray<ROOT_INFO> RootsArray;
	CDSRArray<ROOT_INFO>* pRootsArray = &RootsArray;
	unsigned long i, _rootsQnt, _multipliersQnt;

	_rootsOfPolinom(pPolinom, pRootsArray, rlPrec);
	_rootsQnt = (unsigned long) RootsArray.size();
	_multipliersQnt = 0;

	for( i = 0; i < _rootsQnt; i++ )
	{
		MultipliersArray.resize(_multipliersQnt + 1);
		MultiplicityArray.resize(_multipliersQnt + 1);

		if( RootsArray[ i ].isComplex )
		{
			MultipliersArray[ _multipliersQnt ] = new CDSRArray<CDSRReal>;
			MultipliersArray[ _multipliersQnt ]->resize(3);
			(*MultipliersArray[ _multipliersQnt ])[ 0 ] = 1;
			(*MultipliersArray[ _multipliersQnt ])[ 1 ] = - RootsArray[ i ].real * 2;
			(*MultipliersArray[ _multipliersQnt ])[ 2 ] = RootsArray[ i ].real * RootsArray[ i ].real + RootsArray[ i ].image * RootsArray[ i ].image;
			MultiplicityArray[ _multipliersQnt ] = RootsArray[ i ].multiplicity;
			i++;
			_multipliersQnt++;
		}
		else
		{
			MultipliersArray[ _multipliersQnt ] = new CDSRArray<CDSRReal>;
			MultipliersArray[ _multipliersQnt ]->resize(2);
			(*MultipliersArray[ _multipliersQnt ])[ 0 ] = 1;
			(*MultipliersArray[ _multipliersQnt ])[ 1 ] = - RootsArray[ i ].real ;
			MultiplicityArray[ _multipliersQnt ] = RootsArray[ i ].multiplicity;
			_multipliersQnt++;
		}
	}

	if( (*pPolinom)[ 0 ] != 1.0 )
	{
		MultipliersArray.insert(0, new CDSRArray<CDSRReal>);
		MultiplicityArray.insert(0, 1);
		MultipliersArray[ 0 ]->add((*pPolinom)[ 0 ]);
	}
}

//////////////////////////////////////////////////////////////////////

MATHBASE_API
void _rootsOfCubEquation(CDSRArray<CDSRReal>* pPolynom, CDSRArray<ROOT_INFO>*& pRootsArray)
{
	if( pPolynom->size() != 4 ||  IsNoll((*pPolynom)[ 0 ]) )
		return;		// CUBE_POLYNOMIAL_EXPECTS

	CDSRReal p, q, re, im;
	CDSRComplex r;
	int i, j;
	ROOT_INFO info;
	bool isMul;

	pRootsArray->resize(0);

	p = ( (*pPolynom)[ 2 ] / (*pPolynom)[ 0 ] ) / 3 - 
		( ( ( (*pPolynom)[ 1 ] / (*pPolynom)[ 0 ] ) * ( (*pPolynom)[ 1 ] / (*pPolynom)[ 0 ] ) ) / 9 );
	q = - ( ( (*pPolynom)[ 1 ] / (*pPolynom)[ 0 ] ) * ( (*pPolynom)[ 1 ] / (*pPolynom)[ 0 ] ) *
		( (*pPolynom)[ 1 ] / (*pPolynom)[ 0 ] ) ) / 27 + ((*pPolynom)[ 1 ] / (*pPolynom)[ 0 ]) *
		( (*pPolynom)[ 2 ] / (*pPolynom)[ 0 ] ) / 6 - ( (*pPolynom)[ 3 ] / (*pPolynom)[ 0 ] ) / 2;
	r = q * q + p * p * p;
	r = p != 0.0 ? sqrt(r) + q : 2.0 * q;
	if( IsNoll( abs(r) ) )
	{
		info.real = -((*pPolynom)[ 1 ] / (*pPolynom)[ 0 ]) / 3;
		info.image = 0;
		info.multiplicity = 3;
		info.isComplex = false;
		pRootsArray->push_back(info);
		return;
	}
	for( i = 0; i < 3; i++ )
	{
		re = - ((*pPolynom)[ 1 ] / (*pPolynom)[ 0 ]) / 3 +	( pow( abs(r), (CDSRReal)1/3) ) * 
			cos( ( arg(r) + i * MATH_PI * 2 ) / 3 ) -
			( p / pow( abs(r), (CDSRReal)1/3 ) ) * cos( ( arg(r) + i * MATH_PI * 2 ) / 3 );
		im = ( pow( abs(r), (CDSRReal)1/3) ) * sin( ( arg(r) + i * 2 * MATH_PI ) / 3 ) +
			( p / pow(abs(r), (CDSRReal)1/3 ) ) * sin( ( arg(r) + i * MATH_PI * 2 ) / 3 );
		if( IsNoll(im) )
		{
			isMul = false;
			for( j = 0; j < i; j ++ )
				if( !(*pRootsArray)[ j ].isComplex && IsNoll( (*pRootsArray)[ j ].real - re ) )
				{
					(*pRootsArray)[ j ].multiplicity++;
					isMul = true;
					break;
				}
				if( !isMul )
				{
					info.real = re;
					info.image = 0;
					info.multiplicity = 1;
					info.isComplex = false;
					pRootsArray->push_back(info);
				}
		}
		else
		{
			info.real = IsNoll(re) ? 0 : re;
			info.image = im;
			info.isComplex = true;
			info.multiplicity = 1;
			pRootsArray->push_back(info);
		}
	}
}

//////////////////////////////////////////////////////////////////////

MATHBASE_API
void _rootsOfQuartEquation(CDSRArray<CDSRReal>* pPolynom, CDSRArray<ROOT_INFO>*& pRootsArray)
{
	if( pPolynom->size() != 5 ||  IsNoll((*pPolynom)[ 0 ]) )
		return;		// QUARTIC_POLYNOMIAL_EXPECTS

	CDSRArray<CDSRReal> auxPoly;
	CDSRArray<CDSRReal>* pPoly = &auxPoly;
        long i, j;
	CDSRReal rlY0 = 0.0, rlFstRoot, rlSecRoot, rlA2 = 0.0, rlB2 = 0.0;
	bool isCpx, multpl;
	ROOT_INFO info;

	auxPoly.resize(4);
	auxPoly[ 0 ] = 1;
	auxPoly[ 1 ] = - ( (*pPolynom)[ 2 ] / (*pPolynom)[ 0 ] );
	auxPoly[ 2 ] = ( (*pPolynom)[ 1 ] / (*pPolynom)[ 0 ] ) * ( (*pPolynom)[ 3 ] / (*pPolynom)[ 0 ] )
		- 4 * ( (*pPolynom)[ 4 ]  / (*pPolynom)[ 0 ] );
	auxPoly[ 3 ] = ( (*pPolynom)[ 4 ] / (*pPolynom)[ 0 ] ) * ( 4 * ( (*pPolynom)[ 2 ]  / (*pPolynom)[ 0 ] ) -
		( (*pPolynom)[ 1 ]  / (*pPolynom)[ 0 ] ) * ( (*pPolynom)[ 1 ] / (*pPolynom)[ 0 ] ) ) -
		( (*pPolynom)[ 3 ] / (*pPolynom)[ 0 ] ) * ( (*pPolynom)[ 3 ] / (*pPolynom)[ 0 ] );
	_rootsOfCubEquation(pPoly, pRootsArray);
        for( i = 0; i < (long) pRootsArray->size(); i++ )
		if( !(*pRootsArray)[ i ].isComplex )
		{
			rlA2 = ( ( (*pPolynom)[ 1 ] / (*pPolynom)[ 0 ] ) * ( (*pPolynom)[ 1 ] / (*pPolynom)[ 0 ] ) )
				/ 4 - ( (*pPolynom)[ 2 ] / (*pPolynom)[ 0 ] ) + (*pRootsArray)[ i ].real;
			rlA2 = IsNoll(rlA2) ? 0.0 : rlA2;
			rlB2 = ( (*pRootsArray)[ i ].real * (*pRootsArray)[ i ].real ) / 4 - ( (*pPolynom)[ 4 ] / (*pPolynom)[ 0 ] );
			if( rlA2 >= 0 && rlB2 >= 0 )
			{
				rlY0 = (*pRootsArray)[ i ].real;
				break;
			}
		}
		pRootsArray->resize(0);
		auxPoly.resize(3);
		auxPoly[ 0 ] = 1;
		auxPoly[ 1 ] = ( (*pPolynom)[ 1 ] / (*pPolynom)[ 0 ] ) / 2 + sqrt(rlA2);
		auxPoly[ 2 ] = rlY0 / 2 + ( ( (*pPolynom)[ 1 ] / (*pPolynom)[ 0 ] ) * rlY0 / 2 -
			( (*pPolynom)[ 3 ] / (*pPolynom)[ 0 ] ) > 0 ? sqrt(rlB2) : -sqrt(rlB2) );
		_sqrPoly(pPoly, rlFstRoot, rlSecRoot, isCpx);
		if( isCpx )
		{
			info.real = rlFstRoot;
			info.image = rlSecRoot;
			info.isComplex = true;
			info.multiplicity = 1;
			pRootsArray->push_back(info);
			info.image = -rlSecRoot;
			pRootsArray->push_back(info);
		}
		else
		{
			if( IsNoll(rlFstRoot - rlSecRoot) )
			{
				info.real = rlFstRoot;
				info.image = 0;
				info.isComplex = false;
				info.multiplicity = 2;
				pRootsArray->push_back(info);
			}
			else
			{
				info.real = rlFstRoot;
				info.image = 0;
				info.isComplex = false;
				info.multiplicity = 1;
				pRootsArray->push_back(info);
				info.real = rlSecRoot;
				info.image = 0;
				info.isComplex = false;
				info.multiplicity = 1;
				pRootsArray->push_back(info);
			}
		}
		auxPoly[ 1 ] = ( (*pPolynom)[ 1 ] / (*pPolynom)[ 0 ] ) / 2 - sqrt(rlA2);
		auxPoly[ 2 ] = rlY0 / 2 + ( ( (*pPolynom)[ 1 ] / (*pPolynom)[ 0 ] ) * rlY0 / 2 -
			( (*pPolynom)[ 3 ] / (*pPolynom)[ 0 ] ) > 0 ? -sqrt(rlB2) : sqrt(rlB2) );
		_sqrPoly(pPoly, rlFstRoot, rlSecRoot, isCpx);
		if( isCpx )
		{
			info.real = rlFstRoot;
			info.image = rlSecRoot;
			info.isComplex = true;
			info.multiplicity = 1;
			if( (*pRootsArray)[ 0 ].isComplex )
			{
				if( IsNoll(rlFstRoot - (*pRootsArray)[ 0 ].real) && IsNoll(rlSecRoot - (*pRootsArray)[ 0 ].image) )
				{
					(*pRootsArray)[ 0 ].multiplicity = 2;
					(*pRootsArray)[ 1 ].multiplicity = 2;
				}
				else
				{
					pRootsArray->push_back(info);
					info.image = -rlSecRoot;
					pRootsArray->push_back(info);
				}
			}
			else
			{
				pRootsArray->push_back(info);
				info.image = -rlSecRoot;
				pRootsArray->push_back(info);
			}
		}
		else
		{
			for( j = 0; j < 2; j++ )
			{
				multpl = false;
                                for( i = 0; i < (long) pRootsArray->size(); i++ )
					if( !(*pRootsArray)[ i ].isComplex && IsNoll((*pRootsArray)[ i ].real - rlFstRoot) )
					{
						(*pRootsArray)[ i ].multiplicity++;
						multpl = true;
						break;
					}
					if( !multpl )
					{
						info.real = rlFstRoot;
						info.image = 0;
						info.isComplex = false;
						info.multiplicity = 1;
						pRootsArray->push_back(info);
					}
					rlFstRoot = rlSecRoot;
			}
		}
}

//////////////////////////////////////////////////////////////////////

MATHBASE_API
void _rootsOfSqrEquation(CDSRArray<CDSRReal>* pPolynom, CDSRArray<ROOT_INFO>*& pRootsArray)
{
	if( pPolynom->size() != 3 ||  IsNoll((*pPolynom)[ 0 ]) )
		return;		// SQUARE_POLYNOMIAL_EXPECTS

	CDSRReal rlFstRoot, rlSecRoot;
	bool isComplex;
	_sqrPoly(pPolynom, rlFstRoot, rlSecRoot, isComplex);
	if( isComplex )
	{
		pRootsArray->resize(2);
		(*pRootsArray)[ 0 ].isComplex = true;
		(*pRootsArray)[ 0 ].real = rlFstRoot;
		(*pRootsArray)[ 0 ].image = rlSecRoot;
		(*pRootsArray)[ 0 ].multiplicity = 1;
		(*pRootsArray)[ 1 ].isComplex = true;
		(*pRootsArray)[ 1 ].real = rlFstRoot;
		(*pRootsArray)[ 1 ].image = -rlSecRoot;
		(*pRootsArray)[ 1 ].multiplicity = 1;
	}
	else
		if( IsNoll(rlFstRoot - rlSecRoot) )
		{
			pRootsArray->resize(1);
			(*pRootsArray)[ 0 ].isComplex = false;
			(*pRootsArray)[ 0 ].real = rlFstRoot;
			(*pRootsArray)[ 0 ].image = 0;
			(*pRootsArray)[ 0 ].multiplicity = 2;
		}
		else
		{
			pRootsArray->resize(2);
			(*pRootsArray)[ 0 ].isComplex = false;
			(*pRootsArray)[ 0 ].real = rlFstRoot;
			(*pRootsArray)[ 0 ].image = 0;
			(*pRootsArray)[ 0 ].multiplicity = 1;
			(*pRootsArray)[ 1 ].isComplex = false;
			(*pRootsArray)[ 1 ].real = rlSecRoot;
			(*pRootsArray)[ 1 ].image = 0;
			(*pRootsArray)[ 1 ].multiplicity = 1;
		}
}

//////////////////////////////////////////////////////////////////////

MATHBASE_API
void _powPoly(CDSRArray<CDSRReal>* pPolynom, unsigned power, CDSRArray<CDSRReal>* pResult)
{
        long i, j;
	CDSRArray<CDSRReal> auxPoly;
	CDSRArray<CDSRReal>* pAuxPoly = &auxPoly;

	pResult->resize(1);
	(*pResult)[ 0 ] = 1;
        for( i = 0; i < (long) power; i++ )
	{
		_multyOfPoly(pPolynom, pResult, pAuxPoly);
		pResult->resize(auxPoly.size());
                for( j = 0; j < (long) auxPoly.size(); j++ )
			(*pResult)[ j ] = auxPoly[ j ];
	}
}

//////////////////////////////////////////////////////////////////////

MATHBASE_API
void _firstOrderEquation(CDSRArray<CDSRComplex>& polynom, CDSRComplex& result)
{
	result = - polynom[ 1 ] / polynom[ 0 ];
}

//////////////////////////////////////////////////////////////////////

MATHBASE_API
void _secondOrderEquation(CDSRArray<CDSRComplex>& polynom, CDSRArray<CDSRComplex>& result)
{
	result.resize(2);
	result[ 0 ] = ( - polynom[ 1 ] + sqrt( polynom[ 1 ] * polynom[ 1 ] - 4.0 * polynom[ 0 ] * polynom[ 2 ] ) ) / ( 2.0 * polynom[ 0 ]);
	result[ 1 ] = ( - polynom[ 1 ] - sqrt( polynom[ 1 ] * polynom[ 1 ] - 4.0 * polynom[ 0 ] * polynom[ 2 ] ) ) / ( 2.0 * polynom[ 0 ]);
}

//////////////////////////////////////////////////////////////////////

MATHBASE_API
void _thirdOrderEquation(CDSRArray<CDSRComplex>& polynom, CDSRArray<CDSRComplex>& result)
{
	CDSRComplex r, p, q;

	p = ( polynom[ 2 ] / polynom[ 0 ] ) / 3.0 - 
		( ( ( polynom[ 1 ] / polynom[ 0 ] ) * ( polynom[ 1 ] / polynom[ 0 ] ) ) / 9.0 );
	q = - ( ( polynom[ 1 ] / polynom[ 0 ] ) * ( polynom[ 1 ] / polynom[ 0 ] ) *
		( polynom[ 1 ] / polynom[ 0 ] ) ) / 27.0 + (polynom[ 1 ] / polynom[ 0 ]) *
		( polynom[ 2 ] / polynom[ 0 ] ) / 6.0 - ( polynom[ 3 ] / polynom[ 0 ] ) / 2.0;
	r = q * q + p * p * p;
	r = p != 0.0 ? sqrt(r) + q : 2.0 * q;
	if( r == 0.0 )
	{
		result.resize(3);
		for( int i = 0; i < 3; i++ )
			result[ i ] = - polynom[ 1 ] / ( 3.0 * polynom[ 0 ] );
	}
	else
	{
		ComplexNumberNthRoot(r, 3, result);
		for( int i = 0; i < 3; i++ )
			result[ i ] = - polynom[ 1 ] / ( 3.0 * polynom[ 0 ] ) + result[ i ] - p / result[ i ];
	}
}

//////////////////////////////////////////////////////////////////////

MATHBASE_API
void _fourthOrderEquation(CDSRArray<CDSRComplex>& polynom, CDSRArray<CDSRComplex>& result)
{
	CDSRArray<CDSRComplex> auxPoly, roots;
	CDSRComplex _A2, _t0;

	auxPoly.resize(4);
	auxPoly[ 0 ] = 1.0;
	auxPoly[ 1 ] = - ( polynom[ 2 ] / polynom[ 0 ] );
	auxPoly[ 2 ] = ( polynom[ 1 ] / polynom[ 0 ] ) * ( polynom[ 3 ] / polynom[ 0 ] )
		- 4.0 * ( polynom[ 4 ]  / polynom[ 0 ] );
	auxPoly[ 3 ] = ( polynom[ 4 ] / polynom[ 0 ] ) * ( 4.0 * ( polynom[ 2 ]  / polynom[ 0 ] ) -
		( polynom[ 1 ]  / polynom[ 0 ] ) * ( polynom[ 1 ] / polynom[ 0 ] ) ) -
		( polynom[ 3 ] / polynom[ 0 ] ) * ( polynom[ 3 ] / polynom[ 0 ] );
	_thirdOrderEquation(auxPoly, roots);
	_t0 = roots[ 0 ];
	_A2 = ( ( polynom[ 1 ] / polynom[ 0 ] ) * ( polynom[ 1 ] / polynom[ 0 ] ) )
		/ 4.0 - ( polynom[ 2 ] / polynom[ 0 ] ) + _t0;
	auxPoly.resize(3);
	auxPoly[ 0 ] = 1;
	auxPoly[ 1 ] = ( polynom[ 1 ] / polynom[ 0 ] ) / 2.0 + sqrt(_A2);
	auxPoly[ 2 ] = _t0 / 2.0 + ( _A2 != 0.0 ? ( ( polynom[ 1 ] / polynom[ 0 ] ) * _t0 / 2.0 -
		( polynom[ 3 ] / polynom[ 0 ] ) ) / ( sqrt(_A2) * 2.0 ) : sqrt( ( _t0 * _t0) / 
		4.0 - ( polynom[ 4 ] / polynom[ 0 ] ) ) );
	_secondOrderEquation(auxPoly, roots);
	result.resize(0);
	result.add(roots[ 0 ]);
	result.add(roots[ 1 ]);
	auxPoly[ 1 ] = ( polynom[ 1 ] / polynom[ 0 ] ) / 2.0 - sqrt(_A2);
	auxPoly[ 2 ] = _t0 / 2.0 + ( _A2 != 0.0 ? -( ( polynom[ 1 ] / polynom[ 0 ] ) * _t0 / 2.0 -
		( polynom[ 3 ] / polynom[ 0 ] ) ) / ( sqrt(_A2) * 2.0 ) : -sqrt( ( _t0 * _t0 ) / 
		4.0 - ( polynom[ 4 ] / polynom[ 0 ] ) ) );
	_secondOrderEquation(auxPoly, roots);
	result.add(roots[ 0 ]);
	result.add(roots[ 1 ]);
}

//////////////////////////////////////////////////////////////////////

MATHBASE_API
void _nthOrderEquation(CDSRArray<CDSRComplex>& polynom, CDSRArray<CDSRComplex>& result, CDSRReal rlPrec)
{
        long _size, lPorad, i;
	CDSRArray<CDSRComplex> workPolynom, roots;
	CDSRComplex _x0;
	CDSRReal maxElement, reX, imX;

	_size = polynom.size();
	lPorad = _size - 1;
	workPolynom.resize(_size);
	for( i = 0; i < _size; i++ )
		workPolynom[ i ] = polynom[ i ];

	result.resize(0);
	while( lPorad )
	{
		switch( lPorad )
		{
		case 1:
			_firstOrderEquation(workPolynom, _x0);
			reX = fabs(_x0.real()) > rlPrec ? _x0.real() : 0.0;
			imX = fabs(_x0.imag()) > rlPrec ? _x0.imag() : 0.0;
			result.add(CDSRComplex(reX, imX));
			lPorad--;
			break;

		case 2:
			_secondOrderEquation(workPolynom, roots);
			reX = fabs(roots[ 0 ].real()) > rlPrec ? roots[ 0 ].real() : 0.0;
			imX = fabs(roots[ 0 ].imag()) > rlPrec ? roots[ 0 ].imag() : 0.0;
			result.add(CDSRComplex(reX, imX));
			reX = fabs(roots[ 1 ].real()) > rlPrec ? roots[ 1 ].real() : 0.0;
			imX = fabs(roots[ 1 ].imag()) > rlPrec ? roots[ 1 ].imag() : 0.0;
			result.add(CDSRComplex(reX, imX));
			lPorad -= 2;
			break;

		default:
			maxElement = abs(workPolynom[ 0 ]);
			for( i = 1; i < lPorad - 1; i++ )
				if( abs(workPolynom[ i ]) > maxElement )
					maxElement = abs(workPolynom[ i ]);
			_x0 = CDSRComplex(fabs( 1 / ( 1 + maxElement / abs(workPolynom[ lPorad ] ) ) ),
				workPolynom[ lPorad ].real() < 0.0 ? 0.0 : fabs( 1 / ( 1 + maxElement / abs(workPolynom[ lPorad ] ) ) ) );
			_newtonPolinomsRoot(&workPolynom, _x0, POLYNOM_MAXITERS, rlPrec);
			_newtonPolinomsRoot(&polynom, _x0, POLYNOM_MAXITERS, rlPrec);
			_divOnRoot(&workPolynom, _x0, &roots);
			workPolynom.resize(roots.size());
                        for( i = 0; i < (long) roots.size(); i++ )
				workPolynom[ i ] = roots[ i ];
			lPorad--;
			reX = fabs(_x0.real()) > rlPrec ? _x0.real() : 0.0;
			imX = fabs(_x0.imag()) > rlPrec ? _x0.imag() : 0.0;
			result.add(CDSRComplex(reX, imX));
		}
	}
}

//////////////////////////////////////////////////////////////////////

MATHBASE_API
void _poly4ComplexRoots(CDSRArray<ROOT_INFO>* pRootsArray, CDSRArray<CDSRComplex>* pPolynom)
{
        long i, j;
	CDSRArray<CDSRComplex> poly1, poly2;

	pPolynom->resize(1);
	(*pPolynom)[ 0 ] = CDSRComplex( 1.0, 0.0 );
        for( i = 0; i < (long) pRootsArray->size(); i++ )
	{
		if( (*pRootsArray)[ i ].multiplicity > 1 )
		{
			poly1.resize(2);
			poly1[ 0 ] = CDSRComplex( 1.0, 0.0 );
			poly1[ 1 ] = CDSRComplex( -(*pRootsArray)[ i ].real, -(*pRootsArray)[ i ].image );
			complexBinomNewton(poly1, (*pRootsArray)[ i ].multiplicity, poly2);
		}
		else
		{
			poly2.resize(2);
			poly2[ 0 ] = CDSRComplex( 1.0, 0.0 );
			poly2[ 1 ] = CDSRComplex( -(*pRootsArray)[ i ].real, -(*pRootsArray)[ i ].image );
		}

		poly1.resize(pPolynom->size());
                for( j = 0; j < (long) pPolynom->size(); j++ )
			poly1[ j ] = (*pPolynom)[ j ];

		_multyOfPoly(&poly1, &poly2, pPolynom);
	}
}

//////////////////////////////////////////////////////////////////////
