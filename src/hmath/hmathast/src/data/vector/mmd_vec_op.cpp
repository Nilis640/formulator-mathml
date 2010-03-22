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

#include "HMathBS/islaecommon.h"
#include "mmd_vec_op.h"

///////////////////////////////////////////////////////////////////////////////
// INVERSE
///////////////////////////////////////////////////////////////////////////////
void _dsrop_VectorInverse_real( UniWord *resultDet, MMD_Vector *a, CParseDsrSymbolTable& smbtable )
{
#ifndef _NO_HMATHBS
	CDSRMMatrix<CDSRReal> src, dest;
	smbtable.convert2DSRRealMMatrix( a, src );
	dest.resize_matrix( src.row(), src.col() );
	::_Inverse( &src, &dest );
	*resultDet = smbtable.makeUniWord_Vector( dest );
#endif
}

void _dsrop_VectorInverse_complex( UniWord *resultDet, MMD_Vector *a, CParseDsrSymbolTable& smbtable )
{
#ifndef _NO_HMATHBS
	CDSRMMatrix<CDSRComplex> src, dest;
	smbtable.convert2DSRComplexMMatrix( a, src );
	dest.resize_matrix( src.row(), src.col() );
	::_Inverse( &src, &dest );
	*resultDet = smbtable.makeUniWord_Vector( dest );
#endif
}

///////////////////////////////////////////////////////////////////////////////
// ABS
///////////////////////////////////////////////////////////////////////////////
void _dsrop_abs_Vector_integer( UniWord *resultDet, MMD_Vector *a, CParseDsrSymbolTable& smbtable )
{
#ifndef _NO_HMATHBS
	CDSRMMatrix<CDSRInteger> res;
	smbtable.convert2DSRIntegerMMatrix( a, res );
	CDSRInteger rlDet;
	::_DeterminantCR( &res, rlDet );
	*resultDet = rlDet;
#endif
}

void _dsrop_abs_Vector_real( UniWord *resultDet, MMD_Vector *a, CParseDsrSymbolTable& smbtable )
{
#ifndef _NO_HMATHBS
	CDSRMMatrix<CDSRReal> res;
	smbtable.convert2DSRRealMMatrix( a, res );
	CDSRReal rlDet;
	::_DeterminantCR( &res, rlDet );
	*resultDet = rlDet;
#endif
}

void _dsrop_abs_Vector_complex( UniWord *resultDet, MMD_Vector *a, CParseDsrSymbolTable& smbtable )
{
#ifndef _NO_HMATHBS
	CDSRMMatrix<CDSRComplex> res;
	smbtable.convert2DSRComplexMMatrix( a, res );
	CDSRComplex rlDet;
	::_DeterminantCR( &res, rlDet );
	*resultDet = rlDet;
#endif
}

///////////////////////////////////////////////////////////////////////////////
// ADD
///////////////////////////////////////////////////////////////////////////////
MMD_Vector* _dsrop_VectorAdd_natural( MMD_Vector *a, MMD_Vector *b )
{
	MMD_Vector *ret = a->getSmbTable().makeUniWord( 
		new MMD_Vector( a->getSmbTable(), a->getType(),
						a->getRows(), a->getColumns() ) 
	);
	for( long i = 0; i < (long) ret->getCount(); i++ )
		_dsrop_Add_natural( ret->at( i ), a->at( i ), b->at( i ) );
	return ret;
}

MMD_Vector* _dsrop_VectorAdd_integer( MMD_Vector *a, MMD_Vector *b )
{
	MMD_Vector *ret = a->getSmbTable().makeUniWord( 
		new MMD_Vector( a->getSmbTable(), a->getType(),
						a->getRows(), a->getColumns() ) 
	);
	for( long i = 0; i < (long) ret->getCount(); i++ )
		_dsrop_Add_integer( ret->at( i ), a->at( i ), b->at( i ) );
	return ret;
}

MMD_Vector* _dsrop_VectorAdd_rational( MMD_Vector *a, MMD_Vector *b )
{
	MMD_Vector *ret = a->getSmbTable().makeUniWord( 
		new MMD_Vector( a->getSmbTable(), a->getType(),
						a->getRows(), a->getColumns() ) 
	);
	for( long i = 0; i < (long) ret->getCount(); i++ )
		_dsrop_Add_rational( ret->at( i ), a->at( i ), b->at( i ) );
	return ret;
}

MMD_Vector* _dsrop_VectorAdd_real( MMD_Vector *a, MMD_Vector *b )
{
	MMD_Vector *ret = a->getSmbTable().makeUniWord( 
		new MMD_Vector( a->getSmbTable(), a->getType(),
						a->getRows(), a->getColumns() ) 
	);
	for( long i = 0; i < (long) ret->getCount(); i++ )
		_dsrop_Add_real( ret->at( i ), a->at( i ), b->at( i ) );
	return ret;
}

MMD_Vector* _dsrop_VectorAdd_complex( MMD_Vector *a, MMD_Vector *b )
{
	MMD_Vector *ret = a->getSmbTable().makeUniWord( 
		new MMD_Vector( a->getSmbTable(), a->getType(),
						a->getRows(), a->getColumns() ) 
	);
	for( long i = 0; i < (long) ret->getCount(); i++ )
		_dsrop_Add_complex( ret->at( i ), a->at( i ), b->at( i ) );
	return ret;
}

///////////////////////////////////////////////////////////////////////////////

MMD_Vector* _dsrop_VectorPlus_natural( MMD_Vector *a )
{
	MMD_Vector *ret = a->getSmbTable().makeUniWord( 
		new MMD_Vector( a->getSmbTable(), a->getType(),
						a->getRows(), a->getColumns() ) 
	);
	for( long i = 0; i < (long) ret->getCount(); i++ )
		_dsrop_Plus_natural( ret->at( i ), a->at( i ) );
	return ret;
}

