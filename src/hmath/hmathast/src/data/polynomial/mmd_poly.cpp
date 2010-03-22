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

#include "HMathML/immldefs.h"
#include "../uniword/uniword_op.h"
#include "mmd_poly.h"

QString MMD_Polynomial::getTextView_Term4Complex( long i, UniWord coef, int ishead )
{
	if( coef.getComplex().imag() == 0.0 )
		return getTextView_Term( i, UniWord( CDSRReal( coef.getComplex().real() ) ), ishead );

	QString s_sign = _T(""), s_coef = _T(""), s_x = _T(""), s_pow = _T("");
	QString result = _T("");

	if( !ishead ) s_sign = _T("+");

	QString s_r, s_i, s_s;
	s_coef = QString( _T("(") ) + ::getTextView_Complex2StrPart( getSmbTable(), coef.getComplex(), s_r, s_i, s_s ) + QString( _T(")") );

	if( !::_is_null( term_pow[ i ] ) )
	{
		if( s_coef.length() )
			s_x = QString( _T("*") ) + getVarName();
		else
			s_x = getVarName();
		if( !::_is_number_one( term_pow[ i ] ) )
			s_pow = QString( _T("^") ) + term_pow[ i ].getTextView( getSmbTable() );
	}

	result = s_sign + s_coef + s_x + s_pow;

	return result;
}

QString MMD_Polynomial::getTextView_Term( long i, UniWord coef, int ishead )
{
	QString s_sign = _T(""), s_coef = _T(""), s_x = _T(""), s_pow = _T("");

	QString result = _T("");
	if( ::_is_null( coef ) )
	{
		if( getTermCount() == 1 )
			result = coef.getTextView( getSmbTable() );
		return result;
	}

	if( !::_is_negative_number( coef ) && !ishead )
		s_sign = _T("+");
	if( ::_is_number_one( coef ) && !::_is_null( term_pow[ i ] ) )
	{
		if( ::_is_negative_number( coef ) )
			s_sign = _T("-");
	}
	else
	{
		s_coef = coef.getTextView( getSmbTable() );
	}

	if( !::_is_null( term_pow[ i ] ) )
	{
		if( s_coef.length() )
			s_x = QString( _T("*") ) + getVarName();
		else
			s_x = getVarName();
		if( !::_is_number_one( term_pow[ i ] ) )
			s_pow = QString( _T("^") ) + term_pow[ i ].getTextView( getSmbTable() );
	}

	result = s_sign + s_coef + s_x + s_pow;

	return result;
}

QString MMD_Polynomial::getTextView( void )
{
	Normalize();
	if( getTermCount() == 0 ) return _T("");
	QString result;
	if( getType() == DSRDATA_TYPE_COMPLEX )
	{
		result = getTextView_Term4Complex( getTermCount() - 1, term_coef[ getTermCount() - 1 ], 1 );
		for( long i = getTermCount() - 2; i >= 0; i-- )
			result += getTextView_Term4Complex( i, term_coef[ i ], 0 );
	}
	else
	{
		result = getTextView_Term( getTermCount() - 1, term_coef[ getTermCount() - 1 ], 1 );
		for( long i = getTermCount() - 2; i >= 0; i-- )
			result += getTextView_Term( i, term_coef[ i ], 0 );
	}
	return result;
}

QString MMD_Polynomial::getFormulatorView_Term4Complex( long i, UniWord coef, int ishead )
{
	if( coef.getComplex().imag() == 0.0 )
		return getFormulatorView_Term( i, UniWord( CDSRReal( coef.getComplex().real() ) ), ishead );

	QString s_sign = _T(""), s_coef = _T(""), s_x = _T(""), s_pow = _T("");
	QString result = _T("");

	if( !ishead ) s_sign = _T("+");
	QString s_r, s_i, s_s;
	s_coef = QString( _T("(") ) + ::getFormulatorView_Complex2StrPart( getSmbTable(), coef.getComplex(), s_r, s_i, s_s ) + QString( _T(")") );

	if( !::_is_null( term_pow[ i ] ) )
	{
		if( s_coef.length() )
			s_x = QString( _T("*") ) + getVarName();
		else
			s_x = getVarName();
		if( !::_is_number_one( term_pow[ i ] ) )
			s_pow = QString( _T("^") ) + term_pow[ i ].getFormulatorView( getSmbTable() );
	}

	result = s_sign + s_coef + s_x + s_pow;

	return result;
}

