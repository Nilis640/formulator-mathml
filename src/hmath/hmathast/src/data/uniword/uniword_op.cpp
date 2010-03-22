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

#include "uniword_op.h"
#include "HMathML/immldefs.h"
#include "HMathBS/imathconst.h"

inline
CDSRReal MATH_ROUND( CDSRReal v )
{
	return (fabs(v) < MATH_REAL_EPSILON) ? 0.0 : v;
}

///////////////////////////////////////////////////////////////////////////////
// VIEW
///////////////////////////////////////////////////////////////////////////////
QString getTextView( CParseDsrSymbolTable& /*smbtable*/, const MMD_Address& v_addr )
{
	return QString("(%1, %2, %3, %4)").arg(::ml_Op2String( v_addr.param1 )).arg(v_addr.param2).arg(v_addr.param3).arg(v_addr.param4);
}

QString getFormulatorView( CParseDsrSymbolTable& /*smbtable*/, const MMD_Address& v_addr )
{
	return QString("(%1, %2, %3, %4)").arg(::ml_Op2String( v_addr.param1 )).arg(v_addr.param2).arg(v_addr.param3).arg(v_addr.param4);
}

QString getMmlView( CParseDsrSymbolTable& smbtable, const MMD_Address& v_addr )
{
	return QString("<%1>%2</%3>").arg(FBL_TAG_ci).arg(getTextView( smbtable, v_addr )).arg(FBL_TAG_ci);
}

QString getTextView( CParseDsrSymbolTable& /*smbtable*/, CDSRNaturalStorage v )
{
	return QString("%1").arg(v.value);
}

QString getFormulatorView( CParseDsrSymbolTable& /*smbtable*/, CDSRNaturalStorage v )
{
	return QString("%1").arg(v.value);
}

QString getMmlView( CParseDsrSymbolTable& smbtable, CDSRNaturalStorage v )
{
	// type='integer'
	return QString("<%1>%2</%3>").arg(FBL_TAG_cn).arg(getTextView(smbtable, v)).arg(FBL_TAG_cn);
}

QString getTextView( CParseDsrSymbolTable& /*smbtable*/, CDSRIntegerStorage v )
{
	return QString("%1").arg(v.value);
}

QString getFormulatorView( CParseDsrSymbolTable& /*smbtable*/, CDSRIntegerStorage v )
{
	return QString("%1").arg(v.value);
}

QString getMmlView( CParseDsrSymbolTable& smbtable, CDSRIntegerStorage v )
{
	// type='integer'
	return QString("<%1>%2</%3>").arg(FBL_TAG_cn).arg(getTextView(smbtable, v)).arg(FBL_TAG_cn);
}

QString getTextView( CParseDsrSymbolTable& /*smbtable*/, CDSRRationalStorage v )
{
	return QString("%1/%2").arg(v.c).arg(v.z);
}

QString getFormulatorView( CParseDsrSymbolTable& /*smbtable*/, CDSRRationalStorage v )
{
	return QString("@fraction(%1, %2)").arg(v.c).arg(v.z);
}

QString getMmlView( CParseDsrSymbolTable& /*smbtable*/, CDSRRationalStorage v )
{
	return QString("<%1 type='rational'>%2<%3/>%4</%5>").arg(FBL_TAG_cn).arg(v.c).arg(FBL_TAG_sep).arg(v.z).arg(FBL_TAG_cn);
}

QString getTextView( CParseDsrSymbolTable& smbtable, CDSRRealStorage v )
{
	return QString("%1").arg(MATH_ROUND(v), 0, 'g', int(smbtable.getPrint_DoublePrec()));
}

QString getFormulatorView( CParseDsrSymbolTable& smbtable, CDSRRealStorage v )
{
	return QString("%1").arg(MATH_ROUND(v), 0, 'g', int(smbtable.getPrint_DoublePrec()));
}

QString getMmlView( CParseDsrSymbolTable& smbtable, CDSRRealStorage v )
{
	//  type='real'
	return QString("<%1>%2</%3>").arg(FBL_TAG_cn).arg(getTextView(smbtable, v)).arg(FBL_TAG_cn);
}

