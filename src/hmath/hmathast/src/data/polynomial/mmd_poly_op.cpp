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

#include "mmd_poly_op.h"

///////////////////////////////////////////////////////////////////////////////
// ADD
///////////////////////////////////////////////////////////////////////////////
MMD_Polynomial* _dsrop_PolynomialAdd_integer( MMD_Polynomial *a, MMD_Polynomial *b )
{
	MMD_Polynomial *ret;
	if( !a->isNormal() ) a->Normalize();
	if( !b->isNormal() ) b->Normalize();
	if( a->getMaxPow() < b->getMaxPow() )
	{
		ret = b->getSmbTable().makeUniWord( new MMD_Polynomial( *b ) );
		for( long i = 0; i <= a->getMaxPow(); i++ )
			_dsrop_Add_integer( ret->getCoef( i ), a->getCoef( i ), ret->getCoef( i ) );
	}
	else
	{
		ret = a->getSmbTable().makeUniWord( new MMD_Polynomial( *a ) );
		for( long i = 0; i <= b->getMaxPow(); i++ )
			_dsrop_Add_integer( ret->getCoef( i ), ret->getCoef( i ), b->getCoef( i ) );
	}
	return ret;
}

MMD_Polynomial* _dsrop_PolynomialAdd_real( MMD_Polynomial *a, MMD_Polynomial *b )
{
	MMD_Polynomial *ret;
	if( !a->isNormal() ) a->Normalize();
	if( !b->isNormal() ) b->Normalize();
	if( a->getMaxPow() < b->getMaxPow() )
	{
		ret = b->getSmbTable().makeUniWord( new MMD_Polynomial( *b ) );
		for( long i = 0; i <= a->getMaxPow(); i++ )
			_dsrop_Add_real( ret->getCoef( i ), a->getCoef( i ), ret->getCoef( i ) );
	}
	else
	{
		ret = a->getSmbTable().makeUniWord( new MMD_Polynomial( *a ) );
		for( long i = 0; i <= b->getMaxPow(); i++ )
			_dsrop_Add_real( ret->getCoef( i ), ret->getCoef( i ), b->getCoef( i ) );
	}
	return ret;
}

MMD_Polynomial* _dsrop_PolynomialAdd_complex( MMD_Polynomial *a, MMD_Polynomial *b )
{
	MMD_Polynomial *ret;
	if( !a->isNormal() ) a->Normalize();
	if( !b->isNormal() ) b->Normalize();
	if( a->getMaxPow() < b->getMaxPow() )
	{
		ret = b->getSmbTable().makeUniWord( new MMD_Polynomial( *b ) );
		for( long i = 0; i <= a->getMaxPow(); i++ )
			_dsrop_Add_complex( ret->getCoef( i ), a->getCoef( i ), ret->getCoef( i ) );
	}
	else
	{
		ret = a->getSmbTable().makeUniWord( new MMD_Polynomial( *a ) );
		for( long i = 0; i <= b->getMaxPow(); i++ )
			_dsrop_Add_complex( ret->getCoef( i ), ret->getCoef( i ), b->getCoef( i ) );
	}
	return ret;
}

///////////////////////////////////////////////////////////////////////////////

MMD_Polynomial* _dsrop_PolynomialPlus_integer( MMD_Polynomial *a )
{
	if( !a->isNormal() ) a->Normalize();
	MMD_Polynomial *ret = a->getSmbTable().makeUniWord( new MMD_Polynomial( *a ) );
	for( long i = 0; i < ret->getTermCount(); i++ )
		_dsrop_Plus_integer( ret->getCoef( i ), ret->getCoef( i ) );
	return ret;
}

MMD_Polynomial* _dsrop_PolynomialPlus_real( MMD_Polynomial *a )
{
	if( !a->isNormal() ) a->Normalize();
	MMD_Polynomial *ret = a->getSmbTable().makeUniWord( new MMD_Polynomial( *a ) );
	for( long i = 0; i < ret->getTermCount(); i++ )
		_dsrop_Plus_real( ret->getCoef( i ), ret->getCoef( i ) );
	return ret;
}

