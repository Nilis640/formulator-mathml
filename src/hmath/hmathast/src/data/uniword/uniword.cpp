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

#include "uniword.h"
#include "../data.h"

///////////////////////////////////////////////////////////////////////////////

UniWord::UniWord( void )
{
	setType( DSRDATA_TYPE_NONE );
	v_addr.param1 = v_addr.param2 = v_addr.param3 = v_addr.param4 = 0;
}

void UniWord::operator=( enum CDsrDataTypes tp )
{
	setType( tp );
	switch( tp )
	{
	case DSRDATA_TYPE_NONE:
		v_addr.param1 = v_addr.param2 = v_addr.param3 = v_addr.param4 = 0;
		break;
	case DSRDATA_TYPE_NATURAL:
		v_natural = CDSRNatural( 1 );
		break;
	case DSRDATA_TYPE_INTEGER:
		v_integer = CDSRInteger( 0 );
		break;
	//case DSRDATA_TYPE_BITS:
	case DSRDATA_TYPE_RATIONAL:
		v_rational = CDSRRational( 1, 1 );
		break;
	case DSRDATA_TYPE_REAL:
		v_real = CDSRReal( 0.0 );
		break;
	case DSRDATA_TYPE_COMPLEX:
		v_complex = CDSRComplex( 0.0, 0.0 );
		break;
	case DSRDATA_TYPE_BOOL:
		v_integer = CDSRInteger( 0 );
		break;
	case DSRDATA_TYPE_STRING:
		v_string = 0;
		break;
	case DSRDATA_TYPE_POLYNOM:
		v_poly = 0;
		break;
	//case DSRDATA_TYPE_FUNCTION:
	case DSRDATA_TYPE_RATIONAL_FUN:
		v_rfun = 0;
		break;
	//case DSRDATA_TYPE_COMPOSITION:
	case DSRDATA_TYPE_VECTOR:
		v_vector = 0;
		break;
	//case DSRDATA_TYPE_RANGE:
	//case DSRDATA_TYPE_GROUP_STRING:
	case DSRDATA_TYPE_ABS_GROUP:
		v_group = 0;
		break;
	case DSRDATA_TYPE_ABS_SET:
		v_set = 0;
		break;
	default:
		break;
	}
}

void UniWord::operator=( const UniWord& a )
{
	setType( a.getType() );
	switch( a.getType() )
	{
	case DSRDATA_TYPE_NONE:
		v_addr.param1 = a.v_addr.param1;
		v_addr.param2 = a.v_addr.param2;
		v_addr.param3 = a.v_addr.param3;
		v_addr.param4 = a.v_addr.param4;
		break;
	case DSRDATA_TYPE_NATURAL:
		v_natural = a.getNatural();
		break;
	case DSRDATA_TYPE_INTEGER:
		v_integer = a.getInteger();
		break;
	//case DSRDATA_TYPE_BITS:
	case DSRDATA_TYPE_RATIONAL:
		v_rational = a.getRational();
		break;
	case DSRDATA_TYPE_REAL:
		v_real = a.getReal();
		break;
	case DSRDATA_TYPE_COMPLEX:
		v_complex = a.getComplex();
		break;
	case DSRDATA_TYPE_BOOL:
		v_integer = a.getInteger();
		break;
	case DSRDATA_TYPE_STRING:
		v_string = a.getString();
		break;
	case DSRDATA_TYPE_POLYNOM:
		v_poly = a.getPolynomial();
		break;
	//case DSRDATA_TYPE_FUNCTION:
	case DSRDATA_TYPE_RATIONAL_FUN:
		v_rfun = a.getRationalFun();
		break;
	//case DSRDATA_TYPE_COMPOSITION:
	case DSRDATA_TYPE_VECTOR:
		v_vector = a.getVector();
		break;
	//case DSRDATA_TYPE_RANGE:
	//case DSRDATA_TYPE_GROUP_STRING:
	case DSRDATA_TYPE_ABS_GROUP:
		v_group = a.getGroup();
		break;
	case DSRDATA_TYPE_ABS_SET:
		v_set = a.getSet();
		break;
	default:
		break;
	}
}

void UniWord::operator=( const MMD_Vector* a )
{
	setType( DSRDATA_TYPE_VECTOR );
	v_vector = (MMD_Vector*)a;
}

void UniWord::operator=( const MMD_Group* a )
{
	setType( DSRDATA_TYPE_ABS_GROUP );
	v_group = (MMD_Group*)a;
}

void UniWord::operator=( const MMD_Set* a )
{
	setType( DSRDATA_TYPE_ABS_SET );
	v_set = (MMD_Set*)a;
}

void UniWord::operator=( const CDSRNatural a )
{
	setType( DSRDATA_TYPE_NATURAL );
	v_natural = a;
}

void UniWord::operator=( const CDSRNaturalStorage a )
{
	setType( DSRDATA_TYPE_NATURAL );
	v_natural = a;
}

void UniWord::operator=( long a )
{
	setType( DSRDATA_TYPE_INTEGER );
	v_integer = a;
}