QString getTextView_Complex2StrPart( CParseDsrSymbolTable& smbtable, CDSRComplex v, QString& r, QString& i, QString& s )
{
	CDSRReal v_imag = MATH_ROUND(v.imag()), v_real = MATH_ROUND(v.real());

	s = v_imag < 0 ? _T(" - ") : _T(" + ");

	if( v_imag == 0 )
	{
		i = _T("");
		s = _T("");
	}
	else if( v_imag == 1 || v_imag == -1 )
		i = _T("i");
	else
		i = QString("i * %1").arg( fabs( v_imag ), 0, 'g', smbtable.getPrint_DoublePrec() );

	if( v_real == 0 && i.length() )
	{
		r = _T("");
		if( v_imag < 0 ) s = _T("- ");
		else s = _T("");
	}
	else
		r = QString("%1").arg( v_real, 0, 'g', smbtable.getPrint_DoublePrec() );
	return r + s + i;
}

QString getTextView( CParseDsrSymbolTable& smbtable, CDSRComplex v )
{
	static QString r = _T(""), i = _T(""), s = _T("");
	return getTextView_Complex2StrPart( smbtable, v, r, i, s );
}

QString getFormulatorView_Complex2StrPart( CParseDsrSymbolTable& smbtable, CDSRComplex v, QString& r, QString& i, QString& s )
{
	CDSRReal v_imag = MATH_ROUND(v.imag()), v_real = MATH_ROUND(v.real());

	s = v_imag < 0 ? _T(" - ") : _T(" + ");

	if( v_imag == 0 )
	{
		i = _T("");
		s = _T("");
	}
	else if( v_imag == 1 || v_imag == -1 )
		i = _T("@imaginaryi");
	else
		i = QString("@imaginaryi @times %1").arg( fabs( v_imag ), 0, 'g', smbtable.getPrint_DoublePrec() );

	if( v_real == 0 && i.length() )
	{
		r = _T("");
		if( v_imag < 0 ) s = _T("- ");
		else s = _T("");
	}
	else
		r = QString("%1").arg( v_real, 0, 'g', smbtable.getPrint_DoublePrec() );
	return r + s + i;
}

QString getFormulatorView( CParseDsrSymbolTable& smbtable, CDSRComplex v )
{
	static QString r = _T(""), i = _T(""), s = _T("");
	return getFormulatorView_Complex2StrPart( smbtable, v, r, i, s );
}

QString getMmlView( CParseDsrSymbolTable& /*smbtable*/, CDSRComplex v )
{
	CDSRReal v_imag = MATH_ROUND(v.imag()), v_real = MATH_ROUND(v.real());
	return QString("<%1 type='complex-cartesian'>%2<%3/>%4</%5>").arg(FBL_TAG_cn).arg(v_real).arg(FBL_TAG_sep).arg(v_imag).arg(FBL_TAG_cn);
}

///////////////////////////////////////////////////////////////////////////////
// UTILS
///////////////////////////////////////////////////////////////////////////////
UniWord _create_lparam( long p1, long p2, long p3, long p4 )
{
	UniWord ret;
	ret.v_addr.param1 = p1;
	ret.v_addr.param2 = p2;
	ret.v_addr.param3 = p3;
	ret.v_addr.param4 = p4;
	return ret;
}

void _set_null( UniWord& ret )
{
	ret.v_addr.param1 = ret.v_addr.param2 = ret.v_addr.param3 = ret.v_addr.param4 = 0;
}

int _is_null( const UniWord& a )
{
	int ret = 0;
	switch( a.getType() )
	{
	case DSRDATA_TYPE_NATURAL:
		ret = a.v_natural == 0;
		break;
	case DSRDATA_TYPE_INTEGER:
		ret = a.v_integer == 0;
		break;
	case DSRDATA_TYPE_RATIONAL:
		ret = a.v_rational.c == 0;
		break;
	case DSRDATA_TYPE_REAL:
		ret = (fabs(a.v_real) < MATH_REAL_EPSILON);
		break;
	case DSRDATA_TYPE_COMPLEX:
		ret = (fabs(a.v_complex.real()) < MATH_REAL_EPSILON) && (fabs(a.v_complex.imag()) < MATH_REAL_EPSILON);
		break;
	default:
		break;
	}
	return ret;
}

int _is_negative_number( const UniWord& a )
{
	int ret = 0;
	switch( a.getType() )
	{
	case DSRDATA_TYPE_NATURAL:
		ret = a.v_natural < 0;
		break;
	case DSRDATA_TYPE_INTEGER:
		ret = a.v_integer < 0;
		break;
	case DSRDATA_TYPE_RATIONAL:
		ret =   (a.v_rational.c < 0 && a.v_rational.z > 0) || 
				(a.v_rational.c > 0 && a.v_rational.z < 0);
		break;
	case DSRDATA_TYPE_REAL:
		ret = a.v_real < 0.0;
		break;
	default:
		break;
	}
	return ret;
}