MMD_Vector* _dsrop_VectorPlus_integer( MMD_Vector *a )
{
	MMD_Vector *ret = a->getSmbTable().makeUniWord( 
		new MMD_Vector( a->getSmbTable(), a->getType(),
						a->getRows(), a->getColumns() ) 
	);
	for( long i = 0; i < (long) ret->getCount(); i++ )
		_dsrop_Plus_integer( ret->at( i ), a->at( i ) );
	return ret;
}

MMD_Vector* _dsrop_VectorPlus_rational( MMD_Vector *a )
{
	MMD_Vector *ret = a->getSmbTable().makeUniWord( 
		new MMD_Vector( a->getSmbTable(), a->getType(),
						a->getRows(), a->getColumns() ) 
	);
	for( long i = 0; i < (long) ret->getCount(); i++ )
		_dsrop_Plus_rational( ret->at( i ), a->at( i ) );
	return ret;
}

MMD_Vector* _dsrop_VectorPlus_real( MMD_Vector *a )
{
	MMD_Vector *ret = a->getSmbTable().makeUniWord( 
		new MMD_Vector( a->getSmbTable(), a->getType(),
						a->getRows(), a->getColumns() ) 
	);
	for( long i = 0; i < (long) ret->getCount(); i++ )
		_dsrop_Plus_real( ret->at( i ), a->at( i ) );
	return ret;
}

MMD_Vector* _dsrop_VectorPlus_complex( MMD_Vector *a )
{
	MMD_Vector *ret = a->getSmbTable().makeUniWord( 
		new MMD_Vector( a->getSmbTable(), a->getType(),
						a->getRows(), a->getColumns() ) 
	);
	for( long i = 0; i < (long) ret->getCount(); i++ )
		_dsrop_Plus_complex( ret->at( i ), a->at( i ) );
	return ret;
}

///////////////////////////////////////////////////////////////////////////////
// SUB
///////////////////////////////////////////////////////////////////////////////
MMD_Vector* _dsrop_VectorSub_integer( MMD_Vector *a, MMD_Vector *b )
{
	MMD_Vector *ret = a->getSmbTable().makeUniWord( 
		new MMD_Vector( a->getSmbTable(), a->getType(),
						a->getRows(), a->getColumns() ) 
	);
	for( long i = 0; i < (long) ret->getCount(); i++ )
		_dsrop_Sub_integer( ret->at( i ), a->at( i ), b->at( i ) );
	return ret;
}

MMD_Vector* _dsrop_VectorSub_rational( MMD_Vector *a, MMD_Vector *b )
{
	MMD_Vector *ret = a->getSmbTable().makeUniWord( 
		new MMD_Vector( a->getSmbTable(), a->getType(),
						a->getRows(), a->getColumns() ) 
	);
	for( long i = 0; i < (long) ret->getCount(); i++ )
		_dsrop_Sub_rational( ret->at( i ), a->at( i ), b->at( i ) );
	return ret;
}

MMD_Vector* _dsrop_VectorSub_real( MMD_Vector *a, MMD_Vector *b )
{
	MMD_Vector *ret = a->getSmbTable().makeUniWord( 
		new MMD_Vector( a->getSmbTable(), a->getType(),
						a->getRows(), a->getColumns() ) 
	);
	for( long i = 0; i < (long) ret->getCount(); i++ )
		_dsrop_Sub_real( ret->at( i ), a->at( i ), b->at( i ) );
	return ret;
}

MMD_Vector* _dsrop_VectorSub_complex( MMD_Vector *a, MMD_Vector *b )
{
	MMD_Vector *ret = a->getSmbTable().makeUniWord( 
		new MMD_Vector( a->getSmbTable(), a->getType(),
						a->getRows(), a->getColumns() ) 
	);
	for( long i = 0; i < (long) ret->getCount(); i++ )
		_dsrop_Sub_complex( ret->at( i ), a->at( i ), b->at( i ) );
	return ret;
}

///////////////////////////////////////////////////////////////////////////////

MMD_Vector* _dsrop_VectorMinus_integer( MMD_Vector *a )
{
	MMD_Vector *ret = a->getSmbTable().makeUniWord( 
		new MMD_Vector( a->getSmbTable(), a->getType(),
						a->getRows(), a->getColumns() ) 
	);
	for( long i = 0; i < (long) ret->getCount(); i++ )
		_dsrop_Minus_integer( ret->at( i ), a->at( i ) );
	return ret;
}

MMD_Vector* _dsrop_VectorMinus_rational( MMD_Vector *a )
{
	MMD_Vector *ret = a->getSmbTable().makeUniWord( 
		new MMD_Vector( a->getSmbTable(), a->getType(),
						a->getRows(), a->getColumns() ) 
	);
	for( long i = 0; i < (long) ret->getCount(); i++ )
		_dsrop_Minus_rational( ret->at( i ), a->at( i ) );
	return ret;
}

MMD_Vector* _dsrop_VectorMinus_real( MMD_Vector *a )
{
	MMD_Vector *ret = a->getSmbTable().makeUniWord( 
		new MMD_Vector( a->getSmbTable(), a->getType(),
						a->getRows(), a->getColumns() ) 
	);
	for( long i = 0; i < (long) ret->getCount(); i++ )
		_dsrop_Minus_real( ret->at( i ), a->at( i ) );
	return ret;
}

MMD_Vector* _dsrop_VectorMinus_complex( MMD_Vector *a )
{
	MMD_Vector *ret = a->getSmbTable().makeUniWord( 
		new MMD_Vector( a->getSmbTable(), a->getType(),
						a->getRows(), a->getColumns() ) 
	);
	for( long i = 0; i < (long) ret->getCount(); i++ )
		_dsrop_Minus_complex( ret->at( i ), a->at( i ) );
	return ret;
}