QString MMD_Polynomial::getFormulatorView_Term( long i, UniWord coef, int ishead )
{
	QString s_sign = _T(""), s_coef = _T(""), s_x = _T(""), s_pow = _T("");

	QString result = _T("");
	if( ::_is_null( coef ) )
	{
		if( getTermCount() == 1 )
			result = coef.getFormulatorView( getSmbTable() );
		return result;
	}

	if( !::_is_negative_number( coef ) && !ishead )
		s_sign = _T("+");
	if( ::_is_number_one( coef ) && !::_is_null( term_pow[ i ] ) )
	{
		if( ::_is_negative_number( coef ) )
			s_sign = _T("-");
	}
	else
	{
		s_coef = coef.getFormulatorView( getSmbTable() );
	}

	if( !::_is_null( term_pow[ i ] ) )
	{
		if( s_coef.length() )
			s_x = QString( _T("*") ) + getVarName();
		else
			s_x = getVarName();
		if( !::_is_number_one( term_pow[ i ] ) )
			s_pow = QString( _T("^") ) + term_pow[ i ].getFormulatorView( getSmbTable() );
	}

	result = s_sign + s_coef + s_x + s_pow;

	return result;
}

QString MMD_Polynomial::getFormulatorView( void )
{
	Normalize();
	if( getTermCount() == 0 ) return _T("");
	QString result;
	if( getType() == DSRDATA_TYPE_COMPLEX )
	{
		result = getFormulatorView_Term4Complex( getTermCount() - 1, term_coef[ getTermCount() - 1 ], 1 );
		for( long i = getTermCount() - 2; i >= 0; i-- )
			result += getFormulatorView_Term4Complex( i, term_coef[ i ], 0 );
	}
	else
	{
		result = getFormulatorView_Term( getTermCount() - 1, term_coef[ getTermCount() - 1 ], 1 );
		for( long i = getTermCount() - 2; i >= 0; i-- )
			result += getFormulatorView_Term( i, term_coef[ i ], 0 );
	}
	return result;
}

///////////////////////////////////////////////////////////////////////////////

QString MMD_Polynomial::getMmlValue_One( CParseDsrSymbolTable& smbtable )
{
	switch( getType() )
	{
	case DSRDATA_TYPE_INTEGER:
		{
			CDSRIntegerStorage v;
			v.value = 1;
			return ::getMmlView( smbtable, v );
		}
	case DSRDATA_TYPE_REAL:
		return ::getMmlView( smbtable, CDSRRealStorage(1.0) );
	case DSRDATA_TYPE_COMPLEX:
		return ::getMmlView( smbtable, CDSRComplex(1.0, 0.0) );
	default:
		break;
	}
	return "";
}

QString MMD_Polynomial::getMmlVarName()
{
	QString type;
	switch( getType() )
	{
	case DSRDATA_TYPE_INTEGER:
		type = "type = 'integer'";
		break;
	case DSRDATA_TYPE_REAL:
		type = "type = 'real'";
		break;
	case DSRDATA_TYPE_COMPLEX:
		type = "type = 'complex'";
		break;
	default:
		break;
	}
	QString res = QString("<%1 %2>%3</%4>").arg(FBL_TAG_ci).arg(type).arg(var_name.length() == 0 ? QString("x") : var_name).arg(FBL_TAG_ci);
	return res;
}