int _is_number_one( const UniWord& a )
{
	int ret = 0;
	switch( a.getType() )
	{
	case DSRDATA_TYPE_NATURAL:
		ret = a.v_natural == 1;
		break;
	case DSRDATA_TYPE_INTEGER:
		ret = a.v_integer == 1 || a.v_integer == -1;
		break;
	case DSRDATA_TYPE_RATIONAL:
		ret = (a.v_rational.c == 1 || a.v_rational.c == -1) && 
			  (a.v_rational.z == 1 || a.v_rational.z == -1);
		break;
	case DSRDATA_TYPE_REAL:
		ret = a.v_real == 1.0 || a.v_real == -1.0;
		break;
	default:
		break;
	}
	return ret;
}

///////////////////////////////////////////////////////////////////////////////
// ADD
///////////////////////////////////////////////////////////////////////////////
UniWord _dsrop_Add_natural( UniWord& ret, UniWord a, UniWord b )
{
	ret = CDSRNatural( a.getNatural() ) + CDSRNatural( b.getNatural() );
	return ret;
}

UniWord _dsrop_Add_integer( UniWord& ret, UniWord a, UniWord b )
{
	ret = CDSRInteger( a.getInteger() ) + CDSRInteger( b.getInteger() );
	return ret;
}

UniWord _dsrop_Add_rational( UniWord& ret, UniWord a, UniWord b )
{
	ret = CDSRRational( a.getRational() ) + CDSRRational( b.getRational() );
	return ret;
}

UniWord _dsrop_Add_real( UniWord& ret, UniWord a, UniWord b )
{
	ret = CDSRReal( a.getReal() ) + CDSRReal( b.getReal() );
	return ret;
}

UniWord _dsrop_Add_complex( UniWord& ret, UniWord a, UniWord b )
{
	ret = CDSRComplex( a.getComplex() ) + CDSRComplex( b.getComplex() );
	return ret;
}

///////////////////////////////////////////////////////////////////////////////

UniWord _dsrop_Plus_natural( UniWord& ret, UniWord a )
{
	ret = a.v_natural;
	return ret;
}

UniWord _dsrop_Plus_integer( UniWord& ret, UniWord a )
{
	ret = a.v_integer;
	return ret;
}

UniWord _dsrop_Plus_rational( UniWord& ret, UniWord a )
{
	ret = a.getRational();
	return ret;
}

UniWord _dsrop_Plus_real( UniWord& ret, UniWord a )
{
	ret = a.getReal();
	return ret;
}

UniWord _dsrop_Plus_complex( UniWord& ret, UniWord a )
{
	ret = a.getComplex();
	return ret;
}

///////////////////////////////////////////////////////////////////////////////
// SUB
///////////////////////////////////////////////////////////////////////////////
UniWord _dsrop_Sub_integer( UniWord& ret, UniWord a, UniWord b )
{
	ret = CDSRInteger( a.getInteger() ) - CDSRInteger( b.getInteger() );
	return ret;
}

UniWord _dsrop_Sub_rational( UniWord& ret, UniWord a, UniWord b )
{
	ret = CDSRRational( a.getRational() ) - CDSRRational( b.getRational() );
	return ret;
}

UniWord _dsrop_Sub_real( UniWord& ret, UniWord a, UniWord b )
{
	ret = CDSRReal( a.getReal() ) - CDSRReal( b.getReal() );
	return ret;
}

UniWord _dsrop_Sub_complex( UniWord& ret, UniWord a, UniWord b )
{
	ret = CDSRComplex( a.getComplex() ) - CDSRComplex( b.getComplex() );
	return ret;
}

///////////////////////////////////////////////////////////////////////////////

UniWord _dsrop_Minus_natural( UniWord& ret, UniWord a )
{
	ret = CDSRInteger( - a.getNatural() );
	return ret;
}

UniWord _dsrop_Minus_integer( UniWord& ret, UniWord a )
{
	ret = CDSRInteger( - a.getInteger() );
	return ret;
}

UniWord _dsrop_Minus_rational( UniWord& ret, UniWord a )
{
	ret = a.getRational();
	ret.v_rational.c = - ret.v_rational.c;
	return ret;
}

UniWord _dsrop_Minus_real( UniWord& ret, UniWord a )
{
	ret = - a.getReal();
	return ret;
}

UniWord _dsrop_Minus_complex( UniWord& ret, UniWord a )
{
	ret = - a.getComplex();
	return ret;
}