MMD_Polynomial* _dsrop_PolynomialPlus_complex( MMD_Polynomial *a )
{
	if( !a->isNormal() ) a->Normalize();
	MMD_Polynomial *ret = a->getSmbTable().makeUniWord( new MMD_Polynomial( *a ) );
	for( long i = 0; i < ret->getTermCount(); i++ )
		_dsrop_Plus_complex( ret->getCoef( i ), ret->getCoef( i ) );
	return ret;
}

///////////////////////////////////////////////////////////////////////////////
// SUB
///////////////////////////////////////////////////////////////////////////////
MMD_Polynomial* _dsrop_PolynomialSub_integer( MMD_Polynomial *a, MMD_Polynomial *b )
{
	MMD_Polynomial *ret;
	if( !a->isNormal() ) a->Normalize();
	if( !b->isNormal() ) b->Normalize();
	if( a->getMaxPow() < b->getMaxPow() )
	{
		long i;
		ret = b->getSmbTable().makeUniWord( new MMD_Polynomial( *b ) );
		for( i = 0; i <= a->getMaxPow(); i++ )
			_dsrop_Sub_integer( ret->getCoef( i ), a->getCoef( i ), ret->getCoef( i ) );
		for( ; i <= b->getMaxPow(); i++ )
			_dsrop_Minus_integer( ret->getCoef( i ), ret->getCoef( i ) );
	}
	else
	{
		ret = a->getSmbTable().makeUniWord( new MMD_Polynomial( *a ) );
		for( long i = 0; i <= b->getMaxPow(); i++ )
			_dsrop_Sub_integer( ret->getCoef( i ), ret->getCoef( i ), b->getCoef( i ) );
	}
	return ret;
}

MMD_Polynomial* _dsrop_PolynomialSub_real( MMD_Polynomial *a, MMD_Polynomial *b )
{
	MMD_Polynomial *ret;
	if( !a->isNormal() ) a->Normalize();
	if( !b->isNormal() ) b->Normalize();
	if( a->getMaxPow() < b->getMaxPow() )
	{
		long i;
		ret = b->getSmbTable().makeUniWord( new MMD_Polynomial( *b ) );
		for( i = 0; i <= a->getMaxPow(); i++ )
			_dsrop_Sub_real( ret->getCoef( i ), a->getCoef( i ), ret->getCoef( i ) );
		for( ; i <= b->getMaxPow(); i++ )
			_dsrop_Minus_real( ret->getCoef( i ), ret->getCoef( i ) );
	}
	else
	{
		ret = a->getSmbTable().makeUniWord( new MMD_Polynomial( *a ) );
		for( long i = 0; i <= b->getMaxPow(); i++ )
			_dsrop_Sub_real( ret->getCoef( i ), ret->getCoef( i ), b->getCoef( i ) );
	}
	return ret;
}

MMD_Polynomial* _dsrop_PolynomialSub_complex( MMD_Polynomial *a, MMD_Polynomial *b )
{
	MMD_Polynomial *ret;
	if( !a->isNormal() ) a->Normalize();
	if( !b->isNormal() ) b->Normalize();
	if( a->getMaxPow() < b->getMaxPow() )
	{
		long i;
		ret = b->getSmbTable().makeUniWord( new MMD_Polynomial( *b ) );
		for( i = 0; i <= a->getMaxPow(); i++ )
			_dsrop_Sub_complex( ret->getCoef( i ), a->getCoef( i ), ret->getCoef( i ) );
		for( ; i <= b->getMaxPow(); i++ )
			_dsrop_Minus_complex( ret->getCoef( i ), ret->getCoef( i ) );
	}
	else
	{
		ret = a->getSmbTable().makeUniWord( new MMD_Polynomial( *a ) );
		for( long i = 0; i <= b->getMaxPow(); i++ )
			_dsrop_Sub_complex( ret->getCoef( i ), ret->getCoef( i ), b->getCoef( i ) );
	}
	return ret;
}

///////////////////////////////////////////////////////////////////////////////

MMD_Polynomial* _dsrop_PolynomialMinus_integer( MMD_Polynomial *a )
{
	if( !a->isNormal() ) a->Normalize();
	MMD_Polynomial *ret = a->getSmbTable().makeUniWord( new MMD_Polynomial( *a ) );
	for( long i = 0; i < ret->getTermCount(); i++ )
		_dsrop_Minus_integer( ret->getCoef( i ), ret->getCoef( i ) );
	return ret;
}