QString MMD_Polynomial::getMmlView_Term( long i, UniWord coef, int ishead, QString& s_sign )
{
	s_sign = "";
	QString result, s_coef, s_pow;
	if( ::_is_null( coef ) )
	{
		if( getTermCount() == 1 )
			result = coef.getMmlView( getSmbTable() );
		return result;
	}

	if( !::_is_negative_number( coef ) && !ishead )
		s_sign = QString("<%1/>").arg(FBL_TAG_plus);
	if( ::_is_number_one( coef ) && !::_is_null( term_pow[ i ] ) )
	{
		if( ::_is_negative_number( coef ) )
			s_sign = QString("<%1/>").arg(FBL_TAG_minus);
	}
	else
		s_coef = coef.getMmlView( getSmbTable() );

	if( !::_is_null( term_pow[ i ] ) )
	{
		if( !::_is_number_one( term_pow[ i ] ) )
		{
			s_pow = QString("<%1><%2/>%3%4</%5>")
				.arg(FBL_TAG_apply).arg(FBL_TAG_power).arg(getMmlVarName())
				.arg(term_pow[ i ].getMmlView( getSmbTable() )).arg(FBL_TAG_apply);
		}
		else
			s_pow = getMmlVarName();
	}
	if( s_coef.length() )
	{
		if( s_pow.length() )
			result = QString("<%1><%2/>%3%4</%5>")
				.arg(FBL_TAG_apply).arg(FBL_TAG_times).arg(s_coef)
				.arg(s_pow).arg(FBL_TAG_apply);
		else
			result = s_coef;
	}
	else
	{
		if( s_pow.length() )
			result = s_pow;
		else
			result = getMmlValue_One( getSmbTable() );
	}

	return result;
}

QString MMD_Polynomial::getMmlView( void )
{
	Normalize();
	if( getTermCount() == 0 ) return _T("");
	QString ret, result, sign;
	result = getMmlView_Term( getTermCount() - 1, term_coef[ getTermCount() - 1 ], 1, sign );
	if( sign.length() )
		ret = QString("<%1><%2/>%3</%4>").arg(FBL_TAG_apply).arg(sign).arg(result).arg(FBL_TAG_apply);
	else
		ret = result;
	for( long i = getTermCount() - 2; i >= 0; i-- )
	{
		result = getMmlView_Term( i, term_coef[ i ], 0, sign );
		if( sign.length() )
			ret = QString("<%1><%2/>%3%4</%5>").arg(FBL_TAG_apply).arg(sign).arg(ret).arg(result).arg(FBL_TAG_apply);
		else
			ret += result;
	}
	return ret;
}

///////////////////////////////////////////////////////////////////////////////

MMD_Polynomial::MMD_Polynomial( CParseDsrSymbolTable& _smbtable,
								enum CDsrDataTypes nested_type, 
								long term_c ) :
	MMD_Object( 
		_smbtable, 
		_smbtable.getTypeTable().makeDataTypeNode( 
			new CDsrDataType_Polynomial( 
				_smbtable.getTypeTable().makeDataTypeNode( nested_type ) 
			) ) ),
	term_pow( 8, 0, 8 ), term_coef( 8, 0, 8 )
{
	var_name = _T("");
	if( term_c > 0 )
	{
		long i;
		CDSRInteger iV( 0 );
		CDSRReal rV( 0 );
		CDSRComplex cV( 0 );
		switch( getType() )
		{
		case DSRDATA_TYPE_INTEGER:
			for( i = 0; i < term_c; i++ )
			{
				UniWord uw( iV );
				term_pow.Add( uw );
				term_coef.Add( uw );
			}
			break;
		case DSRDATA_TYPE_REAL:
			for( i = 0; i < term_c; i++ )
			{
				UniWord uw1( iV );
				UniWord uw2( rV );
				term_pow.Add( uw1 );
				term_coef.Add( uw2 );
			}
			break;
		case DSRDATA_TYPE_COMPLEX:
			for( i = 0; i < term_c; i++ )
			{
				UniWord uw1( iV );
				UniWord uw2( cV );
				term_pow.Add( uw1 );
				term_coef.Add( uw2 );
			}
			break;
		default:
			break;
		}
	}
	max_pow = -1;
	is_normal = 0;
}

