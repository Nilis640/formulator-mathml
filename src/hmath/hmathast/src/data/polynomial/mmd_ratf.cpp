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
#include "mmd_ratf.h"

///////////////////////////////////////////////////////////////////////////////

QString MMD_RationalFun::getTextView( void )
{
	return hi && lo ? hi->getTextView() + _T(" / ") + lo->getTextView() : _T("");
}

QString MMD_RationalFun::getFormulatorView( void )
{
	return hi && lo ? ( QString( _T("@fraction(") ) + hi->getTextView() + QString( _T(", ") ) + lo->getTextView() ) : _T("");
}

QString MMD_RationalFun::getMmlView( void )
{
	if( hi && lo )
	{
		QString ret = QString("<%1><%2/>%3%4</%5>").arg(FBL_TAG_apply).arg(FBL_TAG_divide)
							.arg(hi->getMmlView()).arg(lo->getMmlView()).arg(FBL_TAG_apply);
		return ret;
	}
	return "";
}

///////////////////////////////////////////////////////////////////////////////

MMD_RationalFun::MMD_RationalFun( CParseDsrSymbolTable& _smbtable, enum CDsrDataTypes nested_type )
	: MMD_Object( 
		_smbtable, 
		_smbtable.getTypeTable().makeDataTypeNode( 
			new CDsrDataType_RationalFun( _smbtable.getTypeTable().makeDataTypeNode( nested_type ) ) ) )
{
	switch( nested_type )
	{
	case DSRDATA_TYPE_INTEGER:
		hi = _smbtable.makeUniWord( new MMD_Polynomial( _smbtable, CDSRInteger( 0 ) ) );
		lo = _smbtable.makeUniWord( new MMD_Polynomial( _smbtable, CDSRInteger( 1 ) ) );
		break;
	case DSRDATA_TYPE_REAL:
		hi = _smbtable.makeUniWord( new MMD_Polynomial( _smbtable, CDSRReal( 0 ) ) );
		lo = _smbtable.makeUniWord( new MMD_Polynomial( _smbtable, CDSRReal( 1 ) ) );
		break;
	case DSRDATA_TYPE_COMPLEX:
		hi = _smbtable.makeUniWord( new MMD_Polynomial( _smbtable, CDSRComplex( 0 ) ) );
		lo = _smbtable.makeUniWord( new MMD_Polynomial( _smbtable, CDSRComplex( 0 ) ) );
		break;
	default:
		throw _T("MMD_RationalFun::MMD_RationalFun : internal error");
	}
}

void MMD_RationalFun::getValue( UniWord& ret, const UniWord& x )
{
	UniWord a, b;
	hi->getValue( a, x );
	lo->getValue( b, x );
	switch( getType() )
	{
	case DSRDATA_TYPE_INTEGER:
		_dsrop_Div_integer( ret, a, b );
		break;
	case DSRDATA_TYPE_REAL:
		_dsrop_Div_real( ret, a, b );
		break;
	case DSRDATA_TYPE_COMPLEX:
		_dsrop_Div_complex( ret, a, b );
		break;
	default:
#if _DEBUG
		throw _T("MMD_RationalFun::getValue : internal error");
#else
		break;
#endif
	}
}

///////////////////////////////////////////////////////////////////////////////