///////////////////////////////////////////////////////////////////////////////
// MUL
///////////////////////////////////////////////////////////////////////////////
MMD_Vector* _dsrop_VectorMul_natural( MMD_Vector *a, UniWord& b )
{
	MMD_Vector *ret = a->getSmbTable().makeUniWord( 
		new MMD_Vector( a->getSmbTable(), a->getType(),
						a->getRows(), a->getColumns() ) 
	);
	for( long i = 0; i < (long) ret->getCount(); i++ )
		_dsrop_Mul_natural( ret->at( i ), a->at( i ), b );
	return ret;
}

MMD_Vector* _dsrop_VectorMul_integer( MMD_Vector *a, UniWord& b )
{
	MMD_Vector *ret = a->getSmbTable().makeUniWord( 
		new MMD_Vector( a->getSmbTable(), a->getType(),
						a->getRows(), a->getColumns() ) 
	);
	for( long i = 0; i < (long) ret->getCount(); i++ )
		_dsrop_Mul_integer( ret->at( i ), a->at( i ), b );
	return ret;
}

MMD_Vector* _dsrop_VectorMul_rational( MMD_Vector *a, UniWord& b )
{
	MMD_Vector *ret = a->getSmbTable().makeUniWord( 
		new MMD_Vector( a->getSmbTable(), a->getType(),
						a->getRows(), a->getColumns() ) 
	);
	for( long i = 0; i < (long) ret->getCount(); i++ )
		_dsrop_Mul_rational( ret->at( i ), a->at( i ), b );
	return ret;
}

MMD_Vector* _dsrop_VectorMul_real( MMD_Vector *a, UniWord& b )
{
	MMD_Vector *ret = a->getSmbTable().makeUniWord( 
		new MMD_Vector( a->getSmbTable(), a->getType(),
						a->getRows(), a->getColumns() ) 
	);
	for( long i = 0; i < (long) ret->getCount(); i++ )
		_dsrop_Mul_real( ret->at( i ), a->at( i ), b );
	return ret;
}

MMD_Vector* _dsrop_VectorMul_complex( MMD_Vector *a, UniWord& b )
{
	MMD_Vector *ret = a->getSmbTable().makeUniWord( 
		new MMD_Vector( a->getSmbTable(), a->getType(),
						a->getRows(), a->getColumns() ) 
	);
	for( long i = 0; i < (long) ret->getCount(); i++ )
		_dsrop_Mul_complex( ret->at( i ), a->at( i ), b );
	return ret;
}

///////////////////////////////////////////////////////////////////////////////

UniWord _dsrop_VectorScalarMul_natural( MMD_Vector *a, MMD_Vector *b )
{
#if _DEBUG
	if( a->getCount() != b->getCount() )
		throw _T("Internal error");
#endif
	UniWord ret, tmp;
	_dsrop_Mul_natural( ret, a->at( 0 ), b->at( 0 ) );
	for( long i = 1; i < (long) a->getCount(); i++ )
	{
		_dsrop_Mul_natural( tmp, a->at( i ), b->at( i ) );
		_dsrop_Add_natural( ret, ret, tmp );
	}
	return ret;
}

UniWord _dsrop_VectorScalarMul_integer( MMD_Vector *a, MMD_Vector *b )
{
#if _DEBUG
	if( a->getCount() != b->getCount() )
		throw _T("Internal error");
#endif
	UniWord ret, tmp;
	_dsrop_Mul_integer( ret, a->at( 0 ), b->at( 0 ) );
	for( long i = 1; i < (long) a->getCount(); i++ )
	{
		_dsrop_Mul_integer( tmp, a->at( i ), b->at( i ) );
		_dsrop_Add_integer( ret, ret, tmp );
	}
	return ret;
}

UniWord _dsrop_VectorScalarMul_rational( MMD_Vector *a, MMD_Vector *b )
{
#if _DEBUG
	if( a->getCount() != b->getCount() )
		throw _T("Internal error");
#endif
	UniWord ret, tmp;
	_dsrop_Mul_rational( ret, a->at( 0 ), b->at( 0 ) );
	for( long i = 1; i < (long) a->getCount(); i++ )
	{
		_dsrop_Mul_rational( tmp, a->at( i ), b->at( i ) );
		_dsrop_Add_rational( ret, ret, tmp );
	}
	return ret;
}

UniWord _dsrop_VectorScalarMul_real( MMD_Vector *a, MMD_Vector *b )
{
#if _DEBUG
	if( a->getCount() != b->getCount() )
		throw _T("Internal error");
#endif
	UniWord ret, tmp;
	_dsrop_Mul_real( ret, a->at( 0 ), b->at( 0 ) );
	for( long i = 1; i < (long) a->getCount(); i++ )
	{
		_dsrop_Mul_real( tmp, a->at( i ), b->at( i ) );
		_dsrop_Add_real( ret, ret, tmp );
	}
	return ret;
}

UniWord _dsrop_VectorScalarMul_complex( MMD_Vector *a, MMD_Vector *b )
{
#if _DEBUG
	if( a->getCount() != b->getCount() )
		throw _T("Internal error");
#endif
	UniWord ret, tmp;
	_dsrop_Mul_complex( ret, a->at( 0 ), b->at( 0 ) );
	for( long i = 1; i < (long) a->getCount(); i++ )
	{
		_dsrop_Mul_complex( tmp, a->at( i ), b->at( i ) );
		_dsrop_Add_complex( ret, ret, tmp );
	}
	return ret;
}

