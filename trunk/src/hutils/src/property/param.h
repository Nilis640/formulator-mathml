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

#ifndef __COMMON_PARAM_H__
#define __COMMON_PARAM_H__

#include "../basexxx.h"
#include "HUtils/ihkflag.h"
#include "HUtils/ixml.h"
#include "tags.h"
#include "ctrl_type.h"

#define HKParameterType_NONE		-1
#define HKParameterType_BOOL		0
#define HKParameterType_Long		1
#define HKParameterType_Double		2
#define HKParameterType_String		3
#define HKParameterType_Array		4

#define HKParameterTypeName_NONE		_T("none")
#define HKParameterTypeName_BOOL		_T("bool")
#define HKParameterTypeName_Long		_T("long")
#define HKParameterTypeName_Double		_T("double")
#define HKParameterTypeName_String		_T("string")
#define HKParameterTypeName_Array		_T("array")

class HKParameterArray;

class HUTILS_EXPORT HKParameter
{
protected:
	long		type;
	QString		caption;
public:
	union
	{
		long				m_long;
		double				m_double;
		QString*			m_string;
		HKParameterArray*	m_array;
	};

protected:
	void reset( void );

public:
	HKParameter( void );
	HKParameter( const HKParameter& param );

	HKParameter( long value, long tp = HKParameterType_Long );
	HKParameter( double value );
	HKParameter( const QString& value );
	HKParameter( const HKParameterArray& value );
	HKParameter( AST_TXML_Attribute& attr );
	virtual ~HKParameter();

	virtual int isVisual( void );

	void init( long new_type = HKParameterType_NONE );
	void setType( long tp );
	void setCaption( const QString& cp );

	void create( long value, long tp = HKParameterType_Long );
	void create( double value );
	void create( const QString& value );
	void create( const HKParameterArray& value );
	void create( const HKParameter& param );
	void create( AST_TXML_Attribute& attr );

	void operator = ( const HKParameter& param );
	void operator = ( long value );
	void operator = ( double value );
	void operator = ( const QString& value );
	void operator = ( const HKParameterArray& value );
	int operator == ( const HKParameter& param ) const;
	int operator != ( const HKParameter& param ) const;

	long getType( void ) const;
	const QString& getCaption( void ) const;

	int Read_XML( AST_TXML_Tag *root, QString& name, DRMessageManager& dmm, long dmm_page = 0 );

	//virtual void store( CArchive& ar );
	//virtual void load( CArchive& ar );

	//friend CArchive& operator << ( CArchive& ar, HKParameter& param );
	//friend CArchive& operator >> ( CArchive& ar, HKParameter& param );
};

///////////////////////////////////////////////////////////////////////////////

class HKParameterArray : public std::vector<HKParameter>
{
public:
	HKParameterArray( void )
		: std::vector<HKParameter>()
	{
	}
	HKParameterArray( const HKParameterArray& vec )
		: std::vector<HKParameter>( vec )
	{
	}
	virtual ~HKParameterArray()
	{
	}
};

///////////////////////////////////////////////////////////////////////////////

HUTILS_EXPORT
long convertXMLType2ParameterType( enum AST_TXML_ValueTypes tp );

HUTILS_EXPORT
int ConvertAttr2ParameterType( AST_TXML_Attribute *attr, long tp );


///////////////////////////////////////////////////////////////////////////

inline
HKParameter::HKParameter( const HKParameter& param ) : 
	type( HKParameterType_NONE ), caption( _T("") ), m_long( 0 )
{
	create( (HKParameter&) param );
}

inline
HKParameter::HKParameter( long value, long tp ) :
	type( HKParameterType_NONE ), caption( _T("") ), m_long( 0 )
{
	create( value, tp );
}

inline
HKParameter::HKParameter( double value ) : 
	type( HKParameterType_NONE ), caption( _T("") ), m_long( 0 )
{
	create( value );
}

inline
HKParameter::HKParameter( const QString& value ) : 
	type( HKParameterType_NONE ), caption( _T("") ), m_long( 0 )
{
	create( value );
}

inline
HKParameter::HKParameter( const HKParameterArray& value ) : 
	type( HKParameterType_NONE ), caption( _T("") ), m_long( 0 )
{
	create( value );
}

inline
HKParameter::HKParameter( AST_TXML_Attribute& attr ) : 
	type( HKParameterType_NONE ), caption( _T("") ), m_long( 0 )
{
	create( attr );
}

