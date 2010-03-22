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

#include "HUtils/idfas.h"

#include "mml_units.h"
#include "mml_operator.h"
#include "mml_style.h"
#include "mml_defs.h"

///////////////////////////////////////////////////////////////////////////

int mml_attr_paddedunit_2_value( long selfIdx, const QString& body, 
								 RealFde* newValues, RealFde* oldValues,
								 const SYSINFO_Unit2PX& unit2px )
{
	Q_ASSERT( selfIdx >= 0 && selfIdx < MML_MPADDED_UNITS_LEN );
	int is_y = selfIdx > 1;

	//=== init data: local variables
	//===
	static const int mPaddedStdLength = 15;
	static QString mPaddedStd[ mPaddedStdLength ] = 
	{
		MML_UNIT_NAME__EM, MML_UNIT_NAME__EX, MML_UNIT_NAME__PX, MML_UNIT_NAME__IN,
		MML_UNIT_NAME__CM, MML_UNIT_NAME__MM, MML_UNIT_NAME__PT, MML_UNIT_NAME__PC,
		FBL_VAL_veryverythinmathspace, FBL_VAL_verythinmathspace, FBL_VAL_thinmathspace, FBL_VAL_mediummathspace,
		FBL_VAL_thickmathspace, FBL_VAL_verythickmathspace, FBL_VAL_veryverythickmathspace
	};

	//=== init data: values of local variables
	//===
	double mPaddedStd_Coef[ mPaddedStdLength ] = 
	{
		unit2px.em2px,
		unit2px.ex2px, 
		unit2px.px2px,
		(is_y ? unit2px.in2px_y : unit2px.in2px_x),
		(is_y ? unit2px.cm2px_y : unit2px.cm2px_x),
		(is_y ? unit2px.mm2px_y : unit2px.mm2px_x), 
		(is_y ? unit2px.pt2px_y : unit2px.pt2px_x),
		(is_y ? unit2px.pc2px_y : unit2px.pc2px_x),
		HMathOpAttr_SPACE_veryverythinmathspace * unit2px.em2px,
		HMathOpAttr_SPACE_verythinmathspace * unit2px.em2px,
		HMathOpAttr_SPACE_thinmathspace * unit2px.em2px,
		HMathOpAttr_SPACE_mediummathspace * unit2px.em2px,
		HMathOpAttr_SPACE_thickmathspace * unit2px.em2px,
		HMathOpAttr_SPACE_verythickmathspace * unit2px.em2px,
		HMathOpAttr_SPACE_veryverythickmathspace * unit2px.em2px
	};

	//=== Assert arguments validity
	//===
	int sign = 0;
	double percent = 1.0;
	long i = 0, result, j;
	double temp_value;
	QString unitName;

	//=== Depending on the format of the attribute value (+, -, blank),
	//=== a dimension may be set to a new value, or to an incremented or
	//=== decremented version of the content's corresponding dimension
	//===
	while( i < body.length() && body.at(i).isSpace() ) i++;
	if( i >= body.length() ) return -1;
    if( body.at(i).isDigit() || body.at(i) == QChar('.') )
		sign = 0;
	else if( body.at(i) == QChar('-') )
		sign = -1;
	else if( body.at(i) == QChar('+') )
		sign = 1;
	else
		return -1; // error: wrong syntax

	//=== Find an unsigned-number value
	//===
	LXMLSizeSDigit ldigit;
	result = ldigit.Analisys( body.midRef(i) );
	switch( result )
	{
	case LSDigit::Integer:
		temp_value = (double)ldigit.mnt_int;
		break;
	case LSDigit::Float:
		ldigit.mnt_float *= pow( 10.0, (int)ldigit.power );
		temp_value = (double)ldigit.mnt_float;
		break;
	default:
		return -1; // error: wrong syntax
	}
	i += ldigit.GetRead();

	while( i < body.length() && body.at(i).isSpace() ) i++;
	if( i < body.length() && body.at(i) == _T('%') )
	{
		percent = 0.01;
		i++;
		while( i < body.length() && body.at(i).isSpace() ) i++;
	}

	//=== Detect a name of a local variable (deafult is a name of the same dimension)
	//===
	if( i >= body.length() ) return -1;
    if( !(body.at(i).isLetter() || body.at(i) == QChar('_')) )
		unitName = ::mml_getMPaddedUnitName( selfIdx );
	else
	{
		LCVar lvar;
		result = lvar.Analisys( body.midRef(i) );
		if( !result ) return -1;
		unitName = lvar.getID();
		i += lvar.GetRead();
	}

	//=== Find a value if we know the name of the referenced local variable
	//===
	for( j = 0; j < mPaddedStdLength; j++ )
	{
		if( unitName == mPaddedStd[ j ] )
		{
			temp_value *= mPaddedStd_Coef[ j ];	// percents are ignored if this is a std-name
			break;
		}
	}

	if( j == mPaddedStdLength )
	{
		for( j = 0; j < MML_MPADDED_UNITS_LEN; j++ )
		{
			if( unitName == ::mml_getMPaddedUnitName( j ) )
			{
				temp_value *= oldValues[ j ] * percent;
				break;
			}
		}
	}

	if( j == MML_MPADDED_UNITS_LEN )	// error: unknown referenced name
		return -1;

	// If an attribute value begins with a + or - sign, it specifies an 
	// increment or decrement of the corresponding dimension 
	// Otherwise, the corresponding dimension is set directly to the following length value
	switch( sign )
	{
		case -1:
			newValues[ selfIdx ] = (RealFde)(oldValues[ selfIdx ] - temp_value);
			break;
		case 0:
			newValues[ selfIdx ] = (RealFde)(temp_value);
			break;
		case 1:
			newValues[ selfIdx ] = (RealFde)(oldValues[ selfIdx ] + temp_value);
			break;
		default:
			Q_ASSERT( 0 );	// error: internal
	}

	// the + and - do not mean that the following value is positive or negative
	if( newValues[ selfIdx ] < 0 ) newValues[ selfIdx ] = 0;

	while( i < body.length() && body.at(i).isSpace() ) i++;
	if( i < body.length() ) return -1; // error: wrong syntax
	return 0;
}