///////////////////////////////////////////////////////////////////////////////
// MUL
///////////////////////////////////////////////////////////////////////////////
UniWord _dsrop_Mul_natural( UniWord& ret, UniWord a, UniWord b )
{
	ret = CDSRNatural( a.getNatural() ) * CDSRNatural( b.getNatural() );
	return ret;
}

UniWord _dsrop_Mul_integer( UniWord& ret, UniWord a, UniWord b )
{
	ret = CDSRInteger( a.getInteger() ) * CDSRInteger( b.getInteger() );
	return ret;
}

UniWord _dsrop_Mul_rational( UniWord& ret, UniWord a, UniWord b )
{
	ret = CDSRRational( a.getRational() ) * CDSRRational( b.getRational() );
	return ret;
}

UniWord _dsrop_Mul_real( UniWord& ret, UniWord a, UniWord b )
{
	ret = CDSRReal( a.getReal() ) * CDSRReal( b.getReal() );
	return ret;
}

UniWord _dsrop_Mul_complex( UniWord& ret, UniWord a, UniWord b )
{
	ret = CDSRComplex( a.getComplex() ) * CDSRComplex( b.getComplex() );
	return ret;
}

///////////////////////////////////////////////////////////////////////////////
// DIV
///////////////////////////////////////////////////////////////////////////////
UniWord _dsrop_Div_natural( UniWord& ret, UniWord a, UniWord b )
{
	ret = CDSRRational( a.getNatural(), b.getNatural() );
	return ret;
}

UniWord _dsrop_Div_integer( UniWord& ret, UniWord a, UniWord b )
{
	ret = CDSRRational( a.getInteger(), b.getInteger() );
	return ret;
}

UniWord _dsrop_Div_rational( UniWord& ret, UniWord a, UniWord b )
{
	ret = CDSRRational( a.getRational() ) / CDSRRational( b.getRational() );
	return ret;
}

UniWord _dsrop_Div_real( UniWord& ret, UniWord a, UniWord b )
{
	ret = CDSRReal( a.getReal() ) / CDSRReal( b.getReal() );
	return ret;
}

UniWord _dsrop_Div_complex( UniWord& ret, UniWord a, UniWord b )
{
	ret = CDSRComplex( a.getComplex() ) / CDSRComplex( b.getComplex() );
	return ret;
}

///////////////////////////////////////////////////////////////////////////////
// MOD
///////////////////////////////////////////////////////////////////////////////
UniWord _dsrop_Mod_natural( UniWord& ret, UniWord a, UniWord b )
{
	ret = CDSRNatural( a.getNatural() % b.getNatural() );
	return ret;
}

UniWord _dsrop_Mod_integer( UniWord& ret, UniWord a, UniWord b )
{
	ret = CDSRInteger( a.getInteger() % b.getInteger() );
	return ret;
}


///////////////////////////////////////////////////////////////////////////////
// LESS(EQUAL), GREATER(EQUAL)
///////////////////////////////////////////////////////////////////////////////

UniWord _dsrop_Less_natural( UniWord& ret, UniWord a, UniWord b )
{
	ret = CDSRInteger( a.getNatural() < b.getNatural() );
	return ret;
}

UniWord _dsrop_Less_integer( UniWord& ret, UniWord a, UniWord b )
{
	ret = CDSRInteger( a.getInteger() < b.getInteger() );
	return ret;
}

UniWord _dsrop_Less_rational( UniWord& ret, UniWord a, UniWord b )
{
	ret = CDSRInteger( a.getRational() < b.getRational() );
	return ret;
}

UniWord _dsrop_Less_real( UniWord& ret, UniWord a, UniWord b )
{
	ret = CDSRInteger( a.getReal() < b.getReal() );
	return ret;
}

UniWord _dsrop_LessEq_natural( UniWord& ret, UniWord a, UniWord b )
{
	ret = CDSRInteger( a.getNatural() <= b.getNatural() );
	return ret;
}

UniWord _dsrop_LessEq_integer( UniWord& ret, UniWord a, UniWord b )
{
	ret = CDSRInteger( a.getInteger() <= b.getInteger() );
	return ret;
}

UniWord _dsrop_LessEq_rational( UniWord& ret, UniWord a, UniWord b )
{
	ret = CDSRInteger( a.getRational() <= b.getRational() );
	return ret;
}

UniWord _dsrop_LessEq_real( UniWord& ret, UniWord a, UniWord b )
{
	ret = CDSRInteger( a.getReal() <= b.getReal() );
	return ret;
}

