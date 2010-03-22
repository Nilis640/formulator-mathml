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

#include "mmd_ratf_op.h"

///////////////////////////////////////////////////////////////////////////////
// ADD
///////////////////////////////////////////////////////////////////////////////
MMD_RationalFun* _dsrop_RationalFunAdd_integer( MMD_RationalFun *a, MMD_RationalFun *b )
{
	MMD_Polynomial *ahi_blo, *bhi_alo, *hi, *lo;

	ahi_blo = _dsrop_PolynomialMul_integer( a->hi, b->lo );
	bhi_alo = _dsrop_PolynomialMul_integer( a->lo, b->hi );
	hi = _dsrop_PolynomialAdd_integer( ahi_blo, bhi_alo );
	lo = _dsrop_PolynomialMul_integer( a->lo, b->lo );

	MMD_RationalFun *ret = a->getSmbTable().makeUniWord( 
		new MMD_RationalFun( a->getSmbTable(), hi, lo )
	);
	ret->Normalize();
	return ret;
}

MMD_RationalFun* _dsrop_RationalFunAdd_real( MMD_RationalFun *a, MMD_RationalFun *b )
{
	MMD_Polynomial *ahi_blo, *bhi_alo, *hi, *lo;

	ahi_blo = _dsrop_PolynomialMul_real( a->hi, b->lo );
	bhi_alo = _dsrop_PolynomialMul_real( a->lo, b->hi );
	hi = _dsrop_PolynomialAdd_real( ahi_blo, bhi_alo );
	lo = _dsrop_PolynomialMul_real( a->lo, b->lo );

	MMD_RationalFun *ret = a->getSmbTable().makeUniWord( 
		new MMD_RationalFun( a->getSmbTable(), hi, lo )
	);
	ret->Normalize();
	return ret;
}

MMD_RationalFun* _dsrop_RationalFunAdd_complex( MMD_RationalFun *a, MMD_RationalFun *b )
{
	MMD_Polynomial *ahi_blo, *bhi_alo, *hi, *lo;

	ahi_blo = _dsrop_PolynomialMul_complex( a->hi, b->lo );
	bhi_alo = _dsrop_PolynomialMul_complex( a->lo, b->hi );
	hi = _dsrop_PolynomialAdd_complex( ahi_blo, bhi_alo );
	lo = _dsrop_PolynomialMul_complex( a->lo, b->lo );

	MMD_RationalFun *ret = a->getSmbTable().makeUniWord( 
		new MMD_RationalFun( a->getSmbTable(), hi, lo )
	);
	ret->Normalize();
	return ret;
}

///////////////////////////////////////////////////////////////////////////////

MMD_RationalFun* _dsrop_RationalFunPlus_integer( MMD_RationalFun *a )
{
	return a->getSmbTable().makeUniWord( 
		new MMD_RationalFun( a->getSmbTable(),
							_dsrop_PolynomialPlus_integer( a->hi ),
							a->lo->clone() )
	 );
}

MMD_RationalFun* _dsrop_RationalFunPlus_real( MMD_RationalFun *a )
{
	return a->getSmbTable().makeUniWord( 
		new MMD_RationalFun( a->getSmbTable(),
							_dsrop_PolynomialPlus_real( a->hi ),
							a->lo->clone() )
	 );
}

MMD_RationalFun* _dsrop_RationalFunPlus_complex( MMD_RationalFun *a )
{
	return a->getSmbTable().makeUniWord( 
		new MMD_RationalFun( a->getSmbTable(),
							_dsrop_PolynomialPlus_complex( a->hi ),
							a->lo->clone() )
	 );
}

