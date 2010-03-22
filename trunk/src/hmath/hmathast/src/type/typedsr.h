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

#if !defined( __PARSEDSR_OPERATOR_TYPEDSR_H__ )
#define __PARSEDSR_OPERATOR_TYPEDSR_H__

#include "HUtils/ihkstring.h"
#include "HUtils/ihkarray.h"
#include "typemodi.h"

enum CDsrDataTypes
{
	DSRDATA_TYPE_RESERVED = 0,

	DSRDATA_TYPE_ERROR,
	DSRDATA_TYPE_KEYWORD,
	DSRDATA_TYPE_UNKNOWN,
	DSRDATA_TYPE_NONE,
	DSRDATA_TYPE_IGNORE,
	DSRDATA_TYPE_INTERNAL_NESTED,

	DSRDATA_TYPE_NATURAL,
	DSRDATA_TYPE_INTEGER,
	DSRDATA_TYPE_BITS,
	DSRDATA_TYPE_RATIONAL,
	DSRDATA_TYPE_REAL,
	DSRDATA_TYPE_COMPLEX,
	DSRDATA_TYPE_BOOL,
	DSRDATA_TYPE_STRING,

	DSRDATA_TYPE_POLYNOM,
	DSRDATA_TYPE_FUNCTION,
	DSRDATA_TYPE_RATIONAL_FUN,

	DSRDATA_TYPE_COMPOSITION,
	DSRDATA_TYPE_VECTOR,
	DSRDATA_TYPE_RANGE,

	DSRDATA_TYPE_GROUP_STRING,
	DSRDATA_TYPE_ABS_GROUP,

	DSRDATA_TYPE_ABS_SET,

	DSRDATA_TYPE_LAST
};

#define DSRDATA_TYPE_COMPARE_FLAG__NO_CHECK_DIM 0x00000001

__HMATHLNGDLL__
extern int is_scalar_CDsrDataType[ DSRDATA_TYPE_LAST ];
__HMATHLNGDLL__
extern QString CDsrDataTypeNames[ DSRDATA_TYPE_LAST ];

inline
QString getTypeName( enum CDsrDataTypes tp )
{
	QString ret = _T("");
	if( tp >= 0 && tp < DSRDATA_TYPE_LAST )
		ret = CDsrDataTypeNames[ tp ];
	return ret;
}

inline
int isType_Var_and_Function( enum CDsrDataTypes tp )
{
	return tp == DSRDATA_TYPE_POLYNOM || tp == DSRDATA_TYPE_RATIONAL_FUN;
}

/////////////////////////////////////////
//	Node
/////////////////////////////////////////
class __HMATHLNGDLL__ CDsrDataType_Node : public CDsrDataType_Modifier
{
protected:
	enum CDsrDataTypes node_type;
	int type_constructor;	// is type constructor
	long type_table_idx;	// is this object "stand alone" (or index of the type table)

public:
	CDsrDataType_Node( enum CDsrDataTypes _tp = DSRDATA_TYPE_NONE );
	virtual ~CDsrDataType_Node( void );
	int is_type_constructor( void );
	long getTypeTableIdx( void ) const;
	void setTypeTableIdx( long idx );
	void setType( enum CDsrDataTypes tp );
	enum CDsrDataTypes getType( void ) const;
	virtual QString getTypeName( void );
	int isScalar( void );
	virtual int compare( CDsrDataType_Node* b );
	virtual CDsrDataType_Node* getNestedType( void );
	virtual void setNestedType( CDsrDataType_Node *n );
	virtual long getArgumentsNumber( void );

	int operator== (const CDsrDataType_Node a) const
	{
		return node_type == a.node_type &&
			type_constructor == a.type_constructor && 
			type_table_idx == a.type_table_idx;
	}
};


/////////////////////////////////////////
//	Compound
/////////////////////////////////////////
class __HMATHLNGDLL__ CDsrDataType_Compound : public CDsrDataType_Node
{
public:
	CDsrDataType_Node *n_arg;