UniWord _dsrop_Greater_natural( UniWord& ret, UniWord a, UniWord b )
{
	ret = CDSRInteger( a.getNatural() > b.getNatural() );
	return ret;
}

UniWord _dsrop_Greater_integer( UniWord& ret, UniWord a, UniWord b )
{
	ret = CDSRInteger( a.getInteger() > b.getInteger() );
	return ret;
}

UniWord _dsrop_Greater_rational( UniWord& ret, UniWord a, UniWord b )
{
	ret = CDSRInteger( a.getRational() > b.getRational() );
	return ret;
}

UniWord _dsrop_Greater_real( UniWord& ret, UniWord a, UniWord b )
{
	ret = CDSRInteger( a.getReal() > b.getReal() );
	return ret;
}

UniWord _dsrop_GreaterEq_natural( UniWord& ret, UniWord a, UniWord b )
{
	ret = CDSRInteger( a.getNatural() >= b.getNatural() );
	return ret;
}

UniWord _dsrop_GreaterEq_integer( UniWord& ret, UniWord a, UniWord b )
{
	ret = CDSRInteger( a.getInteger() >= b.getInteger() );
	return ret;
}

UniWord _dsrop_GreaterEq_rational( UniWord& ret, UniWord a, UniWord b )
{
	ret = CDSRInteger( a.getRational() >= b.getRational() );
	return ret;
}

UniWord _dsrop_GreaterEq_real( UniWord& ret, UniWord a, UniWord b )
{
	ret = CDSRInteger( a.getReal() >= b.getReal() );
	return ret;
}

///////////////////////////////////////////////////////////////////////////////
// (NOT)EQUAL
///////////////////////////////////////////////////////////////////////////////
UniWord _dsrop_Equal_natural( UniWord& ret, UniWord a, UniWord b )
{
	ret = CDSRInteger( a.getNatural() == b.getNatural() );
	return ret;
}

UniWord _dsrop_Equal_integer( UniWord& ret, UniWord a, UniWord b )
{
	ret = CDSRInteger( a.getInteger() == b.getInteger() );
	return ret;
}

UniWord _dsrop_Equal_rational( UniWord& ret, UniWord a, UniWord b )
{
	ret = CDSRInteger( a.getRational() == b.getRational() );
	return ret;
}

UniWord _dsrop_Equal_real( UniWord& ret, UniWord a, UniWord b )
{
	ret = CDSRInteger( a.getReal() == b.getReal() );
	return ret;
}

UniWord _dsrop_Equal_complex( UniWord& ret, UniWord a, UniWord b )
{
	ret = CDSRInteger( a.getComplex() == b.getComplex() );
	return ret;
}

UniWord _dsrop_Equal_bool( UniWord& ret, UniWord a, UniWord b )
{
	ret = CDSRInteger( a.getInteger() == b.getInteger() );
	return ret;
}

///////////////////////////////////////////////////////////////////////////////

UniWord _dsrop_NotEqual_natural( UniWord& ret, UniWord a, UniWord b )
{
	ret = CDSRInteger( a.getNatural() != b.getNatural() );
	return ret;
}

UniWord _dsrop_NotEqual_integer( UniWord& ret, UniWord a, UniWord b )
{
	ret = CDSRInteger( a.getInteger() != b.getInteger() );
	return ret;
}

UniWord _dsrop_NotEqual_rational( UniWord& ret, UniWord a, UniWord b )
{
	ret = CDSRInteger( a.getRational() != b.getRational() );
	return ret;
}

UniWord _dsrop_NotEqual_real( UniWord& ret, UniWord a, UniWord b )
{
	ret = CDSRInteger( a.getReal() != b.getReal() );
	return ret;
}

UniWord _dsrop_NotEqual_complex( UniWord& ret, UniWord a, UniWord b )
{
	ret = CDSRInteger( a.getComplex() != b.getComplex() );
	return ret;
}

UniWord _dsrop_NotEqual_bool( UniWord& ret, UniWord a, UniWord b )
{
	ret = CDSRInteger( a.getInteger() != b.getInteger() );
	return ret;
}

///////////////////////////////////////////////////////////////////////////////
// CONVERT
///////////////////////////////////////////////////////////////////////////////
UniWord _dsrop_Convert_natural2integer( UniWord& ret, UniWord a )
{
	ret = CDSRInteger( a.getNatural() );
	return ret;
}

UniWord _dsrop_Convert_natural2rational( UniWord& ret, UniWord a )
{
	ret = CDSRRational( a.getNatural() );
	return ret;
}