void UniWord::operator=( const CDSRInteger a )
{
	setType( DSRDATA_TYPE_INTEGER );
	v_integer = a;
}

void UniWord::operator=( const CDSRIntegerStorage a )
{
	setType( DSRDATA_TYPE_INTEGER );
	v_integer = a;
}

void UniWord::operator=( const CDSRRational a )
{
	setType( DSRDATA_TYPE_RATIONAL );
	v_rational = a;
}

void UniWord::operator=( const CDSRRationalStorage a )
{
	setType( DSRDATA_TYPE_RATIONAL );
	v_rational = a;
}

void UniWord::operator=( const CDSRReal a )
{
	setType( DSRDATA_TYPE_REAL );
	v_real = a;
}

void UniWord::operator=( const CDSRComplex a )
{
	setType( DSRDATA_TYPE_COMPLEX );
	v_complex = a;
}

void UniWord::operator=( const MMD_String* a )
{
	setType( DSRDATA_TYPE_STRING );
	v_string = (MMD_String*)a;
}

void UniWord::operator=( const MMD_Address a )
{
	setType( DSRDATA_TYPE_NONE );
	v_addr = a;
}

void UniWord::operator=( const MMD_Polynomial* a )
{
	setType( DSRDATA_TYPE_POLYNOM );
	v_poly = (MMD_Polynomial*)a;
}

void UniWord::operator=( const MMD_RationalFun* a )
{
	setType( DSRDATA_TYPE_RATIONAL_FUN );
	v_rfun = (MMD_RationalFun*)a;
}

///////////////////////////////////////////////////////////////////////////////

int UniWord::operator==( const UniWord& a )
{
	UniWord ret( CDSRInteger( 0 ) );
	if( getType() != a.getType() )
		return ret.getInteger();
	switch( getType() )
	{
	case DSRDATA_TYPE_NONE:
		break;
	case DSRDATA_TYPE_NATURAL:
		_dsrop_Equal_natural( ret, *this, a );
		break;
	case DSRDATA_TYPE_INTEGER:
		_dsrop_Equal_integer( ret, *this, a );
		break;
	//case DSRDATA_TYPE_BITS:
	case DSRDATA_TYPE_RATIONAL:
		_dsrop_Equal_rational( ret, *this, a );
		break;
	case DSRDATA_TYPE_REAL:
		_dsrop_Equal_real( ret, *this, a );
		break;
	case DSRDATA_TYPE_COMPLEX:
		_dsrop_Equal_complex( ret, *this, a );
		break;
	case DSRDATA_TYPE_BOOL:
		_dsrop_Equal_bool( ret, *this, a );
		break;
	case DSRDATA_TYPE_STRING:
		_dsrop_Equal_string( ret, *this, a );
		break;
	case DSRDATA_TYPE_POLYNOM:
		ret = _dsrop_PolynomialEqual_anytype( getPolynomial(), a.getPolynomial() );
		break;
	//case DSRDATA_TYPE_FUNCTION:
	case DSRDATA_TYPE_RATIONAL_FUN:
		ret = _dsrop_RationalFunEqual_anytype( getRationalFun(), a.getRationalFun() );
		break;
	//case DSRDATA_TYPE_COMPOSITION:
	case DSRDATA_TYPE_VECTOR:
		ret = _dsrop_VectorEqual_anytype( getVector(), a.getVector() );
		break;
	//case DSRDATA_TYPE_RANGE:
	//case DSRDATA_TYPE_GROUP_STRING:
	case DSRDATA_TYPE_ABS_GROUP:
		ret = _dsrop_GroupEqual_anytype( getGroup(), a.getGroup() );
		break;
	case DSRDATA_TYPE_ABS_SET:
		ret = _dsrop_SetEqual_anytype( getSet(), a.getSet() );
		break;
	default:
		break;
	}
	return ret.getInteger();
}

///////////////////////////////////////////////////////////////////////////////

QString UniWord::getTextView( CParseDsrSymbolTable& smbtable )
{
	switch( getType() )
	{
	case DSRDATA_TYPE_NONE:
		return ::getTextView( smbtable, v_addr );
		break;
	case DSRDATA_TYPE_NATURAL:
		return ::getTextView( smbtable, v_natural );
	case DSRDATA_TYPE_INTEGER:
		return ::getTextView( smbtable, v_integer );
	//case DSRDATA_TYPE_BITS:
	case DSRDATA_TYPE_RATIONAL:
		return ::getTextView( smbtable, v_rational );
	case DSRDATA_TYPE_REAL:
		return ::getTextView( smbtable, v_real );
	case DSRDATA_TYPE_COMPLEX:
		return ::getTextView( smbtable, v_complex );
	case DSRDATA_TYPE_BOOL:
		return ::getTextView( smbtable, v_integer );
	case DSRDATA_TYPE_STRING:
		return getString() ? getString()->getTextView() : _T("(empty string)");
	case DSRDATA_TYPE_POLYNOM:
		return getPolynomial() ? getPolynomial()->getTextView() : _T("(empty polynomial)");
	//case DSRDATA_TYPE_FUNCTION:
	case DSRDATA_TYPE_RATIONAL_FUN:
		return getRationalFun() ? getRationalFun()->getTextView() : _T("(empty rational function)");
	//case DSRDATA_TYPE_COMPOSITION:
	case DSRDATA_TYPE_VECTOR:
		return getVector() ? getVector()->getTextView() : _T("(empty vector)");
	//case DSRDATA_TYPE_RANGE:
	//case DSRDATA_TYPE_GROUP_STRING:
	case DSRDATA_TYPE_ABS_GROUP:
		return getGroup() ? getGroup()->getTextView() : _T("(empty group)");
	case DSRDATA_TYPE_ABS_SET:
		return getSet() ? getSet()->getTextView() : _T("(empty set)");
	default:
		break;
	}
	return _T("(unknown object)");
}