MMD_Polynomial* _dsrop_PolynomialMinus_real( MMD_Polynomial *a )
{
	if( !a->isNormal() ) a->Normalize();
	MMD_Polynomial *ret = a->getSmbTable().makeUniWord( new MMD_Polynomial( *a ) );
	for( long i = 0; i < ret->getTermCount(); i++ )
		_dsrop_Minus_real( ret->getCoef( i ), ret->getCoef( i ) );
	return ret;
}

MMD_Polynomial* _dsrop_PolynomialMinus_complex( MMD_Polynomial *a )
{
	if( !a->isNormal() ) a->Normalize();
	MMD_Polynomial *ret = a->getSmbTable().makeUniWord( new MMD_Polynomial( *a ) );
	for( long i = 0; i < ret->getTermCount(); i++ )
		_dsrop_Minus_complex( ret->getCoef( i ), ret->getCoef( i ) );
	return ret;
}

///////////////////////////////////////////////////////////////////////////////
// MUL
///////////////////////////////////////////////////////////////////////////////
MMD_Polynomial* _dsrop_PolynomialMul_integer( MMD_Polynomial *a, MMD_Polynomial *b )
{
	if( !a->isNormal() ) a->Normalize();
	if( !b->isNormal() ) b->Normalize();

	long i, j;
	long _sizeFst = a->getMaxPow() + 1;
	long _sizeSec = b->getMaxPow() + 1;

	MMD_Polynomial *ret = a->getSmbTable().makeUniWord( 
		new MMD_Polynomial( a->getSmbTable(), a->getType(), _sizeFst + _sizeSec - 1 ) );

	for ( i = 0; i < _sizeFst + _sizeSec - 1 ; i++ )
	{
		ret->getPow( i ) = CDSRInteger( i );

		UniWord rlSum = CDSRInteger( 0 ), rlMul = CDSRInteger( 0 );
		for ( j = 0; j <= i; j++ )
			if( i - j >= 0 && i - j < _sizeFst && j < _sizeSec )
			{
				_dsrop_Mul_integer( rlMul, a->getCoef( i - j ), b->getCoef( j ) );
				_dsrop_Add_integer( rlSum, rlSum, rlMul );
			}

		ret->getCoef( i ) = rlSum;
	}
	return ret;
}

MMD_Polynomial* _dsrop_PolynomialMul_real( MMD_Polynomial *a, MMD_Polynomial *b )
{
	if( !a->isNormal() ) a->Normalize();
	if( !b->isNormal() ) b->Normalize();

	long i, j;
	long _sizeFst = a->getMaxPow() + 1;
	long _sizeSec = b->getMaxPow() + 1;

	MMD_Polynomial *ret = a->getSmbTable().makeUniWord( 
		new MMD_Polynomial( a->getSmbTable(), a->getType(), _sizeFst + _sizeSec - 1 ) );

	for ( i = 0; i < _sizeFst + _sizeSec - 1 ; i++ )
	{
		ret->getPow( i ) = CDSRInteger( i );

		UniWord rlSum = CDSRReal( 0.0 ), rlMul = CDSRReal( 0.0 );
		for ( j = 0; j <= i; j++ )
			if( i - j >= 0 && i - j < _sizeFst && j < _sizeSec )
			{
				_dsrop_Mul_real( rlMul, a->getCoef( i - j ), b->getCoef( j ) );
				_dsrop_Add_real( rlSum, rlSum, rlMul );
			}

		ret->getCoef( i ) = rlSum;
	}
	return ret;
}

MMD_Polynomial* _dsrop_PolynomialMul_complex( MMD_Polynomial *a, MMD_Polynomial *b )
{
	if( !a->isNormal() ) a->Normalize();
	if( !b->isNormal() ) b->Normalize();

	long i, j;
	long _sizeFst = a->getMaxPow() + 1;
	long _sizeSec = b->getMaxPow() + 1;

	MMD_Polynomial *ret = a->getSmbTable().makeUniWord( 
		new MMD_Polynomial( a->getSmbTable(), a->getType(), _sizeFst + _sizeSec - 1 ) );

	for ( i = 0; i < _sizeFst + _sizeSec - 1 ; i++ )
	{
		ret->getPow( i ) = CDSRInteger( i );

		UniWord rlSum = CDSRComplex( 0.0, 0.0 ), rlMul = CDSRComplex( 0.0, 0.0 );
		for ( j = 0; j <= i; j++ )
			if( i - j >= 0 && i - j < _sizeFst && j < _sizeSec )
			{
				_dsrop_Mul_complex( rlMul, a->getCoef( i - j ), b->getCoef( j ) );
				_dsrop_Add_complex( rlSum, rlSum, rlMul );
			}

		ret->getCoef( i ) = rlSum;
	}
	return ret;
}