UniWord _dsrop_Convert_natural2real( UniWord& ret, UniWord a )
{
	ret = CDSRReal( a.getNatural() );
	return ret;
}

UniWord _dsrop_Convert_natural2complex( UniWord& ret, UniWord a )
{
	ret = CDSRComplex( a.getNatural() );
	return ret;
}

UniWord _dsrop_Convert_integer2rational( UniWord& ret, UniWord a )
{
	ret = CDSRRational( a.getInteger() );
	return ret;
}

UniWord _dsrop_Convert_integer2real( UniWord& ret, UniWord a )
{
	ret = CDSRReal( a.getInteger() );
	return ret;
}

UniWord _dsrop_Convert_integer2complex( UniWord& ret, UniWord a )
{
	ret = CDSRComplex( a.getInteger() );
	return ret;
}

UniWord _dsrop_Convert_rational2real( UniWord& ret, UniWord a )
{
	ret = CDSRReal( a.getRational().c ) / CDSRReal( a.getRational().z );
	return ret;
}

UniWord _dsrop_Convert_rational2complex( UniWord& ret, UniWord a )
{
	ret = CDSRComplex( a.getRational().c ) / CDSRComplex( a.getRational().z );
	return ret;
}

UniWord _dsrop_Convert_real2complex( UniWord& ret, UniWord a )
{
	ret = CDSRComplex( a.getReal() );
	return ret;
}

UniWord _dsrop_Convert_natural2bits( UniWord& ret, UniWord /*a*/ )
{
	// vasya : to do bits
	return ret;
}

UniWord _dsrop_Convert_integer2bits( UniWord& ret, UniWord /*a*/ )
{
	// vasya : to do bits
	return ret;
}

///////////////////////////////////////////////////////////////////////////////
// BOOL
///////////////////////////////////////////////////////////////////////////////
UniWord _dsrop_BoolAnd_bool( UniWord& ret, UniWord a, UniWord b )
{
	ret = CDSRInteger( a.getInteger() && b.getInteger() );
	return ret;
}

UniWord _dsrop_BoolOr_bool( UniWord& ret, UniWord a, UniWord b )
{
	ret = CDSRInteger( a.getInteger() || b.getInteger() );
	return ret;
}

UniWord _dsrop_BoolNot_bool( UniWord& ret, UniWord a )
{
	ret = CDSRInteger( !a.getInteger() );
	return ret;
}

///////////////////////////////////////////////////////////////////////////////
// ABS
///////////////////////////////////////////////////////////////////////////////
void _dsrop_abs_natural( UniWord *arg )
{
	*arg = CDSRNatural( labs( arg->getNatural() ) );
}

void _dsrop_abs_integer( UniWord *arg )
{
	*arg = CDSRInteger( labs( arg->getInteger() ) );
}

void _dsrop_abs_rational( UniWord *arg )
{
	arg->v_rational.c = labs( arg->v_rational.c );
	arg->v_rational.z = labs( arg->v_rational.z );
}

void _dsrop_abs_real( UniWord *arg )
{
	arg->v_real = fabs( arg->v_real );
}

void _dsrop_abs_complex( UniWord *arg )
{
	*arg = abs( arg->getComplex() );
}

///////////////////////////////////////////////////////////////////////////////
// POW
///////////////////////////////////////////////////////////////////////////////
UniWord _dsrop_IntPow_natural( UniWord& ret, UniWord a, UniWord b )
{
	ret = a;
	for( long i = 1; i < b.getNatural(); i++ )
		_dsrop_Mul_natural( ret, ret, a );
	return ret;
}

UniWord _dsrop_IntPow_integer( UniWord& ret, UniWord a, UniWord b )
{
	ret = a;
	for( long i = 1; i < b.getNatural(); i++ )
		_dsrop_Mul_integer( ret, ret, a );
	return ret;
}

UniWord _dsrop_IntPow_rational( UniWord& ret, UniWord a, UniWord b )
{
	ret = a;
	for( long i = 1; i < b.getNatural(); i++ )
		_dsrop_Mul_rational( ret, ret, a );
	return ret;
}

UniWord _dsrop_IntPow_real( UniWord& ret, UniWord a, UniWord b )
{
	ret = a;
	for( long i = 1; i < b.getNatural(); i++ )
		_dsrop_Mul_real( ret, ret, a );
	return ret;
}

UniWord _dsrop_IntPow_complex( UniWord& ret, UniWord a, UniWord b )
{
	ret = a;
	for( long i = 1; i < b.getNatural(); i++ )
		_dsrop_Mul_complex( ret, ret, a );
	return ret;
}