QString UniWord::getFormulatorView( CParseDsrSymbolTable& smbtable )
{
	switch( getType() )
	{
	case DSRDATA_TYPE_NONE:
		return ::getFormulatorView( smbtable, v_addr );
	case DSRDATA_TYPE_NATURAL:
		return ::getFormulatorView( smbtable, v_natural );
	case DSRDATA_TYPE_INTEGER:
		return ::getFormulatorView( smbtable, v_integer );
	//case DSRDATA_TYPE_BITS:
	case DSRDATA_TYPE_RATIONAL:
		return ::getFormulatorView( smbtable, v_rational );
	case DSRDATA_TYPE_REAL:
		return ::getFormulatorView( smbtable, v_real );
	case DSRDATA_TYPE_COMPLEX:
		return ::getFormulatorView( smbtable, v_complex );
	case DSRDATA_TYPE_BOOL:
		return ::getFormulatorView( smbtable, v_integer );
	case DSRDATA_TYPE_STRING:
		return getString() ? getString()->getFormulatorView() : _T("(empty string)");
	case DSRDATA_TYPE_POLYNOM:
		return getPolynomial() ? getPolynomial()->getFormulatorView() : _T("(empty polynomial)");
	//case DSRDATA_TYPE_FUNCTION:
	case DSRDATA_TYPE_RATIONAL_FUN:
		return getRationalFun() ? getRationalFun()->getFormulatorView() : _T("(empty rational function)");
	//case DSRDATA_TYPE_COMPOSITION:
	case DSRDATA_TYPE_VECTOR:
		return getVector() ? getVector()->getFormulatorView() : _T("(empty vector)");
	//case DSRDATA_TYPE_RANGE:
	//case DSRDATA_TYPE_GROUP_STRING:
	case DSRDATA_TYPE_ABS_GROUP:
		return getGroup() ? getGroup()->getFormulatorView() : _T("(empty group)");
	case DSRDATA_TYPE_ABS_SET:
		return getSet() ? getSet()->getFormulatorView() : _T("(empty set)");
	default:
		break;
	}
	return _T("(unknown object)");
}

QString UniWord::getMmlView( CParseDsrSymbolTable& smbtable )
{
	switch( getType() )
	{
	case DSRDATA_TYPE_NONE:
		return ::getMmlView( smbtable, v_addr );
	case DSRDATA_TYPE_NATURAL:
		return ::getMmlView( smbtable, v_natural );
	case DSRDATA_TYPE_INTEGER:
		return ::getMmlView( smbtable, v_integer );
	//case DSRDATA_TYPE_BITS:
	case DSRDATA_TYPE_RATIONAL:
		return ::getMmlView( smbtable, v_rational );
	case DSRDATA_TYPE_REAL:
		return ::getMmlView( smbtable, v_real );
	case DSRDATA_TYPE_COMPLEX:
		return ::getMmlView( smbtable, v_complex );
	case DSRDATA_TYPE_BOOL:
		return ::getMmlView( smbtable, v_integer );
	case DSRDATA_TYPE_STRING:
		return getString() ? getString()->getMmlView() : "";
	case DSRDATA_TYPE_POLYNOM:
		return getPolynomial() ? getPolynomial()->getMmlView() : "";
	//case DSRDATA_TYPE_FUNCTION:
	case DSRDATA_TYPE_RATIONAL_FUN:
		return getRationalFun() ? getRationalFun()->getMmlView() : "";
	//case DSRDATA_TYPE_COMPOSITION:
	case DSRDATA_TYPE_VECTOR:
		return getVector() ? getVector()->getMmlView() : "";
	//case DSRDATA_TYPE_RANGE:
	//case DSRDATA_TYPE_GROUP_STRING:
	case DSRDATA_TYPE_ABS_GROUP:
		return getGroup() ? getGroup()->getMmlView() : "";
	case DSRDATA_TYPE_ABS_SET:
		return getSet() ? getSet()->getMmlView() : "";
	default:
		break;
	}
	return "";
}

///////////////////////////////////////////////////////////////////////////////