///////////////////////////////////////////////////////////////////////////////
MMD_Vector* _dsrop_VectorMul_natural( MMD_Vector *a, MMD_Vector *b )
{
	MMD_Vector *ret = a->getSmbTable().makeUniWord( 
		new MMD_Vector( a->getSmbTable(), a->getType(),
						a->getRows(), b->getColumns() )
	);
	UniWord tmp;
	long i, j, k;
	for( i = 1; i <= a->getRows(); i++ )
	{
		for( j = 1; j <= b->getColumns(); j++ )
		{
			UniWord uw_null( CDSRNatural( 0 ) );
			for( k = 1; k <= a->getColumns(); k++ )
			{
				_dsrop_Mul_natural( tmp, a->get( i, k ), b->get( k, j ) );
				_dsrop_Add_natural( uw_null, uw_null, tmp );
			}
			ret->set_natural( i, j, uw_null );
		}
	}
	return ret;
}

MMD_Vector* _dsrop_VectorMul_integer( MMD_Vector *a, MMD_Vector *b )
{
	MMD_Vector *ret = a->getSmbTable().makeUniWord( 
		new MMD_Vector( a->getSmbTable(), a->getType(),
						a->getRows(), b->getColumns() )
	);
	UniWord tmp;
	long i, j, k;
	for( i = 1; i <= a->getRows(); i++ )
	{
		for( j = 1; j <= b->getColumns(); j++ )
		{
			UniWord uw_null( CDSRInteger( 0 ) );
			for( k = 1; k <= a->getColumns(); k++ )
			{
				_dsrop_Mul_integer( tmp, a->get( i, k ), b->get( k, j ) );
				_dsrop_Add_integer( uw_null, uw_null, tmp );
			}
			ret->set_integer( i, j, uw_null );
		}
	}
	return ret;
}

MMD_Vector* _dsrop_VectorMul_rational( MMD_Vector *a, MMD_Vector *b )
{
	MMD_Vector *ret = a->getSmbTable().makeUniWord( 
		new MMD_Vector( a->getSmbTable(), a->getType(),
						a->getRows(), b->getColumns() )
	);
	UniWord tmp;
	long i, j, k;
	for( i = 1; i <= a->getRows(); i++ )
	{
		for( j = 1; j <= b->getColumns(); j++ )
		{
			UniWord uw_null( CDSRRational( 0, 1 ) );
			for( k = 1; k <= a->getColumns(); k++ )
			{
				_dsrop_Mul_rational( tmp, a->get( i, k ), b->get( k, j ) );
				_dsrop_Add_rational( uw_null, uw_null, tmp );
			}
			ret->set_rational( i, j, uw_null );
		}
	}
	return ret;
}

MMD_Vector* _dsrop_VectorMul_real( MMD_Vector *a, MMD_Vector *b )
{
	MMD_Vector *ret = a->getSmbTable().makeUniWord( 
		new MMD_Vector( a->getSmbTable(), a->getType(),
						a->getRows(), b->getColumns() )
	);
	UniWord tmp;
	long i, j, k;
	for( i = 1; i <= a->getRows(); i++ )
	{
		for( j = 1; j <= b->getColumns(); j++ )
		{
			UniWord uw_null( CDSRReal( 0 ) );
			for( k = 1; k <= a->getColumns(); k++ )
			{
				_dsrop_Mul_real( tmp, a->get( i, k ), b->get( k, j ) );
				_dsrop_Add_real( uw_null, uw_null, tmp );
			}
			ret->set_real( i, j, uw_null );
		}
	}
	return ret;
}

MMD_Vector* _dsrop_VectorMul_complex( MMD_Vector *a, MMD_Vector *b )
{
	MMD_Vector *ret = a->getSmbTable().makeUniWord( 
		new MMD_Vector( a->getSmbTable(), a->getType(),
						a->getRows(), b->getColumns() )
	);
	UniWord tmp;
	long i, j, k;
	for( i = 1; i <= a->getRows(); i++ )
	{
		for( j = 1; j <= b->getColumns(); j++ )
		{
			UniWord uw_null( CDSRComplex( 0 ) );
			for( k = 1; k <= a->getColumns(); k++ )
			{
				_dsrop_Mul_complex( tmp, a->get( i, k ), b->get( k, j ) );
				_dsrop_Add_complex( uw_null, uw_null, tmp );
			}
			ret->set_complex( i, j, uw_null );
		}
	}
	return ret;
}

///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// DIV
///////////////////////////////////////////////////////////////////////////////
MMD_Vector* _dsrop_VectorDiv_natural( MMD_Vector *a, UniWord& b )
{
	MMD_Vector *ret = a->getSmbTable().makeUniWord( 
		new MMD_Vector( a->getSmbTable(), a->getType(),
						a->getRows(), a->getColumns() ) 
	);
	for( long i = 0; i < (long) ret->getCount(); i++ )
		_dsrop_Div_natural( ret->at( i ), a->at( i ), b );
	return ret;
}

MMD_Vector* _dsrop_VectorDiv_integer( MMD_Vector *a, UniWord& b )
{
	MMD_Vector *ret = a->getSmbTable().makeUniWord( 
		new MMD_Vector( a->getSmbTable(), a->getType(),
						a->getRows(), a->getColumns() ) 
	);
	for( long i = 0; i < (long) ret->getCount(); i++ )
		_dsrop_Div_integer( ret->at( i ), a->at( i ), b );
	return ret;
}

MMD_Vector* _dsrop_VectorDiv_rational( MMD_Vector *a, UniWord& b )
{
	MMD_Vector *ret = a->getSmbTable().makeUniWord( 
		new MMD_Vector( a->getSmbTable(), a->getType(),
						a->getRows(), a->getColumns() ) 
	);
	for( long i = 0; i < (long) ret->getCount(); i++ )
		_dsrop_Div_rational( ret->at( i ), a->at( i ), b );
	return ret;
}

MMD_Vector* _dsrop_VectorDiv_real( MMD_Vector *a, UniWord& b )
{
	MMD_Vector *ret = a->getSmbTable().makeUniWord( 
		new MMD_Vector( a->getSmbTable(), a->getType(),
						a->getRows(), a->getColumns() ) 
	);
	for( long i = 0; i < (long) ret->getCount(); i++ )
		_dsrop_Div_real( ret->at( i ), a->at( i ), b );
	return ret;
}