///////////////////////////////////////////////////////////////////////////////
// DIV
///////////////////////////////////////////////////////////////////////////////
MMD_RationalFun* _dsrop_PolynomialDiv_anytype( MMD_Polynomial *a, MMD_Polynomial *b )
{
	if( !a->isNormal() ) a->Normalize();
	if( !b->isNormal() ) b->Normalize();
	MMD_RationalFun *ret = (MMD_RationalFun*)a->getSmbTable().makeUniWord( 
		new MMD_RationalFun( a->getSmbTable(), a->getType() )
	);
	ret->hi = a->clone();
	ret->lo = b->clone();
	return ret;
}

MMD_RationalFun* _dsrop_PolynomialDiv_integer( MMD_Polynomial *a, MMD_Polynomial *b )
{
	return _dsrop_PolynomialDiv_anytype( a, b );
}
MMD_RationalFun* _dsrop_PolynomialDiv_real( MMD_Polynomial *a, MMD_Polynomial *b )
{
	return _dsrop_PolynomialDiv_anytype( a, b );
}
MMD_RationalFun* _dsrop_PolynomialDiv_complex( MMD_Polynomial *a, MMD_Polynomial *b )
{
	return _dsrop_PolynomialDiv_anytype( a, b );
}

///////////////////////////////////////////////////////////////////////////////

MMD_Polynomial* _dsrop_PolynomialDiv_integer( MMD_Polynomial *a, UniWord& b )
{
	if( !a->isNormal() ) a->Normalize();
	MMD_Polynomial *ret = a->getSmbTable().makeUniWord( 
		new MMD_Polynomial( a->getSmbTable(), DSRDATA_TYPE_REAL, a->getTermCount() ) );
	UniWord uw;
	for( long i = 0; i < ret->getTermCount(); i++ )
	{
		ret->getPow( i ) = a->getPow( i );
		_dsrop_Div_integer( uw, a->getCoef( i ), b );
		_dsrop_Convert_rational2real( ret->getCoef( i ), uw );
	}
	QString strName( a->getVarName() );
	ret->setVarName( strName );
	ret->setNormal();
	ret->setMaxPow();
	return ret;
}

MMD_Polynomial* _dsrop_PolynomialDiv_real( MMD_Polynomial *a, UniWord& b )
{
	if( !a->isNormal() ) a->Normalize();
	MMD_Polynomial *ret = a->getSmbTable().makeUniWord( new MMD_Polynomial( *a ) );
	for( long i = 0; i < ret->getTermCount(); i++ )
		_dsrop_Div_real( ret->getCoef( i ), ret->getCoef( i ), b );
	return ret;
}

MMD_Polynomial* _dsrop_PolynomialDiv_complex( MMD_Polynomial *a, UniWord& b )
{
	if( !a->isNormal() ) a->Normalize();
	MMD_Polynomial *ret = a->getSmbTable().makeUniWord( new MMD_Polynomial( *a ) );
	for( long i = 0; i < ret->getTermCount(); i++ )
		_dsrop_Div_complex( ret->getCoef( i ), ret->getCoef( i ), b );
	return ret;
}

///////////////////////////////////////////////////////////////////////////////
// POW
///////////////////////////////////////////////////////////////////////////////
MMD_Polynomial* _dsrop_PolynomialPow_anytype( MMD_Polynomial * /*a*/, UniWord& /*b*/ )
{
	return 0;
}