inline
HKParameter::HKParameter( void ) : 
	type( HKParameterType_NONE ), caption( _T("") ), m_long( 0 )
{
	init( HKParameterType_NONE );
}

inline
HKParameter::~HKParameter()
{
	reset();
}

inline
int HKParameter::isVisual( void )
{
	return 0;
}

inline
void HKParameter::operator = ( const HKParameter& param )
{
	create( param );
}

inline
void HKParameter::operator = ( long value )
{
	create( value, HKParameterType_Long );
}

inline
void HKParameter::operator = ( double value )
{
	create( value );
}

inline
void HKParameter::operator = ( const QString& value )
{
	create( value );
}

inline
void HKParameter::operator = ( const HKParameterArray& value )
{
	create( value );
}

inline
int HKParameter::operator != ( const HKParameter& param ) const
{
	return !((*this) == param);
}

inline
long HKParameter::getType( void ) const
{
	return type;
}

inline
const QString& HKParameter::getCaption( void ) const
{
	return caption;
}

inline
void HKParameter::setType( long tp )
{
	if ( type != tp ) init( tp );
}

inline
void HKParameter::setCaption( const QString& cp )
{
	caption = cp;
}

/*
inline
CArchive& operator << ( CArchive& ar, HKParameter& param )
{
	param.store( ar );
	return ar;
}

inline
CArchive& operator >> ( CArchive& ar, HKParameter& param )
{
	param.load( ar );
	return ar;
}
*/

///////////////////////////////////////////////////////////////////////////

class HUTILS_EXPORT HKParameterList
{
private:
	HKMap<QString, HKParameter> m_list;

public:
	HKParameterList( void );
	HKParameterList( const HKParameterList& p );

	void operator = ( const HKParameterList& p );

	unsigned long getParameterNumber( void ) const;
	void Flush( void );

	void setParameter( const QString& key, const HKParameter& param );
	void delParameter( const QString& key );

	const HKParameter* isParameter( const QString& key );
	HKParameter& getParameter( const QString& key );

	bool tofirst();
	bool tonext();
	void get( QString& key, HKParameter& param );

	int Read_XML( AST_TXML_Tag *root, DRMessageManager& dmm, long dmm_page = 0 );

	//virtual void store( CArchive& ar );
	//virtual void load( CArchive& ar );

	//friend CArchive& operator << ( CArchive& ar, HKParameterList& param );
	//friend CArchive& operator >> ( CArchive& ar, HKParameterList& param );
};

///////////////////////////////////////////////////////////////////////////

inline
HKParameterList::HKParameterList( void )
	: m_list()
{
}

inline
HKParameterList::HKParameterList( const HKParameterList& p )
	: m_list( p.m_list )
{
}

inline
void HKParameterList::operator = ( const HKParameterList& p ) 
{
	m_list = p.m_list;
}

inline
unsigned long HKParameterList::getParameterNumber( void ) const
{
	return (unsigned long) m_list.size();
}

inline
void HKParameterList::Flush( void )
{
	m_list.clear();
}

inline
void HKParameterList::setParameter( const QString& key, const HKParameter& param )
{
	m_list[ key ] = param;
}

inline
void HKParameterList::delParameter( const QString& key )
{
	m_list.erase( key );
}

inline
const HKParameter* HKParameterList::isParameter( const QString& key )
{
	return m_list.is_find( key );
}

inline
HKParameter& HKParameterList::getParameter( const QString& key )
{
	return m_list[ key ];
}

inline
bool HKParameterList::tofirst()
{
	return m_list.tofirst();
}

inline
bool HKParameterList::tonext()
{
	return m_list.tonext();
}

inline
void HKParameterList::get( QString& key, HKParameter& param )
{
	m_list.get( key, param );
}

/*
inline
void HKParameterList::store( CArchive& ar )
{
	ar << m_list;
}

inline
void HKParameterList::load( CArchive& ar )
{
	ar >> m_list;
}

inline
CArchive& operator << ( CArchive& ar, HKParameterList& param )
{
	param.store( ar );
	return ar;
}

inline
CArchive& operator >> ( CArchive& ar, HKParameterList& param )
{
	param.load( ar );
	return ar;
}
*/

///////////////////////////////////////////////////////////////////////////

#endif // __COMMON_PARAM_H__