MMD_Vector* _dsrop_VectorDiv_complex( MMD_Vector *a, UniWord& b )
{
	MMD_Vector *ret = a->getSmbTable().makeUniWord( 
		new MMD_Vector( a->getSmbTable(), a->getType(),
						a->getRows(), a->getColumns() ) 
	);
	for( long i = 0; i < (long) ret->getCount(); i++ )
		_dsrop_Div_complex( ret->at( i ), a->at( i ), b );
	return ret;
}

///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// CONVERT
///////////////////////////////////////////////////////////////////////////////
MMD_Vector* _dsrop_VectorConvert_natural2integer( MMD_Vector *a )
{
	MMD_Vector *ret = a->getSmbTable().makeUniWord( 
		new MMD_Vector( a->getSmbTable(), a->getType(),
						a->getRows(), a->getColumns() ) 
	);
	for( long i = 0; i < (long) ret->getCount(); i++ )
		_dsrop_Convert_natural2integer( ret->at( i ), a->at( i ) );
	return ret;
}

MMD_Vector* _dsrop_VectorConvert_natural2rational( MMD_Vector *a )
{
	MMD_Vector *ret = a->getSmbTable().makeUniWord( 
		new MMD_Vector( a->getSmbTable(), a->getType(),
						a->getRows(), a->getColumns() ) 
	);
	for( long i = 0; i < (long) ret->getCount(); i++ )
		_dsrop_Convert_natural2rational( ret->at( i ), a->at( i ) );
	return ret;
}

MMD_Vector* _dsrop_VectorConvert_natural2real( MMD_Vector *a )
{
	MMD_Vector *ret = a->getSmbTable().makeUniWord( 
		new MMD_Vector( a->getSmbTable(), a->getType(),
						a->getRows(), a->getColumns() ) 
	);
	for( long i = 0; i < (long) ret->getCount(); i++ )
		_dsrop_Convert_natural2real( ret->at( i ), a->at( i ) );
	return ret;
}

MMD_Vector* _dsrop_VectorConvert_natural2complex( MMD_Vector *a )
{
	MMD_Vector *ret = a->getSmbTable().makeUniWord( 
		new MMD_Vector( a->getSmbTable(), a->getType(),
						a->getRows(), a->getColumns() ) 
	);
	for( long i = 0; i < (long) ret->getCount(); i++ )
		_dsrop_Convert_natural2complex( ret->at( i ), a->at( i ) );
	return ret;
}

MMD_Vector* _dsrop_VectorConvert_integer2rational( MMD_Vector *a )
{
	MMD_Vector *ret = a->getSmbTable().makeUniWord( 
		new MMD_Vector( a->getSmbTable(), a->getType(),
						a->getRows(), a->getColumns() ) 
	);
	for( long i = 0; i < (long) ret->getCount(); i++ )
		_dsrop_Convert_integer2rational( ret->at( i ), a->at( i ) );
	return ret;
}

MMD_Vector* _dsrop_VectorConvert_integer2real( MMD_Vector *a )
{
	MMD_Vector *ret = a->getSmbTable().makeUniWord( 
		new MMD_Vector( a->getSmbTable(), a->getType(),
						a->getRows(), a->getColumns() ) 
	);
	for( long i = 0; i < (long) ret->getCount(); i++ )
		_dsrop_Convert_integer2real( ret->at( i ), a->at( i ) );
	return ret;
}

MMD_Vector* _dsrop_VectorConvert_integer2complex( MMD_Vector *a )
{
	MMD_Vector *ret = a->getSmbTable().makeUniWord( 
		new MMD_Vector( a->getSmbTable(), a->getType(),
						a->getRows(), a->getColumns() ) 
	);
	for( long i = 0; i < (long) ret->getCount(); i++ )
		_dsrop_Convert_integer2complex( ret->at( i ), a->at( i ) );
	return ret;
}

MMD_Vector* _dsrop_VectorConvert_rational2real( MMD_Vector *a )
{
	MMD_Vector *ret = a->getSmbTable().makeUniWord( 
		new MMD_Vector( a->getSmbTable(), a->getType(),
						a->getRows(), a->getColumns() ) 
	);
	for( long i = 0; i < (long) ret->getCount(); i++ )
		_dsrop_Convert_rational2real( ret->at( i ), a->at( i ) );
	return ret;
}

MMD_Vector* _dsrop_VectorConvert_rational2complex( MMD_Vector *a )
{
	MMD_Vector *ret = a->getSmbTable().makeUniWord( 
		new MMD_Vector( a->getSmbTable(), a->getType(),
						a->getRows(), a->getColumns() ) 
	);
	for( long i = 0; i < (long) ret->getCount(); i++ )
		_dsrop_Convert_rational2complex( ret->at( i ), a->at( i ) );
	return ret;
}

MMD_Vector* _dsrop_VectorConvert_real2complex( MMD_Vector *a )
{
	MMD_Vector *ret = a->getSmbTable().makeUniWord( 
		new MMD_Vector( a->getSmbTable(), a->getType(),
						a->getRows(), a->getColumns() ) 
	);
	for( long i = 0; i < (long) ret->getCount(); i++ )
		_dsrop_Convert_real2complex( ret->at( i ), a->at( i ) );
	return ret;
}

///////////////////////////////////////////////////////////////////////////////
// (NOT) EQUAL
///////////////////////////////////////////////////////////////////////////////
UniWord _dsrop_VectorEqual_anytype( MMD_Vector *a, MMD_Vector *b )
{
	if( a->getType() != b->getType() )
		return UniWord( CDSRInteger( 0 ) );
	switch( a->getType() )
	{
	case DSRDATA_TYPE_INTEGER:
		return _dsrop_VectorEqual_integer( a, b );
	case DSRDATA_TYPE_REAL:
		return _dsrop_VectorEqual_real( a, b );
	case DSRDATA_TYPE_COMPLEX:
		return _dsrop_VectorEqual_complex( a, b );
	default:
		break;
	}
	return UniWord( CDSRInteger( 0 ) );
}

