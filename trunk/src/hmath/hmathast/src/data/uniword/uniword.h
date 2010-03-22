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

#ifndef __PARSEDSR_DATA_UNIWORD_UNIWORD_H__
#define __PARSEDSR_DATA_UNIWORD_UNIWORD_H__

#include "HMathBS/inumber.h"
#include "../smbtbllink.h"
#include "../object/object.h"

class MMD_String;
class MMD_Vector;
class MMD_Polynomial;
class MMD_RationalFun;
class MMD_Group;
class MMD_Set;

struct __HMATHLNGDLL__ MMD_Address
{
	long param1;	// addressing scheme
	long param2;	// variable or user function number
	long param3;	// reserved for vector cell addressing or arg number in call
	long param4;	// reserved for res number in call
};

class __HMATHLNGDLL__ UniWord
{
public:
	CDSRComplex				v_complex;

	union {
		MMD_Address			v_addr;

		CDSRNaturalStorage	v_natural;
		CDSRIntegerStorage	v_integer;
		CDSRRationalStorage	v_rational;
		CDSRRealStorage		v_real;

		MMD_String*			v_string;
		MMD_Vector*			v_vector;
		MMD_Polynomial*		v_poly;
		MMD_RationalFun*	v_rfun;
		MMD_Group*			v_group;
		MMD_Set*			v_set;
	};

	enum CDsrDataTypes type;

	//===
	
	enum CDsrDataTypes getType( void ) const;
	void setType( enum CDsrDataTypes tp );

	UniWord( void );
	UniWord( const UniWord& a );
	UniWord( const MMD_Address v );
	UniWord( const CDSRNatural v );
	UniWord( const CDSRNaturalStorage v );
	UniWord( long v );
	UniWord( const CDSRInteger v );
	UniWord( const CDSRIntegerStorage v );
	UniWord( const CDSRRational v );
	UniWord( const CDSRRationalStorage v );
	UniWord( const CDSRReal v );
	UniWord( const CDSRComplex v );
	UniWord( const MMD_Vector* a );
	UniWord( const MMD_Group* a );
	UniWord( const MMD_Set* a );
	UniWord( const MMD_String* v );
	UniWord( const MMD_Polynomial* v );
	UniWord( const MMD_RationalFun* v );
        virtual ~UniWord(){}

	MMD_Address getAddress( void ) const;
	integer getNatural( void ) const;
	integer getInteger( void ) const;
	CDSRRationalStorage getRational( void ) const;
	CDSRReal getReal( void ) const;
	CDSRComplex getComplex( void ) const;
	MMD_String* getString( void ) const;
	MMD_Vector* getVector( void ) const;
	MMD_Polynomial* getPolynomial( void ) const;
	MMD_RationalFun* getRationalFun( void ) const;
	MMD_Group* getGroup( void ) const;
	MMD_Set* getSet( void ) const;

	void operator=( const UniWord& a );
	void operator=( const MMD_Address v );
	void operator=( const CDSRNatural v );
	void operator=( const CDSRNaturalStorage v );
	void operator=( long v );
	void operator=( const CDSRInteger v );
	void operator=( const CDSRIntegerStorage v );
	void operator=( const CDSRRational v );
	void operator=( const CDSRRationalStorage v );
	void operator=( const CDSRReal v );
	void operator=( const CDSRComplex v );
	void operator=( const MMD_Vector* a );
	void operator=( const MMD_Group* a );
	void operator=( const MMD_Set* a );
	void operator=( const MMD_String* v );
	void operator=( const MMD_Polynomial* v );
	void operator=( const MMD_RationalFun* v );
	void operator=( enum CDsrDataTypes tp );

	int operator==( const UniWord& a );

	virtual QString getTextView( CParseDsrSymbolTable& smbtable );
	virtual QString getFormulatorView( CParseDsrSymbolTable& smbtable );
	virtual QString getMmlView( CParseDsrSymbolTable& smbtable );
};

///////////////////////////////////////////////////////////////////////////////

inline
enum CDsrDataTypes UniWord::getType( void ) const
{
	return type;
}

inline
void UniWord::setType( enum CDsrDataTypes tp )
{
	type = tp;
}
///////////////////////////////////////////////////////////////////////////////

inline
UniWord::UniWord( const UniWord& a )
{
	*this = a;
}

inline
UniWord::UniWord( const MMD_Vector* a )
{
	*this = a;
}

inline
UniWord::UniWord( const MMD_Group* a )
{
	*this = a;
}