MMD_Polynomial* _dsrop_PolynomialPow_integer( MMD_Polynomial *a, UniWord& b )
{
	if( b.getInteger() < 0 )
		throw "Constant positive power expected in the operation with polynom";

	if( !a->isNormal() ) a->Normalize();
	if( b.getInteger() == 0 )
		return a->getSmbTable().makeUniWord( new MMD_Polynomial( a->getSmbTable(), CDSRInteger( 1 ) ) );

	MMD_Polynomial *ret = a->getSmbTable().makeUniWord( new MMD_Polynomial( *a ) );
	for( long i = 1; i < b.getInteger(); i++ )
		ret = _dsrop_PolynomialMul_integer( ret, a );
	ret->Normalize();
	return ret;
}
MMD_Polynomial* _dsrop_PolynomialPow_real( MMD_Polynomial *a, UniWord& b )
{
	if( b.getInteger() < 0 )
		throw "Constant positive power expected in the operation with polynom";

	if( !a->isNormal() ) a->Normalize();
	if( b.getInteger() == 0 )
		return a->getSmbTable().makeUniWord( new MMD_Polynomial( a->getSmbTable(), CDSRReal( 1 ) ) );

	MMD_Polynomial *ret = a->getSmbTable().makeUniWord( new MMD_Polynomial( *a ) );
	for( long i = 1; i < b.getInteger(); i++ )
		ret = _dsrop_PolynomialMul_real( ret, a );
	ret->Normalize();
	return ret; 
}
MMD_Polynomial* _dsrop_PolynomialPow_complex( MMD_Polynomial *a, UniWord& b )
{
	if( b.getInteger() < 0 )
		throw "Constant positive power expected in the operation with polynom";

	if( !a->isNormal() ) a->Normalize();
	if( b.getInteger() == 0 )
		return a->getSmbTable().makeUniWord( new MMD_Polynomial( a->getSmbTable(), CDSRComplex( 1 ) ) );

	MMD_Polynomial *ret = a->getSmbTable().makeUniWord( new MMD_Polynomial( *a ) );
	for( long i = 1; i < b.getInteger(); i++ )
		ret = _dsrop_PolynomialMul_complex( ret, a );
	ret->Normalize();
	return ret;
}

///////////////////////////////////////////////////////////////////////////////
// (NOT) EQUAL
///////////////////////////////////////////////////////////////////////////////
UniWord _dsrop_PolynomialEqual_anytype( MMD_Polynomial *a, MMD_Polynomial *b )
{
	if( a->getType() != b->getType() )
		return UniWord( CDSRInteger( 0 ) );
	switch( a->getType() )
	{
	case DSRDATA_TYPE_INTEGER:
		return _dsrop_PolynomialEqual_integer( a, b );
	case DSRDATA_TYPE_REAL:
		return _dsrop_PolynomialEqual_real( a, b );
	case DSRDATA_TYPE_COMPLEX:
		return _dsrop_PolynomialEqual_complex( a, b );
	default:
		break;
	}
	return UniWord( CDSRInteger( 0 ) );
}

UniWord _dsrop_PolynomialEqual_integer( MMD_Polynomial *a, MMD_Polynomial *b )
{
	UniWord ret( CDSRInteger( 0 ) );
	if( !a->isNormal() ) a->Normalize();
	if( !b->isNormal() ) b->Normalize();
	if( a->getMaxPow() != b->getMaxPow() ) return ret;

	for( long i = 0; i < a->getTermCount(); i++ )
		if( !_dsrop_Equal_integer( ret, a->getCoef( i ), b->getCoef( i ) ).getInteger() )
			return ret;
	return ret;
}

UniWord _dsrop_PolynomialEqual_real( MMD_Polynomial *a, MMD_Polynomial *b )
{
	UniWord ret( CDSRInteger( 0 ) );
	if( !a->isNormal() ) a->Normalize();
	if( !b->isNormal() ) b->Normalize();
	if( a->getMaxPow() != b->getMaxPow() ) return ret;

	for( long i = 0; i < a->getTermCount(); i++ )
		if( !_dsrop_Equal_real( ret, a->getCoef( i ), b->getCoef( i ) ).getInteger() )
			return ret;
	return ret;
}

UniWord _dsrop_PolynomialEqual_complex( MMD_Polynomial *a, MMD_Polynomial *b )
{
	UniWord ret( CDSRInteger( 0 ) );
	if( !a->isNormal() ) a->Normalize();
	if( !b->isNormal() ) b->Normalize();
	if( a->getMaxPow() != b->getMaxPow() ) return ret;

	for( long i = 0; i < a->getTermCount(); i++ )
		if( !_dsrop_Equal_complex( ret, a->getCoef( i ), b->getCoef( i ) ).getInteger() )
			return ret;
	return ret;
}