///////////////////////////////////////////////////////////////////////////

// regular expr is "number unit"
int mml_attr_unit_2_value( int is_y, RealFde& old_value, const QString& body, const SYSINFO_Unit2PX& unit2px )
{
	long i = 0, result;
	double temp_value;

	while( i < body.length() && body.at(i).isSpace() ) i++;
	if( i >= body.length() ) return -1;
    if( !(body.at(i).isDigit() || body.at(i) == QChar('.') || body.at(i) == QChar('-')) ) return -1;
	LXMLSizeSDigit ldigit;
	result = ldigit.Analisys( body.midRef(i) );
	switch( result )
	{
	case LSDigit::Integer:
		temp_value = double(ldigit.asign) * ldigit.mnt_int;
		break;
	case LSDigit::Float:
		ldigit.mnt_float *= pow( 10.0, (int)ldigit.power );
		temp_value = double(ldigit.asign) * ldigit.mnt_float;
		break;
	default:
		return -1;
	}
	i += ldigit.GetRead();

	while( i < body.length() && body.at(i).isSpace() ) i++;
	int ret = 0;
    if( i < body.length() && (body.at(i).isLetter() || body.at(i) == _T('_')) )
	{
		LCVar lvar;
		result = lvar.Analisys( body.midRef(i) );
		if( !result ) return -1;
		if( lvar.getID() == MML_UNIT_NAME__EM )
			old_value = (RealFde)(temp_value * unit2px.em2px);
		else if( lvar.getID() == MML_UNIT_NAME__EX )
			old_value = (RealFde)(temp_value * unit2px.ex2px);
		else if( lvar.getID() == MML_UNIT_NAME__PX )
			old_value = (RealFde)(temp_value /* * unit2px.px2px*/); // unit2px.px2px is 1
		else if( lvar.getID() == MML_UNIT_NAME__IN )
			old_value = (RealFde)(temp_value * (is_y ? unit2px.in2px_y : unit2px.in2px_x));
		else if( lvar.getID() == MML_UNIT_NAME__CM )
			old_value = (RealFde)(temp_value * (is_y ? unit2px.cm2px_y : unit2px.cm2px_x));
		else if( lvar.getID() == MML_UNIT_NAME__MM )
			old_value = (RealFde)(temp_value * (is_y ? unit2px.mm2px_y : unit2px.mm2px_x));
		else if( lvar.getID() == MML_UNIT_NAME__PT )
			old_value = (RealFde)(temp_value * (is_y ? unit2px.pt2px_y : unit2px.pt2px_x));
		else if( lvar.getID() == MML_UNIT_NAME__PC )
			old_value = (RealFde)(temp_value * (is_y ? unit2px.pc2px_y : unit2px.pc2px_x));
		else
			ret = -1;
		i += lvar.GetRead();
	}
	else if( i < body.length() && body.at(i) == QChar('%') )
	{
		old_value = (RealFde)(old_value * temp_value / 100.0);
		i++;
	}
	else // here % is default unit (linethickness in fractions)
	{
		old_value = (RealFde)(old_value * temp_value);
	}

	while( i < body.length() && body.at(i).isSpace() ) i++;
	if( i < body.length() ) return -1; // error: wrong syntax
	return 0;
}