inline
UniWord::UniWord( const MMD_Set* a )
{
	*this = a;
}

inline
UniWord::UniWord( const CDSRNatural a )
{
	*this = a;
}

inline
UniWord::UniWord( const CDSRNaturalStorage a )
{
	*this = a;
}

inline
UniWord::UniWord( long a )
{
	*this = a;
}

inline
UniWord::UniWord( const CDSRInteger a )
{
	*this = a;
}

inline
UniWord::UniWord( const CDSRIntegerStorage a )
{
	*this = a;
}

inline
UniWord::UniWord( const CDSRRational a )
{
	*this = a;
}

inline
UniWord::UniWord( const CDSRRationalStorage a )
{
	*this = a;
}

inline
UniWord::UniWord( const CDSRReal a )
{
	*this = a;
}

inline
UniWord::UniWord( const CDSRComplex a )
{
	*this = a;
}

inline
UniWord::UniWord( const MMD_String* a )
{
	*this = a;
}

inline
UniWord::UniWord( const MMD_Address a )
{
	*this = a;
}

inline
UniWord::UniWord( const MMD_Polynomial* a )
{
	*this = a;
}

inline
UniWord::UniWord( const MMD_RationalFun* a )
{
	*this = a;
}

///////////////////////////////////////////////////////////////////////////////

inline
integer UniWord::getNatural( void ) const
{
#if _DEBUG
	if( getType() != DSRDATA_TYPE_NATURAL ) 
		throw _T("UniWord::getNatural : internal error");
#endif
	return v_natural.value;
}

inline
integer UniWord::getInteger( void ) const
{
#if _DEBUG
	if( getType() != DSRDATA_TYPE_INTEGER &&
		getType() != DSRDATA_TYPE_BOOL ) 
		throw _T("UniWord::getInteger : internal error");
#endif
	return v_integer.value;
}

inline
CDSRRationalStorage UniWord::getRational( void ) const
{
#if _DEBUG
	if( getType() != DSRDATA_TYPE_RATIONAL ) 
		throw _T("UniWord::getRational : internal error");
#endif
	return v_rational;
}

inline
CDSRReal UniWord::getReal( void ) const
{
#if _DEBUG
	if( getType() != DSRDATA_TYPE_REAL ) 
		throw _T("UniWord::getReal : internal error");
#endif
	return v_real;
}

inline
CDSRComplex UniWord::getComplex( void ) const
{
#if _DEBUG
	if( getType() != DSRDATA_TYPE_COMPLEX ) 
		throw _T("UniWord::getComplex : internal error");
#endif
	return v_complex;
}

inline
MMD_String* UniWord::getString( void ) const
{
#if _DEBUG
	if( getType() != DSRDATA_TYPE_STRING ) 
		throw _T("UniWord::getString : internal error");
#endif
	return v_string;
}

inline
MMD_Vector* UniWord::getVector( void ) const
{
#if _DEBUG
	if( getType() != DSRDATA_TYPE_VECTOR ) 
		throw _T("UniWord::getVector : internal error");
#endif
	return v_vector;
}

inline
MMD_Polynomial* UniWord::getPolynomial( void ) const
{
#if _DEBUG
	if( getType() != DSRDATA_TYPE_POLYNOM ) 
		throw _T("UniWord::getPolynomial : internal error");
#endif
	return v_poly;
}

inline
MMD_RationalFun* UniWord::getRationalFun( void ) const
{
#if _DEBUG
	if( getType() != DSRDATA_TYPE_RATIONAL_FUN ) 
		throw _T("UniWord::getRationalFun : internal error");
#endif
	return v_rfun;
}

inline
MMD_Group* UniWord::getGroup( void ) const
{
#if _DEBUG
	if( getType() != DSRDATA_TYPE_ABS_GROUP ) 
		throw _T("UniWord::getGroup : internal error");
#endif
	return v_group;
}

inline
MMD_Set* UniWord::getSet( void ) const
{
#if _DEBUG
	if( getType() != DSRDATA_TYPE_ABS_SET ) 
		throw _T("UniWord::getSet : internal error");
#endif
	return v_set;
}

inline
MMD_Address UniWord::getAddress( void ) const
{
	return v_addr;
}

///////////////////////////////////////////////////////////////////////////////

class __HMATHLNGDLL__ UniWordVector : public MArray<UniWord>
{
public:
	UniWordVector( long cb = 64 ) : 
		MArray<UniWord>( cb, 0, cb )
	{}
};

///////////////////////////////////////////////////////////////////////////////

#endif