MMD_Polynomial::MMD_Polynomial( CParseDsrSymbolTable& _smbtable, 
								const QString& x_name,
								enum CDsrDataTypes nested_type )
	: MMD_Object( _smbtable, 
		_smbtable.getTypeTable().makeDataTypeNode( 
			new CDsrDataType_Polynomial( _smbtable.getTypeTable().makeDataTypeNode( nested_type ) ) ) )
	, term_pow( 8, 0, 8 )
	, term_coef( 8, 0, 8 )
{
	UniWord uw1( CDSRInteger( 1 ) );
	UniWord uw2( CDSRReal( 1.0 ) );
	UniWord uw3( CDSRComplex( 1.0 ) );
	term_pow.Add( uw1 );

	switch( getType() )
	{
	case DSRDATA_TYPE_INTEGER:
		term_coef.Add( uw1 );
		break;
	case DSRDATA_TYPE_REAL:
		term_coef.Add( uw2 );
		break;
	case DSRDATA_TYPE_COMPLEX:
		term_coef.Add( uw3 );
		break;
	default:
		break;
	}
	var_name = x_name;
	max_pow = -1;
	is_normal = 0;
}

MMD_Polynomial::MMD_Polynomial( CParseDsrSymbolTable& _smbtable, 
								CDSRIntegerStorage is ) :
	MMD_Object( 
		_smbtable, 
		_smbtable.getTypeTable().makeDataTypeNode( 
			new CDsrDataType_Polynomial( 
				_smbtable.getTypeTable().makeDataTypeNode( DSRDATA_TYPE_INTEGER ) 
			) ) ),
	term_pow( 8, 0, 8 ), term_coef( 8, 0, 8 )
{
	var_name = _T("");
	UniWord uw1( CDSRInteger( 0 ) );
	UniWord uw2( is );
	term_pow.Add( uw1 );
	term_coef.Add( uw2 );
	max_pow = -1;
	is_normal = 0;
}

MMD_Polynomial::MMD_Polynomial( CParseDsrSymbolTable& _smbtable, 
								long is ) :
	MMD_Object( 
		_smbtable, 
		_smbtable.getTypeTable().makeDataTypeNode( 
			new CDsrDataType_Polynomial( 
				_smbtable.getTypeTable().makeDataTypeNode( DSRDATA_TYPE_INTEGER ) 
			) ) ),
	term_pow( 8, 0, 8 ), term_coef( 8, 0, 8 )
{
	var_name = _T("");
	UniWord uw1( CDSRInteger( 0 ) );
	CDSRInteger iV( is );
	UniWord uw2( iV );
	term_pow.Add( uw1 );
	term_coef.Add( uw2 );
	max_pow = -1;
	is_normal = 0;
}

MMD_Polynomial::MMD_Polynomial( CParseDsrSymbolTable& _smbtable, 
								CDSRRealStorage is ) :
	MMD_Object( 
		_smbtable, 
		_smbtable.getTypeTable().makeDataTypeNode( 
			new CDsrDataType_Polynomial( 
				_smbtable.getTypeTable().makeDataTypeNode( DSRDATA_TYPE_REAL ) 
			) ) ),
	term_pow( 8, 0, 8 ), term_coef( 8, 0, 8 )
{
	var_name = _T("");
	UniWord uw1( CDSRInteger( 0 ) );
	UniWord uw2( is );
	term_pow.Add( uw1 );
	term_coef.Add( uw2 );
	max_pow = -1;
	is_normal = 0;
}

MMD_Polynomial::MMD_Polynomial( CParseDsrSymbolTable& _smbtable, 
								CDSRComplexStorage is ) :
	MMD_Object( 
		_smbtable, 
		_smbtable.getTypeTable().makeDataTypeNode( 
			new CDsrDataType_Polynomial( 
				_smbtable.getTypeTable().makeDataTypeNode( DSRDATA_TYPE_COMPLEX ) 
			) ) ),
	term_pow( 8, 0, 8 ), term_coef( 8, 0, 8 )
{
	var_name = _T("");
	UniWord uw1( CDSRInteger( 0 ) );
	UniWord uw2( is );
	term_pow.Add( uw1 );
	term_coef.Add( uw2 );
	max_pow = -1;
	is_normal = 0;
}