UniWord _dsrop_VectorEqual_natural( MMD_Vector *a, MMD_Vector *b )
{
	UniWord ret( CDSRInteger( 0 ) );
	if( a->getRows() != b->getRows() || a->getColumns() != b->getColumns() )
		return ret;
	for( long i = 0; i < (long) a->getCount(); i++ )
		if( !_dsrop_Equal_natural( ret, a->at( i ), b->at( i ) ).getInteger() )
			return ret;
	return ret;
}

UniWord _dsrop_VectorEqual_integer( MMD_Vector *a, MMD_Vector *b )
{
	UniWord ret( CDSRInteger( 0 ) );
	if( a->getRows() != b->getRows() || a->getColumns() != b->getColumns() )
		return ret;
	for( long i = 0; i < (long) a->getCount(); i++ )
		if( !_dsrop_Equal_integer( ret, a->at( i ), b->at( i ) ).getInteger() )
			return ret;
	return ret;
}

UniWord _dsrop_VectorEqual_rational( MMD_Vector *a, MMD_Vector *b )
{
	UniWord ret( CDSRInteger( 0 ) );
	if( a->getRows() != b->getRows() || a->getColumns() != b->getColumns() )
		return ret;
	for( long i = 0; i < (long) a->getCount(); i++ )
		if( !_dsrop_Equal_rational( ret, a->at( i ), b->at( i ) ).getInteger() )
			return ret;
	return ret;
}

UniWord _dsrop_VectorEqual_real( MMD_Vector *a, MMD_Vector *b )
{
	UniWord ret( CDSRInteger( 0 ) );
	if( a->getRows() != b->getRows() || a->getColumns() != b->getColumns() )
		return ret;
	for( long i = 0; i < (long) a->getCount(); i++ )
		if( !_dsrop_Equal_real( ret, a->at( i ), b->at( i ) ).getInteger() )
			return ret;
	return ret;
}

UniWord _dsrop_VectorEqual_complex( MMD_Vector *a, MMD_Vector *b )
{
	UniWord ret( CDSRInteger( 0 ) );
	if( a->getRows() != b->getRows() || a->getColumns() != b->getColumns() )
		return ret;
	for( long i = 0; i < (long) a->getCount(); i++ )
		if( !_dsrop_Equal_complex( ret, a->at( i ), b->at( i ) ).getInteger() )
			return ret;
	return ret;
}

///////////////////////////////////////////////////////////////////////////////

UniWord _dsrop_VectorNotEqual_natural( MMD_Vector *a, MMD_Vector *b )
{
	UniWord ret( CDSRInteger( 1 ) );
	if( a->getRows() != b->getRows() || a->getColumns() != b->getColumns() )
		return ret;
	for( long i = 0; i < (long) a->getCount(); i++ )
		if( _dsrop_NotEqual_natural( ret, a->at( i ), b->at( i ) ).getInteger() )
			return ret;
	return ret;
}

UniWord _dsrop_VectorNotEqual_integer( MMD_Vector *a, MMD_Vector *b )
{
	UniWord ret( CDSRInteger( 1 ) );
	if( a->getRows() != b->getRows() || a->getColumns() != b->getColumns() )
		return ret;
	for( long i = 0; i < (long) a->getCount(); i++ )
		if( _dsrop_NotEqual_integer( ret, a->at( i ), b->at( i ) ).getInteger() )
			return ret;
	return ret;
}

UniWord _dsrop_VectorNotEqual_rational( MMD_Vector *a, MMD_Vector *b )
{
	UniWord ret( CDSRInteger( 1 ) );
	if( a->getRows() != b->getRows() || a->getColumns() != b->getColumns() )
		return ret;
	for( long i = 0; i < (long) a->getCount(); i++ )
		if( _dsrop_NotEqual_rational( ret, a->at( i ), b->at( i ) ).getInteger() )
			return ret;
	return ret;
}

UniWord _dsrop_VectorNotEqual_real( MMD_Vector *a, MMD_Vector *b )
{
	UniWord ret( CDSRInteger( 1 ) );
	if( a->getRows() != b->getRows() || a->getColumns() != b->getColumns() )
		return ret;
	for( long i = 0; i < (long) a->getCount(); i++ )
		if( _dsrop_NotEqual_real( ret, a->at( i ), b->at( i ) ).getInteger() )
			return ret;
	return ret;
}

UniWord _dsrop_VectorNotEqual_complex( MMD_Vector *a, MMD_Vector *b )
{
	UniWord ret( CDSRInteger( 1 ) );
	if( a->getRows() != b->getRows() || a->getColumns() != b->getColumns() )
		return ret;
	for( long i = 0; i < (long) a->getCount(); i++ )
		if( _dsrop_NotEqual_complex( ret, a->at( i ), b->at( i ) ).getInteger() )
			return ret;
	return ret;
}

///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// TRANSPOSE
///////////////////////////////////////////////////////////////////////////////
MMD_Vector* _dsrop_VectorTranspose_natural( MMD_Vector *a )
{
	MMD_Vector *ret = a->getSmbTable().makeUniWord( 
		new MMD_Vector( a->getSmbTable(), a->getType(),
						a->getColumns(), a->getRows() )
	);
	long i, j;
	for( i = 1; i <= ret->getRows(); i++ )
		for( j = 1; j <= ret->getColumns(); j++ )
			ret->set_natural( i, j, a->get( j, i ) );
	return ret;
}