	CDsrDataType_Compound( enum CDsrDataTypes _tp, CDsrDataType_Node *_arg = 0 );
	virtual ~CDsrDataType_Compound( void );
	virtual int compare( CDsrDataType_Compound* b );
	virtual QString getTypeName( void );
	virtual CDsrDataType_Node* getNestedType( void );
	virtual void setNestedType( CDsrDataType_Node *n );
	virtual long getArgumentsNumber( void );
};


/////////////////////////////////////////
//	Composition
/////////////////////////////////////////
class __HMATHLNGDLL__ CDsrDataType_Composition : public CDsrDataType_Node
{
public:
	MIArray<CDsrDataType_Node> n_arg;
public:
	CDsrDataType_Composition( void );
	void Add( CDsrDataType_Node *n );
	CDsrDataType_Node* at( long i );
	long getChildNumber( void ) const;
	virtual ~CDsrDataType_Composition( void );
	virtual int compare( CDsrDataType_Composition* b );
	virtual CDsrDataType_Node* getNestedType( void );
	virtual QString getTypeName( void );
	virtual long getArgumentsNumber( void );
};


/////////////////////////////////////////
//	Function
/////////////////////////////////////////
class __HMATHLNGDLL__ CDsrDataType_Function : public CDsrDataType_Node
{
public:
	CDsrDataType_Node *n_arg;
	CDsrDataType_Node *n_res;
public:
	CDsrDataType_Function( CDsrDataType_Node *arg = 0, CDsrDataType_Node *res = 0 );
	virtual ~CDsrDataType_Function( void );
	virtual int compare( CDsrDataType_Function* b );
	virtual CDsrDataType_Node* getNestedType( void );
	virtual QString getTypeName( void );
	virtual long getArgumentsNumber( void );
};


/////////////////////////////////////////
//	Vector
/////////////////////////////////////////
class __HMATHLNGDLL__ CDsrDataType_Vector : public CDsrDataType_Compound
{
public:
	MArray<long> dim;
public:
	CDsrDataType_Vector( CDsrDataType_Node *_arg = 0, long _r = -1, long _c = -1 );
	CDsrDataType_Vector( CDsrDataType_Node *_arg, const MArray<long>& src );
	void setRows( long r );
	long getRows( void );
	void setColumns( long c );
	long getColumns( void );
	long getDim( long i );
	virtual int compare( CDsrDataType_Vector* b );
	virtual QString getTypeName( void );
};


/////////////////////////////////////////
//	Set
/////////////////////////////////////////
class __HMATHLNGDLL__ CDsrDataType_Set : public CDsrDataType_Compound
{
public:
	CDsrDataType_Set( CDsrDataType_Node *_arg = 0 );
	virtual int compare( CDsrDataType_Set* b );
};


/////////////////////////////////////////
//	Polynomial
/////////////////////////////////////////
class __HMATHLNGDLL__ CDsrDataType_Polynomial : public CDsrDataType_Compound
{
public:
	CDsrDataType_Polynomial( CDsrDataType_Node *_arg = 0 );
};


/////////////////////////////////////////
//	RationalFun
/////////////////////////////////////////
class __HMATHLNGDLL__ CDsrDataType_RationalFun : public CDsrDataType_Compound
{
public:
	CDsrDataType_RationalFun( CDsrDataType_Node *_arg = 0 );
};


/////////////////////////////////////////
//	Implementation
/////////////////////////////////////////

/////////////////////////////////////////
//	Node
/////////////////////////////////////////