MMD_Polynomial::MMD_Polynomial( const MMD_Polynomial& ms ) :
	MMD_Object( ms ),
	term_pow( 8, 0, 8 ), term_coef( 8, 0, 8 )
{
	max_pow = ms.max_pow;
	var_name = ms.var_name;
	is_normal = ms.is_normal;
	term_coef.Copy( ms.term_coef );
	term_pow.Copy( ms.term_pow );
}

MMD_Polynomial::MMD_Polynomial( CParseDsrSymbolTable& _smbtable, const CDSRArray<CDSRInteger>& src ) :
	MMD_Object( 
		_smbtable, 
		_smbtable.getTypeTable().makeDataTypeNode( 
			new CDsrDataType_Polynomial( 
				_smbtable.getTypeTable().makeDataTypeNode( DSRDATA_TYPE_INTEGER ) 
			) ) ),
	term_pow( 8, 0, 8 ), term_coef( 8, 0, 8 )
{
	var_name = _T("");
	//for( long i = 0; i < src.size(); i++ )
	long cb = 0;
	for( long i = (long) src.size() - 1; i >= 0; i-- )
	{
		UniWord uw1( CDSRInteger( cb++ ) );
		UniWord uw2( src[ i ] );
		term_pow.Add( uw1 );
		term_coef.Add( uw2 );
	}
	max_pow = (long) src.size() - 1;
	is_normal = 1;
}

MMD_Polynomial::MMD_Polynomial( CParseDsrSymbolTable& _smbtable, const CDSRArray<CDSRReal>& src ) :
	MMD_Object( 
		_smbtable, 
		_smbtable.getTypeTable().makeDataTypeNode( 
			new CDsrDataType_Polynomial( 
				_smbtable.getTypeTable().makeDataTypeNode( DSRDATA_TYPE_REAL ) 
			) ) ),
	term_pow( 8, 0, 8 ), term_coef( 8, 0, 8 )
{
	var_name = _T("");
	long cb = 0;
	for( long i = (long) src.size() - 1; i >= 0; i-- )
	{
		UniWord uw1( CDSRInteger( cb++ ) );
		UniWord uw2( src[ i ] );
		term_pow.Add( uw1 );
		term_coef.Add( uw2 );
	}
	max_pow = (long) src.size() - 1;
	is_normal = 1;
}

MMD_Polynomial::MMD_Polynomial( CParseDsrSymbolTable& _smbtable, const CDSRArray<CDSRComplex>& src ) :
	MMD_Object( 
		_smbtable, 
		_smbtable.getTypeTable().makeDataTypeNode( 
			new CDsrDataType_Polynomial( 
				_smbtable.getTypeTable().makeDataTypeNode( DSRDATA_TYPE_COMPLEX ) 
			) ) ),
	term_pow( 8, 0, 8 ), term_coef( 8, 0, 8 )
{
	var_name = _T("");
	long cb = 0;
	for( long i = (long) src.size() - 1; i >= 0; i-- )
	{
		UniWord uw1( CDSRInteger( cb++ ) );
		UniWord uw2( src[ i ] );
		term_pow.Add( uw1 );
		term_coef.Add( uw2 );
	}
	max_pow = (long) src.size() - 1;
	is_normal = 1;
}

void MMD_Polynomial::getCoef( CDSRArray<CDSRInteger>& dest )
{
	dest.erase( 0, dest.size() );
	if( !isNormal() ) Normalize();
	//for( long i = 0; i < getTermCount(); i++ )
	for( long i = getTermCount() - 1; i >= 0 ; i-- )
		dest.push_back( term_coef[ i ].getInteger() );
}

void MMD_Polynomial::getCoef( CDSRArray<CDSRReal>& dest )
{
	dest.erase( 0, dest.size() );
	if( !isNormal() ) Normalize();
	for( long i = getTermCount() - 1; i >= 0 ; i-- )
		dest.push_back( term_coef[ i ].getReal() );
}