MMD_Vector* _dsrop_VectorTranspose_integer( MMD_Vector *a )
{
	MMD_Vector *ret = a->getSmbTable().makeUniWord( 
		new MMD_Vector( a->getSmbTable(), a->getType(),
						a->getColumns(), a->getRows() )
	);
	long i, j;
	for( i = 1; i <= ret->getRows(); i++ )
		for( j = 1; j <= ret->getColumns(); j++ )
			ret->set_integer( i, j, a->get( j, i ) );
	return ret;
}

MMD_Vector* _dsrop_VectorTranspose_rational( MMD_Vector *a )
{
	MMD_Vector *ret = a->getSmbTable().makeUniWord( 
		new MMD_Vector( a->getSmbTable(), a->getType(),
						a->getColumns(), a->getRows() )
	);
	long i, j;
	for( i = 1; i <= ret->getRows(); i++ )
		for( j = 1; j <= ret->getColumns(); j++ )
			ret->set_rational( i, j, a->get( j, i ) );
	return ret;
}

MMD_Vector* _dsrop_VectorTranspose_real( MMD_Vector *a )
{
	MMD_Vector *ret = a->getSmbTable().makeUniWord( 
		new MMD_Vector( a->getSmbTable(), a->getType(),
						a->getColumns(), a->getRows() )
	);
	long i, j;
	for( i = 1; i <= ret->getRows(); i++ )
		for( j = 1; j <= ret->getColumns(); j++ )
			ret->set_real( i, j, a->get( j, i ) );
	return ret;
}

MMD_Vector* _dsrop_VectorTranspose_complex( MMD_Vector *a )
{
	MMD_Vector *ret = a->getSmbTable().makeUniWord( 
		new MMD_Vector( a->getSmbTable(), a->getType(),
						a->getColumns(), a->getRows() )
	);
	long i, j;
	for( i = 1; i <= ret->getRows(); i++ )
		for( j = 1; j <= ret->getColumns(); j++ )
			ret->set_complex( i, j, a->get( j, i ) );
	return ret;
}

///////////////////////////////////////////////////////////////////////////////
// POW
///////////////////////////////////////////////////////////////////////////////
MMD_Vector* _dsrop_VectorPow_natural( MMD_Vector *a, UniWord& p )
{
	if( a->getRows() != a->getColumns() )
		throw "The matrix is not square";
	if( p.getInteger() < -1 )
		throw "Power value must be positive integer";
	else if( p.getInteger() == 0 )
		return _dsrop_ConstructVector_identity( a->getSmbTable(), a->getRows() );
	MMD_Vector *ret = a->clone();
	long i;
	for( i = 1; i < p.getInteger(); i++ )
		ret = _dsrop_VectorMul_natural( ret, a );
	return ret;
}

MMD_Vector* _dsrop_VectorPow_integer( MMD_Vector *a, UniWord& p )
{
	if( a->getRows() != a->getColumns() )
		throw "The matrix is not square";
	if( p.getInteger() < -1 )
		throw "Power value must be positive integer";
	else if( p.getInteger() == 0 )
		return _dsrop_ConstructVector_identity( a->getSmbTable(), a->getRows() );
	MMD_Vector *ret = a->clone();
	long i;
	for( i = 1; i < p.getInteger(); i++ )
		ret = _dsrop_VectorMul_integer( ret, a );
	return ret;
}

MMD_Vector* _dsrop_VectorPow_rational( MMD_Vector *a, UniWord& p )
{
	if( a->getRows() != a->getColumns() )
		throw "The matrix is not square";
	if( p.getInteger() < -1 )
		throw "Power value must be positive integer";
	else if( p.getInteger() == 0 )
		return _dsrop_VectorConvert_integer2rational( _dsrop_ConstructVector_identity( a->getSmbTable(), a->getRows() ) );
	MMD_Vector *ret = a->clone();
	long i;
	for( i = 1; i < p.getInteger(); i++ )
		ret = _dsrop_VectorMul_rational( ret, a );
	return ret;
}

MMD_Vector* _dsrop_VectorPow_real( MMD_Vector *a, UniWord& p )
{
	if( a->getRows() != a->getColumns() )
		throw "The matrix is not square";
	if( p.getInteger() < -1 )
		throw "Power value must be positive integer";
	else if( p.getInteger() == -1 )
	{
		UniWord retvec;
		_dsrop_VectorInverse_real( &retvec, a, a->getSmbTable() );
		return retvec.getVector();
	}
	else if( p.getInteger() == 0 )
		return _dsrop_VectorConvert_integer2real( _dsrop_ConstructVector_identity( a->getSmbTable(), a->getRows() ) );
	MMD_Vector *ret = a->clone();
	long i;
	for( i = 1; i < p.getInteger(); i++ )
		ret = _dsrop_VectorMul_real( ret, a );
	return ret;
}

MMD_Vector* _dsrop_VectorPow_complex( MMD_Vector *a, UniWord& p )
{
	if( a->getRows() != a->getColumns() )
		throw "The matrix is not square";
	if( p.getInteger() < -1 )
		throw "Power value must be positive integer";
	else if( p.getInteger() == -1 )
	{
		UniWord retvec;
		_dsrop_VectorInverse_complex( &retvec, a, a->getSmbTable() );
		return retvec.getVector();
	}
	else if( p.getInteger() == 0 )
		return _dsrop_VectorConvert_integer2complex( _dsrop_ConstructVector_identity( a->getSmbTable(), a->getRows() ) );
	MMD_Vector *ret = a->clone();
	long i;
	for( i = 1; i < p.getInteger(); i++ )
		ret = _dsrop_VectorMul_complex( ret, a );
	return ret;
}

///////////////////////////////////////////////////////////////////////////////
// ASSIGN
///////////////////////////////////////////////////////////////////////////////
MMD_Vector* _dsrop_VectorAssign_natural( MMD_Vector *a, MMD_Vector *b )
{
	long i, j;
	for( i = 1; i <= a->getRows(); i++ )
		for( j = 1; j <= a->getColumns(); j++ )
			a->set_natural( i, j, b->get( i, j ) );
	return a->clone();
}