///////////////////////////////////////////////////////////////////////////////
// SUB
///////////////////////////////////////////////////////////////////////////////
MMD_RationalFun* _dsrop_RationalFunSub_integer( MMD_RationalFun *a, MMD_RationalFun *b )
{
	MMD_Polynomial *ahi_blo, *bhi_alo, *hi, *lo;

	ahi_blo = _dsrop_PolynomialMul_integer( a->hi, b->lo );
	bhi_alo = _dsrop_PolynomialMul_integer( a->lo, b->hi );
	hi = _dsrop_PolynomialSub_integer( ahi_blo, bhi_alo );
	lo = _dsrop_PolynomialMul_integer( a->lo, b->lo );

	MMD_RationalFun *ret = a->getSmbTable().makeUniWord( 
		new MMD_RationalFun( a->getSmbTable(), hi, lo )
	);
	ret->Normalize();
	return ret;
}

MMD_RationalFun* _dsrop_RationalFunSub_real( MMD_RationalFun *a, MMD_RationalFun *b )
{
	MMD_Polynomial *ahi_blo, *bhi_alo, *hi, *lo;

	ahi_blo = _dsrop_PolynomialMul_real( a->hi, b->lo );
	bhi_alo = _dsrop_PolynomialMul_real( a->lo, b->hi );
	hi = _dsrop_PolynomialSub_real( ahi_blo, bhi_alo );
	lo = _dsrop_PolynomialMul_real( a->lo, b->lo );

	MMD_RationalFun *ret = a->getSmbTable().makeUniWord( 
		new MMD_RationalFun( a->getSmbTable(), hi, lo )
	);
	ret->Normalize();
	return ret;
}

MMD_RationalFun* _dsrop_RationalFunSub_complex( MMD_RationalFun *a, MMD_RationalFun *b )
{
	MMD_Polynomial *ahi_blo, *bhi_alo, *hi, *lo;

	ahi_blo = _dsrop_PolynomialMul_complex( a->hi, b->lo );
	bhi_alo = _dsrop_PolynomialMul_complex( a->lo, b->hi );
	hi = _dsrop_PolynomialSub_complex( ahi_blo, bhi_alo );
	lo = _dsrop_PolynomialMul_complex( a->lo, b->lo );

	MMD_RationalFun *ret = a->getSmbTable().makeUniWord( 
		new MMD_RationalFun( a->getSmbTable(), hi, lo )
	);
	ret->Normalize();
	return ret;
}

///////////////////////////////////////////////////////////////////////////////

MMD_RationalFun* _dsrop_RationalFunMinus_integer( MMD_RationalFun *a )
{
	return a->getSmbTable().makeUniWord( 
		new MMD_RationalFun( a->getSmbTable(),
							_dsrop_PolynomialMinus_integer( a->hi ),
							a->lo->clone() )
	 );
}

MMD_RationalFun* _dsrop_RationalFunMinus_real( MMD_RationalFun *a )
{
	return a->getSmbTable().makeUniWord( 
		new MMD_RationalFun( a->getSmbTable(),
							_dsrop_PolynomialMinus_real( a->hi ),
							a->lo->clone() )
	 );
}

MMD_RationalFun* _dsrop_RationalFunMinus_complex( MMD_RationalFun *a )
{
	return a->getSmbTable().makeUniWord( 
		new MMD_RationalFun( a->getSmbTable(),
							_dsrop_PolynomialMinus_complex( a->hi ),
							a->lo->clone() )
	 );
}

///////////////////////////////////////////////////////////////////////////////
// MUL
///////////////////////////////////////////////////////////////////////////////
MMD_RationalFun* _dsrop_RationalFunMul_integer( MMD_RationalFun *a, MMD_RationalFun *b )
{
	MMD_Polynomial *hi, *lo;

	hi = _dsrop_PolynomialMul_integer( a->hi, b->hi );
	lo = _dsrop_PolynomialMul_integer( a->lo, b->lo );

	MMD_RationalFun *ret = a->getSmbTable().makeUniWord( 
		new MMD_RationalFun( a->getSmbTable(), hi, lo )
	);
	ret->Normalize();
	return ret;
}

MMD_RationalFun* _dsrop_RationalFunMul_real( MMD_RationalFun *a, MMD_RationalFun *b )
{
	MMD_Polynomial *hi, *lo;

	hi = _dsrop_PolynomialMul_real( a->hi, b->hi );
	lo = _dsrop_PolynomialMul_real( a->lo, b->lo );

	MMD_RationalFun *ret = a->getSmbTable().makeUniWord( 
		new MMD_RationalFun( a->getSmbTable(), hi, lo )
	);
	ret->Normalize();
	return ret;
}