inline
CDsrDataType_Node::CDsrDataType_Node( enum CDsrDataTypes _tp )
{
	// _tp must not be one of {DSRDATA_TYPE_FUNCTION, DSRDATA_TYPE_COMPOSITION, DSRDATA_TYPE_VECTOR, DSRDATA_TYPE_ABS_SET}
	switch( _tp )
	{
	case DSRDATA_TYPE_FUNCTION:
	case DSRDATA_TYPE_COMPOSITION:
	case DSRDATA_TYPE_VECTOR:
	case DSRDATA_TYPE_ABS_SET:
		node_type = DSRDATA_TYPE_ERROR;
		break;
	default:
		node_type = _tp;
	}
	type_constructor = 0;
	type_table_idx = 0;
}

inline
CDsrDataType_Node::~CDsrDataType_Node( void )
{}

inline
int CDsrDataType_Node::is_type_constructor( void )
{
	return type_constructor;
}

inline
long CDsrDataType_Node::getTypeTableIdx( void ) const
{
	return type_table_idx;
}

inline
void CDsrDataType_Node::setTypeTableIdx( long idx )
{
	type_table_idx = idx;
}

inline
void CDsrDataType_Node::setType( enum CDsrDataTypes tp )
{
	node_type = tp;
}

inline
enum CDsrDataTypes CDsrDataType_Node::getType( void ) const
{
	return node_type;
}

inline
QString CDsrDataType_Node::getTypeName( void )
{
	return CDsrDataTypeNames[ getType() ];
}

inline
int CDsrDataType_Node::isScalar( void )
{
	return is_scalar_CDsrDataType[ getType() ];
}

inline
CDsrDataType_Node* CDsrDataType_Node::getNestedType( void )
{
	return 0;
}

inline
void CDsrDataType_Node::setNestedType( CDsrDataType_Node * /*n*/ )
{
}

inline
long CDsrDataType_Node::getArgumentsNumber( void )
{
	return 0;
}

/////////////////////////////////////////
//	Function
/////////////////////////////////////////

inline
CDsrDataType_Function::CDsrDataType_Function( CDsrDataType_Node *arg, CDsrDataType_Node *res )
{
	n_arg = arg;
	n_res = res;
	setType( DSRDATA_TYPE_FUNCTION );
	type_constructor = 1;
}

inline
CDsrDataType_Function::~CDsrDataType_Function( void )
{
/*	if( n_arg && !n_arg->getTypeTableIdx() ) delete n_arg;
	if( n_res && !n_res->getTypeTableIdx() ) delete n_res;*/
}

inline
CDsrDataType_Node* CDsrDataType_Function::getNestedType( void )
{
	return 0;
}

inline
QString CDsrDataType_Function::getTypeName( void )
{
	if( n_arg && n_res )
	{
		QString ret = CDsrDataTypeNames[ getType() ] + QString( _T(" : ") );
		ret += n_arg->getTypeName() + QString( _T(" -> ") ) + n_res->getTypeName();
		return ret;
	}
	else
		return CDsrDataTypeNames[ getType() ];
}

inline
long CDsrDataType_Function::getArgumentsNumber( void )
{
	return 1;
}

/////////////////////////////////////////
//	Composition
/////////////////////////////////////////

inline
CDsrDataType_Composition::CDsrDataType_Composition( void ) :
	n_arg(MIArray<CDsrDataType_Node>::LeaveItems, 8, 0, 8)
{
	setType( DSRDATA_TYPE_COMPOSITION );
	type_constructor = 1;
}

inline
void CDsrDataType_Composition::Add( CDsrDataType_Node *n )
{
	n_arg.Add( n );
}

inline
CDsrDataType_Node* CDsrDataType_Composition::at( long i )
{
	return n_arg[ i ];
}

inline
long CDsrDataType_Composition::getChildNumber( void ) const
{
	return n_arg.ArrayDim();
}

inline
CDsrDataType_Node* CDsrDataType_Composition::getNestedType( void )
{
	return 0;
}

inline
long CDsrDataType_Composition::getArgumentsNumber( void )
{
	return getChildNumber();
}