///////////////////////////////////////////////////////////////////////////////

UniWord _dsrop_PolynomialNotEqual_integer( MMD_Polynomial *a, MMD_Polynomial *b )
{
	UniWord ret( CDSRInteger( 1 ) );
	if( !a->isNormal() ) a->Normalize();
	if( !b->isNormal() ) b->Normalize();
	if( a->getMaxPow() != b->getMaxPow() ) return ret;

	for( long i = 0; i < a->getTermCount(); i++ )
		if( _dsrop_NotEqual_integer( ret, a->getCoef( i ), b->getCoef( i ) ).getInteger() )
			return ret;
	return ret;
}

UniWord _dsrop_PolynomialNotEqual_real( MMD_Polynomial *a, MMD_Polynomial *b )
{
	UniWord ret( CDSRInteger( 1 ) );
	if( !a->isNormal() ) a->Normalize();
	if( !b->isNormal() ) b->Normalize();
	if( a->getMaxPow() != b->getMaxPow() ) return ret;

	for( long i = 0; i < a->getTermCount(); i++ )
		if( _dsrop_NotEqual_real( ret, a->getCoef( i ), b->getCoef( i ) ).getInteger() )
			return ret;
	return ret;
}

UniWord _dsrop_PolynomialNotEqual_complex( MMD_Polynomial *a, MMD_Polynomial *b )
{
	UniWord ret( CDSRInteger( 1 ) );
	if( !a->isNormal() ) a->Normalize();
	if( !b->isNormal() ) b->Normalize();
	if( a->getMaxPow() != b->getMaxPow() ) return ret;

	for( long i = 0; i < a->getTermCount(); i++ )
		if( _dsrop_NotEqual_complex( ret, a->getCoef( i ), b->getCoef( i ) ).getInteger() )
			return ret;
	return ret;
}

///////////////////////////////////////////////////////////////////////////////
// ASSIGN
///////////////////////////////////////////////////////////////////////////////
MMD_Polynomial* _dsrop_PolynomialAssign_anytype( MMD_Polynomial *a, MMD_Polynomial *b )
{
	a = b->clone();
	return a->clone();
}

MMD_Polynomial* _dsrop_PolynomialAssign_integer( MMD_Polynomial *a, MMD_Polynomial *b )
{
	return _dsrop_PolynomialAssign_anytype( a, b );
}
MMD_Polynomial* _dsrop_PolynomialAssign_real( MMD_Polynomial *a, MMD_Polynomial *b )
{
	return _dsrop_PolynomialAssign_anytype( a, b );
}
MMD_Polynomial* _dsrop_PolynomialAssign_complex( MMD_Polynomial *a, MMD_Polynomial *b )
{
	return _dsrop_PolynomialAssign_anytype( a, b );
}

///////////////////////////////////////////////////////////////////////////////
// CONVERT
///////////////////////////////////////////////////////////////////////////////
MMD_Polynomial* _dsrop_Convert_natural2Polynomial_integer( UniWord& a, CParseDsrSymbolTable& _smbtable )
{
	UniWord b;
	_dsrop_Convert_natural2integer( b, a );
	return _smbtable.makeUniWord( new MMD_Polynomial( _smbtable, CDSRInteger( b.getInteger() ) ) );
}

MMD_Polynomial* _dsrop_Convert_natural2Polynomial_real( UniWord& a, CParseDsrSymbolTable& _smbtable )
{
	UniWord b;
	_dsrop_Convert_natural2real( b, a );
	return _smbtable.makeUniWord( new MMD_Polynomial( _smbtable, b.getReal() ) );
}

MMD_Polynomial* _dsrop_Convert_natural2Polynomial_complex( UniWord& a, CParseDsrSymbolTable& _smbtable )
{
	UniWord b;
	_dsrop_Convert_natural2complex( b, a );
	return _smbtable.makeUniWord( new MMD_Polynomial( _smbtable, b.getComplex() ) );
}