MMD_RationalFun* _dsrop_RationalFunMul_complex( MMD_RationalFun *a, MMD_RationalFun *b )
{
	MMD_Polynomial *hi, *lo;

	hi = _dsrop_PolynomialMul_complex( a->hi, b->hi );
	lo = _dsrop_PolynomialMul_complex( a->lo, b->lo );

	MMD_RationalFun *ret = a->getSmbTable().makeUniWord( 
		new MMD_RationalFun( a->getSmbTable(), hi, lo )
	);
	ret->Normalize();
	return ret;
}

///////////////////////////////////////////////////////////////////////////////
// DIV
///////////////////////////////////////////////////////////////////////////////
MMD_RationalFun* _dsrop_RationalFunDiv_integer( MMD_RationalFun *a, MMD_RationalFun *b )
{
	MMD_Polynomial *hi, *lo;

	hi = _dsrop_PolynomialMul_integer( a->hi, b->lo );
	lo = _dsrop_PolynomialMul_integer( a->lo, b->hi );

	MMD_RationalFun *ret = a->getSmbTable().makeUniWord( 
		new MMD_RationalFun( a->getSmbTable(), hi, lo )
	);
	ret->Normalize();
	return ret;
}

MMD_RationalFun* _dsrop_RationalFunDiv_real( MMD_RationalFun *a, MMD_RationalFun *b )
{
	MMD_Polynomial *hi, *lo;

	hi = _dsrop_PolynomialMul_real( a->hi, b->lo );
	lo = _dsrop_PolynomialMul_real( a->lo, b->hi );

	MMD_RationalFun *ret = a->getSmbTable().makeUniWord( 
		new MMD_RationalFun( a->getSmbTable(), hi, lo )
	);
	ret->Normalize();
	return ret;
}
MMD_RationalFun* _dsrop_RationalFunDiv_complex( MMD_RationalFun *a, MMD_RationalFun *b )
{
	MMD_Polynomial *hi, *lo;

	hi = _dsrop_PolynomialMul_complex( a->hi, b->lo );
	lo = _dsrop_PolynomialMul_complex( a->lo, b->hi );

	MMD_RationalFun *ret = a->getSmbTable().makeUniWord( 
		new MMD_RationalFun( a->getSmbTable(), hi, lo )
	);
	ret->Normalize();
	return ret;
}

///////////////////////////////////////////////////////////////////////////////
// POW
///////////////////////////////////////////////////////////////////////////////
MMD_RationalFun* _dsrop_RationalFunPow_integer( MMD_RationalFun *a, UniWord& b )
{
	if( b.getInteger() < 0 )
	{
		UniWord tmp;
		_dsrop_Minus_integer( tmp, b );
		return a->getSmbTable().makeUniWord( 
			new MMD_RationalFun( a->getSmbTable(),
							_dsrop_PolynomialPow_integer( a->lo, tmp ),
							_dsrop_PolynomialPow_integer( a->hi, tmp ) ) );
	}
	else
		return a->getSmbTable().makeUniWord( 
			new MMD_RationalFun( a->getSmbTable(),
							_dsrop_PolynomialPow_integer( a->hi, b ),
							_dsrop_PolynomialPow_integer( a->lo, b ) ) );
}

MMD_RationalFun* _dsrop_RationalFunPow_real( MMD_RationalFun *a, UniWord& b )
{
	if( b.getInteger() < 0 )
	{
		UniWord tmp;
		_dsrop_Minus_integer( tmp, b );
		return a->getSmbTable().makeUniWord( 
			new MMD_RationalFun( a->getSmbTable(),
							_dsrop_PolynomialPow_real( a->lo, tmp ),
							_dsrop_PolynomialPow_real( a->hi, tmp ) ) );
	}
	else
		return a->getSmbTable().makeUniWord( 
			new MMD_RationalFun( a->getSmbTable(),
							_dsrop_PolynomialPow_real( a->hi, b ),
							_dsrop_PolynomialPow_real( a->lo, b ) ) );
}