UniWord _dsrop_IntPow_group_string( UniWord& ret, UniWord /*a*/, UniWord /*b*/ )
{
	// vasya : to do group QString pow (-- can be -1 ? --)
	return ret;
}

///////////////////////////////////////////////////////////////////////////////

CDSRReal arch( CDSRReal a )
{
	return log( fabs( a + sqrt( a * a - 1.0 ) ) );
}
CDSRComplex arch( CDSRComplex a )
{
    return log( abs( a + sqrt( a * a - 1.0 ) ) );
}
CDSRReal arccos( CDSRReal a )
{
	return acos( a );
}
CDSRComplex arccos( CDSRComplex a )
{
	return acos( real( a ) );
}
CDSRReal arccosec( CDSRReal a )
{
	return asin( 1.0 / a );
}
CDSRComplex arccosec( CDSRComplex a )
{
	return asin( real( 1.0 / a ));
}
CDSRReal arcsec( CDSRReal a )
{
	return acos( 1.0 / a );
}
CDSRComplex arcsec( CDSRComplex a )
{
	return acos( real( 1.0 / a ) );
}
CDSRReal arcsin( CDSRReal a )
{
	return asin( a );
}
CDSRComplex arcsin( CDSRComplex a )
{
	return asin( real( a ) );
}
CDSRReal arctg( CDSRReal a )
{
	return atan( a );
}
CDSRComplex arctg( CDSRComplex a )
{
	return atan( real( a ) );
}
CDSRReal arcth( CDSRReal a )
{
	return log( fabs( (1.0 - a) / (1.0 + a) ) ) / 2.0;
}
CDSRComplex arcth( CDSRComplex a )
{
	return ( log( abs( (1.0 - a) / (1.0 + a) ) ) / 2.0 );
}
CDSRReal arsh( CDSRReal a )
{
	return log( fabs( a + sqrt( a * a + 1.0 ) ) );
}
CDSRComplex arsh( CDSRComplex a )
{
	return log( abs( a + sqrt( a * a + 1.0 ) ) );
}
CDSRReal ch( CDSRReal a )
{
	return cosh( a );
}
CDSRComplex ch( CDSRComplex a )
{
	return cosh( a );
}
CDSRReal cosec( CDSRReal a )
{
	return 1.0 / sin( a );
}
CDSRComplex cosec( CDSRComplex a )
{
	return 1.0 / sin( a );
}
CDSRReal cosech( CDSRReal a )
{
	return 1.0 / sinh( a );
}
CDSRComplex cosech( CDSRComplex a )
{
	return 1.0 / sinh( a );
}
CDSRReal ctg( CDSRReal a )
{
	return 1.0 / tan( a );
}
CDSRComplex ctg( CDSRComplex a )
{
	return 1.0 / tan( real( a ));
}
CDSRReal cth( CDSRReal a )
{
	return cosh( a ) / sinh( a );
}
CDSRComplex cth( CDSRComplex a )
{
	return cosh( a ) / sinh( a );
}
CDSRReal sec( CDSRReal a )
{
	return 1.0 / cos( a );
}
CDSRComplex sec( CDSRComplex a )
{
	return 1.0 / cos( a );
}
CDSRReal sech( CDSRReal a )
{
	return 1.0 / cosh( a );
}
CDSRComplex sech( CDSRComplex a )
{
	return 1.0 / cosh( a );
}
CDSRReal sh( CDSRReal a)
{
	return sinh( a );
}
CDSRComplex sh( CDSRComplex a )
{
	return sinh( a );
}
CDSRReal tg( CDSRReal a )
{
	return tan( a );
}
CDSRComplex tg( CDSRComplex a )
{
	return tan( real( a ) );
}
CDSRReal th( CDSRReal a )
{
	return tanh( a );
}
CDSRComplex th( CDSRComplex a )
{
	return tanh( real( a ) );
}

CDSRReal lg( CDSRReal a )
{
	return log10( a );
}
CDSRComplex lg( CDSRComplex a )
{
	return log10( a );
}
CDSRReal ln( CDSRReal a )
{
	return log( a );
}
CDSRComplex ln( CDSRComplex a )
{
	return log( a );
}
CDSRReal pow10( CDSRReal a )
{
	return pow( 10, a );
}

///////////////////////////////////////////////////////////////////////////////