MMD_Polynomial* _dsrop_Convert_integer2Polynomial_integer( UniWord& a, CParseDsrSymbolTable& _smbtable )
{
	return _smbtable.makeUniWord( new MMD_Polynomial( _smbtable, a.getInteger() ) );
}

MMD_Polynomial* _dsrop_Convert_integer2Polynomial_real( UniWord& a, CParseDsrSymbolTable& _smbtable )
{
	UniWord b;
	_dsrop_Convert_integer2real( b, a );
	return _smbtable.makeUniWord( new MMD_Polynomial( _smbtable, b.getReal() ) );
}

MMD_Polynomial* _dsrop_Convert_integer2Polynomial_complex( UniWord& a, CParseDsrSymbolTable& _smbtable )
{
	UniWord b;
	_dsrop_Convert_integer2complex( b, a );
	return _smbtable.makeUniWord( new MMD_Polynomial( _smbtable, b.getComplex() ) );
}

MMD_Polynomial* _dsrop_Convert_rational2Polynomial_real( UniWord& a, CParseDsrSymbolTable& _smbtable )
{
	UniWord b;
	_dsrop_Convert_rational2real( b, a );
	return _smbtable.makeUniWord( new MMD_Polynomial( _smbtable, b.getReal() ) );
}

MMD_Polynomial* _dsrop_Convert_rational2Polynomial_complex( UniWord& a, CParseDsrSymbolTable& _smbtable )
{
	UniWord b;
	_dsrop_Convert_rational2complex( b, a );
	return _smbtable.makeUniWord( new MMD_Polynomial( _smbtable, b.getComplex() ) );
}

MMD_Polynomial* _dsrop_Convert_real2Polynomial_real( UniWord& a, CParseDsrSymbolTable& _smbtable )
{
	return _smbtable.makeUniWord( new MMD_Polynomial( _smbtable, a.getReal() ) );
}

MMD_Polynomial* _dsrop_Convert_real2Polynomial_complex( UniWord& a, CParseDsrSymbolTable& _smbtable )
{
	UniWord b;
	_dsrop_Convert_real2complex( b, a );
	return _smbtable.makeUniWord( new MMD_Polynomial( _smbtable, b.getComplex() ) );
}

MMD_Polynomial* _dsrop_Convert_complex2Polynomial_complex( UniWord& a, CParseDsrSymbolTable& _smbtable )
{
	return _smbtable.makeUniWord( new MMD_Polynomial( _smbtable, a.getComplex() ) );
}

///////////////////////////////////////////////////////////////////////////////

MMD_Polynomial* _dsrop_Convert_Polynomial_integer2Polynomial_real( MMD_Polynomial* a )
{
	if( !a->isNormal() ) a->Normalize();
	MMD_Polynomial *ret = a->getSmbTable().makeUniWord( 
		new MMD_Polynomial( a->getSmbTable(), DSRDATA_TYPE_REAL, a->getTermCount() ) );
	for( long i = 0; i < ret->getTermCount(); i++ )
	{
		ret->getPow( i ) = a->getPow( i );
		_dsrop_Convert_integer2real( ret->getCoef( i ), a->getCoef( i ) );
	}
	QString strName( a->getVarName() );
	ret->setVarName( strName );
	ret->setNormal();
	ret->setMaxPow();
	return ret;
}

MMD_Polynomial* _dsrop_Convert_Polynomial_integer2Polynomial_complex( MMD_Polynomial* a )
{
	if( !a->isNormal() ) a->Normalize();
	MMD_Polynomial *ret = a->getSmbTable().makeUniWord( 
		new MMD_Polynomial( a->getSmbTable(), DSRDATA_TYPE_COMPLEX, a->getTermCount() ) );
	for( long i = 0; i < ret->getTermCount(); i++ )
	{
		ret->getPow( i ) = a->getPow( i );
		_dsrop_Convert_integer2complex( ret->getCoef( i ), a->getCoef( i ) );
	}
	QString strName( a->getVarName() );
	ret->setVarName( strName );
	ret->setNormal();
	ret->setMaxPow();
	return ret;
}