MMD_RationalFun* _dsrop_RationalFunPow_complex( MMD_RationalFun *a, UniWord& b )
{
	if( b.getInteger() < 0 )
	{
		UniWord tmp;
		_dsrop_Minus_integer( tmp, b );
		return a->getSmbTable().makeUniWord( 
			new MMD_RationalFun( a->getSmbTable(),
							_dsrop_PolynomialPow_complex( a->lo, tmp ),
							_dsrop_PolynomialPow_complex( a->hi, tmp ) ) );
	}
	else
		return a->getSmbTable().makeUniWord( 
			new MMD_RationalFun( a->getSmbTable(),
							_dsrop_PolynomialPow_complex( a->hi, b ),
							_dsrop_PolynomialPow_complex( a->lo, b ) ) );
}

///////////////////////////////////////////////////////////////////////////////
// (NOT) EQUAL
///////////////////////////////////////////////////////////////////////////////
UniWord _dsrop_RationalFunEqual_anytype( MMD_RationalFun *a, MMD_RationalFun *b )
{
	if( a->getType() != b->getType() )
		return UniWord( CDSRInteger( 0 ) );
	switch( a->getType() )
	{
	case DSRDATA_TYPE_INTEGER:
		return _dsrop_RationalFunEqual_integer( a, b );
	case DSRDATA_TYPE_REAL:
		return _dsrop_RationalFunEqual_real( a, b );
	case DSRDATA_TYPE_COMPLEX:
		return _dsrop_RationalFunEqual_complex( a, b );
	default:
#if _DEBUG
		throw _T("_dsrop_RationalFunEqual_anytype : internal error");
#else
		break;
#endif
	}
	return UniWord( CDSRInteger( 0 ) );
}

UniWord _dsrop_RationalFunEqual_integer( MMD_RationalFun *a, MMD_RationalFun *b )
{
	UniWord ret( CDSRInteger( 0 ) );
	return _dsrop_BoolAnd_bool( ret, 
		_dsrop_PolynomialEqual_integer( a->hi, b->hi ),
		_dsrop_PolynomialEqual_integer( a->lo, b->lo ) );
}

UniWord _dsrop_RationalFunEqual_real( MMD_RationalFun *a, MMD_RationalFun *b )
{
	UniWord ret( CDSRInteger( 0 ) );
	return _dsrop_BoolAnd_bool( ret, 
		_dsrop_PolynomialEqual_real( a->hi, b->hi ),
		_dsrop_PolynomialEqual_real( a->lo, b->lo ) );
}

UniWord _dsrop_RationalFunEqual_complex( MMD_RationalFun *a, MMD_RationalFun *b )
{
	UniWord ret( CDSRInteger( 0 ) );
	return _dsrop_BoolAnd_bool( ret, 
		_dsrop_PolynomialEqual_complex( a->hi, b->hi ),
		_dsrop_PolynomialEqual_complex( a->lo, b->lo ) );
}

///////////////////////////////////////////////////////////////////////////////

UniWord _dsrop_RationalFunNotEqual_integer( MMD_RationalFun *a, MMD_RationalFun *b )
{
	UniWord ret( CDSRInteger( 0 ) );
	return _dsrop_BoolOr_bool( ret, 
		_dsrop_PolynomialNotEqual_integer( a->hi, b->hi ),
		_dsrop_PolynomialNotEqual_integer( a->lo, b->lo ) );
}

UniWord _dsrop_RationalFunNotEqual_real( MMD_RationalFun *a, MMD_RationalFun *b )
{
	UniWord ret( CDSRInteger( 0 ) );
	return _dsrop_BoolOr_bool( ret, 
		_dsrop_PolynomialNotEqual_real( a->hi, b->hi ),
		_dsrop_PolynomialNotEqual_real( a->lo, b->lo ) );
}