/////////////////////////////////////////
//	Compound
/////////////////////////////////////////
inline
CDsrDataType_Compound::CDsrDataType_Compound( enum CDsrDataTypes _tp, CDsrDataType_Node *_arg )
{
	setType( _tp );
	type_constructor = 1;
	n_arg = _arg;
}

inline
CDsrDataType_Compound::~CDsrDataType_Compound( void )
{
//	if( n_arg && !n_arg->getTypeTableIdx() ) delete n_arg;
}

inline
CDsrDataType_Node* CDsrDataType_Compound::getNestedType( void )
{
	return n_arg;
}

inline
void CDsrDataType_Compound::setNestedType( CDsrDataType_Node *n )
{
	n_arg = n;
}

inline
QString CDsrDataType_Compound::getTypeName( void )
{
	if( n_arg )
		return CDsrDataTypeNames[ getType() ] + QString( _T("(") ) + n_arg->getTypeName() + QString( _T(")") );
	return CDsrDataTypeNames[ getType() ];
}

inline
long CDsrDataType_Compound::getArgumentsNumber( void )
{
	return 1;
}

/////////////////////////////////////////
//	Vector
/////////////////////////////////////////
inline
CDsrDataType_Vector::CDsrDataType_Vector( CDsrDataType_Node *_arg, long _r, long _c ) :
	CDsrDataType_Compound( DSRDATA_TYPE_VECTOR, _arg ), dim(4, 0, 4)
{
	if( _r != -1 ) dim.Add( _r );
	if( _c != -1 ) dim.Add( _c );
}

inline
CDsrDataType_Vector::CDsrDataType_Vector( CDsrDataType_Node *_arg, const MArray<long>& src ) :
	CDsrDataType_Compound( DSRDATA_TYPE_VECTOR, _arg ), dim(4, 0, 4)
{
	dim.Copy( src );
}

inline
long CDsrDataType_Vector::getRows( void )
{
	if( dim.ArrayDim() > 0 )
		return dim[ 0 ];
	return 0;
}

inline
long CDsrDataType_Vector::getColumns( void )
{
	if( dim.ArrayDim() > 1 )
		return dim[ 1 ];
	return 0;
}

inline
long CDsrDataType_Vector::getDim( long i )
{
	if( ((long) dim.ArrayDim()) > i )
		return dim[ i ];
	return 0;
}

inline
void CDsrDataType_Vector::setRows( long r )
{
	dim[ 0 ] = r;
}

inline
void CDsrDataType_Vector::setColumns( long c )
{
	dim[ 1 ] = c;
}

inline
QString CDsrDataType_Vector::getTypeName( void )
{
	if( n_arg )
	{
		QString buffer;
		if( getRows() && getColumns() )
			buffer = QString("[ %1 ][ %2 ]").arg( getRows() ).arg( getColumns() );
		else
			buffer = QString("[ unknown dimensions ]");
		return CDsrDataTypeNames[ getType() ] + QString( _T("(") ) + n_arg->getTypeName() + QString( _T(")") ) + QString(buffer);
	}
	return CDsrDataTypeNames[ getType() ];
}

/////////////////////////////////////////
//	Set
/////////////////////////////////////////

inline
CDsrDataType_Set::CDsrDataType_Set( CDsrDataType_Node *_arg ) :
	CDsrDataType_Compound( DSRDATA_TYPE_ABS_SET, _arg )
{
}

/////////////////////////////////////////
//	Polynomial
/////////////////////////////////////////

inline
CDsrDataType_Polynomial::CDsrDataType_Polynomial( CDsrDataType_Node *_arg ) :
	CDsrDataType_Compound( DSRDATA_TYPE_POLYNOM, _arg )
{
}

/////////////////////////////////////////
//	RationalFun
/////////////////////////////////////////
inline
CDsrDataType_RationalFun::CDsrDataType_RationalFun( CDsrDataType_Node *_arg ) :
	CDsrDataType_Compound( DSRDATA_TYPE_RATIONAL_FUN, _arg )
{
}

#endif