void MMD_Polynomial::getCoef( CDSRArray<CDSRComplex>& dest )
{
	dest.erase( 0, dest.size() );
	if( !isNormal() ) Normalize();
	for( long i = getTermCount() - 1; i >= 0 ; i-- )
		dest.push_back( term_coef[ i ].getComplex() );
}

MMD_Object* MMD_Polynomial::copy( const MMD_Polynomial *src )
{
	MMD_Object::copy( src );

	max_pow = src->max_pow;
	var_name = src->var_name;
	is_normal = src->is_normal;
	term_coef.Copy( src->term_coef );
	term_pow.Copy( src->term_pow );
	
	return this;
}

long MMD_Polynomial::getNewMaxPow( void )
{
	max_pow = term_pow[ 0 ].getInteger();
	for( int i = 0; i < getTermCount(); i++ )
		if( max_pow < term_pow[ i ].getInteger() )
			max_pow = term_pow[ i ].getInteger();
	return max_pow;
}

void MMD_Polynomial::_getNormalCoef_integer( void )
{
	if( !term_coef.ArrayDim() ) return;

	MArray<UniWord> tmp( term_coef.ArrayDim(), 0, term_coef.ArrayDim() );
	tmp.Copy( term_coef );
	term_coef.Flush();

	long i, mp;
	UniWord n( CDSRInteger( 0 ) );
	for( i = 0; i < getMaxPow() + 1; i++ )
		term_coef.Add( n );
	for( i = 0; i < (long) term_pow.ArrayDim(); i++ )
		_dsrop_Add_integer( term_coef[ term_pow[ i ].getInteger() ], term_coef[ term_pow[ i ].getInteger() ], tmp[ i ] );
	mp = getMaxPow();
	for( i = mp; i >= 0; i-- )
	{
		UniWord uw( CDSRInteger( 0 ) );
		if( _dsrop_Equal_integer( uw, getCoef( i ), UniWord( CDSRInteger( 0 ) ) ).getInteger() )
		{
			term_coef.erase( term_coef.begin() + i );
			mp--;
		}
		else
			break;
	}
	if( mp < getMaxPow() )
		max_pow = mp;
}


void MMD_Polynomial::_getNormalCoef_real( void )
{
	if( !term_coef.ArrayDim() ) return;

	MArray<UniWord> tmp( term_coef.ArrayDim(), 0, term_coef.ArrayDim() );
	tmp.Copy( term_coef );
	term_coef.Flush();

	long i, mp;
	UniWord n( CDSRReal( 0 ) );
	for( i = 0; i < getMaxPow() + 1; i++ )
		term_coef.Add( n );
	for( i = 0; i < (long) term_pow.ArrayDim(); i++ )
		_dsrop_Add_real( term_coef[ term_pow[ i ].getInteger() ], term_coef[ term_pow[ i ].getInteger() ], tmp[ i ] );
	mp = getMaxPow();
	for( i = mp; i >= 0; i-- )
	{
		UniWord uw1( CDSRInteger( 0 ) );
		if( _dsrop_Equal_real( uw1, getCoef( i ), UniWord( CDSRReal( 0.0 ) ) ).getInteger() )
		{
			term_coef.erase( term_coef.begin() + i );
			mp--;
		}
		else
			break;
	}
	if( mp < getMaxPow() )
		max_pow = mp;
}