UniWord _dsrop_RationalFunNotEqual_complex( MMD_RationalFun *a, MMD_RationalFun *b )
{
	UniWord ret( CDSRInteger( 0 ) );
	return _dsrop_BoolOr_bool( ret, 
		_dsrop_PolynomialNotEqual_complex( a->hi, b->hi ),
		_dsrop_PolynomialNotEqual_complex( a->lo, b->lo ) );
}

///////////////////////////////////////////////////////////////////////////////
// ASSIGN
///////////////////////////////////////////////////////////////////////////////
MMD_RationalFun* _dsrop_RationalFunAssign_anytype( MMD_RationalFun *a, MMD_RationalFun *b )
{
	a = b->clone();
	return a->clone();
}

MMD_RationalFun* _dsrop_RationalFunAssign_integer( MMD_RationalFun *a, MMD_RationalFun *b )
{
	return _dsrop_RationalFunAssign_anytype( a, b );
}
MMD_RationalFun* _dsrop_RationalFunAssign_real( MMD_RationalFun *a, MMD_RationalFun *b )
{
	return _dsrop_RationalFunAssign_anytype( a, b );
}
MMD_RationalFun* _dsrop_RationalFunAssign_complex( MMD_RationalFun *a, MMD_RationalFun *b )
{
	return _dsrop_RationalFunAssign_anytype( a, b );
}

///////////////////////////////////////////////////////////////////////////////
// CONVERT
///////////////////////////////////////////////////////////////////////////////
MMD_RationalFun* _dsrop_Convert_natural2RationalFun_integer( UniWord& a, CParseDsrSymbolTable& _smbtable )
{
	return _smbtable.makeUniWord( 
		new MMD_RationalFun( _smbtable, 
							_dsrop_Convert_natural2Polynomial_integer( a, _smbtable ),
							_smbtable.makeUniWord( new MMD_Polynomial( _smbtable, CDSRInteger( 1 ) ) ) )
	);
}

MMD_RationalFun* _dsrop_Convert_natural2RationalFun_real( UniWord& a, CParseDsrSymbolTable& _smbtable )
{
	return _smbtable.makeUniWord( 
		new MMD_RationalFun( _smbtable, 
							_dsrop_Convert_natural2Polynomial_real( a, _smbtable ),
							_smbtable.makeUniWord( new MMD_Polynomial( _smbtable, CDSRReal( 1.0 ) ) ) )
	);
}

MMD_RationalFun* _dsrop_Convert_natural2RationalFun_complex( UniWord& a, CParseDsrSymbolTable& _smbtable )
{
	return _smbtable.makeUniWord( 
		new MMD_RationalFun( _smbtable, 
							_dsrop_Convert_natural2Polynomial_complex( a, _smbtable ),
							_smbtable.makeUniWord( new MMD_Polynomial( _smbtable, CDSRComplex( 1.0 ) ) ) )
	);
}

MMD_RationalFun* _dsrop_Convert_integer2RationalFun_integer( UniWord& a, CParseDsrSymbolTable& _smbtable )
{
	return _smbtable.makeUniWord( 
		new MMD_RationalFun( _smbtable, 
							_dsrop_Convert_integer2Polynomial_integer( a, _smbtable ),
							_smbtable.makeUniWord( new MMD_Polynomial( _smbtable, CDSRInteger( 1 ) ) ) )
	);
}

MMD_RationalFun* _dsrop_Convert_integer2RationalFun_real( UniWord& a, CParseDsrSymbolTable& _smbtable )
{
	return _smbtable.makeUniWord( 
		new MMD_RationalFun( _smbtable, 
							_dsrop_Convert_integer2Polynomial_real( a, _smbtable ),
							_smbtable.makeUniWord( new MMD_Polynomial( _smbtable, CDSRReal( 1.0 ) ) ) )
	);
}