MMD_Polynomial* _dsrop_Convert_Polynomial_real2Polynomial_complex( MMD_Polynomial* a )
{
	if( !a->isNormal() ) a->Normalize();
	MMD_Polynomial *ret = a->getSmbTable().makeUniWord( 
		new MMD_Polynomial( a->getSmbTable(), DSRDATA_TYPE_COMPLEX, a->getTermCount() ) );
	for( long i = 0; i < ret->getTermCount(); i++ )
	{
		ret->getPow( i ) = a->getPow( i );
		_dsrop_Convert_real2complex( ret->getCoef( i ), a->getCoef( i ) );
	}
	QString strName( a->getVarName() );
	ret->setVarName( strName );
	ret->setNormal();
	ret->setMaxPow();
	return ret;
}

///////////////////////////////////////////////////////////////////////////////

MMD_RationalFun* _dsrop_Convert_Polynomial_integer2RationalFun_integer( MMD_Polynomial* a )
{
	if( !a->isNormal() ) a->Normalize();
	MMD_RationalFun *ret = a->getSmbTable().makeUniWord( new MMD_RationalFun( a->getSmbTable(), a->getType() ) );
	ret->hi = a->clone();
	ret->lo = a->getSmbTable().makeUniWord( new MMD_Polynomial( a->getSmbTable(), CDSRInteger( 1 ) ) );
	return ret;
}

MMD_RationalFun* _dsrop_Convert_Polynomial_integer2RationalFun_real( MMD_Polynomial* a )
{
	if( !a->isNormal() ) a->Normalize();
	MMD_RationalFun *ret = a->getSmbTable().makeUniWord( 
		new MMD_RationalFun( a->getSmbTable(), DSRDATA_TYPE_REAL ) );
	ret->hi = _dsrop_Convert_Polynomial_integer2Polynomial_real( a );
	ret->lo = a->getSmbTable().makeUniWord( 
		new MMD_Polynomial( a->getSmbTable(), CDSRReal( 1.0 ) ) );
	return ret;
}

MMD_RationalFun* _dsrop_Convert_Polynomial_integer2RationalFun_complex( MMD_Polynomial* a )
{
	if( !a->isNormal() ) a->Normalize();
	MMD_RationalFun *ret = a->getSmbTable().makeUniWord( 
		new MMD_RationalFun( a->getSmbTable(), DSRDATA_TYPE_COMPLEX ) );
	ret->hi = _dsrop_Convert_Polynomial_integer2Polynomial_complex( a );
	ret->lo = a->getSmbTable().makeUniWord( 
		new MMD_Polynomial( a->getSmbTable(), CDSRComplex( 1.0, 0.0 ) ) );
	return ret;
}

MMD_RationalFun* _dsrop_Convert_Polynomial_real2RationalFun_real( MMD_Polynomial* a )
{
	if( !a->isNormal() ) a->Normalize();
	MMD_RationalFun *ret = a->getSmbTable().makeUniWord( new MMD_RationalFun( a->getSmbTable(), a->getType() ) );
	ret->hi = a->clone();
	ret->lo = a->getSmbTable().makeUniWord( new MMD_Polynomial( a->getSmbTable(), CDSRReal( 1.0 ) ) );
	return ret;
}

MMD_RationalFun* _dsrop_Convert_Polynomial_real2RationalFun_complex( MMD_Polynomial* a )
{
	if( !a->isNormal() ) a->Normalize();
	MMD_RationalFun *ret = a->getSmbTable().makeUniWord( 
		new MMD_RationalFun( a->getSmbTable(), DSRDATA_TYPE_COMPLEX ) );
	ret->hi = _dsrop_Convert_Polynomial_real2Polynomial_complex( a );
	ret->lo = a->getSmbTable().makeUniWord( 
		new MMD_Polynomial( a->getSmbTable(), CDSRComplex( 1.0, 0.0 ) ) );
	return ret;
}

MMD_RationalFun* _dsrop_Convert_Polynomial_complex2RationalFun_complex( MMD_Polynomial* a )
{
	if( !a->isNormal() ) a->Normalize();
	MMD_RationalFun *ret = a->getSmbTable().makeUniWord( new MMD_RationalFun( a->getSmbTable(), a->getType() ) );
	ret->hi = a->clone();
	ret->lo = a->getSmbTable().makeUniWord( new MMD_Polynomial( a->getSmbTable(), CDSRComplex( 1.0, 0.0 ) ) );
	return ret;
}

///////////////////////////////////////////////////////////////////////////////