MMD_Vector* _dsrop_VectorAssign_integer( MMD_Vector *a, MMD_Vector *b )
{
	long i, j;
	for( i = 1; i <= a->getRows(); i++ )
		for( j = 1; j <= a->getColumns(); j++ )
			a->set_integer( i, j, b->get( i, j ) );
	return a->clone();
}

MMD_Vector* _dsrop_VectorAssign_rational( MMD_Vector *a, MMD_Vector *b )
{
	long i, j;
	for( i = 1; i <= a->getRows(); i++ )
		for( j = 1; j <= a->getColumns(); j++ )
			a->set_rational( i, j, b->get( i, j ) );
	return a->clone();
}

MMD_Vector* _dsrop_VectorAssign_real( MMD_Vector *a, MMD_Vector *b )
{
	long i, j;
	for( i = 1; i <= a->getRows(); i++ )
		for( j = 1; j <= a->getColumns(); j++ )
			a->set_real( i, j, b->get( i, j ) );
	return a->clone();
}

MMD_Vector* _dsrop_VectorAssign_complex( MMD_Vector *a, MMD_Vector *b )
{
	long i, j;
	for( i = 1; i <= a->getRows(); i++ )
		for( j = 1; j <= a->getColumns(); j++ )
			a->set_complex( i, j, b->get( i, j ) );
	return a->clone();
}

///////////////////////////////////////////////////////////////////////////////
// CONSTUCT VECTOR : arg -> rows, columns,	
//							item11, item12, ..., item1N,
//							item21, item22, ..., item2N,
//							...........................
//							itemM1, itemM2, ..., itemMN
///////////////////////////////////////////////////////////////////////////////

MMD_Vector* _dsrop_ConstructVector_natural( CParseDsrSymbolTable& smbtable, UniWord *first_arg )
{
	MMD_Vector *res = smbtable.makeUniWord( 
		new MMD_Vector( smbtable,
						DSRDATA_TYPE_NATURAL,
						first_arg->getInteger(),
						(first_arg + 1)->getInteger() ) );
	first_arg += 2;
	long i;
	for( i = 0; i < (long) res->getCount(); i++ )
	{
		(*res)[ i ] = first_arg->v_natural;
		first_arg++;
	}
	return res;
}

MMD_Vector* _dsrop_ConstructVector_integer( CParseDsrSymbolTable& smbtable, UniWord *first_arg )
{
	MMD_Vector *res = smbtable.makeUniWord( 
		new MMD_Vector( smbtable,
						DSRDATA_TYPE_INTEGER,
						first_arg->getInteger(),
						(first_arg + 1)->getInteger() ) );
	first_arg += 2;
	long i;
	for( i = 0; i < (long) res->getCount(); i++ )
	{
		(*res)[ i ] = first_arg->v_integer;
		first_arg++;
	}
	return res;
}

MMD_Vector* _dsrop_ConstructVector_rational( CParseDsrSymbolTable& smbtable, UniWord *first_arg )
{
	MMD_Vector *res = smbtable.makeUniWord( 
		new MMD_Vector( smbtable,
						DSRDATA_TYPE_RATIONAL,
						first_arg->getInteger(),
						(first_arg + 1)->getInteger() ) );
	first_arg += 2;
	long i;
	for( i = 0; i < (long) res->getCount(); i++ )
	{
		(*res)[ i ] = first_arg->getRational();
		first_arg++;
	}
	return res;
}

MMD_Vector* _dsrop_ConstructVector_real( CParseDsrSymbolTable& smbtable, UniWord *first_arg )
{
	MMD_Vector *res = smbtable.makeUniWord( 
		new MMD_Vector( smbtable,
						DSRDATA_TYPE_REAL,
						first_arg->getInteger(),
						(first_arg + 1)->getInteger() ) );
	first_arg += 2;
	long i;
	for( i = 0; i < (long) res->getCount(); i++ )
	{
		(*res)[ i ] = first_arg->getReal();
		first_arg++;
	}
	return res;
}

MMD_Vector* _dsrop_ConstructVector_complex( CParseDsrSymbolTable& smbtable, UniWord *first_arg )
{
	MMD_Vector *res = smbtable.makeUniWord( 
		new MMD_Vector( smbtable,
						DSRDATA_TYPE_COMPLEX,
						first_arg->getInteger(),
						(first_arg + 1)->getInteger() ) );
	first_arg += 2;
	long i;
	for( i = 0; i < (long) res->getCount(); i++ )
	{
		(*res)[ i ] = first_arg->getComplex();
		first_arg++;
	}
	return res;
}

MMD_Vector* _dsrop_ConstructVector_anytype( CParseDsrSymbolTable& smbtable, UniWord *first_arg, enum CDsrDataTypes nested_type )
{
	MMD_Vector *res = smbtable.makeUniWord( 
		new MMD_Vector( smbtable, nested_type, first_arg->getInteger(), (first_arg + 1)->getInteger() ) );
	first_arg += 2;
	long i;
	for( i = 0; i < (long) res->getCount(); i++ )
	{
		(*res)[ i ] = *first_arg;
		first_arg++;
	}
	return res;
}

MMD_Vector* _dsrop_ConstructVector_identity( CParseDsrSymbolTable& smbtable, long dim )
{
	MMD_Vector *res = smbtable.makeUniWord( new MMD_Vector( smbtable, DSRDATA_TYPE_INTEGER, dim, dim ) );
	long i, j;
	UniWord null = CDSRInteger( 0 ), one = CDSRInteger( 1 );
	for( i = 1; i <= res->getRows(); i++ )
		for( j = 1; j <= res->getColumns(); j++ )
			res->set_integer( i, j, i == j ? one : null );
	return res;
}

///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// OPERATOR_square_brackets
///////////////////////////////////////////////////////////////////////////////