int mml_convertSpaceName2FloatValue( int is_y, const QString& spacename, RealFde& value, const SYSINFO_Unit2PX& unit2px )
{
	if( !xml_strcmp( spacename, FBL_VAL_veryverythinmathspace ) )
		value = (RealFde)(HMathOpAttr_SPACE_veryverythinmathspace * unit2px.em2px);
	else if( !xml_strcmp( spacename, FBL_VAL_verythinmathspace ) )
		value = (RealFde)(HMathOpAttr_SPACE_verythinmathspace * unit2px.em2px);
	else if( !xml_strcmp( spacename, FBL_VAL_thinmathspace ) )
		value = (RealFde)(HMathOpAttr_SPACE_thinmathspace * unit2px.em2px);
	else if( !xml_strcmp( spacename, FBL_VAL_mediummathspace ) )
		value = (RealFde)(HMathOpAttr_SPACE_mediummathspace * unit2px.em2px);
	else if( !xml_strcmp( spacename, FBL_VAL_thickmathspace ) )
		value = (RealFde)(HMathOpAttr_SPACE_thickmathspace * unit2px.em2px);
	else if( !xml_strcmp( spacename, FBL_VAL_verythickmathspace ) )
		value = (RealFde)(HMathOpAttr_SPACE_verythickmathspace * unit2px.em2px);
	else if( !xml_strcmp( spacename, FBL_VAL_veryverythickmathspace ) )
		value = (RealFde)(HMathOpAttr_SPACE_veryverythickmathspace * unit2px.em2px);
	else
	{
		if( mml_attr_unit_2_value( is_y, value, spacename, unit2px ) == -1 )
			return -1;
	}
	return 0;
}

///////////////////////////////////////////////////////////////////////////

int mml_convertFloatValue2SpaceName( QString& destname, RealFde value, const SYSINFO_Unit2PX& unit2px )
{
	const static RealFde minMeaning = 0.001F;
	if( fabs(value - HMathOpAttr_SPACE_veryverythinmathspace * unit2px.em2px) < minMeaning )
		destname = FBL_VAL_veryverythinmathspace;
	else if( fabs(value - HMathOpAttr_SPACE_verythinmathspace * unit2px.em2px) < minMeaning )
		destname = FBL_VAL_verythinmathspace;
	else if( fabs(value - HMathOpAttr_SPACE_thinmathspace * unit2px.em2px) < minMeaning )
		destname = FBL_VAL_thinmathspace;
	else if( fabs(value - HMathOpAttr_SPACE_mediummathspace * unit2px.em2px) < minMeaning )
		destname = FBL_VAL_mediummathspace;
	else if( fabs(value - HMathOpAttr_SPACE_thickmathspace * unit2px.em2px) < minMeaning )
		destname = FBL_VAL_thickmathspace;
	else if( fabs(value - HMathOpAttr_SPACE_verythickmathspace * unit2px.em2px) < minMeaning )
		destname = FBL_VAL_verythickmathspace;
	else if( fabs(value - HMathOpAttr_SPACE_veryverythickmathspace * unit2px.em2px) < minMeaning )
		destname = FBL_VAL_veryverythickmathspace;
	else
	{
		destname = QString("%1px").arg((double)value, 0, 'g', 0);
		return -1;
	}
	return 0;
}

///////////////////////////////////////////////////////////////////////////

int mml_findUnit( QString& destUnitName, double& destVal, int& isNeedRound, const QString& body, int is_y, const SYSINFO_Unit2PX& unit2px )
{
	destUnitName = MML_UNIT_NAME__PX;
	destVal = 1.0;
	isNeedRound = 0;

	long i = 0, result;

	while( i < body.length() && body.at(i).isSpace() ) i++;
	if( i >= body.length() ) return -1;
    if( !(body.at(i).isDigit() || body.at(i) == _T('.') || body.at(i) == _T('-')) ) return -1;
	LXMLSizeSDigit ldigit;
	result = ldigit.Analisys( body.midRef(i) );
	if( result != LSDigit::Integer && result != LSDigit::Float ) return -1;
	i += ldigit.GetRead();
	while( i < body.length() && body.at(i).isSpace() ) i++;

	if( i >= body.length() || !body.at(i).isLetter() ) return -1;
	LCVar lvar;
	result = lvar.Analisys( body.midRef(i) );
	if( !result ) return -1;
	if( lvar.getID() == MML_UNIT_NAME__EM )
	{
		destUnitName = lvar.getID();
		destVal = unit2px.em2px;
	}
	else if( lvar.getID() == MML_UNIT_NAME__EX )
		destVal = unit2px.ex2px;
	else if( lvar.getID() == MML_UNIT_NAME__PX )
	{
		isNeedRound = 1;
		destVal = 1.0;
	}
	else if( lvar.getID() == MML_UNIT_NAME__IN )
		destVal = (is_y ? unit2px.in2px_y : unit2px.in2px_x);
	else if( lvar.getID() == MML_UNIT_NAME__CM )
		destVal = (is_y ? unit2px.cm2px_y : unit2px.cm2px_x);
	else if( lvar.getID() == MML_UNIT_NAME__MM )
		destVal = (is_y ? unit2px.mm2px_y : unit2px.mm2px_x);
	else if( lvar.getID() == MML_UNIT_NAME__PT )
	{
		isNeedRound = 1;
		destVal = (is_y ? unit2px.pt2px_y : unit2px.pt2px_x);
	}
	else if( lvar.getID() == MML_UNIT_NAME__PC )
	{
		isNeedRound = 1;
		destVal = (is_y ? unit2px.pc2px_y : unit2px.pc2px_x);
	}
	else
		return -1;

	destUnitName = lvar.getID();
	return 0;
}

///////////////////////////////////////////////////////////////////////////