void MMD_Polynomial::_getNormalCoef_complex( void )
{
	if( !term_coef.ArrayDim() ) return;

	MArray<UniWord> tmp( term_coef.ArrayDim(), 0, term_coef.ArrayDim() );
	tmp.Copy( term_coef );
	term_coef.Flush();

	long i, mp;
	UniWord n( CDSRComplex( 0 ) );
	for( i = 0; i < getMaxPow() + 1; i++ )
		term_coef.Add( n );
	for( i = 0; i < (long) term_pow.ArrayDim(); i++ )
		_dsrop_Add_complex( term_coef[ term_pow[ i ].getInteger() ], term_coef[ term_pow[ i ].getInteger() ], tmp[ i ] );
	mp = getMaxPow();
	for( i = mp; i >= 0; i-- )
	{
		UniWord uw1( CDSRInteger( 0 ) );
		if( _dsrop_Equal_complex( uw1, getCoef( i ), UniWord( CDSRComplex( 0 ) ) ).getInteger() )
		{
			term_coef.erase( term_coef.begin() + i );
			mp--;
		}
		else
			break;
	}
	if( mp < getMaxPow() )
		max_pow = mp;
}

void MMD_Polynomial::Normalize( void )
{
	if( getNewMaxPow() > MMD_Polynomial_MAX_POW_VALUE )
		throw "Power value is too big in the operation with polynom";
	switch( getType() )
	{
	case DSRDATA_TYPE_INTEGER:
		_getNormalCoef_integer();
		break;
	case DSRDATA_TYPE_REAL:  
		_getNormalCoef_real();
		break;
	case DSRDATA_TYPE_COMPLEX:
		_getNormalCoef_complex();
		break;
	default:
		break;
	}
	term_pow.Flush();
	for( long i = 0; i < getTermCount(); i++ )
	{
		CDSRInteger iV( i );
		UniWord uw( iV );
		term_pow.Add( uw );
	}
	is_normal = true;
}

void MMD_Polynomial::getValue( UniWord& result, const UniWord& x )
{
	long i;
	UniWord ret;
	if( !isNormal() ) Normalize();
	if( !getTermCount() ) return;
	ret = term_coef[ getTermCount() - 1 ];
	switch( getType() )
	{
	case DSRDATA_TYPE_INTEGER:
		for( i = getTermCount() - 2; i >= 0; i-- )
		{
			_dsrop_Mul_integer( ret, ret, x );
			_dsrop_Add_integer( ret, ret, term_coef[ i ] );
		}
		break;
	case DSRDATA_TYPE_REAL:
		for( i = getTermCount() - 2; i >= 0; i-- )
		{
			_dsrop_Mul_real( ret, ret, x );
			_dsrop_Add_real( ret, ret, term_coef[ i ] );
		}
		break;
	case DSRDATA_TYPE_COMPLEX:
		for( i = getTermCount() - 2; i >= 0; i-- )
		{
			_dsrop_Mul_complex( ret, ret, x );
			_dsrop_Add_complex( ret, ret, term_coef[ i ] );
		}
		break;
	default:
		break;
	}
	result = ret;
}

///////////////////////////////////////////////////////////////////////////////


MMD_Polynomial::~MMD_Polynomial( void )
{}


MMD_Polynomial* MMD_Polynomial::clone( void )
{
	return (MMD_Polynomial*)getSmbTable().makeUniWord( new MMD_Polynomial( *this ) );
}


void MMD_Polynomial::setVarName( QString& n )
{
	var_name = n;
}


long MMD_Polynomial::getMaxPow( void )
{
	if( max_pow == -1 )
		return getNewMaxPow();
	return max_pow;
}


int MMD_Polynomial::isNormal( void )
{
	return is_normal;
}


void MMD_Polynomial::setNormal( int v )
{
	is_normal = v;
}


void MMD_Polynomial::setMaxPow( void )
{
	max_pow = term_pow[ getTermCount() - 1 ].getInteger();
}


long MMD_Polynomial::getTermCount( void )
{
	return (long) term_coef.ArrayDim();
}


QString MMD_Polynomial::getVarName( void )
{
	return var_name.length() == 0 ? _T("x") : var_name;
}


UniWord& MMD_Polynomial::getPow( long term_no )
{
	return term_pow[ term_no ];
} 


UniWord& MMD_Polynomial::getCoef( long term_no )
{
	return term_coef[ term_no ];
}


MArray<UniWord>& MMD_Polynomial::getNormalCoef( void )
{
	if( !isNormal() ) Normalize();
	return term_coef;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