MMD_RationalFun* _dsrop_Convert_integer2RationalFun_complex( UniWord& a, CParseDsrSymbolTable& _smbtable )
{
	return _smbtable.makeUniWord( 
		new MMD_RationalFun( _smbtable, 
							_dsrop_Convert_integer2Polynomial_complex( a, _smbtable ),
							_smbtable.makeUniWord( new MMD_Polynomial( _smbtable, CDSRComplex( 1.0 ) ) ) )
	);
}

MMD_RationalFun* _dsrop_Convert_rational2RationalFun_real( UniWord& a, CParseDsrSymbolTable& _smbtable )
{
	return _smbtable.makeUniWord( 
		new MMD_RationalFun( _smbtable, 
							_dsrop_Convert_rational2Polynomial_real( a, _smbtable ),
							_smbtable.makeUniWord( new MMD_Polynomial( _smbtable, CDSRReal( 1.0 ) ) ) )
	);
}

MMD_RationalFun* _dsrop_Convert_rational2RationalFun_complex( UniWord& a, CParseDsrSymbolTable& _smbtable )
{
	return _smbtable.makeUniWord( 
		new MMD_RationalFun( _smbtable, 
							_dsrop_Convert_rational2Polynomial_complex( a, _smbtable ),
							_smbtable.makeUniWord( new MMD_Polynomial( _smbtable, CDSRComplex( 1.0 ) ) ) )
	);
}

MMD_RationalFun* _dsrop_Convert_real2RationalFun_real( UniWord& a, CParseDsrSymbolTable& _smbtable )
{
	return _smbtable.makeUniWord( 
		new MMD_RationalFun( _smbtable, 
							_dsrop_Convert_real2Polynomial_real( a, _smbtable ),
							_smbtable.makeUniWord( new MMD_Polynomial( _smbtable, CDSRReal( 1.0 ) ) ) )
	);
}

MMD_RationalFun* _dsrop_Convert_real2RationalFun_complex( UniWord& a, CParseDsrSymbolTable& _smbtable )
{
	return _smbtable.makeUniWord( 
		new MMD_RationalFun( _smbtable, 
							_dsrop_Convert_real2Polynomial_complex( a, _smbtable ),
							_smbtable.makeUniWord( new MMD_Polynomial( _smbtable, CDSRComplex( 1.0 ) ) ) )
	);
}

MMD_RationalFun* _dsrop_Convert_complex2RationalFun_complex( UniWord& a, CParseDsrSymbolTable& _smbtable )
{
	return _smbtable.makeUniWord( 
		new MMD_RationalFun( _smbtable, 
							_dsrop_Convert_complex2Polynomial_complex( a, _smbtable ),
							_smbtable.makeUniWord( new MMD_Polynomial( _smbtable, CDSRComplex( 1.0 ) ) ) )
	);
}

///////////////////////////////////////////////////////////////////////////////

MMD_RationalFun* _dsrop_Convert_RationalFun_integer2RationalFun_real( MMD_RationalFun* a )
{
	return a->getSmbTable().makeUniWord( 
		new MMD_RationalFun( a->getSmbTable(), 
							_dsrop_Convert_Polynomial_integer2Polynomial_real( a->hi ),
							_dsrop_Convert_Polynomial_integer2Polynomial_real( a->lo ) )
	);
}

MMD_RationalFun* _dsrop_Convert_RationalFun_integer2RationalFun_complex( MMD_RationalFun* a )
{
	return a->getSmbTable().makeUniWord( 
		new MMD_RationalFun( a->getSmbTable(), 
							_dsrop_Convert_Polynomial_integer2Polynomial_complex( a->hi ),
							_dsrop_Convert_Polynomial_integer2Polynomial_complex( a->lo ) )
	);
}

MMD_RationalFun* _dsrop_Convert_RationalFun_real2RationalFun_complex( MMD_RationalFun* a )
{
	return a->getSmbTable().makeUniWord( 
		new MMD_RationalFun( a->getSmbTable(), 
							_dsrop_Convert_Polynomial_real2Polynomial_complex( a->hi ),
							_dsrop_Convert_Polynomial_real2Polynomial_complex( a->lo ) )
	);
}

///////////////////////////////////////////////////////////////////////////////
