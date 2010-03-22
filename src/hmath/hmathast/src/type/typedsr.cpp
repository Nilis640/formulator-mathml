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

#include "typedsr.h"

int is_scalar_CDsrDataType[ DSRDATA_TYPE_LAST ] =
{
	0, //DSRDATA_TYPE_RESERVED = 0,

	0, //DSRDATA_TYPE_ERROR,
	0, //DSRDATA_TYPE_KEYWORD,
	0, //DSRDATA_TYPE_UNKNOWN,
	0, //DSRDATA_TYPE_NONE,
	0, //DSRDATA_TYPE_IGNORE,
	0, //DSRDATA_TYPE_INTERNAL_NESTED,

	1, //DSRDATA_TYPE_NATURAL,
	1, //DSRDATA_TYPE_INTEGER,
	1, //DSRDATA_TYPE_BITS,
	1, //DSRDATA_TYPE_RATIONAL,
	1, //DSRDATA_TYPE_REAL,
	1, //DSRDATA_TYPE_COMPLEX,
	1, //DSRDATA_TYPE_BOOL,
	1, //DSRDATA_TYPE_STRING,

	0, //DSRDATA_TYPE_POLYNOM,
	0, //DSRDATA_TYPE_FUNCTION,
	0, //DSRDATA_TYPE_RATIONAL_FUN,

	0, //DSRDATA_TYPE_COMPOSITION,
	0, //DSRDATA_TYPE_VECTOR,
	0, //DSRDATA_TYPE_RANGE,

	1, //DSRDATA_TYPE_GROUP_STRING,
	0, //DSRDATA_TYPE_ABS_GROUP,

	0  //DSRDATA_TYPE_ABS_SET,
};

QString CDsrDataTypeNames[ DSRDATA_TYPE_LAST ] =
{
	_T("internal"),

	_T("error"),
	_T("keyword"),
	_T("unknown"),
	_T("none"),
	_T("ignore"),
	_T("internal"),

	_T("natural number"),
	_T("integer number"),
	_T("bit QString"),
	_T("rational number"),
	_T("real number"),
	_T("complex number"),
	_T("bool"),
	_T("QString"),

	_T("polynomial"),
	_T("function"),
	_T("rational function"),

	_T("composition"),
	_T("vector"),
	_T("range"),

	_T("group QString"),
	_T("abstract group"),

	_T("abstract set")
};


/////////////////////////////////////////
//	Implementation
/////////////////////////////////////////

CDsrDataType_Composition::~CDsrDataType_Composition( void )
{
/*	for( long i = 0; i < n_arg.ArrayDim(); i++ )
		if( n_arg[ i ] && !n_arg[ i ]->getTypeTableIdx() ) delete n_arg[ i ];*/
}

int CDsrDataType_Node::compare( CDsrDataType_Node* b )
{
	if( getType() == DSRDATA_TYPE_IGNORE || b->getType() == DSRDATA_TYPE_IGNORE )
		return 1;
	else if( getType() != b->getType() )
		return 0;

	switch( getType() )
	{
	case DSRDATA_TYPE_ERROR:
	case DSRDATA_TYPE_UNKNOWN:
	case DSRDATA_TYPE_NONE:
		return 0;
	case DSRDATA_TYPE_FUNCTION:
		return ((CDsrDataType_Function*)this)->compare( (CDsrDataType_Function*)b );
	case DSRDATA_TYPE_COMPOSITION:
		return ((CDsrDataType_Composition*)this)->compare( (CDsrDataType_Composition*)b );
	case DSRDATA_TYPE_VECTOR:
		return ((CDsrDataType_Vector*)this)->compare( (CDsrDataType_Vector*)b );
	case DSRDATA_TYPE_ABS_SET:
		return ((CDsrDataType_Set*)this)->compare( (CDsrDataType_Set*)b );
	case DSRDATA_TYPE_POLYNOM:
		return ((CDsrDataType_Polynomial*)this)->compare( (CDsrDataType_Polynomial*)b );
	case DSRDATA_TYPE_RATIONAL_FUN:
		return ((CDsrDataType_RationalFun*)this)->compare( (CDsrDataType_RationalFun*)b );
	default:
		break;
	}
	return 1;
}

QString CDsrDataType_Composition::getTypeName( void )
{
	if( !getChildNumber() )
		return CDsrDataTypeNames[ getType() ];
	QString ret = _T("(");
	if( n_arg[ 0 ] )
		ret += n_arg[ 0 ]->getTypeName();
	for( long i = 1; i < getChildNumber(); i++ )
		if( n_arg[ i ] )
		{
			ret += QString( _T(", ") ) + n_arg[ i ]->getTypeName();
		}
	ret += _T(")");
	return ret;
}

int CDsrDataType_Composition::compare( CDsrDataType_Composition* b )
{
        if( ((getChildNumber() == 1) && n_arg[ 0 ]->getType() == DSRDATA_TYPE_IGNORE) ||
                ((b->getChildNumber() == 1) && b->n_arg[ 0 ]->getType() == DSRDATA_TYPE_IGNORE) )
		return 1;
	if( getChildNumber() != b->getChildNumber() ) return 0;
	for( long i = 0; i < getChildNumber(); i++ )
	{
		if( n_arg[ i ] )
		{
			if( !b->n_arg[ i ] || !n_arg[ i ]->compare( b->n_arg[ i ] ) )
				return 0;
		}
		else
			if( b->n_arg[ i ] ) return 0;
	}
	return 1;
}

int CDsrDataType_Function::compare( CDsrDataType_Function* b )
{
	if( n_arg )
	{
		if( !b->n_arg || !n_arg->compare( b->n_arg ) )
			return 0;
	}
	else
		if( b->n_arg ) return 0;
	if( n_res )
	{
		if( !b->n_res || !n_res->compare( b->n_res ) )
			return 0;
	}
	else
		if( b->n_res ) return 0;
	return 1;
}


int CDsrDataType_Compound::compare( CDsrDataType_Compound* b )
{
	if( n_arg )
	{
		if( !b->n_arg || !n_arg->compare( b->n_arg ) )
			return 0;
	}
	else
		if( b->n_arg ) return 0;
	return 1;
}

int CDsrDataType_Vector::compare( CDsrDataType_Vector* b )
{
	if( !CDsrDataType_Compound::compare( b ) )
		return 0;
	if( dim.ArrayDim() && b->dim.ArrayDim() )
	{
		if( dim.ArrayDim() != b->dim.ArrayDim() )
			return 0;
		for( long i = 0; i < (long) dim.ArrayDim(); i++ )
			if( dim[ i ] != b->dim[ i ] )
				return 0;
	}
	return 1;
}

int CDsrDataType_Set::compare( CDsrDataType_Set* /*b*/ )
{
	return 1;
}