int _dsrop_Add_scalar( UniWord& ret, UniWord a, UniWord b )
{
	if( a.getType() != b.getType() ) return -1;
	switch( a.getType() )
	{
	case DSRDATA_TYPE_NATURAL:
		_dsrop_Add_natural( ret, a, b );
		break;
	case DSRDATA_TYPE_INTEGER:
		_dsrop_Add_integer( ret, a, b );
		break;
	case DSRDATA_TYPE_RATIONAL:
		_dsrop_Add_rational( ret, a, b );
		break;
	case DSRDATA_TYPE_REAL:
		_dsrop_Add_real( ret, a, b );
		break;
	case DSRDATA_TYPE_COMPLEX:
		_dsrop_Add_complex( ret, a, b );
		break;
	default:
		return -1;
	}
	return 0;
}

int _dsrop_Sub_scalar( UniWord& ret, UniWord a, UniWord b )
{
	if( a.getType() != b.getType() ) return -1;
	switch( a.getType() )
	{
	case DSRDATA_TYPE_INTEGER:
		_dsrop_Sub_integer( ret, a, b );
		break;
	case DSRDATA_TYPE_RATIONAL:
		_dsrop_Sub_rational( ret, a, b );
		break;
	case DSRDATA_TYPE_REAL:
		_dsrop_Sub_real( ret, a, b );
		break;
	case DSRDATA_TYPE_COMPLEX:
		_dsrop_Sub_complex( ret, a, b );
		break;
	default:
		return -1;
	}
	return 0;
}

int _dsrop_Plus_scalar( UniWord& ret, UniWord a )
{
	switch( a.getType() )
	{
	case DSRDATA_TYPE_NATURAL:
		_dsrop_Plus_natural( ret, a );
		break;
	case DSRDATA_TYPE_INTEGER:
		_dsrop_Plus_integer( ret, a );
		break;
	case DSRDATA_TYPE_RATIONAL:
		_dsrop_Plus_rational( ret, a );
		break;
	case DSRDATA_TYPE_REAL:
		_dsrop_Plus_real( ret, a );
		break;
	case DSRDATA_TYPE_COMPLEX:
		_dsrop_Plus_complex( ret, a );
		break;
	default:
		return -1;
	}
	return 0;
}

int _dsrop_Minus_scalar( UniWord& ret, UniWord a )
{
	switch( a.getType() )
	{
	case DSRDATA_TYPE_NATURAL:
		_dsrop_Minus_natural( ret, a );
		break;
	case DSRDATA_TYPE_INTEGER:
		_dsrop_Minus_integer( ret, a );
		break;
	case DSRDATA_TYPE_RATIONAL:
		_dsrop_Minus_rational( ret, a );
		break;
	case DSRDATA_TYPE_REAL:
		_dsrop_Minus_real( ret, a );
		break;
	case DSRDATA_TYPE_COMPLEX:
		_dsrop_Minus_complex( ret, a );
		break;
	default:
		return -1;
	}
	return 0;
}

int _dsrop_Mul_scalar( UniWord& ret, UniWord a, UniWord b )
{
	if( a.getType() != b.getType() ) return -1;
	switch( a.getType() )
	{
	case DSRDATA_TYPE_NATURAL:
		_dsrop_Mul_natural( ret, a, b );
		break;
	case DSRDATA_TYPE_INTEGER:
		_dsrop_Mul_integer( ret, a, b );
		break;
	case DSRDATA_TYPE_RATIONAL:
		_dsrop_Mul_rational( ret, a, b );
		break;
	case DSRDATA_TYPE_REAL:
		_dsrop_Mul_real( ret, a, b );
		break;
	case DSRDATA_TYPE_COMPLEX:
		_dsrop_Mul_complex( ret, a, b );
		break;
	default:
		return -1;
	}
	return 0;
}

int _dsrop_Div_scalar( UniWord& ret, UniWord a, UniWord b )
{
	if( a.getType() != b.getType() ) return -1;
	switch( a.getType() )
	{
	case DSRDATA_TYPE_NATURAL:
		_dsrop_Div_natural( ret, a, b );
		break;
	case DSRDATA_TYPE_INTEGER:
		_dsrop_Div_integer( ret, a, b );
		break;
	case DSRDATA_TYPE_RATIONAL:
		_dsrop_Div_rational( ret, a, b );
		break;
	case DSRDATA_TYPE_REAL:
		_dsrop_Div_real( ret, a, b );
		break;
	case DSRDATA_TYPE_COMPLEX:
		_dsrop_Div_complex( ret, a, b );
		break;
	default:
		return -1;
	}
	return 0;
}

///////////////////////////////////////////////////////////////////////////////
